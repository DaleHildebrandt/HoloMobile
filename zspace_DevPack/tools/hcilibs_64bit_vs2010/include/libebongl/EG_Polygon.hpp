#ifndef EG_POLYGON_HPP_INCLUDED
#define EG_POLYGON_HPP_INCLUDED

#include <vector>
#include "EG_Settings.hpp"

namespace EbonGL
{
    class MH_INTERFACE EG_Polygon
    {
        public:
            //constructors
            EG_Polygon(void);
            EG_Polygon(std::vector<GLdouble> &newVertices);
			EG_Polygon(const EG_Polygon &source);

            //destructor
            ~EG_Polygon(void);

			//Overloaded = operator
            EG_Polygon& operator= (const EG_Polygon &source);

            //Set the polygon's vertices. EG_Polygon will save a pointer to
            //newVertices - be sure to delete this polygon before clearing or
			//deleting newVertices! newVertices should have three elements per vertex.
            void setVertices(std::vector<GLdouble> &newVertices);

            //Adds the vertex beginning at vertices[newIndex * 3] to this polygon.
            //Objects' outer faces should have their vertices arranged in a clockwise
            //direction. We use the first three vertices to calculate lighting effects,
            //and these should not all lie on the same line.
            void addIndex(GLushort newIndex);

            //Draw this polygon. Draw calculates surface normals, and the shape
            //should be flat (2D) for proper lighting. drawingType indicates the type
            //of drawing, eg GL_POLYGON, GL_LINES, etc.
            void draw(GLenum drawingType);

            //Returns the vertex at the specified index
            MHTypes::Point3D getVertex(unsigned int index) const;

            //The number of vertices in this polygon
            unsigned int getNumVertices(void) const;

        private:
            //A pointer to a list of vertices maintained by the containing
            //class
            std::vector<GLdouble> *vertices;

            //Each element points to the index in vertices with the x-coordinate for this
            //vertex. Must be multiplied by three before use.
            std::vector<GLushort> indexes;

			//Copies member variables from source into this instance
			void copyFrom(const EG_Polygon &source);

    };//polygon
}//EbonGL

#endif
