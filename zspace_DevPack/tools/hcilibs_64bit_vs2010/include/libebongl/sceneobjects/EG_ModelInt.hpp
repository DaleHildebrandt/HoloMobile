#ifndef EG_MODELINT_HPP_INCLUDED
#define EG_MODELINT_HPP_INCLUDED

#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <stdio.h>
#include <IL\il.h>
#include <fstream>
#include <string.h>
#include <map>

#include "assimp.hpp"
#include "aiPostProcess.h"
#include "aiScene.h"
#include "DefaultLogger.h"
#include "LogStream.h"
#include "EG_Object.hpp"

namespace EbonGL
{
    class EG_ModelData;

    class EG_ModelInt : public EG_Object
    {
        public:
            //Constructor. numContexts is the number of OpenGL contexts to load textures in.
            EG_ModelInt(unsigned int numContexts);

            //Copy constructor, should only be called by EG_Engine.
            EG_ModelInt(const EG_ModelInt &source);

            //destructor
            ~EG_ModelInt(void);

            //Overloaded = operator, should only be used by EG_Engine.
            EG_ModelInt& EG_ModelInt::operator= (const EG_ModelInt &source);

            //Initialize the libraries we'll use to load models. Should only be called once per
            //program. Returns false on failure and outputs an error message. This function is meant
            //for internal use only
            static bool initialize(void);

            //Shutdown the model loading system, call when finished with all models.
            static void shutdown(void);

            //Load a model from file, returns false on errors and outputs an error message. Textures
            //should be loaded separately with loadTextures().
            bool loadMesh(const EbonGL::EG_String &fileName);

            //Load the textures for this model in the specified OpenGL context. This function should be
            //called once for each context the model will be drawn in, and glContextID should be set to
            //a value ranging from 0 to numContexts -1. Returns false on failure and outputs an error
            //message
            bool loadTextures(unsigned int glContextID);

            //Set the scale to draw the model at
            void setScale(double newScale);

            //Draw the model in the specified OpenGL context.
            void draw(int glContextID);

            //Draw shadows on the specified surface. Currently unimplemented.
            void drawShadows(EbonGL::EG_Light *light,
                             EG_Object *surfaceObject);

            //Inherited function, always returns 0.
            unsigned int numPolygons(void);

            //Inherited function, always returns NULL.
            const EbonGL::EG_Polygon* getPolygon(unsigned int index);

		    //Returns a copy of this object
		    EG_Object* generateInstance(void);

            //The maximum size of a texture's dimension, in pixels. Larger textures may cause
            //glTexImage2D to crash.
            static const unsigned int MAX_TEXTURE_DIMENSION = 1024;

        private:
            //Pointer to the model textures. There's one element for each OpenGL context.
            std::vector<EG_ModelData*> contextData;

            //The global Assimp scene object, contains the object mesh. Deleted by importer.
            const aiScene *scene;

            //Loads the model from file
            Assimp::Importer importer;

            //The scale to draw the model at.
            MHTypes::MHFloat modelScale;

            //True if this is a copy of another EG_ModelInt, and we shouldn't delete contextData
            bool isCopy;

            //True if initialize() was successfully called
            static bool systemInitialized;

            //############### Private Functions ###############

            //Write a log message with the Info or Debug tags
            static void logInfo(std::string logString);
            static void logDebug(const char* logString);

            //Set the OpenGL drawing color to the specified value
            static void applyColor(const struct aiColor4D *color);

            //Return a vector with the specified color stored in OpenGL format
            static std::vector<GLfloat> setGLColor(float red,
                                                   float green,
                                                   float blue,
                                                   float alpha);
            static std::vector<GLfloat> setGLColor(const struct aiColor4D *theColor);

            //Apply the current material or texture to the scene
            void applyMaterial(const struct aiMaterial *theMaterial,
                               int glContextID);

            //Iterate through the nodes in the model recursively, drawing as we go
            void recursiveRender(const struct aiNode* nd,
                                 float scale,
                                 int glContextID);

            //Set member variables to those of source. We don't deep-copy contextData,
            //since every copy can read from a different context (element in contextData)
            //without interference. We're assuming here that the only EG_Model copies will
            //be those made by EG_Engine!
            void copyFrom(const EG_ModelInt &source);

            //Clears contextData, deleting the elements if we're not a copy.
            void clearTextures(void);

            //Looks for the ASCII sequence %20 in the string, and replaces them with spaces.
            static std::string replaceSpaces(const std::string &fileName);

            //Removes trailing white spaces from original
            static std::string removeSpaces(const std::string &original);

    };//EG_ModelInt
}//namespace EbonGL

#endif
