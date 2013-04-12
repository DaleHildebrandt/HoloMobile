#ifndef HOLOLOADER_HPP_INCLUDED
#define HOLOLOADER_HPP_INCLUDED

#include "internal/HoloApp.hpp"
#include "ModelLoader.h"

class HoloLoader : public HoloApp
{
	public:
		//HoloLoader constructor, calls HoloApp's constructor
		HoloLoader(const std::string &newConfigFile,
                   void *newKernel);

		//HoloLoader's destructor, calls HoloApp's destructor
		virtual ~HoloLoader(void);

		//Runs the EbonGL engine in Glut or VRJuggler
		virtual void run(void);

        //The number of OpenGL contexts to create, one for each screen and eye combination
        static const unsigned int NUM_CONTEXTS = 4;

         //The dimensions of the wand laser, in scene coordinates
        static const double LASER_WIDTH;
        static const double LASER_LENGTH;

	private:
        //A light that's centered on the wand
        EbonGL::EG_Light *wandLight;

        //A virtual laser beam emitted from the wand
        EbonGL::EG_Cylinder *wandLaser;

        //The model to display
        EbonGL::EG_Model *theModel;

		EbonGL::EG_Model *modelB;

		//Model Loaders
		ModelLoader *testModel;
		ModelLoader *sofa;

		//Test cube to display
		EbonGL::EG_Cuboid *sampleCube;

        //The name of the HoloLoader config file to read from
        std::string configFile;

		std::string configFileB;


        //############### Internal Functions ###############

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

        //Load the value with the specified label from the configuration file
        double loadDouble(const std::string &valueLabel);

        //Move the wand laser based on the position and orientation of the wand
        void moveWandLaser(MHTypes::Point3D wandPos,
                           MHTypes::EulerAngle wandOrient);

};//HoloLoader

#endif
