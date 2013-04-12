#ifndef EG_MODEL_HPP_INCLUDED
#define EG_MODEL_HPP_INCLUDED

#include <fstream>
#include <string.h>
#include <map>

#include "../EG_Settings.hpp"

namespace EbonGL
{
    class EG_ModelInt;
    class EG_Color;
    class EG_Engine;
    class EG_String;

    class MH_INTERFACE EG_Model
    {
        public:
            //EG_ModelInt functions, see EG_ModelInt.hpp for documentation
            EG_Model(unsigned int numContexts);
            EG_Model(const EG_Model &source);
            ~EG_Model(void);
            EG_Model& EG_Model::operator= (const EG_Model &source);
            static bool initialize(void);
            static void shutdown(void);
            bool loadMesh(const EbonGL::EG_String &fileName);
            bool loadTextures(unsigned int glContextID);
            void setScale(double newScale);

            //EG_Object functions, see EG_Object.hpp for documentation
            MHTypes::Point3D getPosition(void);
			MHTypes::EulerAngle getRotation(void);
			EG_Color getAmbientDiffuse(void);
            void show(void);
            void hide(void);
            void setShadowCasting(bool enabled);
            void setShadowReceiving(bool enabled);
            void setPosition(const MHTypes::Point3D &newPosition);
            void setRotation(const MHTypes::EulerAngle &newRotation);
            void setAmbientDiffuse(const EbonGL::EG_Color &newColor);
            void setPolygonMode(GLenum newMode);
			void setLighting(bool enabled);
            
            //Add or remove this model from the EG_Engine
            void addToEngine(EbonGL::EG_Engine *theEngine);
            void removeFromEngine(EbonGL::EG_Engine *theEngine);

        private:
            //The actual model is stored here
            EG_ModelInt *theModel;

    };//EG_Model
}//namespace EbonGL

#endif
