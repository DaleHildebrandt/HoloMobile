//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       OpenGlScene.cpp
//  Content:    The OpenGlScene class for the zSpace OpenGl stereo samples.
//
//////////////////////////////////////////////////////////////////////////////
#include "GL\glew.h"
#include <GL\glew.h>
#include <GL\openglut.h>
#include "OpenGlScene.h"

#include <SOIL.h>

#include "ZSpace/Common/Math/AxisAlignedBox.h"
#include "ZSpace/Common/Math/MathConverterGl.h"
#include "ZSpace/Common/Math/Ray.h"

#include <iostream>
#include <Windows.h>

#include <stdio.h>
#include <assimp/Importer.hpp>

#pragma comment(lib,"assimp.lib")
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;

//////////////////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////////////////

GLfloat LIGHT_COLOR[]         = {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat LIGHT_POSITION[]      = {0.2f, 0.2f, 0.6f, 1.0f}; 
GLfloat AMBIENT_LIGHT_MODEL[] = {0.5f, 0.5f, 0.5f, 1.0f}; 
GLfloat AMBIENT_MATERIAL[]    = {1.0f, 1.0f, 1.0f, 1.0f};


//////////////////////////////////////////////////////////////////////////
// Static Member Initialization
//////////////////////////////////////////////////////////////////////////

bool OpenGlScene::m_isStylusEnabled = true;
zspace::common::Matrix4 OpenGlScene::m_stylusTransform = zspace::common::Matrix4::IDENTITY();
float OpenGlScene::m_cubeHalfSize = 0.015f;

float OpenGlScene::r = 1.0f;
float OpenGlScene::g = 1.0f;
float OpenGlScene::b = 1.0f;



zspace::common::Vector3 position = zspace::common::Vector3::ZERO();
zspace::common::Vector3 direction = zspace::common::Vector3::ZERO();
Terrain *terrain;
Cube *pointCube;




//////////////////////////////////////////////////////////////////////////
// Public Methods
//////////////////////////////////////////////////////////////////////////

void OpenGlScene::initialize()
{


  // Set up the scene's mono (non-stereoscopic) model-view matrix.
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //Position & orient the camera in the scene with gluLookAt

  //For HoloMobile
  gluLookAt(0.0f,  0.43f,   0.35f,   // Position
            0.0f,   0.0f,     0.0f,     // Lookat
            0.0f,   1.0f,     0.0f);    // Up 

  //For Terrain ScreenShots
  //gluLookAt(0.0001f,  0.4f,   0.0f,   // Position
  //          0.0f,   0.0f,     0.0f,     // Lookat
  //          0.0f,   1.0f,     0.0f);    // Up 

    /*gluLookAt(0.0f,   0.345f,   0.222f,   // Position
            0.0f,   0.0f,     0.0f,     // Lookat
            0.0f,   1.0f,     0.0f);    // Up */
  
  //What is this doing?
  glRotatef((GLfloat)-0.8, (GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);
  
  // Set material light reflection properties
  glMaterialfv(GL_FRONT, GL_AMBIENT, AMBIENT_MATERIAL);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, AMBIENT_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);

  // Enable a single OpenGl light.
  glLightfv(GL_LIGHT0, GL_POSITION, LIGHT_POSITION);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, LIGHT_COLOR);
  glLightfv(GL_LIGHT0, GL_AMBIENT, AMBIENT_LIGHT_MODEL);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AMBIENT_LIGHT_MODEL);

  // Enable lighting.
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glShadeModel(GL_SMOOTH);

  // Enable depth testing.
  glEnable(GL_DEPTH_TEST);

  // Set up textures.
  GLuint textureId;
  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

  // Load texture from bmp file. If load fails, try parent dir.
  if (!createTextureFromBmp("Infinite_Z_Logo.bmp"))
    createTextureFromBmp("..\\Infinite_Z_Logo.bmp");

  // Set up and enable texture mapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);


  //	// Load texture from bmp file. If load fails, try parent dir.
	 //   texture[0] = SOIL_load_OGL_texture
  //      (
  //      "C:\\images\\1.jpg",
  //      SOIL_LOAD_AUTO,
  //      SOIL_CREATE_NEW_ID,
  //      SOIL_FLAG_INVERT_Y
  //      );

		//texture[1] = SOIL_load_OGL_texture
  //      (
  //      "C:\\images\\2.jpg",
  //      SOIL_LOAD_AUTO,
  //      SOIL_CREATE_NEW_ID,
  //      SOIL_FLAG_INVERT_Y
  //      );

		//texture[2] = SOIL_load_OGL_texture
  //      (
  //      "C:\\images\\3.jpg",
  //      SOIL_LOAD_AUTO,
  //      SOIL_CREATE_NEW_ID,
  //      SOIL_FLAG_INVERT_Y
  //      );

		//texture[3] = SOIL_load_OGL_texture
  //      (
  //      "C:\\images\\4.jpg",
  //      SOIL_LOAD_AUTO,
  //      SOIL_CREATE_NEW_ID,
  //      SOIL_FLAG_INVERT_Y
  //      );

		//texture[4] = SOIL_load_OGL_texture
  //      (
  //      "C:\\images\\5.jpg",
  //      SOIL_LOAD_AUTO,
  //      SOIL_CREATE_NEW_ID,
  //      SOIL_FLAG_INVERT_Y
  //      );


  //// Set up and enable texture mapping
  //  glBindTexture(GL_TEXTURE_2D, texture[0]);
  //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

  
  
}


//Renders/Draws the scene
void OpenGlScene::render(bool msg, Calibrator* calibrator, Tablet* samsungTab, Finger* finger, Finger* thumb)
{

  // Clear color and depth buffers
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //glBindTexture(GL_TEXTURE_2D, texture[currTexture]);
  
  //renderNoTexCube();
  renderText();
  renderAxes();

  //calibrator->render();
  terrain = samsungTab->getTerrain();
  //Interaction testing
  terrain->testPinch(finger->getPosition(), thumb->getPosition());
  terrain->testIntersect(finger->getPosition());

  //Drawing mobile-mounted hologram
  samsungTab->renderHolo();
  finger->renderHolo();

  if (m_isStylusEnabled)
    renderStylus();
}


void OpenGlScene::setStylusVisualizationEnabled(bool isStylusEnabled)
{
  m_isStylusEnabled = isStylusEnabled;
}


void OpenGlScene::setStylusTransform(const zspace::common::Matrix4& stylusTransform)
{
  m_stylusTransform = stylusTransform;
}

void OpenGlScene::rotateScene(bool* previousButtonStates)
{
  glMatrixMode(GL_MODELVIEW);
  //if(!previousButtonStates)
  if(previousButtonStates && previousButtonStates[2])
	glRotatef((GLfloat)1.5, (GLfloat)0.0, (GLfloat)1.0, (GLfloat)0.0);
  if(previousButtonStates && previousButtonStates[1])
	glRotatef((GLfloat)1.5, (GLfloat)0.0, (GLfloat)-1.0, (GLfloat)0.0);
}



//////////////////////////////////////////////////////////////////////////
// Private Methods
//////////////////////////////////////////////////////////////////////////

// Fairly limited bmp texture loader. BMP files must be 
// single-plane 24-bit color format.
bool OpenGlScene::createTextureFromBmp(const char* filename)
{
  if (!filename)
    return false;

  FILE* fileHandle = NULL;
  fopen_s(&fileHandle, filename, "rb");

  if (!fileHandle)
    return false;

  // Read the bmp header.
  static const int bmpHeaderSize = 54;
  BYTE header[bmpHeaderSize];
  fread(header, bmpHeaderSize, 1, fileHandle);

  // Make sure we've got a bmp file.
  if ((header[0] != 'B' || header[1] != 'M'))
    goto fail;

  // Make sure have a V3 header.
  int dataOffset = *(int*)&header[10];
  if (dataOffset != bmpHeaderSize)
    goto fail;

  // Only handle single color planes.
  short numColorPlanes = *(short*)&header[26];
  if (numColorPlanes != 1)
    goto fail;

  // Only handle 24-bit color.
  short bitsPerPixel = *(short*)&header[28];
  if (bitsPerPixel != 24)
    goto fail;

  // Get image dimensions from header.
  int imageWidth  = *(int*)&header[18];
  int imageHeight = *(int*)&header[22];

  // Bmp is 4-byte aligned, compute row width.
  int rowWidth = (((imageWidth*3)+3)>>2)<<2;

  // Make sure we have the expected file size.
  int expectedFileSize = bmpHeaderSize + (rowWidth * imageHeight);
  int fileSize = *(int*)&header[2];
  if (fileSize != expectedFileSize)
    goto fail;

  // Allocate buffer.
  BYTE* data = (BYTE*)malloc(fileSize);
  if(!data)
    goto fail;

  // Read pixel data from bmp.
  fread(data, fileSize - bmpHeaderSize, 1, fileHandle);

  // Done with file.
  fclose(fileHandle);

  // Create a texture from the pixel data.
  glTexImage2D(GL_TEXTURE_2D, 0, 4, imageWidth, imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

  // glTexImage2D creates a texture from data[] so we can free it now.
  free(data);

  // Success.
  return true;

fail:
  fclose(fileHandle);
  return false;
}

//NON-FUNCTIONAL
void OpenGlScene::renderText(){

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glLineWidth(0.5f);
	//glScalef(1/100.0f, 1/100.0f, 1/100.0f);
	glScalef(1/5000.0f, 1/5000.0f, 1/50.0f);
	glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"HoloTerrain");
	glPopMatrix();

	//glColor3f(1.0f,1.0f,1.0f);

	//glPushMatrix();
	////glScalef(1/1000.0f, 1/1000.0f, 1/1000.0f);
	//glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"Hello World!");
	
	//glPopMatrix();
/*	glPushMatrix();
	glScalef(1/152.0, 1/152.0, 1/152.0);
	glutStrokeCharacter(GLUT_STROKE_ROMAN, 'a');
	glPopMatrix(); */

	//glPushMatrix();
	//glTranslatef(0.05f, 0.0f, 0.0f);
	//glScalef(0.003,0.003,1.5);
	//glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	//glutStrokeCharacter(GLUT_STROKE_ROMAN, 'B');
	//glPopMatrix(); 

}

void OpenGlScene::renderNoTexCube()
{
	glColor3f(1.0f,1.0f,1.0f);

  //############V I C O N############
  //glColor3f(r,g,b);
  //############V I C O N############
  // Render the cube
  glColor3f(1.0, 1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
   //glTranslatef(xTrans, yTrans, zTrans);
  glBegin(GL_QUADS);			
  const float halfSize = m_cubeHalfSize;

  //glTranslatef(xTrans/10.0f, yTrans/10.0f, zTrans/10.0f);
 

  // Side 1
  glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-halfSize, -halfSize, -halfSize);
	glVertex3f(-halfSize,  halfSize, -halfSize);
	glVertex3f( halfSize,  halfSize, -halfSize);        
	glVertex3f( halfSize, -halfSize, -halfSize);		

  // Side 2
  glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-halfSize,  halfSize, halfSize);
	glVertex3f(-halfSize, -halfSize, halfSize);        
	glVertex3f( halfSize, -halfSize, halfSize);
	glVertex3f( halfSize,  halfSize, halfSize);

  // Side 3
  glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-halfSize, halfSize, -halfSize);
	glVertex3f(-halfSize, halfSize,  halfSize);
	glVertex3f( halfSize, halfSize,  halfSize);        
	glVertex3f( halfSize, halfSize, -halfSize);

  // Side 4
  glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-halfSize, -halfSize,  halfSize);
	glVertex3f(-halfSize, -halfSize, -halfSize);
	glVertex3f( halfSize, -halfSize, -halfSize);
	glVertex3f( halfSize, -halfSize,  halfSize);

  // Side 5
  glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f( halfSize,  halfSize, -halfSize);
	glVertex3f( halfSize,  halfSize,  halfSize);        
	glVertex3f( halfSize, -halfSize,  halfSize);
	glVertex3f( halfSize, -halfSize, -halfSize);

  // Side 6
  glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-halfSize,  halfSize, -halfSize);
	glVertex3f(-halfSize, -halfSize, -halfSize);        
	glVertex3f(-halfSize, -halfSize,  halfSize);
	glVertex3f(-halfSize,  halfSize,  halfSize);

	

  glEnd();
  glPopMatrix();
	

}


void OpenGlScene::renderCube()
{
  //glColor3f(1.0f, 1.0f, 1.0f);

 /* if(white){
	  glColor3f(1.0f, 1.0f, 1.0f);
  }
  else{
	  glColor3f(1.0f, 0.0f, 0.0f);
  }*/
  //############V I C O N############
  //glColor3f(r,g,b);
  //############V I C O N############
  // Render the cube
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
   //glTranslatef(xTrans, yTrans, zTrans - 0.03f);
  //cubePos = zspace::common::Vector3(position.x + direction.x*0.2f, position.y + direction.y*0.2f, position.z + direction.z*0.2f);
	//glTranslatef(position.x + direction.x*0.2f, position.y + direction.y*0.2f, position.z + direction.z*0.2f);
	//glTranslatef(position.x, position.y, position.z);
	
  // Render the cube
  glBegin(GL_QUADS);			
  const float halfSize = m_cubeHalfSize;

  // Side 1
  /*glNormal3f(0.0f, 0.0f, -1.0f);
  glTexCoord2d(1.0,0.0); glVertex3f(-halfSize, -halfSize, -halfSize);
  glTexCoord2d(1.0,1.0); glVertex3f(-halfSize,  halfSize, -halfSize);
  glTexCoord2d(0.0,1.0); glVertex3f( halfSize,  halfSize, -halfSize);        
  glTexCoord2d(0.0,0.0); glVertex3f( halfSize, -halfSize, -halfSize);		

  // Side 2
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2d(0.0,1.0); glVertex3f(-halfSize,  halfSize, halfSize);
  glTexCoord2d(0.0,0.0); glVertex3f(-halfSize, -halfSize, halfSize);        
  glTexCoord2d(1.0,0.0); glVertex3f( halfSize, -halfSize, halfSize);
  glTexCoord2d(1.0,1.0); glVertex3f( halfSize,  halfSize, halfSize);

  // Side 3
  glNormal3f(0.0f, 1.0f, 0.0f);
  glTexCoord2d(0.0,1.0); glVertex3f(-halfSize, halfSize, -halfSize);
  glTexCoord2d(0.0,0.0); glVertex3f(-halfSize, halfSize,  halfSize);
  glTexCoord2d(1.0,0.0); glVertex3f( halfSize, halfSize,  halfSize);        
  glTexCoord2d(1.0,1.0); glVertex3f( halfSize, halfSize, -halfSize);

  // Side 4
  glNormal3f(0.0f, -1.0f, 0.0f);
  glTexCoord2d(0.0,1.0); glVertex3f(-halfSize, -halfSize,  halfSize);
  glTexCoord2d(0.0,0.0); glVertex3f(-halfSize, -halfSize, -halfSize);
  glTexCoord2d(1.0,0.0); glVertex3f( halfSize, -halfSize, -halfSize);
  glTexCoord2d(1.0,1.0); glVertex3f( halfSize, -halfSize,  halfSize);

  // Side 5
  glNormal3f(1.0f, 0.0f, 0.0f);
  glTexCoord2d(1.0,1.0); glVertex3f( halfSize,  halfSize, -halfSize);
  glTexCoord2d(0.0,1.0); glVertex3f( halfSize,  halfSize,  halfSize);        
  glTexCoord2d(0.0,0.0); glVertex3f( halfSize, -halfSize,  halfSize);
  glTexCoord2d(1.0,0.0); glVertex3f( halfSize, -halfSize, -halfSize);

  // Side 6
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glTexCoord2d(0.0,1.0); glVertex3f(-halfSize,  halfSize, -halfSize);
  glTexCoord2d(0.0,0.0); glVertex3f(-halfSize, -halfSize, -halfSize);        
  glTexCoord2d(1.0,0.0); glVertex3f(-halfSize, -halfSize,  halfSize);
  glTexCoord2d(1.0,1.0); glVertex3f(-halfSize,  halfSize,  halfSize);*/


  glEnd();
  glPopMatrix();
	

}

//Draws the axes for the coordinate system for drawing
void OpenGlScene::renderAxes()
{
	glLineWidth(5);
	glBegin(GL_LINES);
	//x-axis
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.1f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	//y-axis
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.1f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);

	//z-axis
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.1f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();
	glLineWidth(1);
}


void OpenGlScene::renderStylus()
{
  float stylusLength = 0.15f;
  zspace::common::Vector4 color = zspace::common::Vector4::WHITE();

  // Create an intersection ray based on the position and direction
  // of the stylus world pose.
  position = m_stylusTransform.getTrans();
  direction = zspace::common::Vector3(-m_stylusTransform[0][2], -m_stylusTransform[1][2], -m_stylusTransform[2][2]);
  /*string str;
  ostringstream ostr;
  ostr << "5tylusWorldDir: " << -m_stylusTransform[0][2] << ", " << -m_stylusTransform[1][2] << ", " << -m_stylusTransform[2][2] <<endl;
  str= ostr.str();
  OutputDebugString(str.c_str());*/

  direction.normalise();
  zspace::common::Ray ray = zspace::common::Ray(position, direction);

  // In the OpenGl stereo samples, it may look like the cube is rotating, 
  // but we are actually orbiting the monoscopic camera about 
  // the cube.  So, we can create an axis aligned box (AABB) based 
  // on the half extents of the cube.  An AABB can be used to represent
  // the cube because the cube is axis-aligned and never has its orientation 
  // modified.  
  zspace::common::AxisAlignedBox box = 
    zspace::common::AxisAlignedBox(-m_cubeHalfSize, -m_cubeHalfSize, -m_cubeHalfSize, m_cubeHalfSize, m_cubeHalfSize, m_cubeHalfSize);

  // Perform a ray vs. AABB intersection query to determine if the stylus
  // is intersecting with the cube.
  std::pair<bool, float> result = ray.intersects(box);

  // If the stylus intersected the cube, change the stylus color to red
  // and set its length to the intersection distance.
  if (result.first == true)
  {
    // Since the intersection uses an infinite ray, we need to check that
    // the intersection distance is less than the stylus beam's original
    // length for it to be considered a valid intersection.
    if (result.second < stylusLength)
    {
      stylusLength = result.second;
      color = zspace::common::Vector4::RED();
    }
  }

  // Render the stylus beam.
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Convert the stylus world pose to OpenGl matrix format and apply it to
  // the virtual stylus beam's transform.
  GLfloat stylusTransformGl[16];
  zspace::common::MathConverterGl::convertMatrix4ToMatrixGl(m_stylusTransform, stylusTransformGl);
  glMultMatrixf(stylusTransformGl);

  glColor3f(color.x, color.y, color.z);

  //Disable Lines
  glBegin(GL_LINES);
 /* glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, -stylusLength);*/
  glEnd();

  glPopMatrix();
}
