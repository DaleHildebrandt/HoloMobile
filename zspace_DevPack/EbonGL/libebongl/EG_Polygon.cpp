#include "EG_Polygon.hpp"
#include "EG_Utils.hpp"

using namespace EbonGL;
using namespace std;
using namespace MHTypes;

EG_Polygon::EG_Polygon(void)
{
    vertices = NULL;
}//constructor

EG_Polygon::EG_Polygon(vector<GLdouble> &newVertices)
{
    vertices = &newVertices;
}//constructor

EG_Polygon::EG_Polygon(const EG_Polygon &source)
{
    copyFrom(source);
}//copy constructor

EG_Polygon::~EG_Polygon(void)
{
    vertices = NULL;
}//destructor

EG_Polygon& EG_Polygon::operator= (const EG_Polygon &source)
{
    if (this != &source)
        copyFrom(source);

    return *this;
}//=

void EG_Polygon::copyFrom(const EG_Polygon &source)
{
	indexes.clear();
	for (unsigned int i = 0; i < source.indexes.size(); i++)
		indexes.push_back(source.indexes.at(i));

	vertices = source.vertices;
}//copyFrom

void EG_Polygon::setVertices(std::vector<GLdouble> &newVertices)
{
    vertices = &newVertices;
}//setVertices

void EG_Polygon::addIndex(GLushort newIndex)
{
    indexes.push_back(newIndex);
}//addIndex

void EG_Polygon::draw(GLenum drawingType)
{
    MHVector normal;//the surface normal vector, used for lighting
    Plane normalPlane;//plane to calculate the surface normal from
    vector<Point3D*> surfacePoints;//three points from the surface, defining a plane
    Point3D *onePoint = NULL;//a single point to add to surfacePoints

    //### Verify that we have a valid polygon ###
    if ((indexes.size() >= 3) && (vertices != NULL))
    {
        //### Extract three points from the vertexList ###
        for (unsigned int i = 0; i < 3; i++)
        {
            onePoint = new Point3D(getVertex(i));

            surfacePoints.push_back(onePoint);
            onePoint = NULL;
        }//for i

        //### Calculate the surface normal vector ###
        normalPlane.fromPoints(*surfacePoints.at(0), *surfacePoints.at(1), *surfacePoints.at(2));
        normal = normalPlane.surfaceNormal();
        glNormal3d(normal.x, normal.y, normal.z);

        //### Draw the polygon ###
        glDrawElements(drawingType, static_cast<GLsizei>(indexes.size()), GL_UNSIGNED_SHORT, &indexes.at(0));

        //### Free memory ###
        for (unsigned int i = 0; i < surfacePoints.size(); i++)
            delete surfacePoints.at(i);
        surfacePoints.clear();
    }//if size() && vertices
}//draw

Point3D EG_Polygon::getVertex(unsigned int index) const
{
    return EG_Utils::getVertex(*vertices, indexes.at(index));
}//getVertex

unsigned int EG_Polygon::getNumVertices(void) const
{
    return static_cast<unsigned int>(indexes.size());
}//getNumVertices
