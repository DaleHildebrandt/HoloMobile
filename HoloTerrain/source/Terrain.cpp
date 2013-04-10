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








#define WINDOW_WIDTH	640				// Window Width  Default
#define WINDOW_HEIGHT 480				// Window Height Default

// definition of PI
#define PI						3.14159265
#define FINGER_PINCH_MAX_DIST	0.04
#define MIN_DELAY				100

// Used to defien the title of the window
#define WINDOW_TITLE  "Vahe Karamian - OpenGL Terrain Generation - CodeProject Version 1.0"

// A simple structure to define a point whose coordinates are integers
/*typedef struct { GLint x, y; } GLintPoint;

// This structure is used to store the vertices of a polyline
typedef struct { int num; GLintPoint pt[100]; } GLintPointArray;

// Data for an Icosahedron
#define ICO_X	0.525731112119133606
#define ICO_Z	0.850650808352039932*/

/*static GLfloat vdataICO[12][3] =
{
	{ -ICO_X, 0.0, ICO_Z }, { ICO_X, 0.0, ICO_Z }, { -ICO_X, 0.0, -ICO_Z }, { ICO_X, 0.0, -ICO_Z },
	{ 0.0, ICO_Z, ICO_X }, { 0.0, ICO_Z, -ICO_X }, { 0.0, -ICO_Z, ICO_X }, { 0.0, -ICO_Z, -ICO_X },
	{ ICO_Z, ICO_X, 0.0 }, { -ICO_Z, ICO_X, 0.0 }, { ICO_Z, -ICO_X, 0.0 }, { -ICO_Z, -ICO_X, 0.0 }
};

static GLuint tindicesICO[20][3] =
{
	{ 1, 4, 0 }, { 4, 9, 0 }, { 4, 5, 9 }, { 8, 5, 4 }, { 1, 8, 4 },
	{ 1, 10, 8 }, { 10, 3, 8 }, { 8, 3, 5 }, { 3, 2, 5 }, { 3, 7, 2 },
	{ 3, 10, 7 }, { 10, 6, 7 }, { 6, 11, 7 }, { 6, 0, 11 }, {6, 1, 0 },7
	{ 10, 1, 6 }, { 11, 0, 9 }, { 2, 11, 9 }, { 5, 2, 9 }, { 11, 2, 7 }
};*/

////// Defines
#define BITMAP_ID 0x4D42		      // the universal bitmap ID
#define MAP_X	32			         // size of map along x-axis
#define MAP_Z	32			         // size of map along z-axis
//#define MAP_SCALE 0.03125f//1.0f	  // the scale of the terrain map
#define MAP_SCALE 0.00625f
//#define MAP_SCALE 0.00166f

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

////// Terrain Data
float terrain[MAP_X][MAP_Z][3];		// heightfield terrain data (0-255); 256x256
//float cameraX, cameraY, cameraZ;	   // camera coordinates
//float lookX, lookY, lookZ;			   // camera look-at coordinates
//
//float radians = 0.0f;			   // camera angle in radians

int currDelay = 0;


	Terrain::Terrain(TabletServer *server) : DEFAULT_MODE(0), ROI_MODE(1), POI_MODE(2), ENDPOINT_DRAWING_MODE(3), DIRECT_DRAWING_MODE(4), HEIGHT_FILTERING_MODE(5), HEIGHT_SELECT_MODE(6)	
	{
		this->server = server;
		loc = new MHTypes::Point3D(0.0f, 0.0f, 0.0f);
		eulerAngle = MHTypes::EulerAngle(0.0f, 0.0f, 0.0f);
		currFingerPos = MHTypes::Point3D(0.0f, 0.0f, 0.0f);
		adjustMinHeight = true;
		minSet = false;
		maxSet = false;
		intersecting = false;
		startPath = false;
		endPath = false;
		drawingPath = false;
		creatingPath = false;
		pinch = false;
		pinchHold = false;
		oldPinchHold = false;
		inRoi = false;
		placeRoi = false;
		setRoiRadius = false;
		setBotClip = false;
		setTopClip = false;
		enableBotClip = false;
		enableTopClip = false;
		enableBotClipVis = false;
		enableTopClipVis = false;
		setHeight = false;
		enableHeightVis = false;
		setRoi = false;
		setPoi = false;
		dispRoiRadius = false;
		isFirst = true;

		setFirstClip = false; //User can adjust position of bottom clipping plane.
		setSecondClip = false; //User can adjust position of top clipping plane.
		enableFirstClip = false; //Enable bottom clipping plane
		enableSecondClip = false; //Enable top clipping plane
		enableSecondClipVis = false;
		enableFirstClipVis = false;
		clipPlaneFirstVisOffset = 0.0;
		clipPlaneSecondVisOffset = 0.0;
		firstClipY = 0.0;
		secondClipY = 0.0;

		radius = 5;
		clipPlaneVisOffset = 0.0f;
		botClipY = 0.001f;
		topClipY = 0.0f;
		mode = DEFAULT_MODE;
		pathColor = new Color3f(1.0f, 1.0f, 1.0f);
		freeformPath = new bool*[MAP_X+1];
		for(int i = 0; i < MAP_X+1; i++){
			freeformPath[i] = new bool[MAP_Z+1];
		}
		for(int i = 0; i < MAP_X+1; i++){
			for(int j = 0; j < MAP_Z+1; j++){
				freeformPath[i][j] = false;
			}
		}

		yawCube = new Cube();
		startCube = new Cube();
		roiCube = new Cube();
		poiCube = new Cube();
		poiCube->hide();
		roiCube->hide();
		endCube = new Cube();
		fingerCube = new Cube();
		lightingCube = new Cube();

		start = NULL;
		end = NULL;

		init();

	}

	Terrain::~Terrain(void)
	{
		delete pathColor;
		free(imageData);
		free(landTexture);

		for(int i = 0; i < MAP_X+1; ++i)
			delete[] freeformPath[i];
		delete[] freeformPath;
	}


	void Terrain::init()
	{
		glClearColor(0.0f, 1.0f, 0.0f, 0.0f);		// clear to black

		glShadeModel(GL_SMOOTH);					   // use smooth shading
		glEnable(GL_DEPTH_TEST);					   // hidden surface removal
		glEnable(GL_CULL_FACE);						   // do not calculate inside of poly's
		glFrontFace(GL_CCW);						      // counter clock-wise polygons are out

		glEnable(GL_TEXTURE_2D);					   // enable 2D texturing

		imageData = loadBitmapFile("Terrain2.bmp", &bitmapInfoHeader);

		startCube->setSize(MAP_SCALE*1.5);
		startCube->setColor(1.0f, 0.0f, 0.0f);

		roiCube->setSize(MAP_SCALE*1.5);
		roiCube->setColor(1.0f, 0.0f, 1.0f);

		poiCube->setSize(MAP_SCALE*1.5);
		poiCube->setColor(1.0f, 1.0f, 0.0f);

		yawCube->setSize(MAP_SCALE * 3.0f);
		yawCube->setColor(0.0f, 1.0f, 0.0f);

		endCube->setSize(MAP_SCALE*1.5);
		endCube->setColor(0.0f, 0.0f, 1.0f);

		fingerCube->setSize(MAP_SCALE* 1.5f);
		fingerCube->setColor(1.0f, 1.0f, 0.0f);

		lightingCube->setSize(MAP_SCALE*1.5f);
		lightingCube->setColor(1.0f, 1.0f, 1.0f);

		// initialize the terrain data and load the textures
		initTerrain();
		loadTextures();
		initClipPlanes();
	}


	//Defines 4 clipping planes to limit the visible region of the map
	void Terrain::initClipPlanes()
	{

		//Create clipping planes
		//Left side
		  botPlaneEq[0] = 0; //A
		  botPlaneEq[1] = -1; //B
		  botPlaneEq[2] = 0; //C (A,B,C) is a normal vector resulting clip plane.
		  botPlaneEq[3] = 0.0; //Value for D in eq. of plane given Ax + By + Cz + D = 0 (where (x,y,z) is any point on the resulting plane)

		//far side
		  topPlaneEq[0] = 0;
		  topPlaneEq[1] = 1;
		  topPlaneEq[2] = 0;
		  topPlaneEq[3] = 0.0;

	}
	void Terrain::send(const char* str){
	// cout << "Sending this event: " << str << endl;
		//if (connectTablet)
		server->sendData(str);
	}

	void Terrain::setPosition(MHTypes::Point3D point)
	{
		loc = new MHTypes::Point3D(point.x, point.y, point.z);

	}

	void Terrain::setRotMat(float* rotMat)
	{
		//Store transformation matrix for use with glMultMatrix();
		for(int i = 0; i < 16; i++){
			rotMatrix[i] = rotMat[i];
		}
		//Store the inverse rotation matrix as well
		for(int i = 0; i <16; i++){
			invRotMatrix[i] = rotMatrix[((i%4)*4) + (i/4)];
		}
	}

	//Sets rotation via Euler angles
	void Terrain::setRotation(MHTypes::Quaternion rot)
	{
		quat = rot;
		/*float yaw = eulerAngle.yaw *(PI/180.0f);
		rotMatrixY[0] = cos(yaw);
		rotMatrixY[1] = 0;
		rotMatrixY[2] = -sin(yaw);
		rotMatrixY[3] = 0;
		rotMatrixY[4] = 0;
		rotMatrixY[5] = 1;
		rotMatrixY[6] = 0;
		rotMatrixY[7] = 0;
		rotMatrixY[8] = sin(yaw);
		rotMatrixY[9] = 0;
		rotMatrixY[10] = cos(yaw);
		rotMatrixY[11] = 0;
		rotMatrixY[12] = 0;
		rotMatrixY[13] = 0;
		rotMatrixY[14] = 0;
		rotMatrixY[15] = 1;*/

	}

	// InitializeTerrain()
	// desc: initializes the heightfield terrain data
	void Terrain::initTerrain()
	{
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

		minDisplayY = minY * (MAP_SCALE/20.0);
		maxDisplayY = maxY * (MAP_SCALE/20.0);
		lightingCube->setPosition(MHTypes::Point3D(16*MAP_SCALE, maxDisplayY, 16*MAP_SCALE));
	}


	// Render
	// desc: handles drawing of scene
	void Terrain::render()
	{
				glEnable(GL_TEXTURE_2D);
		float dist = 100.0f;
		Color3f *color;
		float angle = 60.0f;
		radians = float(PI*(angle-90.0f)/180.0f);

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
		glPushMatrix();

		glTranslatef(loc->x, loc->y, loc->z);
		glMultMatrixf(rotMatrix);
		glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f));

		glPushMatrix();
		if(firstClipY > secondClipY){
			botClipY = secondClipY;
			topClipY = firstClipY;
		}
		else{
			botClipY = firstClipY;
			topClipY = secondClipY;
		}
		glTranslatef(0.0f, botClipY, 0.0f);

		//Set clipping planes
		glClipPlane(GL_CLIP_PLANE0, topPlaneEq);
		
		glTranslatef(0.0f, topClipY - botClipY, 0.0f);
		glClipPlane(GL_CLIP_PLANE1, botPlaneEq);


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

		glPopMatrix();

		
		//float d = TestDistanceFromLineSegmentToPoint(0.0,0.0, 20.0, 20.0,20.0, 32.0);

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

				setVertexColor(x, z);
				glTexCoord2f(0.0f, 0.0f);

				glVertex3f(terrain[x][z][0], terrain[x][z][1] * (MAP_SCALE/20.0f), terrain[x][z][2]);

				//if(!(start && end) || dist > 0.5f || drawingPath){
				//	if(freeformPath[x+1][z]){
				//		pathColor->setRGB(1.0f);
				//	}
				//	/*else if(adjustMinHeight)
				//	{
				//		if(terrain[x+1][z][1]* (MAP_SCALE/20.0f) < selectorRay.y){
				//			pathColor->setRGB(1.0f);
				//		}
				//	}*/
				//	else{
				//		pathColor->setRGB(terrain[x+1][z][1]/255.0f);
				//	}
				//	
				//}

				setVertexColor(x+1, z);
				// draw vertex 1
				glTexCoord2f(1.0f, 0.0f);
				//glColor3f(terrain[x+1][z][1]/255.0f, terrain[x+1][z][1]/255.0f, terrain[x+1][z][1]/255.0f);
				glColor3f(pathColor->getRed(), pathColor->getGreen(), pathColor->getBlue());
				glVertex3f(terrain[x+1][z][0], terrain[x+1][z][1] * (MAP_SCALE/20.0f), terrain[x+1][z][2]);

				//if(!(start && end) || dist > 0.5f || drawingPath){
				//	if(freeformPath[x][z+1]){
				//		pathColor->setRGB(1.0f);
				//	}
				//	//else if(adjustMinHeight)
				//	//{
				//	//	//ostringstream ss;
				//	//	//ss << "Terrain: " << terrain[x][z+1][1] << "testPoint: "<< testPoint.y*(1.0/MAP_SCALE) << std::endl;
				//	//	//OutputDebugString(ss.str().c_str());
				//	//	if(terrain[x][z+1][1]* (MAP_SCALE/20.0f) < selectorRay.y){
				//	//	
				//	//		pathColor->setRGB(1.0f);
				//	//	}
				//	//	else{
				//	//		pathColor->setRGB(0.0f);
				//	//	}
				//	//}
				//	else{
				//		pathColor->setRGB(terrain[x][z+1][1]/255.0f);
				//	}
				//	
				//}
				setVertexColor(x, z+1);
				// draw vertex 2
				glTexCoord2f(0.0f, 1.0f);
				//glColor3f(terrain[x][z+1][1]/255.0f, terrain[x][z+1][1]/255.0f, terrain[x][z+1][1]/255.0f);
				glColor3f(pathColor->getRed(), pathColor->getGreen(), pathColor->getBlue());
				glVertex3f(terrain[x][z+1][0], terrain[x][z+1][1] * (MAP_SCALE/20.0f), terrain[x][z+1][2]);

				//if(!(start && end) || dist > 0.5f || drawingPath){
				//	if(freeformPath[x+1][z+1]){
				//		pathColor->setRGB(1.0f);
				//	}
				//	/*else if(adjustMinHeight)
				//	{
				//		if(terrain[x+1][z+1][1]* (MAP_SCALE/20.0f) < selectorRay.y){
				//			pathColor->setRGB(1.0f);
				//		}
				//	}*/
				//	else{
				//		pathColor->setRGB(terrain[x+1][z+1][1]/255.0f);
				//	}
				//	
				//}
				setVertexColor(x+1, z+1);
				// draw vertex 3
				//glColor3f(terrain[x+1][z+1][1]/255.0f, terrain[x+1][z+1][1]/255.0f, terrain[x+1][z+1][1]/255.0f);
				glColor3f(pathColor->getRed(), pathColor->getGreen(), pathColor->getBlue());
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(terrain[x+1][z+1][0], terrain[x+1][z+1][1] * (MAP_SCALE/20.0f), terrain[x+1][z+1][2]);
			}
			glEnd();


		}
		//glDisable(GL_TEXTURE_2D);
		if(start){
				startCube->setPosition(MHTypes::Point3D(start->x, (terrain[(int)(start->x *(1/MAP_SCALE))][(int)(start->z *(1/MAP_SCALE))][1]* (MAP_SCALE/20.0f)) + 0.02f , start->z));
				startCube->render();
			}

			if(end){
				endCube->setPosition(MHTypes::Point3D(end->x, (terrain[(int)(end->x *(1/MAP_SCALE))][(int)(end->z *(1/MAP_SCALE))][1]* (MAP_SCALE/20.0f)) + 0.02f , end->z));
				endCube->render();
			}
			//if(roi){
				roiCube->setPosition(MHTypes::Point3D(roi.x, (terrain[(int)(roi.x *(1/MAP_SCALE))][(int)(roi.z *(1/MAP_SCALE))][1]* (MAP_SCALE/20.0f)) + 0.02f , roi.z));
				roiCube->render();
			//}
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
				//glTranslatef(loc->x, loc->y, loc->z);
		//glMultMatrixf(rotMatrix);
		////glScalef(0.2f, 0.2f, 0.2f);
		//glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f));

		/*glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_LINES);
				glVertex3f(curr.x, -1000.0f, curr.z);
				glVertex3f(curr.x, 1000.0f, curr.z);
				glEnd();
		glPopMatrix();*/

		//if(showGround){
					//############################################
				//############ Renders Selection Ray #########
				//############################################
				glPushMatrix();
				glTranslatef(loc->x, loc->y, loc->z);
				glMultMatrixf(rotMatrix);
				glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f));

				glLineWidth(10.0f);
				glColor3f(1.0f, 0.0f, 0.0f);
				if(intersecting){
					glColor3f(0.0f, 0.0f, 1.0f);
				}
				if(pinch || pinchHold){
					glColor3f(0.0f, 1.0f, 0.0f);
				}
				glBegin(GL_LINES);
				//Disable SelectorRay
				glVertex3f(selectorRay.x, -1000.0f, selectorRay.z);
				glVertex3f(selectorRay.x, 1000.0f, selectorRay.z);
				glEnd();

				glColor4f(0.0f, 0.0f, 1.0f, 0.5f);

				//#######################################
				//########### Renders Clip Plane ########
				//#######################################
				glEnable (GL_BLEND); 
				glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
				if(enableSecondClipVis){
					glPushMatrix();

					glTranslatef(0.0f, clipPlaneSecondVisOffset, 0.0f);
					glBegin(GL_QUADS);
						glVertex3f(0.0f,secondClipY, -MAP_Z*MAP_SCALE);
						glVertex3f(0.0f, secondClipY, 0.0f);
						glVertex3f(MAP_X*MAP_SCALE, secondClipY, 0.0f);
						glVertex3f(MAP_X*MAP_SCALE, secondClipY, -MAP_Z*MAP_SCALE);
					glEnd();

					glPopMatrix();
				}
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
				/*if(enableTopClipVis && enableBotClipVis){
					glBegin(GL_QUADS);
						glVertex3f(minX,botClipY, maxZ);
						glVertex3f(maxX, botClipY, maxZ);
						glVertex3f(maxX, topClipY, maxZ);
						glVertex3f(minX,topClipY, maxZ);
					glEnd();

				}*/

				

				glPopMatrix();
		//}


		//glPushMatrix();
		//if(originalRay){
		//	
		//	glColor3f(1.0f, 0.0f, 0.0f);
		//	/*float drawMinX = minX, drawMinY = minY, drawMinZ = minZ;
		//	float drawMaxX = maxX, drawMaxY = maxY, drawMaxZ = maxZ;

		//	drawMinX = drawMinX - loc->x;
		//	drawMaxX = drawMaxX - loc->x;
		//	drawMinY = drawMinY - loc->y;
		//	drawMaxY = drawMaxY - loc->y;
		//	drawMinZ = drawMinZ - loc->z;
		//	drawMaxZ = drawMaxZ - loc->z;

		//		glBegin(GL_QUADS);

		//		glVertex3f(drawMinX, drawMaxY* (MAP_SCALE/20.0f), drawMaxZ);
		//		glVertex3f(drawMaxX, drawMaxY* (MAP_SCALE/20.0f), drawMaxZ);
		//		glVertex3f(drawMaxX, drawMaxY* (MAP_SCALE/20.0f), drawMinZ);
		//		glVertex3f(drawMinX, drawMaxY* (MAP_SCALE/20.0f), drawMinZ);
		//		
		//		glEnd();*/

		//		//#############################################
		//		//### Renders untransformed finger position ###
		//		//#############################################
		//		glPushMatrix();
		//		fingerCube->setPosition(MHTypes::Point3D(originalRay->x, originalRay->y, originalRay->z));
		//		fingerCube->render();
		//		glPopMatrix();


		//		//############################################
		//		//############ Renders Selection Ray #########
		//		//############################################
		//		glPushMatrix();
		//		glTranslatef(loc->x, loc->y, loc->z);
		//		//glMultMatrixf(rotMatrix);
		//		glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f));
		//		glLineWidth(10.0f);
		//		glColor3f(0.0f, 0.0f, 1.0f);
		//		glBegin(GL_LINES);
		//		glVertex3f(selectorRay->x, -1000.0f, selectorRay->z);
		//		glVertex3f(selectorRay->x, 1000.0f, selectorRay->z);
		//		glEnd();
		//		glPopMatrix();

		//		//#############################################
		//		//### Renders Untransformed Ray ###############
		//		//#############################################
		//		//Draw Origianl Ray
		//		glPushMatrix();

		//		glColor3f(1.0f, 0.0f, 0.0f);
		//		//glTranslatef(((MAP_X*MAP_SCALE)/2.0f), 0.0, -((MAP_Z*MAP_SCALE)/2.0f));
		//		//glMultMatrixf(rotMatrix);
		//		//glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f));
		//		glBegin(GL_LINES);

		//		MHTypes::Point3D dirVector = MHTypes::Point3D(originalRayExtend->x - originalRay->x, originalRayExtend->y - originalRay->y, originalRayExtend->z - originalRay->z);
		//		glVertex3f(originalRay->x + (dirVector.x * 1000.0f), originalRay->y + (dirVector.y * 1000.0f), originalRay->z + (dirVector.z * 1000.0f));
		//		glVertex3f(originalRay->x - (dirVector.x * 1000.0f), originalRay->y - (dirVector.y * 1000.0f), originalRay->z - (dirVector.z * 1000.0f));

		//		glEnd();
		//		glPopMatrix();
		//}
		//############################################
		//### Renders Untransformed Starting Point ###
		//############################################
		//if(start){
		//	glPushMatrix();
		//	float startX = start->x;
		//	float startZ = start->z;
		//	startX -= ((MAP_X*MAP_SCALE)/2.0f); 
		//	startZ += (MAP_Z*MAP_SCALE)/2.0f;

		//	startX = rotMatrix[0]*startX + rotMatrix[8]*startZ + rotMatrix[12];//rotMatrix[4]*testPoint.y + rotMatrix[8]*testPoint.z + rotMatrix[12];
		//	//float yawY = rotMatrix[1]*0.1f + rotMatrix[13];//+ rotMatrix[5]*testPoint.y + rotMatrix[9]*testPoint.z + rotMatrix[13];
		//	startZ = rotMatrix[2]*startX + rotMatrix[10]*startZ + rotMatrix[14];
		//	startX += ((MAP_X*MAP_SCALE)/2.0f); 
		//	startZ -= (MAP_Z*MAP_SCALE)/2.0f;

		//	//glMultMatrixf(rotMatrix);
		//	//glTranslatef(((MAP_X*MAP_SCALE)/2.0f), 0.0, -((MAP_Z*MAP_SCALE)/2.0f)); //from origin
		//	//glTranslatef(-((MAP_X*MAP_SCALE)/2.0f), 0.0, ((MAP_Z*MAP_SCALE)/2.0f)); //to origin
		//	glBegin(GL_LINES);
		//					
		//	glVertex3f(startX, -1000.0f, startZ);
		//	glVertex3f(startX, 1000.0f, startZ);
		//	glEnd();
		//	glPopMatrix();
		//}

		glFlush();
		
		//SwapBuffers(g_HDC);			// bring backbuffer to foreground
	}

	//Determine color of terrain (on path vs. off-path color)
	void Terrain::setVertexColor(int x, int z)
	{
		float dist;

		if(dispRoiRadius){
			inRoi = testInRoi(roi.x *(1/MAP_SCALE), -(roi.z *(1/MAP_SCALE)), x, z, radius);
		}
		if(start && end){
			dist = TestDistanceFromLineSegmentToPoint(start->x *(1/MAP_SCALE),-(start->z *(1/MAP_SCALE)), end->x *(1/MAP_SCALE), -(end->z *(1/MAP_SCALE)),x, z);
		}

		if((start && end) && (dist < 1.0f))//Endpoint path coloring case
		{
			pathColor->setRed((terrain[x][z][1]/255.0f)/2.0f);
			pathColor->setGreen((terrain[x][z][1]/255.0f)/2.0f);
			pathColor->setBlue(1.0);
			//- (dist));
		}
		else if(freeformPath[x][z]) //Freehand path coloring case
		{
			pathColor->setRed(1.0f);
			pathColor->setGreen((terrain[x][z][1]/255.0f)/2.0f);
			//pathColor->setBlue((terrain[x][z][1]/255.0f)/2.0f);
			pathColor->setBlue(1.0f);
		}	
		else if(inRoi) //Roi radius coloring case
		{ 
			pathColor->setRed(1.0);
			pathColor->setGreen(1.0);
			pathColor->setBlue((terrain[x][z][1]/255.0f)/2.0f);
		}			
		else //Ordinary case
		{
			pathColor->setRGB(terrain[x][z][1]/255.0f);
		}

		////Not highlighted via path endpoints case
		//if(!(start && end) || dist > 1.0f){
		//	
		//	//Roi radius coloring case
		//	if(inRoi){
		//		pathColor->setRed(1.0);
		//		pathColor->setGreen(1.0);
		//		pathColor->setBlue((terrain[x][z][1]/255.0f)/2.0f);
		//	}
		//	//Ordinary case
		//	else{
		//		pathColor->setRGB(terrain[x][z][1]/255.0f);
		//	}
		//			
		//}
		////Near endpoints path case
		//else if (start && end)
		//{
		//	pathColor->setRed((terrain[x][z][1]/255.0f)/2.0f);
		//	pathColor->setGreen((terrain[x][z][1]/255.0f)/2.0f);
		//	pathColor->setBlue(1.0 - (dist));
		//}
		////Freeform path case
		//else if(freeformPath[x][z]){
		//	pathColor->setRed(1.0f);
		//	pathColor->setGreen((terrain[x][z][1]/255.0f)/2.0f);
		//	pathColor->setBlue((terrain[x][z][1]/255.0f)/2.0f);
		//}

		glColor3f(pathColor->getRed(), pathColor->getGreen(), pathColor->getBlue());

	}

	// Calculating the Normalized Cross Product of Two Vectors
	void Terrain::normalize( float v[3] )
	{
		GLfloat d = sqrt( float(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]) );
		if( d==0.0 )
		{
			cerr<<"zero length vector"<<endl;
			return;
		}
		v[0] /= d;
		v[1] /= d;
		v[2] /= d;
	}

	void Terrain::normcrossprod(float v1[3], float v2[3], float out[3])
	{
		out[0] = v1[1]*v2[2] - v1[2]*v2[1];
		out[1] = v1[2]*v2[0] - v1[0]*v2[2];
		out[2] = v1[0]*v2[1] - v1[1]*v2[0];
		normalize( out );
	}

	//void Terrain::setStart()
	//{
	//	//startPath = true;
	//	creatingPath = true;
	//}

	//void Terrain::setEnd()
	//{
	//	endPath = true;
	//}

	//void Terrain::setDrawing()
	//{
	//	drawingPath = true;
	//}

	//void Terrain::setDrawingEnd()
	//{
	//	drawingPath = false;
	//}

	//void Terrain::setMinHeightFilter()
	//{
	//	adjustMinHeight = true;
	//}


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

		/*if(pinch != prevPinch){
			currDelay = 10;
		}*/
		if (currDelay > 0){
			currDelay--;
		}

		//Invoke handler for current mode
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

	//Sets the interaction mode based on command send from tablet
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
				setPoi = false;
				//roi = NULL;
				if(pinchHold && currDelay == 0)
				{
					state = PLACING;
					currDelay = 40;
				}
				break;
			case PLACING: //Currently drawing path, pinch to finish
				poiCube->show();
				setPoi = true;
				if(pinch && currDelay == 0){
					state = FINISH;
					currDelay = 40;
				}
				break;
			case FINISH: //Currently drawing path, pinch to finish
				setPoi = false;
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
				setRoi = false;
				//roi = NULL;
				if(pinchHold && currDelay == 0)
				{
					state = PLACING;
					currDelay = 40;
				}
				break;
			case PLACING: //Currently drawing path, pinch to finish
				roiCube->show();
				placeRoi = true;
				setRoi = true;
				if(pinch && currDelay == 0){
					state = SET_RADIUS;
					currDelay = 40;
				}
				break;
			case SET_RADIUS: //Currently drawing path, pinch to finish
				dispRoiRadius = true;
				placeRoi = false;
				setRoiRadius = true;
				if(pinch && currDelay == 0){
					state = FINISH;
					currDelay = 40;
				}
				break;
			case FINISH: //Start and End have both been placed, pinch to go back to starting state
				setRoi = false;
				setRoiRadius = false;
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
					startPath = true;
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
					endPath = true;
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
		startPath = false;
		endPath = false;

		//Reset direct freehand drawing states
		drawingPath = false;

		//Reset ROI drawing states
		placeRoi = false;
		setRoiRadius = false;

		//Reset POI drawing states
		setPoi = false;

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
			if(startPath){
				start = new MHTypes::Point3D(testPoint.x, testPoint.y, testPoint.z);
				ostringstream msg;
				msg << "startpoint," << (start->x*(1.0f/MAP_SCALE)) << "," << (start->z*(1.0f/MAP_SCALE)) << std::endl;
				send(msg.str().c_str());
				ostringstream ss;
				ss << "Start point created at: " << "X: " << testPoint.x << " Z: " << testPoint.z << std::endl;
				//OutputDebugString(ss.str().c_str());
			}
			if(endPath){
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
			if(placeRoi){
				roi = MHTypes::Point3D(testPoint.x, testPoint.y, testPoint.z);
				ostringstream msg;
				msg << "roi," << (roi.x*(1.0f/MAP_SCALE)) << "," << (roi.z*(1.0f/MAP_SCALE)) << ",0" << std::endl;
				send(msg.str().c_str());
				ostringstream ss;
				ss << "ROI created at: " << "X: " << testPoint.x << " Z: " << testPoint.z << std::endl;
				//OutputDebugString(ss.str().c_str());
			}
			if(setPoi){
				poi = MHTypes::Point3D(testPoint.x, testPoint.y, testPoint.z);
				ostringstream msg;
				msg << "poi," << (poi.x*(1.0f/MAP_SCALE)) << "," << (poi.z*(1.0f/MAP_SCALE)) << std::endl;
				send(msg.str().c_str());
				ostringstream ss;
				ss << "POI created at: " << "X: " << testPoint.x << " Z: " << testPoint.z << std::endl;
			}

			if(setRoiRadius){
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
		startPath = false;
		endPath = false;
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