#include "EG_EngineInt.hpp"
#include "EG_SafeScene.hpp"
#include "EG_SceneElement.hpp"
#include "sceneobjects/EG_Object.hpp"
#include "utilities/EG_LogWriter.hpp"
#include "EG_Color.hpp"

using namespace std;
using namespace EbonGL;

unsigned int EG_EngineInt::engineStamper = 0;

EG_EngineInt::EG_EngineInt(void)
{
    unsigned int numViewports = EG_Utils::getNumViewports();

	myID = engineStamper;
	engineStamper++;

	for (unsigned int i = 0; i < EG_Light::MAX_LIGHTS; i++)
        lights.push_back(new EG_Light(i));

	for (unsigned int i = 0; i < numViewports; i++)
		threadScenes.push_back(new EG_SafeScene(i, lights));

	lockRequested = false;
}//constructor

EG_EngineInt::~EG_EngineInt(void)
{
	for (unsigned int i = 0; i < lights.size(); i++)
        delete lights.at(i);
    lights.clear();

	updateMutex.lock();
	internalClear();
	updateMutex.unlock();

	threadScenes.clear();
}//destructor

void EG_EngineInt::contextInit(void)
{
    #ifndef EBONGL_WINDOWS
        GLint samplingBuffers = 0;//the number of sampling buffers in use
        GLint numSamples = 0;//the number of subpixel samples for antialiasing
    #endif

    //### Display OpenGL details ###
    showVersion();

	EG_Utils::writeLog(EG_String("Initializing OpenGL context..."));

    //### Reset the projection matrix ###
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

    //### Reset the model view matrix ###
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    //### Use nice perspective correction ###
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    //### Set the color to clear the window ###
    glClearColor(0.0, 0.0, 0.0, 0.0);

    //### Enable backface culling and occlusion ###
    //glCullFace(GL_BACK);
    //glEnable(GL_CULL_FACE);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);

    //### Re-normalize surface normals after scaling operations ###
    glEnable(GL_NORMALIZE);

    //### Configure the lighting model ###
    glEnable(GL_LIGHTING);
    setAmbientLight(EG_Color(0.9f, 0.9f, 0.9f, 1.0f));
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    //### Enable smooth shading ###
	glShadeModel(GL_SMOOTH);

    //### Configure blending ###
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //### Configure antialiasing ###
    #ifndef EBONGL_WINDOWS
        glGetIntegerv(GL_SAMPLE_BUFFERS, &samplingBuffers);
        glGetIntegerv(GL_SAMPLES, &numSamples);

        if ((samplingBuffers == 1) && (numSamples > 1))
        {
            glEnable(GL_MULTISAMPLE);
            glEnable(GL_POINT_SMOOTH);
            glEnable(GL_LINE_SMOOTH);
            glEnable(GL_POLYGON_SMOOTH);

            EG_Utils::writeLog(EG_String("    Antialiasing detected."));

            EG_Utils::addLog(EG_String("    OpenGL Sampling buffers: "));
            EG_Utils::writeLog(EG_String::fromInt(samplingBuffers));

            EG_Utils::addLog(EG_String("    Num samples: "));
            EG_Utils::writeLog(EG_String::fromInt(numSamples));
        }//if samplingBuffers && numSamples
        else
			EG_Utils::writeLog("    No antialiasing available.");
    #endif
}//contextInit

void EG_EngineInt::showVersion(void)
{
    const GLubyte *infoChars = NULL;
    string infoString;

    infoChars = glGetString(GL_VERSION);
    infoString.append((char*)infoChars);
    EG_Utils::addLog(EG_String("OpenGL Version: "));
    EG_Utils::writeLog(EG_String(infoString.c_str()));

    infoChars = glGetString(GL_VENDOR);
    infoString.clear();
    infoString.append((char*)infoChars);
    EG_Utils::addLog(EG_String("Vendor: "));
    EG_Utils::writeLog(EG_String(infoString.c_str()));

    infoChars = glGetString(GL_RENDERER);
    infoString.clear();
    infoString.append((char*)infoChars);
    EG_Utils::addLog(EG_String("Renderer: "));
    EG_Utils::writeLog(EG_String(infoString.c_str()));

    EG_Utils::writeLog(EG_String(" "));
}//showVersion

void EG_EngineInt::setNavMatrix(const vector<GLfloat> &newNavMatrix)
{
    if (!lockRequested)
	{
		EG_Utils::writeLog(EG_String("EG_EngineInt::setNavMatrix(): call requestLock() before calling this function"));
		EG_Utils::fatalError();
	}//if !lockRequested

    for (unsigned int i = 0; i < threadScenes.size(); i++)
        threadScenes.at(i)->setNavMatrix(newNavMatrix);
}//setNavMatrix

void EG_EngineInt::clearBuffer(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}//clearBuffer

void EG_EngineInt::draw(int glContextID)
{
	EG_SafeScene *myScene = threadScenes.at(glContextID);//the thread-safe scene for this drawing operation

    //### Update my scene with any changes ###
	updateMutex.lock();
	myScene->update(solidObjects, transparentObjects, lights);
	updateMutex.unlock();

	//### Draw my scene ###
	myScene->draw(glContextID);
}//draw

void EG_EngineInt::setShadows(bool enabled)
{
    if (!lockRequested)
	{
		EG_Utils::writeLog(EG_String("EG_EngineInt::setShadows(): call requestLock() before calling this function"));
		EG_Utils::fatalError();
	}//if !lockRequested

    for (unsigned int i = 0; i < threadScenes.size(); i++)
        threadScenes.at(i)->setShadows(enabled);
}//setShadows

void EG_EngineInt::setAmbientLight(EG_Color newColor)
{
    EG_Utils::setColor(ambientColor, newColor.red(), newColor.green(), newColor.blue(), 1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &ambientColor.at(0));
}//setAmbientLight

EG_Color EG_EngineInt::getAmbientLight(void)
{
    EG_Color result;//the current level of ambient light

    result.setColor(ambientColor.at(0), ambientColor.at(1), ambientColor.at(2), 1);

    return result;
}//getAmbientLight

EG_Light* EG_EngineInt::getLight(unsigned int index)
{
	if (!lockRequested)
	{
		EG_Utils::writeLog(EG_String("EG_EngineInt::getLight(): call requestLock() before calling this function"));
		EG_Utils::fatalError();
	}//if !lockRequested

    return lights.at(index);
}//getLight

void EG_EngineInt::addSolid(EG_Object *newObject)
{
	vector<list<EG_Object*>::iterator> safeIters;//the object's position in each safe scene
	EG_SceneElement *objectPos = newObject->getElement();

	if (!lockRequested)
	{
		EG_Utils::writeLog(EG_String("EG_EngineInt::addSolid(): call requestLock() before calling this function"));
		EG_Utils::fatalError();
	}//if !lockRequested

    solidObjects.push_front(newObject);

	for (unsigned int i = 0; i < threadScenes.size(); i++)
		safeIters.push_back(threadScenes.at(i)->addSolid(newObject));

	objectPos->setEngine(myID, solidObjects.begin(), safeIters, true);
}//addSolid

void EG_EngineInt::addTransparent(EG_Object *newObject)
{
	vector<list<EG_Object*>::iterator> safeIters;//the object's position in each safe scene
	EG_SceneElement *objectPos = newObject->getElement();

	if (!lockRequested)
	{
		EG_Utils::writeLog(EG_String("EG_EngineInt::addTransparent(): call requestLock() before calling this function"));
		EG_Utils::fatalError();
	}//if !lockRequested

	transparentObjects.push_front(newObject);

	for (unsigned int i = 0; i < threadScenes.size(); i++)
		safeIters.push_back(threadScenes.at(i)->addTransparent(newObject));

	objectPos->setEngine(myID, transparentObjects.begin(), safeIters, false);
}//addTransparent

void EG_EngineInt::remove(EG_Object *theObject)
{
	EG_SceneElement *objectPos = theObject->getElement();
	vector <list<EG_Object*>::iterator> safeIters;//points to the element in the tread-safe scenes
	list<EG_Object*>::iterator objectIter;//points to the element to remove
	bool isSolid = objectPos->getIsSolid();

	if (!lockRequested)
	{
		EG_Utils::writeLog(EG_String("EG_EngineInt::remove: call requestLock() before calling this function"));
		EG_Utils::fatalError();
	}//if !lockRequested

	objectPos->removeEngine(myID, objectIter, safeIters);
	if (isSolid)
		solidObjects.erase(objectIter);
	else
		transparentObjects.erase(objectIter);

	for (unsigned int i = 0; i < threadScenes.size(); i++)
		threadScenes.at(i)->remove(safeIters.at(i), isSolid);
}//remove

void EG_EngineInt::clearScene(void)
{
	if (!lockRequested)
	{
		EG_Utils::writeLog(EG_String("EG_EngineInt::clearScene(): call requestLock() before calling this function"));
		EG_Utils::fatalError();
	}//if !lockRequested

	internalClear();
}//clearScene

void EG_EngineInt::internalClear(void)
{
	list<EG_Object*>::iterator objectIter;//points to the current object to delete

    EG_Utils::addLog(EG_String("EG_EngineInt::internalClear(): Number of solid objects to clear: "));
    EG_Utils::writeLog(EG_String::fromInt(static_cast<int>(solidObjects.size())));

	for (objectIter = solidObjects.begin(); objectIter != solidObjects.end(); objectIter++)
    {
        EG_Utils::writeLog(EG_String("Deleting an object..."));
        delete *objectIter;
        EG_Utils::writeLog(EG_String("Complete!"));
    }//for objectIter
    solidObjects.clear();

	for (objectIter = transparentObjects.begin(); objectIter != transparentObjects.end(); objectIter++)
        delete *objectIter;
    transparentObjects.clear();

	for (unsigned int i = 0; i < threadScenes.size(); i++)
		threadScenes.at(i)->clearScene();
}//internalClear

void EG_EngineInt::requestLock(void)
{
	if (!lockRequested)
	{
		updateMutex.lock();
		lockRequested = true;
	}//if !lockRequested
}//requestLock

void EG_EngineInt::releaseLock(void)
{
	if (lockRequested)
	{
		updateMutex.unlock();
		lockRequested = false;
	}//if lockRequested
}//releaseLock
