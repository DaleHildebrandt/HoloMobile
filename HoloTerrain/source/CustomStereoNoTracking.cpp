//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       StereoNoTracking.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include <Windows.h>

#include <iostream>
#include <iostream>  
#include <boost/thread.hpp>  
#include <boost/date_time.hpp> 
#include "SocketCom.h"
#include "CustomStereoNoTracking.h"
using namespace std;
	
#include "Model/Calibrator.h"
#include "Model/Tablet.h"
#include "Model/Finger.h"

#include "OpenGlScene.h"
#include "OpenGlWindow.h"
#include "ObjectTracker.h"

#include "ZSpace/Common/Math/MathConverterGl.h"
#include "ZSpace/Common/Math/Matrix4.h"

#include "ZSpace/Stereo/StereoFrustum.h"
#include "ZSpace/Stereo/StereoLeftRightDetect.h"
#include "ZSpace/Stereo/StereoViewport.h"
#include "ZSpace/Stereo/StereoWindow.h"

#include "ZSpace/Tracker/TrackerSystem.h"
#include "ZSpace/Tracker/TrackerTarget.h"

#include "ZSpace/Tracker/ITrackerButtonCapability.h"

#include "Input/ViconDataStreamClient.h"


#include "SDL\SDL.h"

using namespace std;


//////////////////////////////////////////////////////////////////////////
// Globals
//////////////////////////////////////////////////////////////////////////

OpenGlWindow* g_applicationWindow = NULL;


zspace::stereo::StereoWindow* g_stereoWindow     = NULL;
zspace::stereo::StereoViewport* g_stereoViewport = NULL;
zspace::stereo::StereoFrustum* g_stereoFrustum   = NULL;

Tablet *samsungTab; //Tracks the samsung_tablet Vicon object, manages any attached hologram objects
Finger *finger; //Tracks index finger
Finger *thumb; //Tracks thumb

ObjectTracker* objectTracker = NULL; //Handles tracking of head and stylus
ViconDataStreamClient *viconClient = NULL; //Handles reading data from the Vicon system
Calibrator* calibrator = NULL; //Handles calibration between zSpace and Vicon coordinate systems

bool* buttonStates = NULL; //Records whether or not each button on the zspace stylus is pressed
bool msg_recv = false; //Is a message received through socket connection? (ie. from tablet)


//////////////////////////////////////////////////////////////////////////
// Function Prototypes
//////////////////////////////////////////////////////////////////////////

void zSpaceInitialize();
void viconInit();
void zSpaceUpdate();
void zSpaceShutdown();
void renderFrame();
void renderScene(zspace::stereo::StereoFrustum::StereoEye eye);
void setupViewMatrix(zspace::stereo::StereoFrustum::StereoEye eye);
void setupProjectionMatrix(zspace::stereo::StereoFrustum::StereoEye eye);
void convertPoseToWorldSpace(const zspace::common::Matrix4& pose, zspace::common::Matrix4& worldPose);
void calibratorInit();
void setMessageRecv();


//////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
                   HINSTANCE	hPrevInstance,	// Previous Instance
                   LPSTR		  lpCmdLine,			// Command Line Parameters
                   int			  nCmdShow)				// Window Show State
{

  OutputDebugString("***STARTING PROGRAM***\n\n");
  g_applicationWindow = new OpenGlWindow("Stereo No Tracking - OpenGl", 0, 0, 1024, 768);

  

  //Initialization
  zSpaceInitialize();
  viconInit();
  OpenGlScene::initialize();
  calibratorInit();

  MSG msg;

  //## Main Loop ##
  while (TRUE)
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }

	//Tests for keyboard input using GetAsyncKeyState([key-code])
    // If 'escape' was pressed, exit the application.
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
      PostMessage(g_applicationWindow->getWindowHandle(), WM_DESTROY, 0, 0);
	   
	//Press the E-Key to take a screenshot 
	if (GetAsyncKeyState(0x45)) //E-Key Test
	{
		SDL_Surface * image = SDL_CreateRGBSurface(SDL_SWSURFACE, g_applicationWindow->getWidth(), g_applicationWindow->getHeight(), 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
 
		glReadBuffer(GL_FRONT);
		glReadPixels(0, 0, g_applicationWindow->getWidth(), g_applicationWindow->getHeight(), GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
 
		SDL_SaveBMP(image, "level_.bmp");
		SDL_FreeSurface(image);

	}

	// If 'cntrl + c' pressed, begin calibration
	if (GetAsyncKeyState(VK_CONTROL) & GetAsyncKeyState(0x43) & 0x8000)
      calibrator->startViconCalibration();

    // Render the current frame.
    zSpaceUpdate(); //main loop
    renderFrame();
  }

  zSpaceShutdown();

  if (g_applicationWindow)
    delete g_applicationWindow;

  return 0;
}

//Sets up calibration system, which converts Vicon coordinates to
// zspace coordinates.
void calibratorInit()
{
	calibrator = new Calibrator();
	calibrator->init();

	samsungTab = new Tablet(*calibrator);

	finger = new Finger(*calibrator);
	thumb = new Finger(*calibrator);
}

//////////////////////////////////////////////////////////////////////////
// Function Definitions
//////////////////////////////////////////////////////////////////////////


void zSpaceInitialize()
{
  // zSpace Stereo Initialization.
  g_stereoWindow   = new zspace::stereo::StereoWindow();
  g_stereoViewport = new zspace::stereo::StereoViewport();

  g_stereoWindow->addStereoViewport(g_stereoViewport);
  g_stereoViewport->setUsingWindowSize(true);

  g_stereoFrustum = g_stereoViewport->getStereoFrustum();
  //g_stereoFrustum->setWorldScale(8.0f);

  // Initialize zSpace Left/Right Detection.
  zspace::stereo::StereoLeftRightDetect::initialize(g_stereoWindow, zspace::stereo::StereoLeftRightDetect::WINDOW_TYPE_GL);

  //Create Tracker for head movement and stylus movement + input
  objectTracker = new ObjectTracker(g_stereoFrustum, g_stereoWindow);
}

//Sets up a thread for recieving Vicon data
void viconInit(){

	HANDLE   ht1;
    unsigned  uiThread1ID;

    viconClient = new ViconDataStreamClient();
    viconClient->createViconClient();

    // Start thread for listening to incoming 3D Data from Vicon
    ht1 = (HANDLE)_beginthreadex( NULL, // security
        0,             // stack size
        ViconDataStreamClient::ListenViconWrapper,// entry-point-function
        viconClient,           // arg list holding the "this" pointer
        //CREATE_SUSPENDED, // so we can later call ResumeThread()
        0,
        &uiThread1ID );
	
}

void zSpaceUpdate()
{

  // Update zSpace Left/Right Frame Detection.
  zspace::stereo::StereoLeftRightDetect::update();

  // Check if the application window has moved.  If so, update the 
  // StereoWindow's position to reflect the application window's new position.
  int x = g_applicationWindow->getX();
  int y = g_applicationWindow->getY();

  if (g_stereoWindow->getX() != x || g_stereoWindow->getY() != y)
    g_stereoWindow->move(x, y);

  // Check if the application window has been resized.  If so, update
  // the zSpace StereoWindow's dimensions to reflect the application 
  // window's new size.
  int width  = g_applicationWindow->getWidth();
  int height = g_applicationWindow->getHeight();

  if (g_stereoWindow->getWidth() != width || g_stereoWindow->getHeight() != height)
  {
    g_stereoWindow->resize(width, height);
    glViewport(0, 0, width, height);
  }

  //Update head and stylus tracking
  objectTracker->UpdateHead();
  objectTracker->UpdateStylus(buttonStates);

}


void zSpaceShutdown()
{
  // Shutdown zSpace Left/Right Frame Detection.
  zspace::stereo::StereoLeftRightDetect::shutdown();

  // Remove references to any zSpace objects that have been allocated.
  if (g_stereoWindow)
    g_stereoWindow->removeReference();

  if (g_stereoViewport)
    g_stereoViewport->removeReference();

  if(objectTracker)
	  delete objectTracker;
}


void renderFrame()
{
	//const std::string TABLET_NAME = "samsung_tablet";
	const std::string FINGER_NAME = "terrain_index2";
	const std::string THUMB_NAME = "terrain_thumb";
	const std::string TABLET_NAME = "samsung_tablet";

	//Update tablet information
	viconClient->getTabletPosition(TABLET_NAME, *samsungTab);
	viconClient->getTabletRotation(TABLET_NAME, *samsungTab);

	viconClient->getFingerPosition(FINGER_NAME, *finger);\
	viconClient->getFingerPosition(THUMB_NAME, *thumb);

	calibrator->setWandViconPosition(samsungTab->getCenter(), samsungTab->getTopRight(), samsungTab->getBottomRight());
	calibrator->setWandViconRotation(samsungTab->getRotation());

	//Continue calibration process if not yet calibrated
	if(!calibrator->isDisplayCalibrated()){
		MHTypes::Point3D pos = viconClient->getObjectPosition("zspace_stylus4");
		calibrator->setWandViconPosition(MHTypes::Point3D(pos.x, pos.y, pos.z));
		calibrator->setWandVisBoxPosition(objectTracker->getStylusPos(), objectTracker->getStylusTipPos());
		calibrator->updateCalibrationState();
	}

  // Set the application window's rendering context as the current rendering context.
  wglMakeCurrent(g_applicationWindow->getDeviceContext(), g_applicationWindow->getRenderingContext());

  // Orbit the monoscopic camera about the center of the scene.
  OpenGlScene::rotateScene(buttonStates);


  // Render the scene for each eye.
  //renderScene(zspace::stereo::StereoFrustum::STEREO_EYE_LEFT);
  renderScene(zspace::stereo::StereoFrustum::STEREO_EYE_RIGHT);

  // Flush the render buffers.
  SwapBuffers(g_applicationWindow->getDeviceContext());

}


void renderScene(zspace::stereo::StereoFrustum::StereoEye eye)
{

  // Push the stereo view and projection matrices onto the OpenGl matrix stack
  // so that we can pop them off after we're done rendering the scene
  // for a specified eye.  This will allow us to restore the mono (non-
  // stereoscopic) model-view and projection matrices.
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();

  // Set up the view and projection matrices for the specified eye.
  setupViewMatrix(eye);
  setupProjectionMatrix(eye);

  // Select appropriate back buffer to render to based on the specified eye.
  if (eye == zspace::stereo::StereoFrustum::STEREO_EYE_LEFT)
    glDrawBuffer(GL_BACK_LEFT);
  else if (eye == zspace::stereo::StereoFrustum::STEREO_EYE_RIGHT)
    glDrawBuffer(GL_BACK_RIGHT);

  // ### Render the scene. ###
  OpenGlScene::render(msg_recv, calibrator, samsungTab, finger, thumb);

  msg_recv = false;
  // Restore the mono (non-stereoscopic) model-view and projection matrices.
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}



//Sets up the view matrix for stereoscopic rendering
void setupViewMatrix(zspace::stereo::StereoFrustum::StereoEye eye)
{
  GLfloat viewMatrixGl[16];
  GLfloat monoModelViewGl[16];

  // Get the view matrix from the zSpace StereoFrustum for a specified eye
  // and convert it into OpenGl matrix format.
  zspace::common::Matrix4 viewMatrix;
  g_stereoFrustum->getViewMatrix(eye, viewMatrix);
  zspace::common::MathConverterGl::convertMatrix4ToMatrixGl(viewMatrix, viewMatrixGl);

  // Set the OpenGl MatrixMode to GL_MODELVIEW and get the mono (non-stereoscopic) 
  // model-view matrix.
  glMatrixMode(GL_MODELVIEW);
  glGetFloatv(GL_MODELVIEW_MATRIX, monoModelViewGl);

  // Set the model-view matrix for the specified eye and multiply it by the
  // mono (non-stereoscopic) model-view matrix.  This must be done because the
  // eye's view matrix only contains the eye offset (relative to the center of
  // the viewer's head) plus any rotation required for off-axis projection.
  glLoadMatrixf(viewMatrixGl);
  glMultMatrixf(monoModelViewGl);
}



//Sets up the projection matrix for stereoscopic rendering
void setupProjectionMatrix(zspace::stereo::StereoFrustum::StereoEye eye)
{
  GLfloat projectionMatrixGl[16];

  // Get the projection matrix from the zSpace StereoFrustum for a specified eye
  // and convert it into OpenGl matrix format.
  zspace::common::Matrix4 projectionMatrix;
  g_stereoFrustum->getProjectionMatrix(eye, projectionMatrix);
  zspace::common::MathConverterGl::convertMatrix4ToMatrixGl(projectionMatrix, projectionMatrixGl);

  // Set OpenGl MatrixMode to GL_PROJECTION and set the projection matrix.
  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(projectionMatrixGl);
}

void setMessageRecv(){
	msg_recv = true;
}
