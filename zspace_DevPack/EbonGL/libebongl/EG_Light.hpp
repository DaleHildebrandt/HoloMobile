#ifndef EG_LIGHT_HPP_INCLUDED
#define EG_LIGHT_HPP_INCLUDED

#include <vector>
#include <iostream>
#include "EG_Utils.hpp"
#include "EG_Color.hpp"
#include "utilities/EG_BoolVector.hpp"

namespace EbonGL
{
    enum EG_LightTypes
    {
        EG_LOCAL,
        EG_DIRECTIONAL
    };//EG_LightTypes

    class MH_INTERFACE EG_Light
    {
        public:
            //constructor
            EG_Light(unsigned int newIndex);

			//copy constructor
			EG_Light(const EG_Light &source);

			//destructor
            ~EG_Light(void);

			//Overloaded = operator
            EG_Light& operator= (const EG_Light &source);

            //Set the type of light, either local or directional. Local lights have a position,
            //and directional lights are treated as being infinitely far away. (for example, the sun)
            //newDirection should be normalized
            void setToLocal(const MHTypes::Point3D &newPosition,
                            const MHTypes::EulerAngle &newDirection);
            void setToDirectional(const MHTypes::EulerAngle &newDirection);

            //Turn the light on and off
            void enable(void);
            void disable(void);

            //Returns true if this light was enabled, false otherwise
            bool getEnabled(void);

            //Set the light position, only has an effect on local lights
            void setPosition(const MHTypes::Point3D &newPosition);

            //accessor
            MHTypes::Point3D getPosition(void);

            //Set the light's direction. Note that this currently does nothing
            //for local lights, as they're not set to spotlight mode.
            void setDirection(const MHTypes::EulerAngle &newDirection);

			//If isChanged is true, calls gl functions to position the light and
			//set its details
			void applyChanges(void);

			//True if this light has changes that have not been propagated to the specified scene.
			//Automatically sets isChanged to false for that scene.
			bool getIsChanged(unsigned int sceneIndex);

            //Set the light color and intensity
            void setAmbient(EG_Color newColor);
            void setDiffuse(EG_Color newColor);
            void setSpecular(EG_Color newColor);

            //Given an integer, returns the equivalent OpenGL light index
            static GLenum getGLIndex(unsigned int index);

            //The maximum number of lights supported by OpenGL
            static const unsigned int MAX_LIGHTS = 8;

        private:
			//True if this light was changed by the user, and has yet to be updated in the thread-safe scene.
            //There's one scene per element.
			EG_BoolVector isChanged;

            //The color of this light
            EG_Color ambientColor;
            EG_Color diffuseColor;
            EG_Color specularColor;

            //This light's position, if it is a local light
            MHTypes::Point3D position;

			//The light's direction
			MHTypes::EulerAngle direction;

            //This light's OpenGL index
            GLenum openGLIndex;

            //The type of light, either spot or directional
            EG_LightTypes myType;

            //True if this light was enabled, false otherwise
            bool isEnabled;

			//True if this light is a copy of another and is read-only. There are a limited number
			//of lights in OpenGL, so copies use the same openGLIndex and don't increment numLights.
			bool isCopy;

            //The current number of created lights
            static unsigned int numLights;

			//############### Functions ###############

			//Copies member variables from source into this instance
			void copyFrom(const EG_Light &source);

			//Called by functions that modify the light. If isCopy is true, we display
			//an error message and throw an exception.
			void checkCopy(void);

    };//Light
}//namespace EbonGL
#endif
