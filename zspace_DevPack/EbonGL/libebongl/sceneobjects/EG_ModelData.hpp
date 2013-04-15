/* DESCRIPTION
    Context-specific model data, contains the textures for models loaded
    with Assimp. We need to load the textures seperately in each OpenGL
    context for them to take effect.
*/

#ifndef EG_MODELDATA_HPP_INCLUDED
#define EG_MODELDATA_HPP_INCLUDED

#include <map>
#include <string>
#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>

namespace EbonGL
{
    class EG_ModelData
    {
        public:
            //constructor
            EG_ModelData(void);

            //destructor
            ~EG_ModelData(void);

            //Create numTextures elements in textureIDs
            void createTextureIDs(unsigned int numTextures);

            //Accessors
            std::map<std::string, GLuint*> *getTextureMap(void);
            GLuint *getTextureIDs(void);

        private:
            //maps image filenames to textureIds
            std::map<std::string, GLuint*> *textureIDMap;

            //pointer to the OpenGL texture Array
            GLuint *textureIDs;

    };//EG_ModelData
}//namespace EbonGL

#endif
