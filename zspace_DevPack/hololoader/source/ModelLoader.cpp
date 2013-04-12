#include "ModelLoader.h"


using namespace std;

ModelLoader::ModelLoader(const string &newConfigFile)
{
	configFile = newConfigFile;
}

void ModelLoader::init()
{
	EbonGL::EG_String confFileName(configFile.c_str());//the configuration file to read from
    EbonGL::EG_String fileID;//the ID of the current value to extract from the file
    EbonGL::EG_String modelName;//The model's filename
    double charVelocity = loadDouble("Navigation Velocity");
    double modelScale = loadDouble("Scale");

    //### Load the model name ###
    fileID.appendCString("Model Name");
    modelName = EbonGL::EG_ConfigWriter::loadString(confFileName, fileID);

	//### Load the model ###
    cout << "Loading model..." << endl;
    theModel = new EbonGL::EG_Model(NUM_CONTEXTS);
	//modelB = new EbonGL::EG_Model(NUM_CONTEXTS);
	//sampleCube = new EbonGL::EG_Cuboid();
    if (theModel->loadMesh(modelName))
        theModel->setScale(modelScale);
    else
        EbonGL::EG_Utils::fatalError();

	/*if (modelB->loadMesh("Sofa.dae"))
        modelB->setScale(modelScale);
    else
        EbonGL::EG_Utils::fatalError();*/

}

void ModelLoader::contextInit(unsigned int myContext)
{
	cout << "Loading textures..." << endl;
    if (!theModel->loadTextures(myContext))
        EbonGL::EG_Utils::fatalError();

	/*if (!modelB->loadTextures(myContext))
		EbonGL::EG_Utils::fatalError();*/


}

void ModelLoader::postInit(EbonGL::EG_Engine *glEngine)
{
	double modelPitch = loadDouble("Pitch");
    double modelYaw = loadDouble("Yaw");
    double modelRoll = loadDouble("Roll");
    double offsetX = loadDouble("X Offset");
    double offsetY = loadDouble("Y Offset");
    double offsetZ = loadDouble("Z Offset");

	theModel->setRotation(MHTypes::EulerAngle(modelPitch, modelYaw, modelRoll));
    theModel->setPosition(MHTypes::Point3D(0, -1, -1));
    theModel->addToEngine(glEngine);
}

ModelLoader::~ModelLoader(void)
{
	theModel = NULL;
}

double ModelLoader::loadDouble(const string &valueLabel)
{
    EbonGL::EG_String confFileName(configFile.c_str());//the configuration file to read from
    EbonGL::EG_String fileID;//the ID of the current value to extract from the file

    fileID.appendCString(valueLabel.c_str());    

    return EbonGL::EG_ConfigWriter::loadDouble(confFileName, fileID);
}//loadDouble
