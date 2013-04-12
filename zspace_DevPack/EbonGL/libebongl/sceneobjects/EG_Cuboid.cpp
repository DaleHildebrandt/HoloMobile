#include "EG_Cuboid.hpp"
#include "../EG_Utils.hpp"
#include "../EG_Polygon.hpp"
#include "../utilities/EG_LogWriter.hpp"
#include "../utilities/EG_String.hpp"

using namespace EbonGL;
using namespace std;
using namespace MHTypes;

EG_Cuboid::EG_Cuboid(void) : EG_Object()
{
    setSize(1, 1, 1);
}//constructor

EG_Cuboid::EG_Cuboid(const EG_Cuboid &source)
{
	copyFrom(source);
}//copy constructor

EG_Cuboid::~EG_Cuboid(void)
{
    clearVertices();
}//destructor

EG_Cuboid& EG_Cuboid::operator= (const EG_Cuboid &source)
{
	if (this != &source)
		copyFrom(source);

    return *this;
}//=

EG_Object* EG_Cuboid::generateInstance(void)
{
	return new EG_Cuboid(*this);
}//generateInstance

void EG_Cuboid::copyFrom(const EG_Object &source)
{
	EG_Polygon *newPoly = NULL;//the new polygon to create
	const EG_Cuboid *cubSource = dynamic_cast<const EG_Cuboid*>(&source);

	//### Call parent's copyFrom ###
	EG_Object::copyFrom(source);

	//### Verify that source is a Cuboid ###
	if (cubSource == NULL)
	{
		EG_Utils::writeLog(EG_String("EG_Cuboid::copyFrom(): couldn't cast to Cuboid*"));
		EG_Utils::fatalError();
	}//if cubSource

	//### Copy vertices ###
	clearVertices();//clears thePolygons
	for (unsigned int i = 0; i < cubSource->vertices.size(); i++)
		vertices.push_back(cubSource->vertices.at(i));

	//### Copy polygons ###
	for (unsigned int i = 0; i < cubSource->thePolygons.size(); i++)
	{
		newPoly = new EG_Polygon(*(cubSource->thePolygons.at(i)));

		newPoly->setVertices(vertices);
		thePolygons.push_back(newPoly);
	}//for i

	//### Copy simple variables ###
    width = cubSource->width;
    height = cubSource->height;
    depth = cubSource->depth;
}//copyFrom

void EG_Cuboid::draw(int glContextID)
{
    if (isVisible)
    {
        //### Set object position, orientation, properties ###
        startDrawing(false);

        //### Enable the vertex array ###
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_DOUBLE, 0, &vertices.at(0));

        //### Draw each face ###
        for (unsigned int i = 0; i < thePolygons.size(); i++)
            thePolygons.at(i)->draw(GL_POLYGON);

        //### Finish drawing ###
        glDisableClientState(GL_VERTEX_ARRAY);
        finishDrawing();
    }//if isVisible
}//draw

void EG_Cuboid::drawShadows(EG_Light *light,
                            EG_Object *surfaceObject)
{
    const EG_Polygon *surface = NULL;//the current surface to shadow

    if (isVisible)
    {
        if (surfaceObject != this)
        {
            //### Enable the vertex array ###
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_DOUBLE, 0, &vertices.at(0));

            //### Loop through each surface ###
            for (unsigned int i = 0; i < surfaceObject->numPolygons(); i++)
            {
                surface = surfaceObject->getPolygon(i);

                //Set the shadow transformation and color
                if (startShadow(light, surfaceObject, surface))
                {
                    //Draw each face onto the surface
                    for (unsigned int i = 0; i < thePolygons.size(); i++)
                        thePolygons.at(i)->draw(GL_POLYGON);

                    //Finish drawing to this surface
                    finishShadow();
                }//if startShadow
            }//for i

            glDisableClientState(GL_VERTEX_ARRAY);
        }//if surfaceObject
    }//if isVisible
}//drawShadows

unsigned int EG_Cuboid::numPolygons(void)
{
    return static_cast<unsigned int>(thePolygons.size());
}//numPolygons

const EG_Polygon* EG_Cuboid::getPolygon(unsigned int index)
{
    return thePolygons.at(index);
}//getPolygon

void EG_Cuboid::setSize(GLdouble newWidth,
                     GLdouble newHeight,
                     GLdouble newDepth)
{
    EG_Polygon *oneFace = NULL;//a single cube face to store
    GLdouble xDist = 0;//the distances from the origin to draw the vertices
    GLdouble yDist = 0;
    GLdouble zDist = 0;

	setChanged();

    //### Verify that the size is valid ###
    if ((newWidth <= 0) || (newHeight <= 0) || (newDepth <= 0))
    {
		EG_Utils::writeLog(EG_String("EG_Cuboid::setSize(): got a dimension <= zero"));
        EG_Utils::fatalError();
    }//if newWidth || newHeight || newDepth

    //### Assign the new dimensions ###
    width = newWidth;
    height = newHeight;
    depth = newDepth;

    //### Find the distance to each vertex ###
    xDist = width / 2;
    yDist = height / 2;
    zDist = depth / 2;

    //### Delete thePolygons and clear vertices ###
    clearVertices();

    //### Define the front face ###
    EG_Utils::addVertex(vertices, -xDist, -yDist, -zDist);
    EG_Utils::addVertex(vertices, -xDist,  yDist, -zDist);
    EG_Utils::addVertex(vertices,  xDist,  yDist, -zDist);
    EG_Utils::addVertex(vertices,  xDist, -yDist, -zDist);

    //### Define the back face ###
    EG_Utils::addVertex(vertices, -xDist, -yDist, zDist);
    EG_Utils::addVertex(vertices, -xDist,  yDist, zDist);
    EG_Utils::addVertex(vertices,  xDist,  yDist, zDist);
    EG_Utils::addVertex(vertices,  xDist, -yDist, zDist);

    //### Store the front face ###
    oneFace = new EG_Polygon(vertices);
    oneFace->addIndex(EGCV_FRONT_BOTTOM_LEFT);
    oneFace->addIndex(EGCV_FRONT_TOP_LEFT);
    oneFace->addIndex(EGCV_FRONT_TOP_RIGHT);
    oneFace->addIndex(EGCV_FRONT_BOTTOM_RIGHT);
    thePolygons.push_back(oneFace);

    //### Store the back face ###
    oneFace = new EG_Polygon(vertices);
    oneFace->addIndex(EGCV_BACK_BOTTOM_LEFT);
    oneFace->addIndex(EGCV_BACK_BOTTOM_RIGHT);
    oneFace->addIndex(EGCV_BACK_TOP_RIGHT);
    oneFace->addIndex(EGCV_BACK_TOP_LEFT);
    thePolygons.push_back(oneFace);

    //### Store the top face ###
    oneFace = new EG_Polygon(vertices);
    oneFace->addIndex(EGCV_FRONT_TOP_LEFT);
    oneFace->addIndex(EGCV_BACK_TOP_LEFT);
    oneFace->addIndex(EGCV_BACK_TOP_RIGHT);
    oneFace->addIndex(EGCV_FRONT_TOP_RIGHT);
    thePolygons.push_back(oneFace);

    //### Store the bottom face ###
    oneFace = new EG_Polygon(vertices);
    oneFace->addIndex(EGCV_FRONT_BOTTOM_LEFT);
    oneFace->addIndex(EGCV_FRONT_BOTTOM_RIGHT);
    oneFace->addIndex(EGCV_BACK_BOTTOM_RIGHT);
    oneFace->addIndex(EGCV_BACK_BOTTOM_LEFT);
    thePolygons.push_back(oneFace);

    //### Store the right face ###
    oneFace = new EG_Polygon(vertices);
    oneFace->addIndex(EGCV_FRONT_TOP_RIGHT);
    oneFace->addIndex(EGCV_BACK_TOP_RIGHT);
    oneFace->addIndex(EGCV_BACK_BOTTOM_RIGHT);
    oneFace->addIndex(EGCV_FRONT_BOTTOM_RIGHT);
    thePolygons.push_back(oneFace);

    //### Store the left face ###
    oneFace = new EG_Polygon(vertices);
    oneFace->addIndex(EGCV_FRONT_TOP_LEFT);
    oneFace->addIndex(EGCV_FRONT_BOTTOM_LEFT);
    oneFace->addIndex(EGCV_BACK_BOTTOM_LEFT);
    oneFace->addIndex(EGCV_BACK_TOP_LEFT);
    thePolygons.push_back(oneFace);
}//setSize

void EG_Cuboid::clearVertices(void)
{
    for (unsigned int i = 0; i < thePolygons.size(); i++)
        delete thePolygons.at(i);
    thePolygons.clear();

    vertices.clear();
}//clearVertices

Point3D EG_Cuboid::getVertex(unsigned int index)
{
    return EG_Utils::getVertex(vertices, index);
}//getVertex

GLdouble EG_Cuboid::getWidth(void)
{
    return width;
}//getWidth

GLdouble EG_Cuboid::getHeight(void)
{
    return height;
}//getHeight

GLdouble EG_Cuboid::getDepth(void)
{
    return depth;
}//getDepth

bool EG_Cuboid::inside(const Point3D &thePoint)
{
    GLdouble xDist = 0;//the distance from the center to the corresponding face
    GLdouble yDist = 0;
    GLdouble zDist = 0;
    bool result = false;//true if thePoint is inside of this cuboid

    //### Calculate the distance from the center to the corresponding face ###
    if (width != 0)
        xDist = width / static_cast<double>(2);

    if (height != 0)
        yDist = height / static_cast<double>(2);

    if (depth != 0)
        zDist = depth / static_cast<double>(2);

    //### Perform collision detection ###
    if (thePoint.x >= position.x - xDist)
    {
        if (thePoint.x <= position.x + xDist)
        {
            if (thePoint.y >= position.y - yDist)
            {
                if (thePoint.y <= position.y + yDist)
                {
                    if (thePoint.z >= position.z - zDist)
                    {
                        if (thePoint.z <= position.z + zDist)
                            result = true;
                    }//if z
                }//if y
            }//if y
        }//if x
    }//if x

    return result;
}//inside
