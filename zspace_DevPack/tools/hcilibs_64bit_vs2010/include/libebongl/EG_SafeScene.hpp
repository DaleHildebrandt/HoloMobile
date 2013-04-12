#ifndef EG_SAFESCENE_HPP_INCLUDED
#define EG_SAFESCENE_HPP_INCLUDED

#include "EG_Utils.hpp"
#include "utilities/EG_Mutex.hpp"

namespace EbonGL
{
	class EG_Object;
    class EG_Light;

	class EG_SafeScene
	{
		public:
			//constructor
			EG_SafeScene(unsigned int newIndex,
				         const std::vector<EbonGL::EG_Light*> &newLights);

			//destructor
			~EG_SafeScene(void);

			//Returns false if this scene is available for drawing, true if another thread
			//has already claimed it. If it returns false, it will start returning true the next
			//time it's called, until releaseScene is called.
			bool getInUse(void);

			//Indicate that this scene can be used by another thread.
			void releaseScene(void);

			//delete all items contained in the scene
			void clearScene(void);

			//Add or remove an object from the scene, should be called by Engine whenever the equivalent
			//Engine function is called;
			std::list<EG_Object*>::iterator addSolid(EbonGL::EG_Object *newObject);
			std::list<EG_Object*>::iterator addTransparent(EbonGL::EG_Object *newObject);
			void remove(std::list<EG_Object*>::iterator objectIter,
					    bool isSolid);

			//Copy any changes from the master copy of the scene to this object
			void update(std::list<EbonGL::EG_Object*> &newSolid,
						std::list<EbonGL::EG_Object*> &newTransparent,
						const std::vector<EbonGL::EG_Light*> &newLights);

			//Draw the scene in the specified OpenGL context, thread-safe.
			void draw(int glContextID);

            //Set the 4x4 navigation matrix representing the camera's viewpoint.
            void setNavMatrix(const std::vector<GLfloat> &newNavMatrix);

            //Enable or disable shadows. Shadows can be disabled to improve performance when there are many
            //objects in the scene.
            void setShadows(bool enabled);

		private:
			//Non-transparent objects that belong to the scene
			std::list<EbonGL::EG_Object*> solidObjects;

			//Transparent objects need to be drawn after solid objects
			std::list<EbonGL::EG_Object*> transparentObjects;

			//The lights available through OpenGL
			std::vector<EbonGL::EG_Light*> lights;

            //A 4x4 navigation matrix, used to move the camera through the virtual world
            //Has a size of zero if not in use, we'll default to the identity matrix.
            std::vector<GLfloat> navMatrix;

			//locked by draw, addSolid, addTransparent and remove. Prevents users from
			//modifying the SafeScene during drawing operations.
			EbonGL::EG_Mutex drawMutex;

			//A unique index for this scene, used to report that we're up-to-date in Object
			unsigned int sceneIndex;

			//True if this scene is being used for a drawing operation, false otherwise
			volatile bool inUse;

            //True if we should draw shadows, false otherwise
            volatile bool shadowsEnabled;

			//############### FUNCTIONS ###############

			//Update the objects in dest to contain any changes that are present in source
			void updateObjects(std::list<EbonGL::EG_Object*> &source,
							   std::list<EbonGL::EG_Object*> &dest);

			//Draws the shadows cast by the specified object
			void drawShadows(EbonGL::EG_Object *shadowObject);

	};//SafeScene
}//namespace EbonGL

#endif
