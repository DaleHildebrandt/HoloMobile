#ifndef EG_ENGINEINT_HPP_INCLUDED
#define EG_ENGINEINT_HPP_INCLUDED

#include "EG_Utils.hpp"
#include "EG_Light.hpp"
#include "utilities/EG_Mutex.hpp"

namespace EbonGL
{
	class EG_SafeScene;
    class EG_Object;

	class EG_EngineInt
	{
		public:
			//constructor
			EG_EngineInt(void);

			//destructor
			~EG_EngineInt(void);

			//Call contextInit immediately after a new OGL context is created.
			//Initializes the GL state. (lights, shading, etc) and creates
			//context specific information.
			void contextInit(void);

			//Clears the current buffer before we begin drawing to it. If you're using
			//VRJuggler, it should be called by bufferPreDraw.
			void clearBuffer(void);

			//Re-draw the scene.
			void draw(int glContextID);

			//Add an object to the scene. It's up to you to call the proper function
			//for objects that will be solid or transparent, since transparent objects
			//must be drawn last to avoid graphical artifacts. The Engine takes ownership
			//of the objects, and will delete them when it is destroyed, or when clearScene()
			//is called.
			void addSolid(EG_Object *newObject);
			void addTransparent(EG_Object *newObject);

			//Removes theObject from the scene. Engine relinquishes ownership of theObject,
			//and the caller is responsible for deleting it.
			void remove(EG_Object *theObject);

			//Set the intensity and color of the directonless, ambient light in the
			//scene. Values should be between 0 and 1 inclusive. The alpha value is ignored.
			void setAmbientLight(EG_Color newColor);

			//Returns the light at the specified index. Use Light::MAX_LIGHTS to determine
			//the number of available lights.
			EG_Light* getLight(unsigned int index);

			//Returns the current level of ambient light. Should be called after initialize().
			EG_Color getAmbientLight(void);

			//Deletes all objects currently contained in the scene
			void clearScene(void);

            //Set the 4x4 navigation matrix representing the camera's viewpoint.
            void setNavMatrix(const std::vector<GLfloat> &newNavMatrix);

			//The EbonGL engine may be using a multi-threaded drawing architecture. Any changes made to scene objects
			//or lights should be done after calling requestLock(). Call releaseLock() when finished to allow drawing
			//to resume.
			void requestLock(void);
			void releaseLock(void);

            //Enable or disable shadows. Shadows can be disabled to improve performance when there are many
            //objects in the scene. They're disabled by default.
            void setShadows(bool enabled);

		protected:
			//The lights available through OpenGL
			std::vector<EbonGL::EG_Light*> lights;

			//Non-transparent objects that belong to the scene
			std::list<EbonGL::EG_Object*> solidObjects;

			//Transparent objects need to be drawn after solid objects
			std::list<EbonGL::EG_Object*> transparentObjects;

			//Each element contains the solidObjects, transparentObjects, and lights making up
			//the scene. Each drawing thread will access its own element, to avoid setting the Objects
			//and Lights to a conflicting state.
			std::vector<EbonGL::EG_SafeScene*> threadScenes;

			//The intensity of ambient light. There are three elements, each with a value
			//between 0 and 1 for red, green and blue
			std::vector<GLfloat> ambientColor;

			//Controls acces to solidObjects, transparentObjects, lights and threadScenes
            EbonGL::EG_Mutex updateMutex;

			//A unique identifier for this engine
			unsigned int myID;

			//True if the user has locked updateMutex.
			bool lockRequested;

		private:
            //assigns a unique ID to each engine
			static unsigned int engineStamper;

            //############### FUNCTIONS ###############

			//Deletes all objects from the engine and threadScenes. updateMutex should be locked before
			//calling this function.
			void internalClear(void);

            //If this is the first time showVersion is called, write OpenGL verison and details
            //to log file
            void showVersion(void);

	};//EG_EngineInt
}//namespace EbonGL

#endif
