//Terrain.cpp
//## Description ##
//3D terrain which can be drawn to the screen and interacted with
//via a connected mobile devie and Vicon-tracked pinch gestures.
#define WIN32_LEAN_AND_MEAN

#include "Terrain.h"
#include <windows.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include <gl/gl.h>

#include "PointLineDist.h"
#include <sstream>
#include <math.h>
#include <iostream>

//## Defines ##
#define WINDOW_WIDTH	640				// Window Width  Default
#define WINDOW_HEIGHT 480				// Window Height Default

#define PI						3.14159265 //Mathematical PI constant
#define FINGER_PINCH_MAX_DIST	0.04	   //Furthest distance between fingers before a pinch is registered.
#define MIN_DELAY				100

#define BITMAP_ID 0x4D42		     // the universal bitmap ID
#define MAP_X	32			         // size of map along x-axis
#define MAP_Z	32			         // size of map along z-axis
#define MAP_SCALE 0.00625f		     // the scale of the terrain map

////// Texture Information
BITMAPINFOHEADER	bitmapInfoHeader;	// temp bitmap info header
BITMAPINFOHEADER	landInfo;			// land texture info header
BITMAPINFOHEADER	waterInfo;			// water texture info header

// Data for Tetrahedron
static GLfloat P1T[3] = { -2, 3, 0 }; 
static GLfloat P2T[3] = { -3, 0, 0 };
static GLfloat P3T[3] = { -1, 0, 3 }; 
static GLfloat P4T[3] = { -4, 0, 0 };

//AUX_RGBImageRec
unsigned char*	      imageData;		   // the map image data
unsigned char*       landTexture;	   // land texture data
unsigned int		   land;			      // the land texture object

//## Terrain Data ##
float terrain[MAP_X][MAP_Z][3];		// heightfield terrain data (0-255); 256x256


int currDelay = 0;

	//## Constructor ##
	Terrain::Terrain(TabletServer *server) : DEFAULT_MODE(0), ROI_MODE(1), POI_MODE(2), ENDPOINT_DRAWING_MODE(3), DIRECT_DRAWING_MODE(4), HEIGHT_FILTERING_MODE(5), HEIGHT_SELECT_MODE(6)	
	{
		this->server = server;
		loc = new MHTypes::Point3D(0.0f, 0.0f, 0.0f);
		eulerAngle = MHTypes::EulerAngle(0.0f, 0.0f, 0.0f);
		currFingerPos = MHTypes::Point3D(0.0f, 0.0f, 0.0f);

		minSet = false; //have we found a potential minumum, Y-value for the terrain?
		maxSet = false; //have we found a potential maximum Y-value for the terrain?

		intersecting = false; //does the selection ray intersect the terrain
		pinch = false;				//Have we just completed a pinching gesture?
		pinchHold = false;			//Are we currently holding a pinch
		oldPinchHold = false;		//What was the previous state of pinchHold last time we checked?

		inRoi = false;				//Is a point within the ROI?

		//## Interaction State Flags ##
		settingStartPath = false;	//Currently setting location of the start of an endpoint-defined path
		settingEndPath = false;		//Currently setting location of the end of an endpoint-defined path
		drawingPath = false;		//Currently drawing a freehand-defined path
		settingRoi = false;			//Currently setting the center of an ROI
		settingRoiRadius = false;	//Either currently setting the radius of an ROI, or have already set it
		setHeight = false;			//Currently setting the height filter.
		enableHeightVis = false;	//Should the visualization of the current height filter plane be shown?
		roiCenterPlaced = false;	//The center of the ROI has already been placed. (dispRoiCenter)				
		settingPoi = false;			//Currently setting location of the POI
		dispRoiRadius = false;		//Can ROI radius be displayed?
		setFirstClip = false;		//User can adjust position of bottom clipping plane.
		setSecondClip = false;		//User can adjust position of top clipping plane.
		enableFirstClip = false;	//Enable bottom clipping plane
		enableSecondClip = false;	//Enable top clipping plane
		enableSecondClipVis = false;//Show visualization for second clipping plane?
		enableFirstClipVis = false;	//Show visualization for first clipping plane?


		isFirst = true;				//Is first time through render loop?

		//Offset prevents z-fighting between clip planes and their visualizations
		clipPlaneFirstVisOffset = 0.0;	
		clipPlaneSecondVisOffset = 0.0;

		//Height of clipping planes
		firstClipY = 0.0;
		secondClipY = 0.0;

		botClipY = 0.001f;
		topClipY = 0.0f;

		radius = 5;

		mode = DEFAULT_MODE; //Set current mode to the default interaction mode

		pathColor = new Color3f(1.0f, 1.0f, 1.0f); //Initialize color of path to an arbitrary color

		//Initialize 2D array for freeform path 
		//true at [x][z]  => color terrain vertex at x,z to path color
		//false at [x][z] => leave terrain vertex at x,z default color
		freeformPath = new bool*[MAP_X+1];
		for(int i = 0; i < MAP_X+1; i++){
			freeformPath[i] = new bool[MAP_Z+1];
		}
		for(int i = 0; i < MAP_X+1; i++){
			for(int j = 0; j < MAP_Z+1; j++){
				freeformPath[i][j] = false;
			}
		}

		//## Displayed Markers ##
		startCube = new Cube();		//Denotes the start of an endpoint-defined path
		endCube = new Cube();		//Denotes end of an endpoint-defined path
		roiCube = new Cube();		//Denotes center of an ROI
		poiCube = new Cube();		//Denotes location of a POI
		fingerCube = new Cube();	//Denotes current finger position

		lightingCube = new Cube();  //Provides light to terrain (not actually rendered)

		//Hide markers initially
		poiCube->hide();	
		roiCube->hide();
		start = NULL;
		end = NULL;

		init();

	}


	//## Destructor ##
	Terrain::~Terrain(void)
	{
		delete pathColor;
		free(imageData);
		free(landTexture);

		for(int i = 0; i < MAP_X+1; ++i)
			delete[] freeformPath[i];
		delete[] freeformPath;

		delete startCube;
		delete endCube;
		delete roiCube;
		delete poiCube;
		delete fingerCube;
		delete lightingCube;
	}


	//## Initalization ##
	void Terrain::init()
	{
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);		// clear to black

		glShadeModel(GL_SMOOTH);					   // use smooth shading
		glEnable(GL_DEPTH_TEST);					   // hidden surface removal
		glEnable(GL_CULL_FACE);						   // do not calculate inside of poly's
		glFrontFace(GL_CCW);						      // counter clock-wise polygons are out

		glEnable(GL_TEXTURE_2D);					   // enable 2D texturing

		imageData = loadBitmapFile("Terrain2.bmp", &bitmapInfoHeader); //Loads the source height map from which the terrain is generated

		//## Initialize the cubes
		//TODO: Modify cube constructor, moving these up to constructor
		startCube->setSize(MAP_SCALE*1.5);
		startCube->setColor(1.0f, 0.0f, 0.0f);

		roiCube->setSize(MAP_SCALE*1.5);
		roiCube->setColor(1.0f, 0.0f, 1.0f);

		poiCube->setSize(MAP_SCALE*1.5);
		poiCube->setColor(1.0f, 1.0f, 0.0f);

		endCube->setSize(MAP_SCALE*1.5);
		endCube->setColor(0.0f, 0.0f, 1.0f);

		fingerCube->setSize(MAP_SCALE* 1.5f);
		fingerCube->setColor(1.0f, 1.0f, 0.0f);

		lightingCube->setSize(MAP_SCALE*1.5f);
		lightingCube->setColor(1.0f, 1.0f, 1.0f);

		// initialize the terrain data, load the textures, and set up clipping planes
		initTerrain();
		loadTextures();
		initClipPlanes();
	}

	//Defines 2 clipping planes to restrict the visible terrain to a particular height range.
	//These are only activated once height filtering mode is used.
	void Terrain::initClipPlanes()
	{

		//Create clipping planes
		//Top Clip Plane
		  topPlaneEq[0] = 0; //A
		  topPlaneEq[1] = -1; //B
		  topPlaneEq[2] = 0; //C (A,B,C) is a normal vector of resulting clip plane.
		  topPlaneEq[3] = 0.0; //Value for D in eq. of plane given by Ax + By + Cz + D = 0 (where (x,y,z) is any point on the resulting plane)

		//Bottom Clip Plane
		  botPlaneEq[0] = 0;
		  botPlaneEq[1] = 1;
		  botPlaneEq[2] = 0;
		  botPlaneEq[3] = 0.0;
	}

	//Sends data to mobile device through socket
	void Terrain::send(const char* str){
		server->sendData(str);
	}

	//## Setters ##
	void Terrain::setPosition(MHTypes::Point3D point)
	{
		loc = new MHTypes::Point3D(point.x, point.y, point.z);
	}

	void Terrain::setRotMat(float* rotMat)
	{
		//Stores 4x4 rotation matrix for use with glMultMatrix();
		for(int i = 0; i < 16; i++){
			rotMatrix[i] = rotMat[i];
		}
		//Store the 4x4 inverse rotation matrix as well
		for(int i = 0; i <16; i++){
			invRotMatrix[i] = rotMatrix[((i%4)*4) + (i/4)];
		}
	}

	//Sets rotation via Quaternion
	void Terrain::setRotation(MHTypes::Quaternion rot)
	{
		quat = rot;
	}

	// Initializes the heightfield terrain data
	void Terrain::initTerrain()
	{
		//Record boundaries of the terrain
		maxX = MAP_X * MAP_SCALE;
		minX = 0.0f;
		maxZ = 0.0f;
		minZ = -MAP_Z*MAP_SCALE;

		// loop through all of the heightfield points, calculating
		// the coordinates for each point
		for (int z = 0; z < MAP_Z; z++)
		{
			for (int x = 0; x < MAP_X; x++)
			{
				terrain[x][z][0] = float(x)*MAP_SCALE;				
				terrain[x][z][1] = (float)imageData[(z*MAP_Z+x)*3];
				terrain[x][z][2] = -float(z)*MAP_SCALE;

				//Record minimum and maximum Y-values of terrain
				if(!minSet || minY > (float)imageData[(z*MAP_Z+x)*3])
				{
					minSet = true;
					minY = (float)imageData[(z*MAP_Z+x)*3];
				}
				if(!maxSet || maxY < (float)imageData[(z*MAP_Z+x)*3])
				{
					maxSet = true;
					maxY = (float)imageData[(z*MAP_Z+x)*3];
				}
			}
		}

		//Get maxY & minY in terms of the coordinate system for displaying
		minDisplayY = minY * (MAP_SCALE/20.0);
		maxDisplayY = maxY * (MAP_SCALE/20.0);

		//Position cube to produce lighting for terrain (note, this cube is not actually displayed
		// but affects the lighting of the terrain)
		lightingCube->setPosition(MHTypes::Point3D(16*MAP_SCALE, maxDisplayY, 16*MAP_SCALE));
	}


	// Render
	// desc: handles drawing of scene
	void Terrain::render()
	{
		glEnable(GL_TEXTURE_2D);
		Color3f *color;

		//server->parseData();
		handleMessages();
		//###########################################
		//## For taking cross-section screenshots ###
		//###########################################
		//glMatrixMode(GL_MODELVIEW);

		//glPushMatrix();
		//glTranslatef(0.0f, minY*(MAP_SCALE/20) + 0.045f, 0.0f);
		//glClipPlane(GL_CLIP_PLANE2, topPlaneEq);
		//glPopMatrix();

		//glPushMatrix();
		//glTranslatef(0.0f, minY*(MAP_SCALE/20) +0.045f, 0.0f);
		//glClipPlane(GL_CLIP_PLANE3, botPlaneEq);
		//glPopMatrix();
		//glEnable(GL_CLIP_PLANE2);
		//glEnable(GL_CLIP_PLANE3);

		
		//#################################
		//***Drawing original Terrain***//
		//#################################
		// set the current texture to the land texture
		//glBindTexture(GL_TEXTURE_2D, land);
		//glPushMatrix();
		////glTranslatef(((MAP_X*MAP_SCALE)/2.0f), 0.0, -((MAP_Z*MAP_SCALE)/2.0f)); //translate back from origin
		////glMultMatrixf(rotMatrix); //rotate via Vicon
		//glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f)); //translate to origin

		//for (int z = 0; z < MAP_Z-1; z++)
		//{
		//	glBegin(GL_TRIANGLE_STRIP);
		//	for (int x = 0; x < MAP_X-1; x++)
		//	{

		//		pathColor->setRGB(terrain[x][z][1]/255.0f);

		//		glColor3f(pathColor->getRed(), pathColor->getGreen(), pathColor->getBlue());
		//		//glColor3f(d, d, d);
		//		glTexCoord2f(0.0f, 0.0f);
		//		glVertex3f(terrain[x][z][0], terrain[x][z][1] * (MAP_SCALE/20.0f), terrain[x][z][2]);
		//		pathColor->setRGB(terrain[x+1][z][1]/255.0f);


		//		// draw vertex 1
		//		glTexCoord2f(1.0f, 0.0f);
		//		//glColor3f(terrain[x+1][z][1]/255.0f, terrain[x+1][z][1]/255.0f, terrain[x+1][z][1]/255.0f);
		//		glColor3f(pathColor->getRed(), pathColor->getGreen(), pathColor->getBlue());
		//		glVertex3f(terrain[x+1][z][0], terrain[x+1][z][1] * (MAP_SCALE/20.0f), terrain[x+1][z][2]);
		//		pathColor->setRGB(terrain[x][z+1][1]/255.0f);

		//		// draw vertex 2
		//		glTexCoord2f(0.0f, 1.0f);
		//		glColor3f(pathColor->getRed(), pathColor->getGreen(), pathColor->getBlue());
		//		glVertex3f(terrain[x][z+1][0], terrain[x][z+1][1] * (MAP_SCALE/20.0f), terrain[x][z+1][2]);

		//		pathColor->setRGB(terrain[x+1][z+1][1]/255.0f);
		//		// draw vertex 3
		//		glColor3f(pathColor->getRed(), pathColor->getGreen(), pathColor->getBlue());
		//		glTexCoord2f(1.0f, 1.0f);
		//		glVertex3f(terrain[x+1][z+1][0], terrain[x+1][z+1][1] * (MAP_SCALE/20.0f), terrain[x+1][z+1][2]);
		//	}
		//	glEnd();
		//}
		//glPopMatrix();

		glMatrixMode(GL_MODELVIEW);
		//############################
		//##Draw Holo-Mobile Terrain##
		//############################
		glBindTexture(GL_TEXTURE_2D, land);
		glPushMatrix(); //Save pre-terrain view matrix

		glTranslatef(loc->x, loc->y, loc->z); //Positions Terrain
		glMultMatrixf(rotMatrix); //Rotates Terrain
		glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f)); //Moves terrain to origin

		glPushMatrix(); //Save pre-clip view matrix

		//Determine which clipping plane is the upper and which is lower
		//based on their height relative to one another.
		// ------first => upper     OR    -------second => upper
		// ------second => lower          -------first  => lower
		if(firstClipY > secondClipY){
			botClipY = secondClipY;
			topClipY = firstClipY;
		}
		else{
			botClipY = firstClipY;
			topClipY = secondClipY;
		}
		//Position & initialize clipping planes
		glTranslatef(0.0f, botClipY, 0.0f);
		glClipPlane(GL_CLIP_PLANE0, botPlaneEq);
		
		glTranslatef(0.0f, topClipY - botClipY, 0.0f);
		glClipPlane(GL_CLIP_PLANE1, topPlaneEq);

		//Enable clipping planes
		if(enableFirstClip){
			if(firstClipY > secondClipY){
				glEnable(GL_CLIP_PLANE0);
			}
			else{
				glEnable(GL_CLIP_PLANE1);
			}
		}
			
		if(enableSecondClip){
			if(secondClipY > firstClipY){
				glEnable(GL_CLIP_PLANE0);
			}
			else{
				glEnable(GL_CLIP_PLANE1);
			}
		}

		glPopMatrix(); //Restore pre-clip (post-terrain) view matrix


		// we are going to loop through all of our terrain's data points,
		// but we only want to draw one triangle strip for each set along the x-axis.
		for (int z = 0; z < MAP_Z-1; z++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for (int x = 0; x < MAP_X-1; x++)
			{
				// for each vertex, we calculate the grayscale shade color, 
				// we set the texture coordinate, and we draw the vertex.
				/*
				   the vertices are drawn in this order:

				   0  ---> 1
						  /
						/
					 |/
				   2  ---> 3
				*/

				// draw vertex 0
				setVertexColor(x, z);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(terrain[x][z][0], terrain[x][z][1] * (MAP_SCALE/20.0f), terrain[x][z][2]);

				// draw vertex 1
				setVertexColor(x+1, z);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(terrain[x+1][z][0], terrain[x+1][z][1] * (MAP_SCALE/20.0f), terrain[x+1][z][2]);

				// draw vertex 2
				setVertexColor(x, z+1);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(terrain[x][z+1][0], terrain[x][z+1][1] * (MAP_SCALE/20.0f), terrain[x][z+1][2]);

				// draw vertex 3
				setVertexColor(x+1, z+1);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(terrain[x+1][z+1][0], terrain[x+1][z+1][1] * (MAP_SCALE/20.0f), terrain[x+1][z+1][2]);
			}
			glEnd();


		}

		//############################
		//##      Draw Markers      ##
		//############################

			if(start){
				startCube->setPosition(MHTypes::Point3D(start->x, (terrain[(int)(start->x *(1/MAP_SCALE))][(int)(start->z *(1/MAP_SCALE))][1]* (MAP_SCALE/20.0f)) + 0.02f , start->z));
				startCube->render();
			}

			if(end){
				endCube->setPosition(MHTypes::Point3D(end->x, (terrain[(int)(end->x *(1/MAP_SCALE))][(int)(end->z *(1/MAP_SCALE))][1]* (MAP_SCALE/20.0f)) + 0.02f , end->z));
				endCube->render();
			}

			roiCube->setPosition(MHTypes::Point3D(roi.x, (terrain[(int)(roi.x *(1/MAP_SCALE))][(int)(roi.z *(1/MAP_SCALE))][1]* (MAP_SCALE/20.0f)) + 0.02f , roi.z));
			roiCube->render();

			poiCube->setPosition(MHTypes::Point3D(poi.x, (terrain[(int)(poi.x *(1/MAP_SCALE))][(int)(poi.z *(1/MAP_SCALE))][1]* (MAP_SCALE/20.0f)) + 0.02f , poi.z));
			poiCube->render();

			if(isFirst){
				lightingCube->render();
				isFirst = false;
			}
				
		glPopMatrix();
		
		// enable blending
		glEnable(GL_BLEND);

		// enable read-only depth buffer
		glDepthMask(GL_FALSE);

		// set the blend function to what we use for transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);

		// set back to normal depth buffer mode (writable)
		glDepthMask(GL_TRUE);

		// disable blending
		glDisable(GL_BLEND);
				
		//############################################
		//############ Renders Selection Ray #########
		//############################################
		glPushMatrix();
		glTranslatef(loc->x, loc->y, loc->z);
		glMultMatrixf(rotMatrix);
		glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f));

		glLineWidth(10.0f);

		//Determine selection ray color
		//red => not intersecting, not pinching
		//blue => intersecting, not pinching
		//green => pinching
		glColor3f(1.0f, 0.0f, 0.0f);
		if(intersecting){
			glColor3f(0.0f, 0.0f, 1.0f);
		}
		if(pinch || pinchHold){
			glColor3f(0.0f, 1.0f, 0.0f);
		}
		//Draw SelectorRay
		glBegin(GL_LINES);
		//Disable SelectorRay (comment following 2 lines out to disable)
		glVertex3f(selectorRay.x, -1000.0f, selectorRay.z);
		glVertex3f(selectorRay.x, 1000.0f, selectorRay.z);
		glEnd();


		//#######################################
		//########### Renders Clip Plane ########
		//#######################################

		//Set up transparency for clip plane visualizations
		glColor4f(0.0f, 0.0f, 1.0f, 0.5f);
		glEnable (GL_BLEND); 
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//If enabled, render the visualization of the first clip plane
		if(enableFirstClipVis){
			glPushMatrix();

			glTranslatef(0.0f, clipPlaneFirstVisOffset, 0.0f);
			glBegin(GL_QUADS);
				glVertex3f(0.0f, firstClipY, -MAP_Z*MAP_SCALE);
				glVertex3f(0.0f, firstClipY, 0.0f);
				glVertex3f(MAP_X*MAP_SCALE, firstClipY, 0.0f);
				glVertex3f(MAP_X*MAP_SCALE, firstClipY, -MAP_Z*MAP_SCALE);
			glEnd();

			glPopMatrix();
		}
		//If enabled, render the visualization of the second clip plane
		if(enableSecondClipVis){
			glPushMatrix();

			glTranslatef(0.0f, clipPlaneSecondVisOffset, 0.0f);
			glBegin(GL_QUADS);
				glVertex3f(0.0f, secondClipY, -MAP_Z*MAP_SCALE);
				glVertex3f(0.0f, secondClipY, 0.0f);
				glVertex3f(MAP_X*MAP_SCALE, secondClipY, 0.0f);
				glVertex3f(MAP_X*MAP_SCALE, secondClipY, -MAP_Z*MAP_SCALE);
			glEnd();

			glPopMatrix();
		}
		//If enabled, render the height selector
		if(enableHeightVis){
			glPushMatrix();

			glTranslatef(0.0f, 0.0f, 0.0f);
			glBegin(GL_QUADS);
				glVertex3f(0.0f, selectorRay.y, -MAP_Z*MAP_SCALE);
				glVertex3f(0.0f, selectorRay.y, 0.0f);
				glVertex3f(MAP_X*MAP_SCALE, selectorRay.y, 0.0f);
				glVertex3f(MAP_X*MAP_SCALE, selectorRay.y, -MAP_Z*MAP_SCALE);
			glEnd();

			glPopMatrix();
		}

		glPopMatrix();

		glFlush();
		
	}

	//Determines color of terrain (on path vs. off-path color)
	void Terrain::setVertexColor(int x, int z)
	{
		float dist;

		if(dispRoiRadius){ //Tests if point is within ROI
			inRoi = testInRoi(roi.x *(1/MAP_SCALE), -(roi.z *(1/MAP_SCALE)), x, z, radius);
		}
		if(start && end){ //Tests if point is sufficiently close to the path (from start to end).
			dist = TestDistanceFromLineSegmentToPoint(start->x *(1/MAP_SCALE),-(start->z *(1/MAP_SCALE)), end->x *(1/MAP_SCALE), -(end->z *(1/MAP_SCALE)),x, z);
		}
		if((start && end) && (dist < 1.0f))//Endpoint path coloring case
		{
			pathColor->setRGB((terrain[x][z][1]/255.0f)/2.0f, (terrain[x][z][1]/255.0f)/2.0f, 1.0);
		}
		else if(freeformPath[x][z]) //Freehand path coloring case
		{
			pathColor->setRGB(1.0f, (terrain[x][z][1]/255.0f)/2.0f, 1.0f);
		}	
		else if(inRoi) //Roi radius coloring case
		{ 
			pathColor->setRGB(1.0, 1.0, (terrain[x][z][1]/255.0f)/2.0f);
		}			
		else //Ordinary case
		{
			pathColor->setRGB(terrain[x][z][1]/255.0f);
		}

		//setColor
		glColor3f(pathColor->getRed(), pathColor->getGreen(), pathColor->getBlue());
	}

	//Determines whether a pinch is being held (pinchHold) or has just been released (pinch)
	void Terrain::testPinch(MHTypes::Point3D finger, MHTypes::Point3D thumb)
	{
		//Set current interaction mode
		handleMessages();

		//update current pinching state
		if(finger.distance(thumb) < FINGER_PINCH_MAX_DIST)
		{
			oldPinchHold = pinchHold;
			pinchHold = true;
		}
		else{
			pinchHold = false;
			if(oldPinchHold && !pinchHold){
				pinch = true;
				pinchHold = false;
			}else{
				pinch = false;
			}
			oldPinchHold = false;
		}

		if (currDelay > 0){
			currDelay--;
		}

		//Invoke state machine for current mode
		if(mode == ENDPOINT_DRAWING_MODE)
			handleEndpointDrawing(state);
		else if (mode == DIRECT_DRAWING_MODE)
			handleDirectDrawing(state);
		else if (mode == ROI_MODE)
			handleROIPlacement(state);
		else if (mode == POI_MODE)
			handlePOIPlacement(state);
		else if (mode == HEIGHT_FILTERING_MODE)
			handleHeightFiltering(state);
		else if (mode == HEIGHT_SELECT_MODE)
			handleHeightSelection(state);

	}

	//Sets the interaction mode based on command sent from tablet
	//Echoes a confirmation message back to tablet
	void Terrain::handleMessages(){
		std::string msg;
		msg = server->parseData();
		ostringstream ss;
		ss << "Handling Message: " << msg << std::endl;
		if(!msg.empty()){
			OutputDebugString(ss.str().c_str());
		}

		//Selecting type of interaction
		if(msg == "placepath\n"){
			setInteractionMode(ENDPOINT_DRAWING_MODE);
			server->sendData("placepath\n");
		}
		else if(msg == "drawpath\n"){
			setInteractionMode(DIRECT_DRAWING_MODE);
			server->sendData("drawpath\n");
		}	
		else if(msg == "placeroi\n"){
			setInteractionMode(ROI_MODE);
			server->sendData("placeroi\n");
		}	
		else if(msg == "placepoi\n"){
			setInteractionMode(POI_MODE);
			server->sendData("placepoi\n");
		}
		else if(msg == "filterheight\n"){
			setInteractionMode(HEIGHT_FILTERING_MODE);
			server->sendData("filterheight\n");
		}
		else if(msg == "selectheight\n"){
			setInteractionMode(HEIGHT_SELECT_MODE);
			server->sendData("selectheight\n");
		}
		
	}
	void Terrain::handlePOIPlacement(int &state){

		const int RESET = 0;
		const int PLACING = 1;
		const int FINISH = 2;


		switch(state){
			case RESET: //Resets any previously drawn path
				server->sendData("clear,poi\n");
				poiCube->hide();
				settingPoi = false;
				//roi = NULL;
				if(pinchHold && currDelay == 0)
				{
					state = PLACING;
					currDelay = 40;
				}
				break;
			case PLACING: //Currently drawing path, pinch to finish
				poiCube->show();
				settingPoi = true;
				if(pinch && currDelay == 0){
					state = FINISH;
					currDelay = 40;
				}
				break;
			case FINISH: //Currently drawing path, pinch to finish
				settingPoi = false;
				if(pinch && currDelay == 0){
					state = RESET;
					currDelay = 40;
				}
				break;
		}

	}

	void Terrain::handleROIPlacement(int &state){

		const int RESET = 0;
		const int PLACING = 1;
		const int SET_RADIUS = 2;
		const int FINISH = 3;


		switch(state){
			case RESET: //Resets any previously drawn path
				dispRoiRadius = false;
				server->sendData("clear,roi\n");
				roiCube->hide();
				radius = 2;
				roiCenterPlaced = false;

				if(pinchHold && currDelay == 0)
				{
					state = PLACING;
					currDelay = 40;
				}
				break;
			case PLACING: //Currently drawing path, pinch to finish
				roiCube->show();
				settingRoi = true;
				roiCenterPlaced = true;
				if(pinch && currDelay == 0){
					state = SET_RADIUS;
					currDelay = 40;
				}
				break;
			case SET_RADIUS: //Currently drawing path, pinch to finish
				dispRoiRadius = true;
				settingRoi = false;
				settingRoiRadius = true;
				if(pinch && currDelay == 0){
					state = FINISH;
					currDelay = 40;
				}
				break;
			case FINISH: //Start and End have both been placed, pinch to go back to starting state
				roiCenterPlaced = false;
				settingRoiRadius = false;
				if(pinch && currDelay == 0){
					state = RESET;
					currDelay = 40;
				}
				break;
		}

	}

	void Terrain::handleDirectDrawing(int &state){

		const int RESET = 0;
		const int DRAWING = 1;
		const int FINISH = 2;


		switch(state){
			case RESET: //Resets any previously drawn path
				server->sendData("clear,freehand\n");
				for(int i = 0; i < MAP_X+1; i++){
					for(int j = 0; j < MAP_Z+1; j++){
						freeformPath[i][j] = false;
					}
				}
				if(pinch && currDelay == 0)
				{
					state = DRAWING;
					currDelay = 40;
				}
				break;
			case DRAWING: //Currently drawing path, pinch to finish
				drawingPath = true;
				if(pinch && currDelay == 0){
					state = FINISH;
					currDelay = 40;
				}
				break;
			case FINISH: //Start and End have both been placed, pinch to go back to starting state
				drawingPath = false;
				if(pinch && currDelay == 0){
					state = RESET;
					currDelay = 40;
				}
				break;

		}

	}

	void Terrain::handleEndpointDrawing(int &state)
	{
		const int RESET = 0;
		const int PLACING_START = 1;
		const int GRABBING_END = 4;
		const int PLACING_END = 2;
		const int FINISH = 3;

			switch(state){
				case RESET: //Resets start and end positions of path
					server->sendData("clear,endpoints\n");
					start = NULL;
					end = NULL;
					if(pinchHold)// && currDelay == 0)
					{
						state = PLACING_START;
						currDelay = 40;
					}
					break;
				case PLACING_START: //Placing the starting position of path
					settingStartPath = true;
					if(pinch)//&& currDelay == 0){
					{
						state = PLACING_END;
						currDelay = 40;
					}
					break;
				case GRABBING_END:
					if(pinchHold)// && currDelay == 0)
					{
						state = PLACING_END;
						currDelay = 40;
					}
					break;
				case PLACING_END: //Placing the ending position of path
					settingEndPath = true;
					if(pinch && currDelay == 0){
						state = FINISH;
						currDelay = 40;
					}
					break;
				case FINISH: //Start and End have both been placed, pinch to go back to starting state
					if(pinch && currDelay == 0)
					{
						//state = RESET;
						currDelay = 40;
					}
						
					break;
			}
	}

	void Terrain::handleHeightFiltering(int &state)
	{

		const int RESET = 0;
		const int PLACING_FIRST = 1;
		const int PLACING_SECOND = 2;
		const int FINISH = 3;

		switch(state){
			case RESET: //Resets start and end positions of path
				enableFirstClip = false;
				enableSecondClip = false;
				disableClipPlanes();
				firstClipY = 0.0f;
				secondClipY = 0.0f;
				if(pinch && currDelay == 0)
				{
					state = PLACING_FIRST;
					currDelay = 40;
				}
				break;
			case PLACING_FIRST: //Placing the bottom clip plane
				//enableBotClip = true;
				enableFirstClipVis = true;
				setFirstClip = true;
				//clipPlaneFirstVisOffset = 0.0001f;
				if(pinch && currDelay == 0){

					state = PLACING_SECOND;
					currDelay = 40;
				}
				break;
			case PLACING_SECOND: //Placing the top clip plane
				//enableTopClip = true;
				enableSecondClipVis = true;
				setFirstClip = false;
				setSecondClip = true;
				if(firstClipY > secondClipY){
					clipPlaneFirstVisOffset = -0.0001f;
					clipPlaneSecondVisOffset = 0.0001f;
				}
				else{
					clipPlaneFirstVisOffset = 0.0001f;
					clipPlaneSecondVisOffset = -0.0001f;
				}
				
				if((pinch && currDelay == 0) && !((firstClipY > maxDisplayY && secondClipY > maxDisplayY) || (firstClipY < minDisplayY && secondClipY < minDisplayY))){
					//((firstClipY < maxDisplayY && firstClipY > minDisplayY) || (secondClipY < maxDisplayY && secondClipY > minDisplayY)) &&  ){
					state = FINISH;
					currDelay = 40;
				}
				break;
			case FINISH: //Start and End have both been placed, pinch to go back to starting state
				setSecondClip = false;
				enableFirstClip = true;
				enableSecondClip = true;
				enableSecondClipVis = false;
				enableFirstClipVis = false;
				if(pinch && currDelay == 0)
				{
					state = RESET;
					clipPlaneFirstVisOffset = -0.0001f; //Ensures rendering of clipPlane visualizations
					clipPlaneSecondVisOffset = 0.0001f;
					currDelay = 40;
				}
						
				break;
		}
	}

	void Terrain::handleHeightSelection(int &state)
	{
		const int RESET = 0;
		const int PLACING_HEIGHT = 1;
		const int PLACING_SECOND = 2;
		const int FINISH = 3;

		switch(state){
			case RESET: //Resets start and end positions of path
				setHeight = false;
				enableHeightVis = false;
				if(pinchHold && currDelay == 0)
				{
					state = PLACING_HEIGHT;
					currDelay = 40;
				}
				break;
			case PLACING_HEIGHT: //Placing the bottom clip plane
				setHeight = true;
				enableHeightVis = true;
				if(pinch && currDelay == 0){

					state = RESET;
					currDelay = 40;
				}
				break;
		}

	}

	void Terrain::setInteractionMode(int mode)
	{
		
		resetInteractionStates();
		this->mode = mode;
		//clear any remaining state information
		
	}

	void Terrain::resetInteractionStates()
	{
		state = 0;

		//Reset height-selecting states
		setHeight = false;
		enableHeightVis = false;

		//Reset clippling-Plane information
		enableFirstClip = false;
		enableSecondClip = false;
		disableClipPlanes();
		firstClipY = 0.0f;
		secondClipY = 0.0f;
		setFirstClip = false;
		setSecondClip = false;
		enableSecondClipVis = false;
		enableFirstClipVis = false;
		clipPlaneFirstVisOffset = -0.0001f; //Ensures rendering of clipPlane visualizations at start
		clipPlaneSecondVisOffset = 0.0001f;

		//Reset endpoint-drawing states
		settingStartPath = false;
		settingEndPath = false;

		//Reset direct freehand drawing states
		drawingPath = false;

		//Reset ROI drawing states
		settingRoi = false;
		settingRoiRadius = false;

		//Reset POI drawing states
		settingPoi = false;

	}

	void Terrain::disableClipPlanes(){
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_CLIP_PLANE1);

	}
	//Tests for intersection of selecting finger and terrain plane
	void Terrain::testIntersect(MHTypes::Point3D point)
	{
		currFingerPos = MHTypes::Point3D(point.x, point.y, point.z);
		//selectorRay = new MHTypes::Point3D(point.x, point.y, point.z);
		MHTypes::Point3D newPoint = MHTypes::Point3D(point.x, point.y, point.z);
		//Undo final translation: glTranslatef(loc->x, loc->y, loc->z);
		newPoint.x = point.x - loc->x;
		newPoint.y = point.y - loc->y;
		newPoint.z = point.z - loc->z;

		testPoint = MHTypes::Point3D(newPoint.x, newPoint.y, newPoint.z);
		MHTypes::Point3D testPointRay = MHTypes::Point3D(newPoint.x, newPoint.y + 0.12f, newPoint.z);

		//OpenGLUtility::InverseMatrix(m_mat, inverse);
		//Undo rotation: glMultMatrixf(rotMatrix); by multiplying by inverse rot matrix
		/*testPoint.x = rotMatrix[0]*testPoint.x + rotMatrix[4]*testPoint.y + rotMatrix[8]*testPoint.z + rotMatrix[12];
		testPoint.y = rotMatrix[1]*testPoint.x + rotMatrix[5]*testPoint.y + rotMatrix[9]*testPoint.z + rotMatrix[13];
		testPoint.z = rotMatrix[2]*testPoint.x + rotMatrix[6]*testPoint.y + rotMatrix[10]*testPoint.z + rotMatrix[14];*/

		//good inverse transform

		/*testPoint.x = rotMatrix[0]*testPoint.x + rotMatrix[1]*testPoint.y + rotMatrix[2]*testPoint.z + rotMatrix[3];
		testPoint.y = rotMatrix[4]*testPoint.x + rotMatrix[5]*testPoint.y + rotMatrix[6]*testPoint.z + rotMatrix[7];
		testPoint.z = rotMatrix[8]*testPoint.x + rotMatrix[9]*testPoint.y + rotMatrix[10]*testPoint.z + rotMatrix[11];*/

		testPointRay.x = rotMatrix[0]*testPointRay.x + rotMatrix[1]*testPointRay.y + rotMatrix[2]*testPointRay.z + rotMatrix[3];
		testPointRay.y = rotMatrix[4]*testPointRay.x + rotMatrix[5]*testPointRay.y + rotMatrix[6]*testPointRay.z + rotMatrix[7];
		testPointRay.z = rotMatrix[8]*testPointRay.x + rotMatrix[9]*testPointRay.y + rotMatrix[10]*testPointRay.z + rotMatrix[11];

		/*float yaw = eulerAngle.yaw *(PI/180.0f);
		testPoint.x = cos(-yaw)*testPoint.x + sin(-yaw)*testPoint.z;
		testPoint.z = -sin(-yaw)*testPoint.x + cos(-yaw)*testPoint.z;*/


		

		//Undo first translation: glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f));
		newPoint.x = newPoint.x + ((MAP_X*MAP_SCALE)/2.0f);
		newPoint.z = newPoint.z - ((MAP_Z*MAP_SCALE)/2.0f);

		testPoint.x = testPoint.x + ((MAP_X*MAP_SCALE)/2.0f);
		testPoint.z = testPoint.z - ((MAP_Z*MAP_SCALE)/2.0f);

		testPointRay.x = testPointRay.x + ((MAP_X*MAP_SCALE)/2.0f);
		testPointRay.z = testPointRay.z - ((MAP_Z*MAP_SCALE)/2.0f);

		selectorRay = MHTypes::Point3D(newPoint.x, newPoint.y, newPoint.z);

		if(newPoint.x <= maxX && newPoint.x >= minX && newPoint.z <= maxZ && newPoint.z >= minZ)
		{
			intersecting = true;
			//selectorRay = MHTypes::Point3D(newPoint.x, newPoint.y, newPoint.z);
			originalRay = MHTypes::Point3D(testPoint.x, testPoint.y, testPoint.z);
			originalRayExtend = MHTypes::Point3D(testPointRay.x, testPointRay.y, testPointRay.z);
			if(settingStartPath){
				start = new MHTypes::Point3D(testPoint.x, testPoint.y, testPoint.z);
				ostringstream msg;
				msg << "startpoint," << (start->x*(1.0f/MAP_SCALE)) << "," << (start->z*(1.0f/MAP_SCALE)) << std::endl;
				send(msg.str().c_str());
				ostringstream ss;
				ss << "Start point created at: " << "X: " << testPoint.x << " Z: " << testPoint.z << std::endl;
				//OutputDebugString(ss.str().c_str());
			}
			if(settingEndPath){
				end = new MHTypes::Point3D(testPoint.x, testPoint.y, testPoint.z);
				ostringstream msg;
				msg << "endpoint," << (end->x*(1.0f/MAP_SCALE)) << "," << (end->z*(1.0f/MAP_SCALE)) << std::endl;
				send(msg.str().c_str());
				ostringstream ss;
				ss << "Start point created at: " << "X: " << testPoint.x << " Z: " << testPoint.z << std::endl;
				//OutputDebugString(ss.str().c_str());
			}
			if(drawingPath){
				curr = MHTypes::Point3D(testPoint.x, testPoint.y, testPoint.z);
				ostringstream msg;
				msg << "freehand," << (curr.x*(1.0f/MAP_SCALE)) << "," << (curr.z*(1.0f/MAP_SCALE)) << std::endl;
				send(msg.str().c_str());
				int vertexX = round(testPoint.x*(1/MAP_SCALE));
				int vertexZ = round(testPoint.z*(1/MAP_SCALE));
				ostringstream ss;
				ss << "Drawing at Vertex: " << "X: " << vertexX << " Z: " << vertexZ << std::endl;
				//OutputDebugString(ss.str().c_str());
				if(vertexX < MAP_X && -vertexZ < MAP_Z){
					freeformPath[vertexX][-vertexZ] = true;
				}
			}
			if(settingRoi){
				roi = MHTypes::Point3D(testPoint.x, testPoint.y, testPoint.z);
				ostringstream msg;
				msg << "roi," << (roi.x*(1.0f/MAP_SCALE)) << "," << (roi.z*(1.0f/MAP_SCALE)) << ",0" << std::endl;
				send(msg.str().c_str());
				ostringstream ss;
				ss << "ROI created at: " << "X: " << testPoint.x << " Z: " << testPoint.z << std::endl;
				//OutputDebugString(ss.str().c_str());
			}
			if(settingPoi){
				poi = MHTypes::Point3D(testPoint.x, testPoint.y, testPoint.z);
				ostringstream msg;
				msg << "poi," << (poi.x*(1.0f/MAP_SCALE)) << "," << (poi.z*(1.0f/MAP_SCALE)) << std::endl;
				send(msg.str().c_str());
				ostringstream ss;
				ss << "POI created at: " << "X: " << testPoint.x << " Z: " << testPoint.z << std::endl;
			}

			if(settingRoiRadius){
				radius = calcDist(roi, testPoint);
				ostringstream msg;
				msg << "roi," << (roi.x*(1.0f/MAP_SCALE)) << "," << (roi.z*(1.0f/MAP_SCALE)) << "," << radius << std::endl;
				send(msg.str().c_str());

			}
			if(setFirstClip){
				firstClipY = selectorRay.y;
			}
			if(setSecondClip){
				secondClipY = selectorRay.y;
			}
			if(setHeight){
				ostringstream msg;
				msg << "height," << (selectorRay.y - (minY*(MAP_SCALE/20.0f))) << std::endl;//(roi->x*(1.0f/MAP_SCALE)) << "," << (roi->z*(1.0f/MAP_SCALE)) << "," << radius << std::endl;
				send(msg.str().c_str());
				OutputDebugString(msg.str().c_str());
			}
		}
		else{
			intersecting = false;
		}
		settingStartPath = false;
		settingEndPath = false;
	}




	// LoadBitmapFile
	// desc: Returns a pointer to the bitmap image of the bitmap specified
	//       by filename. Also returns the bitmap header information.
	//		   No support for 8-bit bitmaps.
	unsigned char *Terrain::loadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
	{
		FILE *filePtr;							      // the file pointer
		BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
		unsigned char		*bitmapImage;			// bitmap image data
		int					imageIdx = 0;		   // image index counter
		unsigned char		tempRGB;				   // swap variable

		// open filename in "read binary" mode
		filePtr = fopen(filename, "rb");
		if (filePtr == NULL)
			return NULL;

		// read the bitmap file header
		fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
		// verify that this is a bitmap by checking for the universal bitmap id
		if (bitmapFileHeader.bfType != BITMAP_ID)
		{
			fclose(filePtr);
			return NULL;
		}

		// read the bitmap information header
		fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

		// move file pointer to beginning of bitmap data
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		// allocate enough memory for the bitmap image data
		bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

		// verify memory allocation
		if (!bitmapImage)
		{
			free(bitmapImage);
			fclose(filePtr);
			return NULL;
		}

		// read in the bitmap image data
		fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

		// make sure bitmap image data was read
		if (bitmapImage == NULL)
		{
			fclose(filePtr);
			return NULL;
		}

		// swap the R and B values to get RGB since the bitmap color format is in BGR
		for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
		{
			tempRGB = bitmapImage[imageIdx];
			bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
			bitmapImage[imageIdx + 2] = tempRGB;
		}

		// close the file and return the bitmap image data
		fclose(filePtr);
		return bitmapImage;
	}

	bool Terrain::loadTextures()
	{
		// load the land texture data
		landTexture = loadBitmapFile("green.bmp", &landInfo);
		if (!landTexture)
			return false;

		// generate the land texture as a mipmap
		glGenTextures(1, &land);                  
		glBindTexture(GL_TEXTURE_2D, land);       
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, landInfo.biHeight, landInfo.biWidth, GL_RGB, GL_UNSIGNED_BYTE, landTexture);

		return true;
	}

	int Terrain::round(float number)
	{
		return number < 0.0 ? ceil(number - 0.5) : floor(number + 0.5);
	}
	
	bool Terrain::testInRoi(int roiX, int roiZ, int x, int z, int radius){
		bool result = false; 
		double deltaX = roiX - x;
		double deltaZ = roiZ - z;
		int dist = sqrt((deltaX*deltaX) + (deltaZ*deltaZ));
		if(dist < radius)
			result = true;

		return result;

	}

	int Terrain::calcDist(MHTypes::Point3D p1, MHTypes::Point3D p2)
	{
		double deltaX = (p1.x - p2.x)*(1/MAP_SCALE);
		double deltaZ = (p1.z - p2.z)*(1/MAP_SCALE);
		int dist = sqrt((deltaX*deltaX) + (deltaZ*deltaZ));

		return dist;
	}