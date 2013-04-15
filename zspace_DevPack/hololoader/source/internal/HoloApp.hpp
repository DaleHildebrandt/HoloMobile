/* DESCRIPTION
    HoloApp is the primary class that provides access to graphics, wand input
    and logic updates. It's implemented for both GLUT and VRJuggler, allowing
    the same code to run on both systems. HoloApp should be inherited from, and
    provides four main methods of interest:

    initApp             -load model meshes here
    contextInitApp      -load model textures here
    postInitialization  -all other initialization, add models to EG_Engine
    mainLoop            -called 100 times per second, logic goes here
                         provides access to device data

    Some differences between GLUT and VRJuggler aren't resolved by HoloApp,
    and must be dealt with by the child class. In VRJuggler, navigation is
    done with the moveCamera, rotateCamera, setCameraRotation and updateNav functions.
    In GLUT, we use the lookAt function to specify the camera position.

    These could potentially be combined some time in the future if someone is able
    to generate the appropriate 4x4 transformation matrix from a gluLookAt command.

    HoloApp is a fairly complex piece of code due to the complexity of combining GLUT
    and VRJuggler into a single interface, and should be modified with caution.
*/

#ifndef HOLOAPP_HPP_INCLUDED
#define HOLOAPP_HPP_INCLUDED

#ifndef EG_GLUT_MODE
    #include <vrj/vrjConfig.h>
    #include <vrj/Draw/OGL/GlContextData.h>
    #include <vrj/Kernel/Kernel.h>
    #include <vrj/Draw/OGL/GlApp.h>
#endif
#ifdef EG_ZSPACE_MODE
	#include <zspace/Stereo/StereoFrustum.h>
	#include <zspace\Stereo\StereoWindow.h>
	#include <zspace\Stereo\StereoLeftRightDetect.h>
	#include <zspace\Common\Math\MathConverterGl.h>
	#include <ZSpace\Common\Math\Ray.h>
	#include "ObjectTracker.h"
#endif

#include <windows.h>
#include <iostream>
#include <math.h>
#include <deque>
#include <GL/gl.h>
#include "GL/glut.h"

#include "EbonGL.hpp"
#include "MathHelper.hpp"


#ifndef EG_GLUT_MODE
    class Navigation;
    class InputHandler;
#endif

#ifdef EG_GLUT_MODE
    class HoloApp
#else
    class HoloApp : public vrj::GlApp
#endif
{
    public:
        //constructor
        HoloApp(void *newKernel,
                unsigned int newNumContexts);

        //destructor
        virtual ~HoloApp(void);

        //Start updating the application logic
        virtual void run(void);

        //Changes the camera's orienation and location in the scene. Only takes effect when
        //using EG_GLUT_MODE, use the Navigation and InputHandler classes otherwise.
        void lookAt(MHTypes::Point3D newCameraOrigin,
                    MHTypes::Point3D newTargetPoint,
                    MHTypes::MHVector newUpVector);

        //Move the camera viewpoint by the distance specified in movementVector.
        //This function only takes effect in VRJuggler mode
        void moveCamera(MHTypes::MHVector movementVector);

        //Rotate the camera viewpoint by the specified amount, in degrees
        //This function only takes effect in VRJuggler mode
        void rotateCamera(MHTypes::EulerAngle amount);

        //Set the camera to the new orientation
        //This function only takes effect in VRJuggler mode
        void setCameraRotation(MHTypes::EulerAngle newRotation);

        //Move the camera position when the appropriate wand buttons are pressed
        //This function only takes effect in VRJuggler mode
        void updateNav(bool forwardButtonDown,
                        bool reverseButtonDown,
                        bool lookButtonDown,
                        MHTypes::EulerAngle absWandOrient,
                        MHTypes::EulerAngle cameraOrient);

        //Set the movement velocity per logic update, default is 0.05
        //This function only takes effect in VRJuggler mode
        void setNavVelocity(double newValue);            

        //Sets the isRunning variable to false
        void stopApp();

        //The number of milliseconds to wait between logic updates
        static const int ENGINE_INTERVAL = 10;

        //The scene camera's viewport width
        static const int FIELD_OF_VIEW = 60;

        //Objects outside of these boundaries will not be shown
        //Unclear if these are actually used with VRJuggler
        static const int NEAR_CLIPPING_DIST = 1;
        static const int FAR_CLIPPING_DIST = 1000;
#ifdef EG_ZSPACE_MODE
		//static const int WORLD_SCALE = 20; //May require further adjustments
		static const int WORLD_SCALE = 20; //May require further adjustments
#endif

    protected:
        //Creates and manages a 3D scene, with lighting, transparency, shadows
        //and anti-aliasing.
        EbonGL::EG_Engine *glEngine;

        //############### EVENT HANDLERS ###############

        //Event called before contextInit (loading model meshes)
        virtual void initApp(void);

        //Event called during contextInit (loading model textures)
        virtual void contextInitApp(unsigned int myContext);

        //Initialization called during the first logic update. This is the first time we can
        //add loaded models into EbonGL::Engine.
        virtual void postInitialization(void);

        //Main loop called 100 times a second (logic)
        virtual void mainLoop(MHTypes::EulerAngle cameraOrient,
                                MHTypes::EulerAngle wandOrient,
                                MHTypes::EulerAngle absWandOrient,
                                MHTypes::Point3D wandPos,
                                std::vector<bool> buttonPressed,
                                std::vector<bool> buttonReleased,
                                std::vector<bool> buttonDown);

        //Glut Event handler for keyboard input
        virtual void keyboard(unsigned char key,
                                int x,
                                int y);

        //Glut Event handler for mouse input
        virtual void mouse(int button,
                            int state,
                            int x,
                            int y);

        //Glut Event handler for mouse motion while 
        //a button pressed
        virtual void motion(int x,
                            int y);

    private:
        #ifdef EG_GLUT_MODE
            //GLUT can only use static functions as event handlers. In those static
            //functions, we refer to instance to call the equivalent non-static member
            //variable
            static HoloApp* instance;

            //The width and height of the GLUT window in pixels
            int windowWidth;
            int windowHeight;

            //controls the speed at which we apply logic updates
            EbonGL::EG_TimeKeeper *glutTimer;

            //Parameters that specify the camera's position and orientation in the scene
            MHTypes::Point3D cameraOrigin;
            MHTypes::Point3D targetPoint;
            MHTypes::MHVector upVector;
        #else
            //Pointer to the VR Juggler kernel, manages events and lets us stop the
            //program.
            vrj::Kernel *jugglerKernel;

            //Handles user navigation through the scene
            Navigation *navModule;

            //Receives positions, orientations and button presses from connected devices
            InputHandler *deviceInput;

                //A unique identifier for each OpenGL context
            vrj::GlContextData<int> myContextID;
        #endif
#ifdef EG_ZSPACE_MODE
			zspace::stereo::StereoWindow* g_stereoWindow;
			zspace::stereo::StereoViewport* g_stereoViewport;
			zspace::stereo::StereoFrustum* g_stereoFrustum;
			zspace::common::Matrix4 m_stylusTransform;

			ObjectTracker* objectTracker; //Handles tracking of head and stylus

			void renderStylus();
			void renderCube();
			void renderForEye(zspace::stereo::StereoFrustum::StereoEye currEye);
			void setupViewMatrix(zspace::stereo::StereoFrustum::StereoEye eye);
			void setupProjectionMatrix(zspace::stereo::StereoFrustum::StereoEye eye);
#endif

        //Controls access to the contextInit function
        EbonGL::EG_Mutex contextInitMutex;
            
        //Global utilities used by EbonGL
        EbonGL::EG_Utils ebonUtils;

        //Assigns a different context ID to each OpenGL context
        volatile int contextIDStamper;

        //The number of OpenGL contexts to create
        volatile int numContexts;

        //False if we've called updateLogic() once before.
        bool firstLogicUpdate;

        //Set it to false to end program
        bool isRunning;

        //############### Glut Methods ################

        //Do drawing, update logic
        void display(void);

		//Custom zspace displaying
		void customDisplay(void);

		

        //Event handler, called when the window is resized
        void resize(int width, 
                    int height);

        //Wrapper functions - glut doesn't let us hook up a member function unless
        //it's static. These function use the static instance variable, which is a pointer
        //to this object, to call non-static members.
        static void displayWrapper(void);

        static void resizeWrapper(int newWidth,
                                    int newHeight);

        static void keyboardWrapper(unsigned char key,
                                    int x,
                                    int y);
        static void mouseWrapper(int button,
                                    int state,
                                    int x,
                                    int y);
        static void motionWrapper(int x,
                                    int y);
        static void timerWrapper(int value);

        //Periodically calls mainLoop
        void timerCallback(void);

        //############### VRJuggler Event Handlers ###############

        //Executes any initialization needed before the API is started.
        //Device interfaces are initialized with the device names we want to use.
        //This is called once before OpenGL is initialized.
        virtual void init(void);

        //Executes any initialization needed after API is started but before the
        //drawManager starts the drawing loops.
        //This is called once after OpenGL is initialized.
        virtual void apiInit(void);

        //contextInit is called immediately after a new OGL context is created.
        //Initialize the GL state here. (lights, shading, etc) Allocate static context-specific
        //data here. Say, load a model into vrj::GlContextData<>.
        virtual void contextInit(void);

        //Function that is called upon entry into a buffer of an OpenGL context(window).
        //This function is designed to be used when you want to do something
        //only once per buffer (ie.once for left buffer, once for right buffer).
        //Put the call to glClear() in this method so that this application will work with
        //configurations using two or more viewports per display window.
        virtual void bufferPreDraw(void);

        //Called after entry into an OpenGL context. Called after bufferPreDraw. Dynamic
        //context-specific data should be allocated or modified here. (Say, if the user
        //wants to load a model once the program is already running.)
        virtual void contextPreDraw(void);

        //Called before start of frame.
        //preFrame is called after device updates but before start of drawing.
        virtual void preFrame(void);

        //Called during the Frame.
        //Function called after drawing has been triggered but BEFORE it completes.
        virtual void intraFrame(void);

        //Called at end of frame.
        //Function called before updating trackers but after the frame is drawn.
        virtual void postFrame(void);

        //Function to draw the scene, called 1 or more times per frame.
        //When this function is called, the OpenGL state has correct transformation
        //and buffer selected.
        virtual void draw(void);

        //############### INTERNAL FUNCTIONS ###############

        //Called periodically to update the program logic, which is decoupled from
        //drawing.
        void updateLogic(void);

};//class HoloApp

#endif
