#include "EG_Model.hpp"
#include "EG_ModelInt.hpp"
#include "../EG_Settings.hpp"
#include "../utilities/EG_String.hpp"
#include "../EG_Engine.hpp"

using namespace std;
using namespace EbonGL;
using namespace MHTypes;

EG_Model::EG_Model(unsigned int numContexts)
{
    theModel = new EG_ModelInt(numContexts);
}//constructor

EG_Model::EG_Model(const EG_Model &source)
{
    EG_Object *sourceObject = source.theModel->generateInstance();

    theModel = dynamic_cast<EG_ModelInt*>(sourceObject);
    if (theModel == NULL)
    {
        EG_Utils::writeLog(EG_String("EG_Model::EG_Model(): couldn't cast to EG_ModelInt!"));
        EG_Utils::fatalError();
    }//if theModel
}//copy constructor

EG_Model::~EG_Model(void)
{
    delete theModel;
    theModel = NULL;
}//destructor

EG_Model& EG_Model::operator= (const EG_Model &source)
{
    if (this != &source)
        *(theModel) = *(source.theModel);

    return *this;
}//equal operator

bool EG_Model::initialize(void)
{
    return EG_ModelInt::initialize();
}//initialize

void EG_Model::shutdown(void)
{
    EG_ModelInt::shutdown();
}//shutdown

bool EG_Model::loadMesh(const EbonGL::EG_String &fileName)
{
    return theModel->loadMesh(fileName);
}//loadMesh

bool EG_Model::loadTextures(unsigned int glContextID)
{
    return theModel->loadTextures(glContextID);
}//loadTextures

void EG_Model::setScale(double newScale)
{
    theModel->setScale(newScale);
}//setScale

Point3D EG_Model::getPosition(void)
{
    return theModel->getPosition();
}//getPosition

MHTypes::EulerAngle EG_Model::getRotation(void)
{
    return theModel->getRotation();
}//getRotation

EbonGL::EG_Color EG_Model::getAmbientDiffuse(void)
{
    return theModel->getAmbientDiffuse();
}//getAmbientDiffuse

void EG_Model::show(void)
{
    theModel->show();
}//show

void EG_Model::hide(void)
{
    theModel->hide();
}//hide

void EG_Model::setShadowCasting(bool enabled)
{
    theModel->setShadowCasting(enabled);
}//setShadowCasting

void EG_Model::setShadowReceiving(bool enabled)
{
    theModel->setShadowReceiving(enabled);
}//setShadowReceiving

void EG_Model::setPosition(const MHTypes::Point3D &newPosition)
{
    theModel->setPosition(newPosition);
}//setPosition

void EG_Model::setRotation(const MHTypes::EulerAngle &newRotation)
{
    theModel->setRotation(newRotation);
}//setRotation

void EG_Model::setAmbientDiffuse(const EbonGL::EG_Color &newColor)
{
    theModel->setAmbientDiffuse(newColor);
}//setAmbientDiffuse

void EG_Model::setPolygonMode(GLenum newMode)
{
    theModel->setPolygonMode(newMode);
}//setPolygonMode

void EG_Model::setLighting(bool enabled)
{
    theModel->setLighting(enabled);
}//setLighting

void EG_Model::addToEngine(EbonGL::EG_Engine *theEngine)
{
    theEngine->addSolid(theModel);
}//addToEngine

void EG_Model::removeFromEngine(EbonGL::EG_Engine *theEngine)
{
    theEngine->remove(theModel);
}//removeFromEngine
