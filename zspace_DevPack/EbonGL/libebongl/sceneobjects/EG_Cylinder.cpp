#include "EG_Cylinder.hpp"
#include "../EG_Utils.hpp"
#include "../utilities/EG_LogWriter.hpp"
#include "../utilities/EG_String.hpp"

using namespace EbonGL;
using namespace std;

EG_Cylinder::EG_Cylinder(void) : EG_Object()
{
	base = 0.5;
	top = 0.5;
	height = 5;
    zOffset = height / -2.0;
	slices = 20;
	stacks = 20;

    initQuad();
}//Default constructor

EG_Cylinder::EG_Cylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks) : EG_Object()
{
	this->base = radius;
	this->top = radius;
	this->height = height;
    if (height > 0.00001)
        zOffset = height / -2.0;

	this->slices = slices;
	this->stacks = stacks;

    initQuad();
}//Normal constructor

EG_Cylinder::EG_Cylinder(GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks) : EG_Object()
{
	this->base = base;
	this->top = top;
	this->height = height;
    if (height > 0.00001)
        zOffset = height / -2.0;

	this->slices = slices;
	this->stacks = stacks;

    initQuad();
}//glu like constructor

EG_Cylinder::EG_Cylinder(const EG_Cylinder &source)
{
    initQuad();
	copyFrom(source);
}//copy constructor

EG_Cylinder::~EG_Cylinder(void)
{
    gluDeleteQuadric(quad);
}//empty destructor

EG_Cylinder& EG_Cylinder::operator= (const EG_Cylinder &source)
{
	if (this != &source)
		copyFrom(source);

    return *this;
}//=

void EG_Cylinder::initQuad(void)
{
    quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
}

EG_Object* EG_Cylinder::generateInstance(void)
{
	return new EG_Cylinder(*this);
}//generateInstance

void EG_Cylinder::copyFrom(const EG_Object &source)
{
	const EG_Cylinder *cylSource = dynamic_cast<const EG_Cylinder*>(&source);

	//### Call parent's copyFrom ###
	EG_Object::copyFrom(source);

	//### Verify that source is a Cylinder ###
	if (cylSource == NULL)
	{
		EG_Utils::writeLog(EG_String("EG_Cylinder::copyFrom(): couldn't cast to Cylinder*"));
		EG_Utils::fatalError();
	}//if cylSource

	//### Copy simple variables ###
	base = cylSource->base;
	top = cylSource->top;
    height = cylSource->height;
    slices = cylSource->slices;
    stacks = cylSource->stacks;
}//copyFrom

void EG_Cylinder::draw(int glContextID)
{
    if (isVisible)
    {
        GLenum drawStyle;

        //### Set object position, orientation, properties ###
        startDrawing(false);
		
		//call glut method
		//glutSolidSphere(radius, slices, stacks);

        if (polygonMode == GL_FILL)
            drawStyle = GLU_FILL;
        else if (polygonMode == GL_LINE)
            drawStyle = GLU_LINE;
        else if (polygonMode == GL_POINT)
            drawStyle = GLU_POINT;
        else
        {
            EG_Utils::writeLog(EG_String("EG_Cylinder::draw Unknown drawStyle"));
            EG_Utils::fatalError();
        }//else polygonMode

        gluQuadricDrawStyle(quad, drawStyle);
		gluCylinder(quad, base, top, height, slices, stacks);

		//finish drawing step
        finishDrawing();
        
    }//if isVisible
}//draw

void EG_Cylinder::drawShadows(EG_Light *light,
                              EG_Object *surfaceObject)
{
    //not implemented
}//drawShadows

unsigned int EG_Cylinder::numPolygons(void)
{
    return 0;
}//numPolygons

const EG_Polygon* EG_Cylinder::getPolygon(unsigned int index)
{
    return NULL;
}//getPolygon

GLdouble EG_Cylinder::getBase(void)
{
    return base;
}//getBase

GLdouble EG_Cylinder::getTop(void)
{
	return top;
}//getTop

GLdouble EG_Cylinder::getHeight(void)
{
	return height;
}//getHeight

GLint EG_Cylinder::getSlices(void)
{
    return slices;
}//getSlices

GLint EG_Cylinder::getStacks(void)
{
    return stacks;
}//getStacks

void EG_Cylinder::setBase(GLdouble newBase)
{
	base = newBase;
	setChanged();
}//setBase

void EG_Cylinder::setTop(GLdouble newTop)
{
	top = newTop;
	setChanged();
}//setTop

void EG_Cylinder::setHeight(GLdouble newHeight)
{
    if (newHeight > 0.00001)
        zOffset = newHeight / -2.0;

	height = newHeight;
	setChanged();
}//setHeight

void EG_Cylinder::setSlices(GLint newSlices)
{
	slices = newSlices;
	setChanged();
}//setSlices

void EG_Cylinder::setStacks(GLint newStacks)
{
	stacks = newStacks;
	setChanged();
}//setStacks

