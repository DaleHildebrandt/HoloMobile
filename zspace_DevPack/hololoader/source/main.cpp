/* DESCRIPTION
    This program loads a 3D model with AssImp, and displays it holographically
    with VRJuggler. Multiple model formats are supported, but the model should not have
    the mesh and textures in the same file. (Each texture should be a seperate image file).
    
    The model details are read from a configuration file, specified on the command line.
    The config file tells us the model's file name, initial orientation and position, and 
    what scaling factor should be applied when drawing it.

    The command line arguments should then contain the VRJuggler configuration file(s).
    A typical invocation of HoloLoader might be:

        HoloLoader holodwarf.txt c2-hd.jconf
*/

#include "HoloLoader.hpp"
#include <zspace\Stereo\StereoWindow.h>

#ifndef EG_GLUT_MODE
    #include <vrj/Kernel/Kernel.h>
#endif



using namespace std;

int main(int argc,
		 char* argv[])
{
	
    #ifndef EG_GLUT_MODE
        vrj::Kernel *kernel = vrj::Kernel::instance();//Manages VRJuggler events
    #endif
    HoloLoader *application = NULL;//The application code
	int result = 0;//the program return code, non-zero on errors

    #ifdef EG_GLUT_MODE
        //### Verify that the model config file was specified ###
        if (argc >= 2)
        {
            application = new HoloLoader(argv[1], NULL);
            application->run();
        }//if argc
        else
        {
            cout << endl;
            cout << "Usage: " << argv[0] << " HoloConfigFile" << endl;
        }//else argc
    #else
        //### Verify that configuration files were specified ###
        if (argc > 2)
            application = new HoloLoader(argv[1], kernel);
        else
        {
            cout << endl;
            cout << "Usage: " << argv[0] << "HoloConfigFile VjConfigFile[0] VjConfigFile[1] ... VjConfigFile[n]" << endl;
            result = 1;
        }//else argc

       //### Load VRJuggler configuration files ###
        if (result == 0)
        {
		    for (int argNum = 2; argNum < argc; argNum++)
			    kernel->loadConfigFile(argv[argNum]);
	    }//if result

        //### Start the application ###
	    if (result == 0)
	    {
		    kernel->start();
		    kernel->setApplication(application);
            application->run();
	    }//if result

        if (kernel->isRunning())
            kernel->stop();
    #endif

    //### Free memory ###
    delete application;
	application = NULL;

	cout << "Program exiting..." << endl;

	return result;
}//main
