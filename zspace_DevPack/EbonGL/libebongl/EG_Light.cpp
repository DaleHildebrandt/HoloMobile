#include "EG_Light.hpp"
#include "utilities/EG_LogWriter.hpp"
#include "utilities/EG_String.hpp"

using namespace EbonGL;
using namespace std;
using namespace MHTypes;

unsigned int EG_Light::numLights = 0;

EG_Light::EG_Light(unsigned int newIndex)
{
    unsigned int numViewports = EG_Utils::getNumViewports();

	isCopy = false;

    numLights++;
    if (numLights > MAX_LIGHTS)
    {
		EG_Utils::writeLog(EG_String("EbonGL::EG_Light::EG_Light(): too many lights created!"));
        EG_Utils::fatalError();
    }//if numLights

    EulerAngle newDirection(1, 0, 0);

    openGLIndex = getGLIndex(newIndex);
    setToDirectional(newDirection);
    isChanged.resize(numViewports, false);

    disable();
}//constructor

EG_Light::EG_Light(const EG_Light &source)
{
    copyFrom(source);
}//copy constructor

EG_Light::~EG_Light(void)
{
	if (!isCopy)
		disable();
}//destructor

EG_Light& EG_Light::operator= (const EG_Light &source)
{
    if (this != &source)
        copyFrom(source);

    return *this;
}//=

void EG_Light::copyFrom(const EG_Light &source)
{
	isCopy = true;

	position = source.position;
	openGLIndex = source.openGLIndex;
	myType = source.myType;
    isEnabled = source.isEnabled;

    ambientColor = source.ambientColor;
    diffuseColor = source.diffuseColor;
    specularColor = source.specularColor;
}//copyFrom

void EG_Light::setToLocal(const Point3D &newPosition,
                          const EulerAngle &newDirection)
{
	checkCopy();

    myType = EG_LOCAL;
    setPosition(newPosition);
    setDirection(newDirection);
}//setToLocal

void EG_Light::setToDirectional(const EulerAngle &newDirection)
{
	checkCopy();

    myType = EG_DIRECTIONAL;
    setDirection(newDirection);
}//setToDirectional

void EG_Light::setPosition(const Point3D &newPosition)
{
	checkCopy();

    position = newPosition;
}//setPosition

Point3D EG_Light::getPosition(void)
{
    return position;
}//getPosition

void EG_Light::setDirection(const EulerAngle &newDirection)
{
    bool validOrient = true;//false if newDirection isn't normalized

	checkCopy();

    //### Verify that the orientation is normalized ###
    if ((newDirection.pitch > 1) || (newDirection.pitch < 0))
        validOrient = false;
    if ((newDirection.yaw > 1) || (newDirection.yaw < 0))
        validOrient = false;
    if ((newDirection.roll > 1) || (newDirection.roll < 0))
        validOrient = false;

    if (!validOrient)
    {
		EG_Utils::writeLog(EG_String("EbonGL::EG_Light::setDirection(): orientation not normalized!"));
        EG_Utils::fatalError();
    }//if !validOrient

    //### Store the direction ###
	direction = newDirection;
}//setDirection

void EG_Light::enable(void)
{
	checkCopy();
    isEnabled = true;
}//enable

void EG_Light::disable(void)
{
	checkCopy();
    isEnabled = false;
}//disable

bool EG_Light::getEnabled(void)
{
    return isEnabled;
}//getEnabled

void EG_Light::setAmbient(EG_Color newColor)
{
    checkCopy();
    ambientColor = newColor;
}//setAmbient

void EG_Light::setDiffuse(EG_Color newColor)
{
    checkCopy();
    diffuseColor = newColor;
}//setDiffuse

void EG_Light::setSpecular(EG_Color newColor)
{
    checkCopy();
    specularColor = newColor;
}//setSpecular

GLenum EG_Light::getGLIndex(unsigned int index)
{
    GLenum result = GL_LIGHT0;//the light index to return

    switch (index)
    {
        case 0:
            result = GL_LIGHT0;
            break;

        case 1:
            result = GL_LIGHT1;
            break;

        case 2:
            result = GL_LIGHT2;
            break;

        case 3:
            result = GL_LIGHT3;
            break;

        case 4:
            result = GL_LIGHT4;
            break;

        case 5:
            result = GL_LIGHT5;
            break;

        case 6:
            result = GL_LIGHT6;
            break;

        case 7:
            result = GL_LIGHT7;
            break;

        default:
			EG_Utils::addLog(EG_String("EG_Light::getGLIndex(): unsupported index "));
            EG_Utils::writeLog(EG_String::fromInt(index));
            EG_Utils::fatalError();
    }//switch index

    return result;
}//getGLIndex

void EG_Light::checkCopy(void)
{
	if (isCopy)
	{
		EG_Utils::writeLog(EG_String("EG_Light::checkCopy(): can't modify deep copies of existing lights"));
		EG_Utils::fatalError();
	}//if isCopy

	for (unsigned int i = 0; i < isChanged.size(); i++)
		isChanged.set(i, true);
}//checkCopy

bool EG_Light::getIsChanged(unsigned int sceneIndex)
{
	bool result = false;//True if this light was changed and needs updating

    result = isChanged.get(sceneIndex);
    isChanged.set(sceneIndex, false);

	return result;
}//getIsChanged

void EG_Light::applyChanges(void)
{
	vector<GLfloat> lightPosition;//sets the light's position
	vector<GLfloat> lightDirection;//the direction to assign

	if (isEnabled)
	{
		//### Enable the light ###
		glEnable(openGLIndex);

		//### Set the light's position ###
		if (myType == EG_LOCAL)
		{
			lightPosition.push_back(static_cast<GLfloat>(position.x));
			lightPosition.push_back(static_cast<GLfloat>(position.y));
			lightPosition.push_back(static_cast<GLfloat>(position.z));
			lightPosition.push_back(1);//w non-zero for positional

			glLightfv(openGLIndex, GL_POSITION, &lightPosition.at(0));
		}//if myType

		//### Set the light's direction ###
		lightDirection.push_back(static_cast<GLfloat>(direction.pitch));
		lightDirection.push_back(static_cast<GLfloat>(direction.yaw));
		lightDirection.push_back(static_cast<GLfloat>(direction.roll));

		if (myType == EG_DIRECTIONAL)
		{
			lightDirection.push_back(0);
			glLightfv(openGLIndex, GL_POSITION, &lightDirection.at(0));
		}//if myType

		else if (myType == EG_LOCAL)
			glLightfv(openGLIndex, GL_SPOT_DIRECTION, &lightDirection.at(0));

        //### Set the light's color ###
        glLightfv(openGLIndex, GL_AMBIENT, ambientColor.toArray());
        glLightfv(openGLIndex, GL_DIFFUSE, diffuseColor.toArray());
        glLightfv(openGLIndex, GL_SPECULAR, specularColor.toArray());
	}//if isEnabled
	else
		glDisable(openGLIndex);
}//applyChanges
