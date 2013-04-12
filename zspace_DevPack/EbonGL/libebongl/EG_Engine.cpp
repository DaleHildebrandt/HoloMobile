#include "EG_Engine.hpp"
#include "EG_EngineInt.hpp"

using namespace EbonGL;
using namespace std;

EG_Engine::EG_Engine(void)
{
    theEngine = new EG_EngineInt;
}//constructor

EG_Engine::~EG_Engine(void)
{
    delete theEngine;
    theEngine = NULL;
}//destructor

void EG_Engine::contextInit(void)
{
    theEngine->contextInit();
}//contextInit

void EG_Engine::clearBuffer(void)
{
    theEngine->clearBuffer();
}//clearBuffer

void EG_Engine::draw(int glContextID)
{
    theEngine->draw(glContextID);
}//draw

void EG_Engine::addSolid(EG_Object *newObject)
{
    theEngine->addSolid(newObject);
}//addSolid

void EG_Engine::addTransparent(EG_Object *newObject)
{
    theEngine->addTransparent(newObject);
}//addTransparent

void EG_Engine::remove(EG_Object *theObject)
{
    theEngine->remove(theObject);
}//remove

void EG_Engine::setAmbientLight(EG_Color newColor)
{
    theEngine->setAmbientLight(newColor);
}//setAmbientLight

EG_Light* EG_Engine::getLight(unsigned int index)
{
    return theEngine->getLight(index);
}//getLight

EG_Color EG_Engine::getAmbientLight(void)
{
    return theEngine->getAmbientLight();
}//getAmbientLight

void EG_Engine::clearScene(void)
{
    theEngine->clearScene();
}//clearScene

void EG_Engine::setNavMatrix(const std::vector<GLfloat> &newNavMatrix)
{
    theEngine->setNavMatrix(newNavMatrix);
}//setNavMatrix

void EG_Engine::requestLock(void)
{
    theEngine->requestLock();
}//requestLock

void EG_Engine::releaseLock(void)
{
    theEngine->releaseLock();
}//releaseLock

void EG_Engine::setShadows(bool enabled)
{
    theEngine->setShadows(enabled);
}//setShadows
