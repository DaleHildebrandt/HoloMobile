#ifndef EG_SCENEELEMENT_HPP_INCLUDED
#define EG_SCENEELEMENT_HPP_INCLUDED

#include "EG_Utils.hpp"

namespace EbonGL
{
    class EG_Object;

	class EG_SceneElement
	{
		public:
			//constructor
			EG_SceneElement(void);

			//destructor
			~EG_SceneElement(void);

			//Notify the object that it is present in an engine's scene. Used to quickly remove
			//the object from the scene using engineIter.
			void setEngine(unsigned int newEngineID,
						   std::list<EG_Object*>::iterator newEngineIter,
						   std::vector<std::list<EG_Object*>::iterator> newSafeIters,
						   bool newIsSolid);

			//Remove this item from the Engine's scene. Throws an exception if the object isn't currently
			//in a scene, or if the engine ID doesn't match our engine.
			void removeEngine(unsigned int removeID,
							  std::list<EG_Object*>::iterator &removeIter,
							  std::vector<std::list<EG_Object*>::iterator> &removeSafe);

			//True if this object belongs to the Engine's solid list, false for transparent
			bool getIsSolid(void);

		private:
			//Our position in the thread-safe scenes' object lists
			std::vector<std::list<EG_Object*>::iterator> safeIters;

			//Our position in the engine's object list, used for quick deletion
			std::list<EG_Object*>::iterator engineIter;

			//The unique identifier for the engine we belong to.
			unsigned int engineID;

			//True if the object is contained within an Engine, false otherwise.
			//Used to make sure we're only part of one Engine at a time.
			bool haveEngine;

			//True if this object belongs to the Engine's solid list, false for the transparent list
			bool isSolid;

	};//SceneElement
}//namespace EbonGL

#endif
