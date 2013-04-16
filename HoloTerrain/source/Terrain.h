#ifndef TERRAIN_H
#define TERRAIN_H

// Author:		Vahe Karamian
// Date:			03/10/2006
// Quick Note:	OpenGL Library Fundamentals
// Filename:	vkgllib.h

/* ********************************************************************
   *                 SOMETHINGS TO KEEP IN MIND                       *
	*                                                                  *
	* Suffix		Data Type	Typical C++ Type		 OpenGL Type Name     *
	* ======   =========   ================		 ================     *
	* b		   08-bit int	signed char				   GLbyte             *
	* s			16-bit int	short                   GLshort            *
	* i			32-bit int	int or long					GLint, GLsizei		 *
	* f			32-bit flp	float							GLfloat, GLclampf  *
	* d			64-bit flp	double						GLdouble, GLclampd *
	* ub			08-bit unc	unsigned char				GLubyte, GLboolean *
	* us			16-bit unn	unsigned short			   GLushort				 *
	* ui			32-bit unn	unsigned int or long    GLunint, GLenum,   *
	*                                               GLbitfield         *
	******************************************************************** */

// We don't need to include the gl.h and glu.h files, because glut.h does
// it for us!

#include "Cube.h"
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>


#include "Color3f.h"


//#include <GL\glut.h>


#include "MathHelper.hpp"
#include "TabletServer.h"


using namespace std;



class Terrain{

	public:
		Terrain(TabletServer *server);
		~Terrain(void);
		void init(void);
		void render(void);
		void setPosition(MHTypes::Point3D point);
		void testIntersect(MHTypes::Point3D point);
		void setRotation(MHTypes::Quaternion rot);
		void setRotMat(float* rotMat);

		void testPinch(MHTypes::Point3D finger, MHTypes::Point3D thumb);
		void send(const char* str);
		bool testInRoi(int roiX, int roiZ, int x, int z, int radius);


		void setInteractionMode(int mode);

	private:
		void initTerrain(void);
		void initClipPlanes();
		void normalize(float v[3]);
		void normcrossprod(float v1[3], float v2[3], float out[3]);
		unsigned char *loadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
		bool loadTextures();
		int round(float number);
		int calcDist(MHTypes::Point3D p1, MHTypes::Point3D p2);
		void disableClipPlanes();
		void setVertexColor(int x, int z);


		//Manages the state machines for each interaction type
		void handleEndpointDrawing(int &state );
		void handleDirectDrawing(int &state);
		void handleROIPlacement(int &state);
		void handlePOIPlacement(int &state);
		void handleHeightFiltering(int &state);
		void handleHeightSelection(int &state);

		void handleMessages();
		void resetInteractionStates();

		const int DEFAULT_MODE;
		const int ROI_MODE;
		const int POI_MODE;
		const int ENDPOINT_DRAWING_MODE;
		const int DIRECT_DRAWING_MODE;
		const int HEIGHT_FILTERING_MODE;
		const int HEIGHT_SELECT_MODE;

		GLdouble botPlaneEq[4]; //4 clipping planes
		GLdouble topPlaneEq[4];
		GLdouble ssBotPlaneEq[4]; //bottom clipping plane for taking cross-section screenshots;
		GLdouble ssTopPlaneEq[4]; //top clipping plane for taking cross-section screenshots;


		MHTypes::Point3D testPoint;
		
		MHTypes::Point3D *start; //Position of starting point of path
		MHTypes::Point3D *end; //Position of ending point of a path
		MHTypes::Point3D roi; //Position of the center of a region of interest
		MHTypes::Point3D poi; //Position of a point of interest;
		MHTypes::Point3D curr; //Current location of finger (not used?)

		MHTypes::Point3D *origStart;// ??not used?
		MHTypes::Point3D *origEnd; //??not used?
		bool pinch; //Is a pinching gesture currently happening?
		bool pinchHold;
		bool oldPinchHold;
		bool settingStartPath; //Enable user to position start point of path if true;
		bool settingEndPath; //Enable user to position end point of path if true;
		bool drawingPath; //Enables freehand drawing of path 
		bool settingRoi; //Enables user to position ROI
		bool settingRoiRadius; //Enables user to set radius of a placed ROI
		bool enableBotClip; //Enable bottom clipping plane
		bool enableTopClip; //Enable top clipping plane
		bool enableTopClipVis;
		bool enableBotClipVis;
		bool enableHeightVis;
		bool setHeight;
		bool roiCenterPlaced;
		bool settingPoi;
		bool dispRoiRadius;
		bool isFirst;

		bool setFirstClip; //User can adjust position of bottom clipping plane.
		bool setSecondClip; //User can adjust position of top clipping plane.
		bool enableFirstClip; //Enable bottom clipping plane
		bool enableSecondClip; //Enable top clipping plane
		bool enableSecondClipVis;
		bool enableFirstClipVis;
		float clipPlaneFirstVisOffset;
		float clipPlaneSecondVisOffset;
		float firstClipY;
		float secondClipY;

		float clipPlaneVisOffset;
		float cameraX, cameraY, cameraZ;	   // camera coordinates
		float lookX, lookY, lookZ;			   // camera look-at coordinates
		MHTypes::Point3D *loc; //location of terrain relative to end of tablet
		MHTypes::EulerAngle eulerAngle; //rotation of terrain (not used?)
		float rotMatrix[16]; //roation matrix from vicon quat
		float rotMatrixY[16]; //Roation matrix around Y axis
		float invRotMatrix[16]; //Inverse matrix of roMatrix, performs opposite transformation
		float glRotMat[16];
		bool **freeformPath; //Determines which polygons on terrain are to appear drawn on during freehand path interaction.
		bool inRoi; //Is something within a ROI?
		int radius; //radius of our ROI

		int mode; //Keeps track of the current interaction mode
		int state; //Keeps track of a specific state in the current interaction mode.
	
		float topClipY; //Height value of the top clipping plane
		float botClipY; //Height value of the bottom clipping plane


		MHTypes::Quaternion quat; //quaternion specifying rotation of object

		Color3f *pathColor;

		float radians;		   // camera angle in radians

		float maxX, maxY, maxZ;
		float minX, minY, minZ;
		float maxDisplayX, maxDisplayY, maxDisplayZ;
		float minDisplayX, minDisplayY, minDisplayZ;
		bool minSet, maxSet;
		bool intersecting;

		//Visual Objects
		MHTypes::Point3D selectorRay; //Indicator for selection of region
		MHTypes::Point3D originalRay; //Original, untransformed version of selection ray
		MHTypes::Point3D originalRayExtend;

		MHTypes::Point3D currFingerPos;
		Cube *roiCube;
		Cube *poiCube;
		Cube *startCube;
		Cube *endCube;
		Cube *lightingCube;

		Cube *fingerCube;

		TabletServer *server;
};


#endif TERRAIN_H



