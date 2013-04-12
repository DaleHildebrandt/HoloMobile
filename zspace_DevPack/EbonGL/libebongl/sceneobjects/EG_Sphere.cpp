#include "EG_Sphere.hpp"
#include "../EG_Utils.hpp"
#include "../utilities/EG_LogWriter.hpp"
#include "../utilities/EG_String.hpp"

using namespace EbonGL;
using namespace std;
using namespace MHTypes;

EG_Sphere::EG_Sphere(void) : EG_Object()
{
	radius = 5;
	slices = 10;
	stacks = 10;

    initQuad();
}//Default constructor

EG_Sphere::EG_Sphere(GLdouble newRadius, 
                     GLint newSlices, 
                     GLint newStacks) : EG_Object()
{
	radius = newRadius;
	slices = newSlices;
	stacks = newStacks;

    initQuad();
}//Normal constructor

EG_Sphere::EG_Sphere(const EG_Sphere &source)
{
    initQuad();
	copyFrom(source);
}//copy constructor

EG_Sphere::~EG_Sphere(void)
{
    gluDeleteQuadric(quad);
}//empty destructor

EG_Sphere& EG_Sphere::operator= (const EG_Sphere &source)
{
	if (this != &source)
		copyFrom(source);

    return *this;
}//=

void EG_Sphere::initQuad(void)
{
    quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
}

EG_Object* EG_Sphere::generateInstance(void)
{
	return new EG_Sphere(*this);
}//generateInstance

void EG_Sphere::copyFrom(const EG_Object &source)
{
	const EG_Sphere *sphereSource = dynamic_cast<const EG_Sphere*>(&source);

	//### Call parent's copyFrom ###
	EG_Object::copyFrom(source);

	//### Verify that source is a Sphere ###
	if (sphereSource == NULL)
	{
		EG_Utils::writeLog(EG_String("EG_Sphere::copyFrom(): couldn't cast to Sphere*"));
		EG_Utils::fatalError();
	}//if sphereSource

	//### Copy simple variables ###
    radius = sphereSource->radius;
    slices = sphereSource->slices;
    stacks = sphereSource->stacks;
}//copyFrom

void EG_Sphere::draw(int glContextID)
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
            EG_Utils::writeLog(EG_String("EG_Sphere::draw Unknown drawStyle"));
            EG_Utils::fatalError();
        }//else polygonMode

        gluQuadricDrawStyle(quad, drawStyle);
		gluSphere(quad, radius, slices, stacks);

		//finish drawing step
        finishDrawing();
        
    }//if isVisible
}//draw

void EG_Sphere::drawShadows(EG_Light *light,
                            EG_Object *surfaceObject)
{
    //not implemented
}//drawShadows

unsigned int EG_Sphere::numPolygons(void)
{
    return 0;
}//numPolygons

const EG_Polygon* EG_Sphere::getPolygon(unsigned int index)
{
    return NULL;
}//getPolygon

GLdouble EG_Sphere::getRadius(void)
{
    return radius;
}//getRadius

GLint EG_Sphere::getSlices(void)
{
    return slices;
}//getSlices

GLint EG_Sphere::getStacks(void)
{
    return stacks;
}//getStacks

void EG_Sphere::setRadius(GLdouble newRadius)
{
	radius = newRadius;
	setChanged();
}//setRadius

void EG_Sphere::setSlices(GLint newSlices)
{
	slices = newSlices;
	setChanged();
}//setSlices

void EG_Sphere::setStacks(GLint newStacks)
{
	stacks = newStacks;
	setChanged();
}//setStacks
