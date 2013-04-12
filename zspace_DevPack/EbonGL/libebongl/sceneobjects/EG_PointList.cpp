#include "EG_PointList.hpp"
#include "../utilities/EG_LogWriter.hpp"
#include "../utilities/EG_String.hpp"

using namespace std;
using namespace EbonGL;
using namespace MHTypes;

EG_PointList::EG_PointList(void) : EG_Object()
{
    numVertices = 0;
}//constructor

EG_PointList::EG_PointList(const EG_PointList &source)
{
	copyFrom(source);
}//copy constructor

EG_PointList::~EG_PointList(void)
{
}//destructor

EG_PointList& EG_PointList::operator= (const EG_PointList &source)
{
	if (this != &source)
		copyFrom(source);

    return *this;
}//=

EG_Object* EG_PointList::generateInstance(void)
{
	return new EG_PointList(*this);
}//generateInstance

void EG_PointList::copyFrom(const EG_Object &source)
{
	const EG_PointList *pointSource = dynamic_cast<const EG_PointList*>(&source);

	//### Call parent's copyFrom ###
	EG_Object::copyFrom(source);

	//### Verify that source is an EG_PointList ###
	if (pointSource == NULL)
	{
		EG_Utils::writeLog(EG_String("EG_PointList::copyFrom(): couldn't cast to EG_PointList*"));
		EG_Utils::fatalError();
	}//if pointSource

    //### Copy member variables ###
    numVertices = pointSource->numVertices;
    vertices.clear();
    for (unsigned int i = 0; i < pointSource->vertices.size(); i++)
        vertices.push_back(pointSource->vertices.at(i));
}//copyFrom

void EG_PointList::addPoint(const Point3D &newPoint)
{
    setChanged();

    vertices.push_back(static_cast<GLfloat>(newPoint.x));
    vertices.push_back(static_cast<GLfloat>(newPoint.y));
    vertices.push_back(static_cast<GLfloat>(newPoint.z));
    numVertices++;
}//addPoint

void EG_PointList::clearPoints(void)
{
    if (vertices.size() != 0)
    {
        setChanged();
        vertices.clear();
        numVertices = 0;
    }//if size()
}//clearPoints

void EG_PointList::draw(int glContextID)
{
    if (isVisible)
    {
        //### Set object position, orientation, properties ###
        startDrawing(false);

        //### Enable the vertex array ###
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, &vertices.at(0));

        //### Draw the points ###
        glDrawArrays(GL_POINTS, 0, numVertices);

        //### Finish drawing ###
        glDisableClientState(GL_VERTEX_ARRAY);
        finishDrawing();
    }//if isVisible
}//draw

void EG_PointList::drawShadows(EG_Light *light,
                               EG_Object *surfaceObject)
{
}//drawShadows

unsigned int EG_PointList::numPolygons(void)
{
    return 0;
}//numPolygons

const EG_Polygon* EG_PointList::getPolygon(unsigned int index)
{
    return NULL;
}//getPolygon
