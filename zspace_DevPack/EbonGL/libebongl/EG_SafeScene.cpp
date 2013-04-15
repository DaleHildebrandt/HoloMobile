#include "EG_SafeScene.hpp"
#include "EG_SceneElement.hpp"
#include "EG_Light.hpp"
#include "sceneobjects/EG_Object.hpp"
#include "utilities/EG_LogWriter.hpp"

using namespace std;
using namespace EbonGL;

EG_SafeScene::EG_SafeScene(unsigned int newIndex,
                           const vector<EG_Light*> &newLights)
{
	EG_Light *currentSource = NULL;//the current light to copy

	sceneIndex = newIndex;
	inUse = false;
    shadowsEnabled = false;

	for (unsigned int i = 0; i < newLights.size(); i++)
	{
		currentSource = newLights.at(i);
        lights.push_back(new EG_Light(*currentSource));
	}//for i
}//constructor

EG_SafeScene::~EG_SafeScene(void)
{
	clearScene();

	for (unsigned int i = 0; i < lights.size(); i++)
        delete lights.at(i);
    lights.clear();
}//destructor

void EG_SafeScene::clearScene(void)
{
	list<EG_Object*>::iterator objectIter;//points to the current object to delete

    for (objectIter = solidObjects.begin(); objectIter != solidObjects.end(); objectIter++)
        delete *objectIter;
    solidObjects.clear();

	for (objectIter = transparentObjects.begin(); objectIter != transparentObjects.end(); objectIter++)
        delete *objectIter;
    transparentObjects.clear();
}//clearScene

bool EG_SafeScene::getInUse(void)
{
	bool result = inUse;

	if (!result)
		inUse = true;

	return result;
}//getInUse

void EG_SafeScene::releaseScene(void)
{
	inUse = false;
}//releaseScene

void EG_SafeScene::setNavMatrix(const std::vector<GLfloat> &newNavMatrix)
{
    if (newNavMatrix.size() != 16)
    {
        EG_Utils::addLog(EG_String("EG_SafeScene::setNavMatrix(): got a matrix with "));
        EG_Utils::addLog(EG_String::fromInt(static_cast<int>(newNavMatrix.size())));
        EG_Utils::writeLog(EG_String(" elements, expected 16"));
        EG_Utils::fatalError();
    }//if size()

    navMatrix = newNavMatrix;
}//setNavMatrix

void EG_SafeScene::setShadows(bool enabled)
{
    shadowsEnabled = enabled;
}//setShadows

void EG_SafeScene::draw(int glContextID)
{
	list<EG_Object*>::iterator objectIter;//points to the current object to draw

    drawMutex.lock();

	//### Clear the depth buffer ###
	glClear(GL_DEPTH_BUFFER_BIT);

	//### Clear the matrices ###
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    if (navMatrix.size() == 16)
        glMultMatrixf(&navMatrix.at(0));

    //### Draw solid objects ###
    for (objectIter = solidObjects.begin(); objectIter != solidObjects.end(); objectIter++)
        (*objectIter)->draw(glContextID);

    //### Draw transparent objects ###
    glDepthMask(GL_FALSE);
    for (objectIter = transparentObjects.begin(); objectIter != transparentObjects.end(); objectIter++)
        (*objectIter)->draw(glContextID);
    glDepthMask(GL_TRUE);

    //### Draw shadows ###
    if (shadowsEnabled)
    {
        glMatrixMode(GL_MODELVIEW);
        //glLoadIdentity();
        if (navMatrix.size() == 16)
            glMultMatrixf(&navMatrix.at(0));
        EG_Object::resetShadowStamper();

	    for (objectIter = solidObjects.begin(); objectIter != solidObjects.end(); objectIter++)
            drawShadows(*objectIter);

        for (objectIter = transparentObjects.begin(); objectIter != transparentObjects.end(); objectIter++)
            drawShadows(*objectIter);
    }//if shadowsEnabled

	drawMutex.unlock();
}//draw

void EG_SafeScene::drawShadows(EG_Object *shadowObject)
{
    EG_Light *currentLight = NULL;//the current light to use for shadow projection
	list<EG_Object*>::iterator objectIter;//points to the current shadow-receiving object

    for (unsigned int i = 0; i < lights.size(); i++)
    {
        currentLight = lights.at(i);

        if (currentLight->getEnabled())
        {
			for (objectIter = solidObjects.begin(); objectIter != solidObjects.end(); objectIter++)
				shadowObject->drawShadows(currentLight, *objectIter);

			for (objectIter = transparentObjects.begin(); objectIter != transparentObjects.end(); objectIter++)
				shadowObject->drawShadows(currentLight, *objectIter);
        }//if getEnabled()
    }//for i
}//drawShadows

void EG_SafeScene::update(list<EG_Object*> &newSolid,
					      list<EG_Object*> &newTransparent,
					      const vector<EG_Light*> &newLights)
{

	updateObjects(newSolid, solidObjects);
	updateObjects(newTransparent, transparentObjects);

	for (unsigned int i = 0; i < lights.size(); i++)
	{
		if (newLights.at(i)->getIsChanged(sceneIndex))
		{
			*(lights.at(i)) = *(newLights.at(i));
			lights.at(i)->applyChanges();
		}//if getIsChanged()
	}//for i
}//update

void EG_SafeScene::updateObjects(list<EG_Object*> &source,
							     list<EG_Object*> &dest)
{
	list<EG_Object*>::iterator destIter;//points to an object in dest
	list<EG_Object*>::iterator sourceIter;//points to an object in source
	EG_Object *currentSource = NULL;//the current object in source to examine
	EG_Object *currentDest = NULL;//the current object in dest to update

	//### Verify that no objects have been added or removed behind our back ###
	if (source.size() != dest.size())
	{
		EG_Utils::writeLog(EG_String("EG_SafeScene::updateObjects(): objects were removed, but the scene wasn't notified"));
		EG_Utils::fatalError();
	}//if size()

	//### Update existing objects ###
	sourceIter = source.begin();
	for (destIter = dest.begin(); destIter != dest.end(); destIter++)
	{
		currentSource = *sourceIter;
		sourceIter++;

		if (currentSource->getChanged(sceneIndex))
		{
			currentDest = *destIter;
			*currentDest = *currentSource;
		}//if getChanged()
	}//for destIter
}//updateObjects

list<EG_Object*>::iterator EG_SafeScene::addSolid(EG_Object *newObject)
{
	drawMutex.lock();
    solidObjects.push_front(newObject->generateInstance());
	drawMutex.unlock();

	return solidObjects.begin();
}//addSolid

list<EG_Object*>::iterator EG_SafeScene::addTransparent(EG_Object *newObject)
{
	drawMutex.lock();
	transparentObjects.push_front(newObject->generateInstance());
	drawMutex.unlock();

	return transparentObjects.begin();
}//addTransparent

void EG_SafeScene::remove(list<EG_Object*>::iterator objectIter,
					     bool isSolid)
{
	drawMutex.lock();

	delete *objectIter;

	if (isSolid)
		solidObjects.erase(objectIter);
	else
		transparentObjects.erase(objectIter);

	drawMutex.unlock();
}//remove
