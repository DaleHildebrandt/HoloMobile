#include "HoloApp.hpp"
#include "MathHelper.hpp"
#include "InputHandler.hpp"
#include "Navigation.hpp"



using namespace std;
using namespace EbonGL;
using namespace MHTypes;

#ifdef EG_GLUT_MODE
	HoloApp *HoloApp::instance = NULL;
#endif
//#ifdef EG_ZSPACE_MODE
//	zspace::stereo::StereoWindow* g_stereoWindow     = NULL;
//	zspace::stereo::StereoViewport* g_stereoViewport = NULL;
//	zspace::stereo::StereoFrustum* g_stereoFrustum   = NULL;
//
//	ObjectTracker* objectTracker = NULL; //Handles tracking of head and stylus
//#endif

//########################################################
//############### CONSTRUCTOR, DESTRUCTOR, ###############
//########################################################

HoloApp::HoloApp(void *newKernel,
                 unsigned int newNumContexts)
{
    //### Initialize member variables ###
	contextIDStamper = 0;
    firstLogicUpdate = true;
	isRunning = true;
    glEngine = NULL;
    numContexts = newNumContexts;

    //### Initialize EbonGL ###
    ebonUtils.initEbonGL(numContexts);
    EbonGL::EG_Sphere *testSphere = new EbonGL::EG_Sphere;

	#ifdef EG_GLUT_MODE
        glutTimer = new EG_TimeKeeper;
        windowWidth = 0;
		windowHeight = 0;
        init();
	#else
		jugglerKernel = static_cast<vrj::Kernel*>(newKernel);
		if (jugglerKernel == NULL)
		{
			EG_Utils::writeLog(EG_String("HoloApp::EbonApp : kernel == NULL"));
			throw 1;
		}//if jugglerKernel

        deviceInput = new InputHandler;
		navModule = NULL;
	#endif    
	#ifdef EG_ZSPACE_MODE
	  // zSpace Stereo Initialization.
	  g_stereoWindow   = new zspace::stereo::StereoWindow();
	  g_stereoViewport = new zspace::stereo::StereoViewport();

	  g_stereoWindow->addStereoViewport(g_stereoViewport);
	  g_stereoViewport->setUsingWindowSize(true);
	  g_stereoFrustum = g_stereoViewport->getStereoFrustum();
	  g_stereoFrustum->setWorldScale(WORLD_SCALE);


	  //Create Tracker for head movement and stylus movement + input
	  objectTracker = new ObjectTracker(g_stereoFrustum, g_stereoWindow);

	  // Initialize zSpace Left/Right Detection.
	  zspace::stereo::StereoLeftRightDetect::initialize(g_stereoWindow, zspace::stereo::StereoLeftRightDetect::WINDOW_TYPE_GL);
	#endif
		
}//constructor

HoloApp::~HoloApp(void)
{

	#ifdef EG_GLUT_MODE
        delete glutTimer;
        glutTimer = NULL;
    #else
		delete navModule;
		navModule = NULL;

        delete deviceInput;
        deviceInput = NULL;
	#endif

	#ifdef EG_ZSPACE_MODE
			g_stereoWindow->removeReference();
			g_stereoViewport->removeReference();
			g_stereoFrustum->removeReference();
			if(objectTracker)
				delete objectTracker;
	#endif

    delete glEngine;
    glEngine = NULL;
}//destructor

//########################################################
//############### INITIALIZATION FUNCTIONS ###############
//########################################################

void HoloApp::init(void)
{    
    glEngine = new EbonGL::EG_Engine;

	#ifdef EG_GLUT_MODE
		if (instance == NULL)
			instance = this;
		else
		{
			EG_Utils::writeLog(EG_String("HoloApp::init : Can't have more than one instance of EbonApp"));
			throw 1;
		}//else instance

		//we don't want to pass any parameters to glut so pass empty values
		int argc = 0;
		glutInit(&argc, NULL);

		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO);
		glutInitWindowSize(800, 600);
		resize(800, 600);
		glutCreateWindow("EbonApp Window");

		//Function callbacks with wrapper functions
		glutDisplayFunc(displayWrapper);
		glutReshapeFunc(resizeWrapper);
        glutKeyboardFunc(keyboardWrapper);
        glutMouseFunc(mouseWrapper);
        glutMotionFunc(motionWrapper);
	#else
		//### Initialize input devices ###
		deviceInput->initialize();

		//### Initialize the navigation module ###
		navModule = new Navigation(glEngine, deviceInput);
		navModule->setVelocity(0.05);

        //### Call child event handler ###
        initApp();
	#endif
}//init

void HoloApp::initApp(void)
{
}//initApp

void HoloApp::apiInit(void)
{
}//apiInit

void HoloApp::contextInit(void)
{
    contextInitMutex.lock();
    
	#ifndef EG_GLUT_MODE
		(*myContextID) = contextIDStamper;
	#endif

	//### Initialize the OpenGL context ###
	glEngine->requestLock();

	glEngine->contextInit();
    glEngine->setAmbientLight(EG_Color(1, 1, 1, 1));

	glEngine->releaseLock();

	contextInitApp(contextIDStamper);
    contextIDStamper++;
    
    contextInitMutex.unlock();
}//contextInit

void HoloApp::contextInitApp(unsigned int myContext)
{
}//contextInitApp

void HoloApp::postInitialization(void)
{
}//postInitialization

//#############################################
//############### PROGRAM LOGIC ###############
//#############################################

void HoloApp::run(void)
{
	#ifdef EG_GLUT_MODE
		initApp();
		glutTimer->updateTime();
        glutTimerFunc(ENGINE_INTERVAL, timerWrapper, 0);    
		glutMainLoop();
	#else
		EG_TimeKeeper engineTimer;//controls the speed at which we apply logic updates

		if (!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
			EG_Utils::writeLog(EG_String("Warning: Unable to set process to high priority."));

		engineTimer.updateTime();

		while (isRunning)
		{
			#ifndef EG_GLUT_MODE
				if (!jugglerKernel->isRunning())
					isRunning = false;
			#endif

			if (engineTimer.timeElapsed() > ENGINE_INTERVAL)
			{
				engineTimer.addTime(ENGINE_INTERVAL);
				if (engineTimer.timeElapsed() > ENGINE_INTERVAL * 10)
					engineTimer.updateTime();

				if (contextIDStamper >= numContexts)
					updateLogic();
			}//if timeElapsed()
			else
				EG_TimeKeeper::sleep(1);
		}//while runProgram
	#endif
}//run

void HoloApp::timerCallback(void)
{
    #ifdef EG_GLUT_MODE
        while ((isRunning) && (glutTimer->timeElapsed() > ENGINE_INTERVAL))
        {
	        glutTimer->addTime(ENGINE_INTERVAL);
	        if (glutTimer->timeElapsed() > ENGINE_INTERVAL * 10)
		        glutTimer->updateTime();
		
	        updateLogic();
        }//while isRunning && timeElapsed()    
    #endif
}//timerCallback

void HoloApp::stopApp(void)
{
	isRunning = false;
}//stopApp

void HoloApp::updateLogic(void)
{
    MHTypes::EulerAngle cameraOrient;//the camera's orientation in the scene
    MHTypes::EulerAngle wandOrient;//the wand's orientation relative to cameraOrient
    MHTypes::EulerAngle absWandOrient;//the wand's absolute position (relative to the screen)
    MHTypes::Point3D wandPos;//the wand's position relative to the camera position
    vector<bool> buttonPressed;//button information
    vector<bool> buttonReleased;
    vector<bool> buttonDown;

    //### Perform post-initialization ###
    if (firstLogicUpdate)
    {
        #ifdef EG_GLUT_MODE
            contextInit();
        #endif

        firstLogicUpdate = false;
        postInitialization();
    }//if firstLogicUpdate

    buttonPressed.resize(NUM_DEVICE_BUTTONS, false);
    buttonReleased.resize(NUM_DEVICE_BUTTONS, false);
    buttonDown.resize(NUM_DEVICE_BUTTONS, false);

    //### Obtain device information ###
	#ifndef EG_GLUT_MODE
		deviceInput->getData(wandOrient, wandPos, cameraOrient, absWandOrient,
                             buttonDown, buttonPressed, buttonReleased);
	#endif

#ifdef EG_ZSPACE_MODE

  // Update zSpace Left/Right Frame Detection.
  zspace::stereo::StereoLeftRightDetect::update();

  // Check if the application window has moved.  If so, update the 
  // StereoWindow's position to reflect the application window's new position.
  int x = glutGet((GLenum)GLUT_WINDOW_X);
  int y = glutGet((GLenum)GLUT_WINDOW_Y);
  //int x = g_applicationWindow->getX();
  //int y = g_applicationWindow->getY();

  if (g_stereoWindow->getX() != x || g_stereoWindow->getY() != y)
    g_stereoWindow->move(x, y);



  // Check if the application window has been resized.  If so, update
  // the zSpace StereoWindow's dimensions to reflect the application 
  // window's new size.
  int width = glutGet((GLenum)GLUT_WINDOW_WIDTH);
  int height = glutGet((GLenum)GLUT_WINDOW_HEIGHT);
  //int width  = g_applicationWindow->getWidth();
  //int height = g_applicationWindow->getHeight();

  if (g_stereoWindow->getWidth() != width || g_stereoWindow->getHeight() != height)
  {
    g_stereoWindow->resize(width, height);
    glViewport(0, 0, width, height);
  }

  //Update head and stylus tracking
  objectTracker->UpdateHead();
  objectTracker->updateStylus(m_stylusTransform);

#endif

	mainLoop(cameraOrient, wandOrient, absWandOrient, wandPos,
             buttonPressed, buttonReleased, buttonDown);

}//updateLogic

void HoloApp::mainLoop(MHTypes::EulerAngle cameraOrient,
					   MHTypes::EulerAngle wandOrient,
					   MHTypes::EulerAngle absWandOrient,
					   MHTypes::Point3D wandPos,
					   vector<bool> buttonPressed,
					   vector<bool> buttonReleased,
					   vector<bool> buttonDown)
{

}//mainLoop

//#######################################
//############### DRAWING ###############
//#######################################

void HoloApp::bufferPreDraw(void)
{
    glEngine->clearBuffer();
}//bufferPreDraw

void HoloApp::contextPreDraw(void)
{

}//contextPreDraw

void HoloApp::preFrame(void)
{
	#ifndef EG_GLUT_MODE
		deviceInput->update();
	#endif
}//preFrame

void HoloApp::intraFrame(void)
{
}//intraFrame

void HoloApp::postFrame(void)
{
}//postFrame

void HoloApp::draw(void)
{
	#ifndef EG_GLUT_MODE
		glEngine->draw(*myContextID);
	#else
		EG_Utils::writeLog(EG_String("HoloApp::draw(): called in glut mode! This is a bug"));
        EG_Utils::fatalError();
	#endif
}//draw

void HoloApp::display(void)
{
	
    #ifdef EG_GLUT_MODE
	    double aspectRatio;//the viewing volume's aspect ratio

	    //### Update graphics ###
        aspectRatio = static_cast<double>(windowWidth) / static_cast<double>(windowHeight);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(FIELD_OF_VIEW, aspectRatio, NEAR_CLIPPING_DIST, FAR_CLIPPING_DIST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		#ifdef EG_ZSPACE_MODE
			glDrawBuffer(GL_BACK_LEFT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDrawBuffer(GL_BACK_RIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			  /*gluLookAt(0.0f,   0.345f,   0.222f,   // Position
				0.0f,   0.0f,     0.0f,     // Lookat
				0.0f,   1.0f,     0.0f);    // Up */
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		#endif

		gluLookAt(cameraOrigin.x, cameraOrigin.y, cameraOrigin.z,
		    targetPoint.x, targetPoint.y, targetPoint.z,
			upVector.x, upVector.y, upVector.z);

		/*gluLookAt(0.0f,   6.0f,   12.0f,   // Position
			0.0f,   0.0f,     0.0f,     // Lookat
			0.0f,   1.0f,     0.0f);    // Up*/
		//Original zSpace Camera Position
		  //gluLookAt(0.0f,   0.345f,   0.222f,   // Position
    //        0.0f,   0.0f,     0.0f,     // Lookat
    //        0.0f,   1.0f,     0.0f);    // Up 

		#ifdef EG_ZSPACE_MODE
			customDisplay();
		#else
			glEngine->draw(0);
		#endif
	    glutSwapBuffers();
    #endif
}//display

void HoloApp::customDisplay(void)
{
#ifdef EG_ZSPACE_MODE
	renderForEye(zspace::stereo::StereoFrustum::StereoEye::STEREO_EYE_LEFT);
	renderForEye(zspace::stereo::StereoFrustum::StereoEye::STEREO_EYE_RIGHT);
#endif
		 


}
#ifdef EG_ZSPACE_MODE
void HoloApp::renderForEye(zspace::stereo::StereoFrustum::StereoEye eye)
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
	//glMatrixMode(GL_MODELVIEW);

	// Select appropriate back buffer to render to based on the specified eye.
	if (eye == zspace::stereo::StereoFrustum::STEREO_EYE_LEFT)
	glDrawBuffer(GL_BACK_LEFT);
	else if (eye == zspace::stereo::StereoFrustum::STEREO_EYE_RIGHT)
	glDrawBuffer(GL_BACK_RIGHT);


	/*float halfSize = 1.0f;
	glColor3f(1.0f, 1.0f, 1.0f);*/
	//glDisable(GL_LIGHTING);

	//glMatrixMode(GL_MODELVIEW);
	/*gluLookAt(0.0f,   0.345f,   0.222f,   // Position
    0.0f,   0.0f,     0.0f,     // Lookat
    0.0f,   1.0f,     0.0f);    // Up */


	//glDisable(GL_LIGHTING);
	//glBegin(GL_QUADS);
	//glPushMatrix();
	//glRotatef(45.0, 0.0f, 1.0f, 0.0f);
	//glRotatef(45.0, 1.0f, 0.0f, 0.0f);
	//glRotatef(45.0, 0.0f, 0.0f, 1.0f);
	////glRotatef(45.0, 0.0f, 1.0f, 0.0f);

	//glVertex3f(-halfSize, -halfSize, -halfSize);
	//glVertex3f(-halfSize,  halfSize, -halfSize);
	//glVertex3f( halfSize,  halfSize, -halfSize);        
	//glVertex3f( halfSize, -halfSize, -halfSize);		

	//			// Side 2
	//			//glNormal3f(0.0f, 0.0f, 1.0f);

	//glVertex3f(-halfSize,  halfSize, halfSize);
	//glVertex3f(-halfSize, -halfSize, halfSize);        
	//glVertex3f( halfSize, -halfSize, halfSize);
	//glVertex3f( halfSize,  halfSize, halfSize);

	//			// Side 3
	//			//glNormal3f(0.0f, 1.0f, 0.0f);

	//glVertex3f(-halfSize, halfSize, -halfSize);
	//glVertex3f(-halfSize, halfSize,  halfSize);
	//glVertex3f( halfSize, halfSize,  halfSize);        
	//glVertex3f( halfSize, halfSize, -halfSize);

	//			// Side 4
	//			//glNormal3f(0.0f, -1.0f, 0.0f);
	//glVertex3f(-halfSize, -halfSize,  halfSize);
	//glVertex3f(-halfSize, -halfSize, -halfSize);
	//glVertex3f( halfSize, -halfSize, -halfSize);
	//glVertex3f( halfSize, -halfSize,  halfSize);

	//			// Side 5
	//			//glNormal3f(1.0f, 0.0f, 0.0f);
	//glVertex3f( halfSize,  halfSize, -halfSize);
	//glVertex3f( halfSize,  halfSize,  halfSize);        
	//glVertex3f( halfSize, -halfSize,  halfSize);
	//glVertex3f( halfSize, -halfSize, -halfSize);

	//			// Side 6
	//			//glNormal3f(-1.0f, 0.0f, 0.0f);
	//glVertex3f(-halfSize,  halfSize, -halfSize);
	//glVertex3f(-halfSize, -halfSize, -halfSize);        
	//glVertex3f(-halfSize, -halfSize,  halfSize);
	//glVertex3f(-halfSize,  halfSize,  halfSize);
	//glPopMatrix();

	//glEnd();

	/*glLineWidth(20.0f);
	
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glEnd();*/

	//glPushMatrix();
	//if(eye == zspace::stereo::StereoFrustum::STEREO_EYE_RIGHT){
	//	glTranslated(-5.0f, 0.0f, 0.0f);
	//}

	//glScalef(10.0f, 10.0f, 1.0f);



		glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glScalef(0.5, 0.5, 0.5);
	glEngine->draw(0);
	glPopMatrix();

	renderCube();


			glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glScalef(20, 20, 20);
		renderStylus();
	glPopMatrix();


	//glPopMatrix();

	//glPopMatrix();

	//glMatrixMode(GL_MODELVIEW);

		// Restore the mono (non-stereoscopic) model-view and projection matrices.
	  glMatrixMode(GL_PROJECTION);
	  glPopMatrix();
	  glMatrixMode(GL_MODELVIEW);
	  glPopMatrix();


}
#endif

void HoloApp::renderStylus()
{
  float stylusLength = 0.5f;
  zspace::common::Vector4 color = zspace::common::Vector4::WHITE();

  // Create an intersection ray based on the position and direction
  // of the stylus world pose.
  zspace::common::Vector3 position = m_stylusTransform.getTrans();
  zspace::common::Vector3 direction(-m_stylusTransform[0][2], -m_stylusTransform[1][2], -m_stylusTransform[2][2]);
  zspace::common::Ray ray = zspace::common::Ray(position, direction);

  // In the OpenGl stereo samples, it may look like the cube is rotating, 
  // but we are actually orbiting the monoscopic camera about 
  // the cube.  So, we can create an axis aligned box (AABB) based 
  // on the half extents of the cube.  An AABB can be used to represent
  // the cube because the cube is axis-aligned and never has its orientation 
  // modified.  
  /*zspace::common::AxisAlignedBox box = 
    zspace::common::AxisAlignedBox(-m_cubeHalfSize, -m_cubeHalfSize, -m_cubeHalfSize, m_cubeHalfSize, m_cubeHalfSize, m_cubeHalfSize);*/

  // Perform a ray vs. AABB intersection query to determine if the stylus
  // is intersecting with the cube.
  //std::pair<bool, float> result = ray.intersects(box);

  // If the stylus intersected the cube, change the stylus color to red
  // and set its length to the intersection distance.
  //if (result.first == true)
  //{
  //  // Since the intersection uses an infinite ray, we need to check that
  //  // the intersection distance is less than the stylus beam's original
  //  // length for it to be considered a valid intersection.
  //  if (result.second < stylusLength)
  //  {
  //    stylusLength = result.second;
  //    color = zspace::common::Vector4::RED();
  //  }
  //}

  // Render the stylus beam.
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Convert the stylus world pose to OpenGl matrix format and apply it to
  // the virtual stylus beam's transform.
  GLfloat stylusTransformGl[16];
  zspace::common::MathConverterGl::convertMatrix4ToMatrixGl(m_stylusTransform, stylusTransformGl);
  //stylusTransformGl[12] *= 5.0;
  //stylusTransformGl[13] *= 5.0;
  //stylusTransformGl[14] *= 20.0;

  glMultMatrixf(stylusTransformGl);

  glColor3f(color.x, color.y, color.z);
  glLineWidth(20.0f);
  glBegin(GL_LINES);

  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, -stylusLength);
  glEnd();
  glLineWidth(1.0f);

  glPopMatrix();
}

void HoloApp::renderCube()
{
  glColor3f(1.0f, 1.0f, 1.0f);

  // Render the cube
  glBegin(GL_QUADS);			
  const float halfSize = 0.03f;
  glColor3f(1.0f, 1.0f, 1.0f);
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
}


void HoloApp::resize(int newWidth, int newHeight)
{
	if ((newWidth > 0) && (newHeight > 0))
	{
        #ifdef EG_GLUT_MODE
		    windowWidth = newWidth;
		    windowHeight = newHeight;
            glMatrixMode(GL_MODELVIEW);
		    glViewport(0, 0, static_cast<GLsizei>(newWidth), static_cast<GLsizei>(newHeight));
        #endif
	}//if newWidth && newHeight
}//resize

// <SampleCode zSpace Setup View Matrix>
void HoloApp::setupViewMatrix(zspace::stereo::StereoFrustum::StereoEye eye)
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
// </SampleCode zSpace Setup View Matrix>


// <SampleCode zSpace Setup Projection Matrix>
void HoloApp::setupProjectionMatrix(zspace::stereo::StereoFrustum::StereoEye eye)
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
  /*gluLookAt(cameraOrigin.x, cameraOrigin.y, cameraOrigin.z + 5,
		          targetPoint.x, targetPoint.y, targetPoint.z,
			      upVector.x, upVector.y, upVector.z);*/

}
// </SampleCode zSpace Setup Projection Matrix>

//#####################################################
//############### CAMERA AND NAVIGATION ###############
//#####################################################

void HoloApp::lookAt(MHTypes::Point3D newCameraOrigin,
					 MHTypes::Point3D newTargetPoint,
					 MHTypes::MHVector newUpVector)
{
    #ifdef EG_GLUT_MODE
	    cameraOrigin = newCameraOrigin;
	    targetPoint = newTargetPoint;
	    upVector = newUpVector;
    #endif
}//lookAt

void HoloApp::updateNav(bool forwardButtonDown,
                        bool reverseButtonDown,
                        bool lookButtonDown,
                        MHTypes::EulerAngle absWandOrient,
                        MHTypes::EulerAngle cameraOrient)
{
    #ifndef EG_GLUT_MODE
        navModule->update(forwardButtonDown, reverseButtonDown, lookButtonDown,
                          absWandOrient, cameraOrient);
    #endif
}//updateNav

void HoloApp::setNavVelocity(double newValue)
{
    #ifndef EG_GLUT_MODE
        navModule->setVelocity(newValue);
    #endif
}//setNavVelocity

void HoloApp::moveCamera(MHVector movementVector)
{
    movementVector.x *= -1;
    movementVector.y *= -1;
    movementVector.z *= -1;

    #ifndef EG_GLUT_MODE
        glEngine->setNavMatrix(deviceInput->moveCamera(movementVector));
    #endif
}//moveCamera

void HoloApp::rotateCamera(MHTypes::EulerAngle amount)
{
    #ifndef EG_GLUT_MODE
        glEngine->setNavMatrix(deviceInput->rotateCamera(amount));
    #endif
}//rotateCamera

void HoloApp::setCameraRotation(MHTypes::EulerAngle newRotation)
{
    #ifndef EG_GLUT_MODE
        glEngine->setNavMatrix(deviceInput->setRotation(newRotation));
    #endif
}//setCameraRotation

//#######################################################
//############### GLUT INPUT AND WRAPPERS ###############
//#######################################################

void HoloApp::keyboard(unsigned char key,
                       int x,
                       int y)
{
}//keyboard

//Event handler for mouse input
void HoloApp::mouse(int button,
                    int state,
                    int x,
                    int y)
{
}//mouse

void HoloApp::motion(int x,
                     int y)
{
}//motion

void HoloApp::resizeWrapper(int newWidth,
							int newHeight)
{
    #ifdef EG_GLUT_MODE
	    instance->resize(newWidth, newHeight);
    #endif
}//resizeWrapper

void HoloApp::displayWrapper(void)
{
    #ifdef EG_GLUT_MODE
	    instance->display();
    #endif
}//displayWrapper

void HoloApp::keyboardWrapper(unsigned char key,
                              int x,
                              int y)
{
    #ifdef EG_GLUT_MODE
        instance->keyboard(key, x, y);
    #endif
}//keyboardWrapper

void HoloApp::mouseWrapper(int button,
                           int state,
                           int x,
                           int y)
{
    #ifdef EG_GLUT_MODE
        instance->mouse(button, state, x, y);
    #endif
}//mouseWrapper

void HoloApp::motionWrapper(int x,
                            int y)
{
    #ifdef EG_GLUT_MODE
        instance->motion(x, y);
    #endif
}//motionWrapper

void HoloApp::timerWrapper(int value)
{
    #ifdef EG_GLUT_MODE
        instance->timerCallback();
    #endif

    glutPostRedisplay();
    glutTimerFunc(ENGINE_INTERVAL, timerWrapper, 0);
}//timerWrapper
