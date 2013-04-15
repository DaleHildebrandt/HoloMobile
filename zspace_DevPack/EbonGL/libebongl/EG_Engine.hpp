/* DESCRIPTION
    This class is a wrapper around EG_Engine. We can't export EG_Engine
    through a DLL interface directly since it uses stl::list.
*/

#ifndef EG_ENGINE_HPP_INCLUDED
#define EG_ENGINE_HPP_INCLUDED

#include "EG_Utils.hpp"
#include "EG_Light.hpp"
#include "utilities/EG_Mutex.hpp"

namespace EbonGL
{
	class EG_EngineInt;
    class EG_Object;

	class MH_INTERFACE EG_Engine
	{
		public:
            //These functions are identical to the functions in EG_EngineInt, see EG_EngineInt.hpp
            //for documentaton.
			EG_Engine(void);
			~EG_Engine(void);
			void contextInit(void);
			void clearBuffer(void);
			void draw(int glContextID);
			void addSolid(EG_Object *newObject);
			void addTransparent(EG_Object *newObject);
			void remove(EG_Object *theObject);
			void setAmbientLight(EG_Color newColor);
			EbonGL::EG_Light* getLight(unsigned int index);
			EG_Color getAmbientLight(void);
			void clearScene(void);
            void setNavMatrix(const std::vector<GLfloat> &newNavMatrix);
			void requestLock(void);
			void releaseLock(void);
            void setShadows(bool enabled);

		private:
            //The actual OpenGL engine that will perform operations
			EG_EngineInt *theEngine;

	};//EG_Engine
}//namespace EbonGL

#endif
