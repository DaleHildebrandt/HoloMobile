#ifndef EG_OBJECT_HPP_INCLUDED
#define EG_OBJECT_HPP_INCLUDED

#include <vector>
#include <iostream>
#include "MathHelper.hpp"

#include "../EG_Color.hpp"
#include "../utilities/EG_BoolVector.hpp"

namespace EbonGL
{
    class EG_Light;
    class EG_Polygon;
	class EG_SceneElement;
    class EG_Color;

    class MH_INTERFACE EG_Object
    {
        public:
            //constructor
            EG_Object(void);

			//copy constructor
			EG_Object(const EG_Object &source);

            //destructor
            virtual ~EG_Object(void);

			//Overloaded = operator
            EG_Object& operator= (const EG_Object &source);

            //Draw this object in the specified OpenGL context. Should be set to zero
            //if there's only one context
            virtual void draw(int glContextID) = 0;

            //Draws shadows onto the specified object's surfaces
            virtual void drawShadows(EG_Light *light,
                                     EG_Object *surfaceObject) = 0;

            //Returns the number of polygons used by this object, 0 for points
            virtual unsigned int numPolygons(void) = 0;

            //Returns the polygon at the specified index
            virtual const EG_Polygon* getPolygon(unsigned int index) = 0;

			//Returns a copy of this object with the proper child type
			virtual EG_Object* generateInstance(void) = 0;

			//Accessors
            MHTypes::Point3D getPosition(void);
			MHTypes::EulerAngle getRotation(void);
			EG_Color getAmbientDiffuse(void);

			//Sets all elements in isChanged to true, reporting that no viewport has an up-to-date
			//copy of this object.
			void setChanged(void);

			//If true, then this object has been changed and should be updated the Engine's
			//thread-safe copies. Sets isChanged to false. Should only be called by Engine.
			bool getChanged(unsigned int viewportIndex);

			//Returns a pointer to our positions in the scene object lists
			EbonGL::EG_SceneElement* getElement(void);

			//############### Functions That Set isChanged ###############

            //Show or hide this object
            void show(void);
            void hide(void);

            //Set to true to allow this object to cast shadows, false otherwise
            void setShadowCasting(bool enabled);

            //Set to true to cause this object to become shadowed, false otherwise. Rotated objects
			//currently don't receive shadows properly.
            void setShadowReceiving(bool enabled);

            //Set the object's position in scene coordinates
            void setPosition(const MHTypes::Point3D &newPosition);

            //Set the object's rotation, in degrees
            void setRotation(const MHTypes::EulerAngle &newRotation);

            //Set this object's color
            void setAmbientDiffuse(const EG_Color &newColor);

            //Set the mode in which polygon faces will be drawn.
            //GL_POINT, GL_LINE or GL_FILL
            void setPolygonMode(GLenum newMode);

			//Set to true to have this object use light sources, set to false to have it always
			//drawn at 100% brightness
			void setLighting(bool enabled);

			//############### Static Functions ###############

            //Sets shadowStamper to 0, should be done before shadows are drawn in paintGL
            static void resetShadowStamper(void);

            //Applies a matrix for projecting shadows onto the specified surface, from the specified light source
            static void shadowProjection(const MHTypes::Point3D &lightPos,
                                         const MHTypes::Point3D &surfacePoint,
                                         const MHTypes::MHVector &surfaceNormal);

            //Returns the GLenum for the matching clipping plane. Results take the form
            //GL_CLIP_PLANE?, where ? is the specified index.
            static GLenum getGlClipPlane(unsigned int index);

            //Offset polygons to avoid flickering, amount is in scene coordinates
            static const double POLYGON_OFFSET;

        protected:
            //The object's ambient and diffuse colors. There are four elements,
            //one for each of R, G, B and A
            EG_Color *ambientDiffuse;

            //The object's center coordinates
            MHTypes::Point3D position;

            //This object's orientation
            MHTypes::EulerAngle rotation;

            //The polygon drawing mode, use to specify filled polygons,
            //lines only, etc.
            GLenum polygonMode;
           
            //Some GLUT shapes aren't drawn centered on the x, y, z coordinates
            //Child classes can specify the z offset to use to properly center
            //the object.
            MHTypes::MHFloat zOffset;

            //True if this object should be drawn, false otherwise
            bool isVisible;

            //############### Protected Functions ###############

            //Should be called by child classes before doing any drawing. Sets the object
            //color, position and orientation
            void startDrawing(bool enableTextures);

            //Should be called after drawing is complete. Restores the matrix stack to its
            //previous position and orientation
            void finishDrawing(void);

            //Same as above, but for drawing shadows on a surface. Returns false if the surface
            //normal points away from the shadow-casting object.
            bool startShadow(EG_Light *light,
                             EG_Object *surfaceObject,
                             const EG_Polygon *surface);
            void finishShadow(void);

			//Copy member variables from source into this object
			virtual void copyFrom(const EG_Object &source);

        private:
            //rotated and translated vertices from the surface to shadow
            std::vector<MHTypes::Point3D*> shadowPlane;

			//Contains iterators to our position within the engine's scene, used to quickly remove
			//this object.
			EbonGL::EG_SceneElement *scenePos;

			//If true, the Engine should update its copy of this object for use in thread-safe
			//drawing. There's one value for every viewport handled by the system, each of which
			//has its own thread.
			EG_BoolVector isChanged;

            //We'll change the offset for each object casting shadows, to avoid flicker
            static unsigned int shadowStamper;

            //The number of clipping planes that were created by startShadow, and should be disabled by
            //finishShadow
            unsigned int clipPlanesUsed;

            //Set to true when startDrawing is called, set to false when finishDrawing is called.
            //Makes sure each call to startDrawing is matched with a call to finishDrawing.
            bool drawingActive;

            //Ensure that each call to startShadow is matched with a call to finishShadow
            bool shadowActive;

            //True if this object can cast shadows, false otherwise
            bool shadowCasting;

            //True if this object reflects shadows, false otherwise
            bool shadowReceiving;

			//True if this object should use light from light sources, false if it's always at 100% brightness
			bool enableLighting;

            //############### Private Functions ###############

            //Clip shadows that reach past the surface's edges
            void addShadowClipping(EG_Object *surfaceObject,
                                   const EG_Polygon *surface,
                                   const MHTypes::MHVector &normal);

            //populates shadowPlane with the vertices making up a plane to project a shadow onto
            void calcShadowPlane(const EG_Polygon *surface,
                                 EG_Object *shadowObject);

    };//class Object
}//namespace EbonGL

#endif
