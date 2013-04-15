#ifndef EG_PointListLIST_HPP_INCLUDED
#define EG_PointListLIST_HPP_INCLUDED

#include "EG_Object.hpp"

namespace EbonGL
{
    class MH_INTERFACE EG_PointList : public EG_Object
    {
        public:
            //constructor
            EG_PointList(void);

			//copy constructor
			EG_PointList(const EG_PointList &source);

            //destructor
            ~EG_PointList(void);

			//Overloaded = operator
            EG_PointList& operator= (const EG_PointList &source);

			//Returns a copy of this EG_PointList
			virtual EG_Object* generateInstance(void);

            //Draw this point in the specified OpenGL context.
            void draw(int glContextID);

            //Shadow casting not implemented for points.
            void drawShadows(EG_Light *light,
                             EG_Object *surfaceObject);

            //Returns the number of polygons being used - will always be 0.
            unsigned int numPolygons(void);

            //Returns NULL - no polygons in a point.
            const EG_Polygon* getPolygon(unsigned int index);

            //Add a point to the list
            void addPoint(const MHTypes::Point3D &newPoint);

            //Clear all points from the list
            void clearPoints(void);

        private:
            //the vertices making up the points
            std::vector<GLfloat> vertices;

            //The number of vertices being stored, will be equal to vertices.size() / 3
            unsigned int numVertices;

            //############### FUNCTIONS ###############

            //Set this point to be a copy of source
            void copyFrom(const EG_Object &source);

    };//class EG_PointList
}//namespace EbonGL

#endif
