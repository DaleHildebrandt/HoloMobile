/* DESCRIPTION
    A simple sphere that can be drawn in EG_Engine.
*/

#ifndef EG_SPHERE_HPP_INCLUDED
#define EG_SPHERE_HPP_INCLUDED

#include "EG_Object.hpp"
#include "../EG_Settings.hpp"
#include <vector>

namespace EbonGL
{
    class MH_INTERFACE EG_Sphere : public EG_Object
    {
        public:
            //constructors
            EG_Sphere(void);
			EG_Sphere(GLdouble newRadius, 
                      GLint newSlices, 
                      GLint newStacks);
			
			//copy constructor
			EG_Sphere(const EG_Sphere &source);

            //destructor
            ~EG_Sphere(void);

			//Overloaded = operator
            EG_Sphere& operator= (const EG_Sphere &source);

			//Returns a copy of this Sphere
			EG_Object* generateInstance(void);

            //Draw this Sphere in the specified OpenGL context
            void draw(int glContextID);

            //Does nothing, since EbonGL's shadows don't scale to curved surfaces
            void drawShadows(EG_Light *light,
                             EG_Object *surfaceObject);

            //Always returns 0.
            unsigned int numPolygons(void);

            //Always returns NULL.
            const EG_Polygon* getPolygon(unsigned int index);
            
            //Accessors
            GLdouble getRadius(void);
            GLint getSlices(void);
            GLint getStacks(void);

			//############### Functions That Set isChanged (Mutators) ###############

            //Set the sphere size in scene units
            void setRadius(GLdouble newRadius);
            
            //The number of subdivisions along the z-axis (longitude)
            void setSlices(GLint newSlices);
            
            //The number of subdivisions along the z-axis (latitude)
            void setStacks(GLint newStacks);
            
        protected:        
			//Copy member variables from source into this object
			void copyFrom(const EG_Object &source);

		private:
            //Describes how the sphere is drawn
            GLUquadricObj* quad;

			//The radius of the sphere in scene coordinates
            GLdouble radius;
            
			//The number of subdivisions around the Z axis 
			//(similar to lines of longitude).
			GLint slices;
			
			//The number of subdivisions along the Z axis 
			//(similar to lines of latitude)
			GLint stacks;
			
            //########### Functions #################

            void initQuad(void);

    };//class EG_Sphere
}//namespace EbonGL

#endif
