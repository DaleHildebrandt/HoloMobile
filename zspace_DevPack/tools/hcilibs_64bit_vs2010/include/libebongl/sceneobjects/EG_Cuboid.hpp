#ifndef EG_CUBOID_HPP_INCLUDED
#define EG_CUBOID_HPP_INCLUDED

#include "EG_Object.hpp"
#include <vector>

namespace EbonGL
{
    class EG_Polygon;

    //The indexes of the vertices for a cube in our OpenGL vertex array
    enum EG_CubeVertices
    {
        EGCV_FRONT_BOTTOM_LEFT = 0,
        EGCV_FRONT_TOP_LEFT = 1,
        EGCV_FRONT_TOP_RIGHT = 2,
        EGCV_FRONT_BOTTOM_RIGHT = 3,
        EGCV_BACK_BOTTOM_LEFT = 4,
        EGCV_BACK_TOP_LEFT = 5,
        EGCV_BACK_TOP_RIGHT = 6,
        EGCV_BACK_BOTTOM_RIGHT = 7
    };//EG_CubeVertices

    class MH_INTERFACE EG_Cuboid : public EG_Object
    {
        public:
            //constructor
            EG_Cuboid(void);

			//copy constructor
			EG_Cuboid(const EG_Cuboid &source);

            //destructor
            ~EG_Cuboid(void);

			//Overloaded = operator
            EG_Cuboid& operator= (const EG_Cuboid &source);

			//Returns a copy of this Cuboid
			virtual EG_Object* generateInstance(void);

            //Draw this cuboid in the specified OpenGL context.
            void draw(int glContextID);

            //Draws shadows onto the specified object's surfaces
            void drawShadows(EG_Light *light,
                             EG_Object *surfaceObject);

            //Returns the number of polygons being used
            unsigned int numPolygons(void);

            //Returns the polygon at the specified index
            const EG_Polygon* getPolygon(unsigned int index);

            //Returns true if thePoint is inside of this cuboid. Doesn't work if
            //this cuboid is rotated away from (0, 0, 0)
            bool inside(const MHTypes::Point3D &thePoint);

            //Returns a vertex at the specified index from this cuboid.
            //(There are 8 vertices in a cuboid).
            MHTypes::Point3D getVertex(unsigned int index);

            //accessors
            GLdouble getWidth(void);
            GLdouble getHeight(void);
            GLdouble getDepth(void);

			//############### Functions That Set isChanged ###############

            //Set the cuboid dimensions
            void setSize(GLdouble newWidth,
                         GLdouble newHeight,
                         GLdouble newDepth);

        protected:
            //the vertices making up the cube
            std::vector<GLdouble> vertices;

            //Each element is a side of the cube
            std::vector<EG_Polygon*> thePolygons;

            //The cuboid's dimensions
            GLdouble width;
            GLdouble height;
            GLdouble depth;

			//############### FUNCTIONS ###############

            //Deletes every item in thePolygons and clears it, and clears vertices
            void clearVertices(void);

			//Copy member variables from source into this object
			virtual void copyFrom(const EG_Object &source);

    };//class EG_Cuboid
}//namespace EbonGL

#endif
