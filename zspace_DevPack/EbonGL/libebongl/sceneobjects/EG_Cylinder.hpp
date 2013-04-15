#ifndef EG_CYLINDER_HPP_INCLUDED
#define EG_CYLINDER_HPP_INCLUDED

#include "EG_Object.hpp"
#include "../EG_Settings.hpp"

namespace EbonGL
{

    class MH_INTERFACE EG_Cylinder : public EG_Object
    {
        public:
            //void constructor
            EG_Cylinder(void);

			//reg. constructor
			EG_Cylinder(GLdouble radius, GLdouble height, GLint slices, GLint stacks);
			
			//glu like constructor
			EG_Cylinder(GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks);
			
			//copy constructor
			EG_Cylinder(const EG_Cylinder &source);

            //destructor
            ~EG_Cylinder(void);

			//Overloaded = operator
            EG_Cylinder& operator= (const EG_Cylinder &source);

			//Returns a copy of this Cylinder [Required]
			EG_Object* generateInstance(void);

            //Draw this Cylinder in the specified OpenGL context. [Required]
            void draw(int glContextID);

            //Draws shadows onto the specified object's surfaces [Required]
            void drawShadows(EG_Light *light,
                             EG_Object *surfaceObject);

            //Returns the number of polygons being used [Required]
            unsigned int numPolygons(void);

            //Returns the polygon at the specified index [Required]
            const EG_Polygon* getPolygon(unsigned int index);

            //################ Basic Accessor Methods ##################
            
            //get sphere characteristics
            GLdouble getBase(void);
            
            GLdouble getTop(void);
            
            GLdouble getHeight(void);
            
            GLint getSlices(void);
            
            GLint getStacks(void);

			//############### Functions That Set isChanged (Mutators) ###############

            //Set the sphere size & shape
            void setBase(GLdouble newBase);
            
            void setTop(GLdouble newTop);
            
            void setHeight(GLdouble newHeight);
            
            void setSlices(GLint newSlices);
            
            void setStacks(GLint newStacks);
            

        protected:
        
			//############### FUNCTIONS ###############
			//Copy member variables from source into this object
			void copyFrom(const EG_Object &source);

		private:
			//########## Variables ############

            //Describes how the sphere is drawn
            GLUquadricObj* quad;

			//Radius of the base of the Cylinder
			GLdouble base;
			
			//Radius of the top of the Cylinder
			GLdouble top;
            
            //The height of the Cylinder
            GLdouble height;
            
			//The number of subdivisions around the Z axis 
			//(similar to lines of longitude).
			GLint slices;
			
			//The number of subdivisions along the Z axis 
			//(similar to lines of latitude)
			GLint stacks;
			
            //###########Functions#################
            void initQuad(void);
	};//class EG_Cylinder
}//namespace EbonGL

#endif
