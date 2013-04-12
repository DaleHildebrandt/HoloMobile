#include "HoloLoader.hpp"
#include "EbonGL.hpp"
#include "internal/InputHandler.hpp"
#include <time.h>

const double HoloLoader::LASER_WIDTH = 0.1;
const double HoloLoader::LASER_LENGTH = 100;

using namespace std;

HoloLoader::HoloLoader(const string &newConfigFile,
                       void *newKernel) : HoloApp(newKernel, NUM_CONTEXTS)
{
	testModel = new ModelLoader("holodwarf.txt");
	sofa = new ModelLoader("phone.txt");
    configFile = newConfigFile;
	configFileB = "sofa.txt";
    theModel = NULL;
	modelB = NULL;
	sampleCube = NULL;
    wandLaser = NULL;
}//constructor

HoloLoader::~HoloLoader(void)
{
    //deleted by EG_Engine
	delete testModel;
    theModel = NULL;
	modelB = NULL;
    wandLaser = NULL;
}//destructor

void HoloLoader::run(void)
{
	HoloApp::run();
}////run

void HoloLoader::initApp(void)
{    
	sofa->init();
	testModel->init();
    EbonGL::EG_String confFileName(configFile.c_str());//the configuration file to read from
    EbonGL::EG_String fileID;//the ID of the current value to extract from the file
    EbonGL::EG_String modelName;//The model's filename
    double charVelocity = loadDouble("Navigation Velocity");
    double modelScale = loadDouble("Scale");

    //### Load the model name ###
    fileID.appendCString("Model Name");
    modelName = EbonGL::EG_ConfigWriter::loadString(confFileName, fileID);

    //### Initialize the navigation module ###
    setNavVelocity(charVelocity);

    //### Create the wand laser ###
    wandLaser = new EbonGL::EG_Cylinder(LASER_WIDTH, LASER_LENGTH, 20, 20);

    //### Load the model ###
    cout << "Loading model..." << endl;
    theModel = new EbonGL::EG_Model(NUM_CONTEXTS);
	modelB = new EbonGL::EG_Model(NUM_CONTEXTS);
	sampleCube = new EbonGL::EG_Cuboid();
    if (theModel->loadMesh(modelName))
        theModel->setScale(modelScale);
    else
        EbonGL::EG_Utils::fatalError();

	if (modelB->loadMesh("Sofa.dae"))
        modelB->setScale(modelScale);
    else
        EbonGL::EG_Utils::fatalError();


}//initApp

void HoloLoader::contextInitApp(unsigned int myContext)
{
	sofa->contextInit(myContext);
	testModel->contextInit(myContext);
    double ambientLight = 0;//the intensity of ambient light in the scene
    double localLight = 0;//the intensity of a local light centered on the wand

    glEngine->requestLock();

    //### Load the ambient light ###
    ambientLight = loadDouble("Ambient Light");
    if (ambientLight < 0)
        ambientLight = 0;
    if (ambientLight > 1)
        ambientLight = 1;

    //### Load the local light ###
    localLight = loadDouble("Local Light");
    if (localLight < 0)
        localLight = 0;
    if (localLight > 1)
        localLight = 1;

    ambientLight *= 5;
    localLight *= 5;

    //### Create the wand light ###
    wandLight = glEngine->getLight(1);

    wandLight->enable();
    wandLight->setToLocal(MHTypes::Point3D(0, 0, 0), MHTypes::EulerAngle(0, 0, 0));
    wandLight->setDiffuse(EbonGL::EG_Color(localLight, localLight, localLight, 1));
    wandLight->setAmbient(EbonGL::EG_Color(ambientLight, ambientLight, ambientLight, 1));
    wandLight->setSpecular(EbonGL::EG_Color(0.1f, 0.1f, 0.1f, 1.0f));

    //### Disable global ambient light, we'll use ambient from the wand ###
    glEngine->setAmbientLight(EbonGL::EG_Color(0, 0, 0, 1));

    glEngine->releaseLock();

    cout << "Loading textures..." << endl;
    if (!theModel->loadTextures(myContext))
        EbonGL::EG_Utils::fatalError();

	if (!modelB->loadTextures(myContext))
		EbonGL::EG_Utils::fatalError();
}//contextInitApp

void HoloLoader::postInitialization(void)
{
    vector<GLfloat> navMatrix;//the navigation matrix representing the initial camera position
    double modelPitch = loadDouble("Pitch");
    double modelYaw = loadDouble("Yaw");
    double modelRoll = loadDouble("Roll");
    double offsetX = loadDouble("X Offset");
    double offsetY = loadDouble("Y Offset");
    double offsetZ = loadDouble("Z Offset");

    glEngine->requestLock();

    
    //### Add the wand laser to the scene ####
    wandLaser->setAmbientDiffuse(EbonGL::EG_Color(1.0f, 0.0f, 1.0f, 1.0f));
    wandLaser->setLighting(false);
    wandLaser->hide();
    //glEngine->addSolid(wandLaser);

	testModel->postInit(glEngine);
	sofa->postInit(glEngine);
    //### Add the model to the scene ###
    theModel->setRotation(MHTypes::EulerAngle(modelPitch, modelYaw, modelRoll));
    theModel->setPosition(MHTypes::Point3D(0,-2, 0));
    //theModel->addToEngine(glEngine);

	modelB->setRotation(MHTypes::EulerAngle(modelPitch, modelYaw, modelRoll));
    modelB->setPosition(MHTypes::Point3D(2, -2, 0));
    //modelB->addToEngine(glEngine);

	sampleCube->setRotation(MHTypes::EulerAngle(0.0f,45.0,0.0));
	//sampleCube->setPosition(MHTypes::Point3D(offsetX, offsetY + 3.5, offsetZ - 100));
	//sampleCube->setPosition(MHTypes::Point3D(0.2f, 0.0f, 0.0f));
	sampleCube->setAmbientDiffuse(EbonGL::EG_Color(1.0f, 0.0f, 1.0f, 1.0f));
	//sampleCube->setSize(0.03f, 0.03f, 0.03f);
	sampleCube->setSize(20.0f, 20.0f, 20.0f);

	//glEngine->addSolid(sampleCube);
    
    //### Set the camera offset ###
    #ifdef EG_GLUT_MODE
        HoloApp::lookAt(MHTypes::Point3D(offsetX, offsetY + 3.5, offsetZ + 4),
                        MHTypes::Point3D(offsetX, offsetY + 3.5, offsetZ - 100),
                        MHTypes::MHVector(0, 1, 0));
    #else
        moveCamera(MHTypes::MHVector(offsetX, offsetY, offsetZ));
    #endif
    
    glEngine->releaseLock();
}//postInitialization

void HoloLoader::mainLoop(MHTypes::EulerAngle cameraOrient,
                          MHTypes::EulerAngle wandOrient,
                          MHTypes::EulerAngle absWandOrient,
                          MHTypes::Point3D wandPos,
                          std::vector<bool> buttonPressed,
                          std::vector<bool> buttonReleased,
                          std::vector<bool> buttonDown)
{
    glEngine->requestLock();

    //### Enable or disable the wand laser ###
    if (buttonPressed.at(BT_TRIGGER))
        wandLaser->show();
    else if (buttonReleased.at(BT_TRIGGER))
        wandLaser->hide();

    //### Move the wand laser along with the wand ###
    moveWandLaser(wandPos, wandOrient);
    wandLight->setPosition(wandPos);

    //### Allow navigation through the scene ###
    updateNav(buttonDown.at(BT_FAR_LEFT), buttonDown.at(BT_FAR_RIGHT), buttonDown.at(BT_HAT),
              absWandOrient, cameraOrient);

    glEngine->releaseLock();
}//mainLoop

void HoloLoader::moveWandLaser(MHTypes::Point3D wandPos,
                               MHTypes::EulerAngle wandOrient)
{
    MHTypes::Matrix3x3 laserMatrix;//rotates laser center to the desired position
    MHTypes::Point3D laserCenter;//the laser's new center position

    //### Extract the laser depth ###
    laserCenter.z = wandLaser->getHeight();
    if (laserCenter.z <= 0)
    {
        EbonGL::EG_Utils::addLog(EbonGL::EG_String("HoloLoader::moveWandLaser(): got a wand depth of "));
        EbonGL::EG_Utils::writeLog(EbonGL::EG_String::fromDouble(laserCenter.z));
        EbonGL::EG_Utils::fatalError();
    }//if laserCenter

    //### Find the laser center point ###
    laserCenter.z = laserCenter.z / 2.0f;
    laserCenter.z *= -1;

    //### Rotate the laser center point to the desired position ###
    laserMatrix.fromEuler(wandOrient);
    laserCenter = laserMatrix.rotatePoint(laserCenter);

    //### Invert the position since z is negative ###
    laserCenter.x *= -1;
    laserCenter.y *= -1;

    //### Position the laser relative to the wand ###
    laserCenter.x += wandPos.x;
    laserCenter.y += wandPos.y;
    laserCenter.z += wandPos.z;

    //### Move the laser ###
    wandLaser->setPosition(laserCenter);
    wandLaser->setRotation(wandOrient);
}//moveWandLaser

double HoloLoader::loadDouble(const string &valueLabel)
{
    EbonGL::EG_String confFileName(configFile.c_str());//the configuration file to read from
    EbonGL::EG_String fileID;//the ID of the current value to extract from the file

    fileID.appendCString(valueLabel.c_str());    

    return EbonGL::EG_ConfigWriter::loadDouble(confFileName, fileID);
}//loadDouble
