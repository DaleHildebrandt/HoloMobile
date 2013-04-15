#include "EG_ModelData.hpp"
#include "../utilities/EG_String.hpp"
#include "../EG_Utils.hpp"

using namespace std;
using namespace EbonGL;

EG_ModelData::EG_ModelData(void)
{
    textureIDMap = new map<string, GLuint*>;
    textureIDs = NULL;
}//constructor

EG_ModelData::~EG_ModelData(void)
{
	delete[] textureIDs;
	textureIDs = NULL;

    //contents deleted by textureIDs
    delete textureIDMap;
    textureIDMap = NULL;
}//destructor

map<string, GLuint*>* EG_ModelData::getTextureMap(void)
{
    return textureIDMap;
}//getTextureMap

GLuint* EG_ModelData::getTextureIDs(void)
{
    return textureIDs;
}//getTextureIDs

void EG_ModelData::createTextureIDs(unsigned int numTextures)
{
    if (textureIDs != NULL)
    {
        EG_Utils::writeLog(EG_String("EG_ModelData::createTextureIDs(): textureIDs have already been created"));
        EG_Utils::fatalError();
    }//if textureIDs

    textureIDs = new GLuint[numTextures];
}//createTextureIDs
