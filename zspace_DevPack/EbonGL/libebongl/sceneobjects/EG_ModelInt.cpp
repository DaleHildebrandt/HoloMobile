#include "EG_ModelInt.hpp"
#include "EG_ModelData.hpp"
#include "../EG_Settings.hpp"
#include "../utilities/EG_String.hpp"

using namespace std;
using namespace EbonGL;

bool EG_ModelInt::systemInitialized = false;

EG_ModelInt::EG_ModelInt(unsigned int numContexts)
{
    scene = NULL;
    isCopy = false;
    modelScale = 1;

    for (unsigned int i = 0; i < numContexts; i++)
        contextData.push_back(new EG_ModelData);
}//constructor

EG_ModelInt::EG_ModelInt(const EG_ModelInt &source)
{
    copyFrom(source);
}//copy constructor

EG_ModelInt::~EG_ModelInt(void)
{
    clearTextures();
}//destructor

EG_ModelInt& EG_ModelInt::operator= (const EG_ModelInt &source)
{
    if (this != &source)
    {
        clearTextures();
        copyFrom(source);
    }//if this

    return *this;
}//=

void EG_ModelInt::copyFrom(const EG_ModelInt &source)
{
    EG_Object::copyFrom(source);

    isCopy = true;
    scene = source.scene;
    modelScale = source.modelScale;

    for (unsigned int i = 0; i < source.contextData.size(); i++)
        contextData.push_back(source.contextData.at(i));
}//copyFrom

EG_Object* EG_ModelInt::generateInstance(void)
{
    return new EG_ModelInt(*this);
}//generateInstance

bool EG_ModelInt::initialize(void)
{
	Assimp::Logger::LogSeverity severity = Assimp::Logger::VERBOSE;
    bool result = true;//false on initialization errors

    //### Verify that we aren't already initialized ###
    if (systemInitialized)
    {
        EG_Utils::writeLog(EG_String("EG_ModelInt::initialize(): already called!"));
        result = false;
    }//if systemInitialized

	//### Direct Assimp messages to file and stdout ###
    if (result)
    {
	    Assimp::DefaultLogger::create("", severity, aiDefaultLogStream_STDOUT);
	    Assimp::DefaultLogger::create("AssimpLog.txt", severity, aiDefaultLogStream_FILE);
    }//if result

    //### Verify that the DevIL DLL matches our version ###
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		ILint test = ilGetInteger(IL_VERSION_NUM);

        EG_Utils::addLog(EG_String("The DevIL DLL is version "));
        EG_Utils::addLog(EG_String::fromInt(ilGetInteger(IL_VERSION_NUM)));
        EG_Utils::addLog(EG_String(", but we require version "));
        EG_Utils::addLog(EG_String::fromInt(IL_VERSION));
        EG_Utils::writeLog(EG_String(" or later."));
		result = false;
	}//if ilGetInteger()

    //### Initialize DevIL ###
    if (result)
    {
	    ilInit();
        systemInitialized = true;
    }//if result

    return result;
}//initialize

void EG_ModelInt::shutdown(void)
{
    Assimp::DefaultLogger::kill();
}//shutdown

bool EG_ModelInt::loadMesh(const EG_String &fileName)
{
    ifstream infile;//try to open fileName to see if it exists
    bool result = true;//false on file access error

    EG_Utils::addLog(EG_String("EG_ModelInt::loadMesh(): loading "));
    EG_Utils::writeLog(fileName);

    //### Verify that the file exists ###
    infile.open(fileName.toCString());
    if (infile.is_open())
        infile.close();
    else
    {
        EG_Utils::addLog(EG_String("EG_ModelInt::loadMesh(): couldn't open "));
        EG_Utils::writeLog(fileName);
        result = false;
    }//else is_open()

    //### Load the model ###
    if (result)
    {
	    scene = importer.ReadFile(fileName.toCString(), aiProcessPreset_TargetRealtime_Quality);
	    if (scene == NULL)
	    {
            string importErr = importer.GetErrorString();

		    EG_Utils::addLog(EG_String("EG_ModelInt::loadMesh(): "));
            EG_Utils::writeLog(EG_String(importErr.c_str()));
		    result = false;
        }//if scene
	}//if result

    return result;
}//loadMesh

bool EG_ModelInt::loadTextures(unsigned int glContextID)
{
    EG_ModelData *myModelData = contextData.at(glContextID);
    map<std::string, GLuint*> *textureIdMap = myModelData->getTextureMap();
    std::map<std::string, GLuint*>::iterator idMapIter;//points to an element in textureIdMap
    GLuint* textureIds = NULL;//pointer to the OpenGL texture array for this model
    ILuint* imageIds = NULL;//an array of DevIL image IDs
    string currentFile;//the current texture file to open
    aiString texturePath;//the path to the current texture file
    aiReturn foundTexture = AI_SUCCESS;//AI_SUCCESS if we've found another texture to load
    int textureIndex = 0;//the current texture's index in the scene
    int numTextures = 0;//the total number of textures in the scene
    int textureWidth = 0;//the size of the current texture in pixels
    int textureHeight = 0;
    bool loadResult = 0;//false if we were unable to load a texture
    bool result = true;//false on loading error

    //### Verify that the scene doesn't have embedded textures ###
	if (scene->HasTextures())
    {
        EG_Utils::writeLog(EG_String("Support for meshes with embedded textures is not implemented"));
        result = false;
    }//if HasTextures()

	//### Read the texture filenames ###
    if (result)
    {
	    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
	    {
		    textureIndex = 0;
		    foundTexture = AI_SUCCESS;

		    while (foundTexture == AI_SUCCESS)
		    {
			    foundTexture = scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, textureIndex, &texturePath);

                currentFile = texturePath.data;
                if (currentFile.size() == 0)
                {
                    EG_Utils::writeLog(EG_String("Found a texture with an empty name, using default.jpg"));
                    currentFile = "default.jpg";
                }//if size()

			    (*textureIdMap)[currentFile] = NULL;
			    textureIndex++;
		    }//while foundTexture
	    }//for i
    }//if result

    //### Generate DevIL image IDs for each texture ###
    if (result)
    {
	    numTextures = static_cast<int>(textureIdMap->size());
	    imageIds = new ILuint[numTextures];
	    ilGenImages(numTextures, imageIds);
    }//if result

    //### Create OpenGL image IDs for each texture ###
    if (result)
    {
        myModelData->createTextureIDs(numTextures);
	    textureIds = myModelData->getTextureIDs();
	    glGenTextures(numTextures, textureIds);
    }//if result

    //### Load the textures ###
	if (result)
    {
	    textureIndex = 0;
        idMapIter = textureIdMap->begin();

        while ((textureIndex < numTextures) && (result))
        {
		    //save the IL image ID
		    currentFile = idMapIter->first;
            currentFile = replaceSpaces(currentFile);
            currentFile = removeSpaces(currentFile);
		    idMapIter->second = &textureIds[textureIndex];
		    idMapIter++;
            
            EG_Utils::addLog(EG_String("Loading '"));
            EG_Utils::addLog(EG_String(currentFile.c_str()));
            EG_Utils::writeLog(EG_String("'"));

            //Load the image
		    ilBindImage(imageIds[textureIndex]);
            loadResult = true;

            #ifdef _UNICODE
                wchar_t* wideString = EG_Utils::charToWchar(currentFile.c_str());

                wcout << "Wide String: '" << wideString << "'" << endl;
                if (!ilLoadImage(wideString))
                    loadResult = false;

                delete wideString;
                wideString = NULL;
            #else
                if (!ilLoadImage(currentFile.c_str()))
                    loadResult = false;
            #endif

            if (!loadResult)
		    {
                EG_Utils::addLog(EG_String("EG_ModelInt::loadTextures(): Couldn't load '"));
                EG_Utils::addLog(EG_String(currentFile.c_str()));
                EG_Utils::writeLog(EG_String("'"));
                result = false;
            }//if !loadResult

            //Convert color components into unsigned byte format
            //If your image contains an alpha channel you can replace IL_RGB with IL_RGBA
            if (result)
            {
			    if (!ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE))
			    {
				    EG_Utils::writeLog(EG_String("EG_ModelInt::loadTextures(): Couldn't convert image to unsigned byte"));
				    result = false;
			    }//if !ilConvertImage()
            }//if result

            //Bind the textures into OpenGL
            if (result)
            {
                EG_Utils::writeLog(EG_String("Binding to OpenGL..."));
			    glBindTexture(GL_TEXTURE_2D, textureIds[textureIndex]);

			    //redefine standard texture values
			    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

                textureWidth = ilGetInteger(IL_IMAGE_WIDTH);
                textureHeight = ilGetInteger(IL_IMAGE_HEIGHT);

                if ((textureWidth <= MAX_TEXTURE_DIMENSION) && (textureHeight <= MAX_TEXTURE_DIMENSION))
                    glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP),
                                 textureWidth, textureHeight, 0, ilGetInteger(IL_IMAGE_FORMAT),
                                 GL_UNSIGNED_BYTE, ilGetData());
                else
                {
                    EG_Utils::addLog(EG_String("EG_ModelInt::loadTextures(): texture "));
                    EG_Utils::addLog(EG_String(currentFile.c_str()));
                    EG_Utils::addLog(EG_String(" has a size of "));
                    EG_Utils::addLog(EG_String::fromInt(textureWidth));
                    EG_Utils::addLog(EG_String(" x "));
                    EG_Utils::addLog(EG_String::fromInt(textureHeight));
                    EG_Utils::addLog(EG_String(", but the largest possible size is "));
                    EG_Utils::addLog(EG_String::fromInt(MAX_TEXTURE_DIMENSION));
                    EG_Utils::addLog(EG_String(" x "));
                    EG_Utils::writeLog(EG_String::fromInt(MAX_TEXTURE_DIMENSION));
                    result = false;
                }//else textureWidth && textureHeight
		    }//if result

            textureIndex++;
	    }//while textureIndex && result
    }//if result

    //### Free memory ###
	ilDeleteImages(numTextures, imageIds);

	delete [] imageIds;
	imageIds = NULL;

	return result;
}//loadTextures

string EG_ModelInt::replaceSpaces(const string &fileName)
{
    string result = fileName;//the file name to return
    size_t spacePos = 0;//the index of the first occurrence in the string

    spacePos = result.find("%20");
    while (spacePos != result.npos)
    {
        result = result.replace(spacePos, 3, " ");
        spacePos = result.find("%20");
    }//while spacePos

    return result;
}//replaceSpaces

void EG_ModelInt::draw(int glContextID)
{
    EG_Object::startDrawing(true);
    glEnable(GL_TEXTURE_2D);
    recursiveRender(scene->mRootNode, static_cast<float>(modelScale), glContextID);
    EG_Object::finishDrawing();
}//draw

void EG_ModelInt::recursiveRender(const struct aiNode* currentNode,
                                   float scale,
                                   int glContextID)
{
    const struct aiMesh* mesh = NULL;//the current mesh to draw
    const struct aiFace* face = NULL;//the current face to draw
    GLenum faceMode = GL_POLYGON;//the OpenGL drawing mode for the current face
	struct aiMatrix4x4 transMatrix = currentNode->mTransformation;//transforms the component relative to its parent
    int vertexIndex = 0;//the current group index in a face

    glPushMatrix();

    //### Scale the model ###
	transMatrix.Scaling(aiVector3D(scale, scale, scale), transMatrix);

	//### Transform this component ###
	transMatrix.Transpose();
	glMultMatrixf((float*)&transMatrix);

	//### Draw all meshes assigned to this node ###
	for (unsigned int n = 0; n < currentNode->mNumMeshes; n++)
	{
		mesh = scene->mMeshes[currentNode->mMeshes[n]];

		applyMaterial(scene->mMaterials[mesh->mMaterialIndex], glContextID);

        //Disable lighting if surface normals not available
		if (mesh->mNormals == NULL)
			glDisable(GL_LIGHTING);
		else
			glEnable(GL_LIGHTING);

        //Disable colors if not available
		if (mesh->mColors[0] != NULL)
			glEnable(GL_COLOR_MATERIAL);
		else
			glDisable(GL_COLOR_MATERIAL);

        //Draw each face in the mesh
		for (unsigned int t = 0; t < mesh->mNumFaces; t++)
        {
			face = &mesh->mFaces[t];

			switch (face->mNumIndices)
			{
				case 1:
                    faceMode = GL_POINTS;
                break;

				case 2:
                    faceMode = GL_LINES;
                break;

				case 3:
                    faceMode = GL_TRIANGLES;
                break;

				default:
                    faceMode = GL_POLYGON;
                break;
			}//switch mNumIndices

			glBegin(faceMode);

			for (unsigned int i = 0; i < face->mNumIndices; i++)
			{
				vertexIndex = face->mIndices[i];

				if (mesh->mColors[0] != NULL)
					applyColor(&mesh->mColors[0][vertexIndex]);

				if (mesh->mNormals != NULL)
                {
					if (mesh->HasTextureCoords(0))
						glTexCoord2f(mesh->mTextureCoords[0][vertexIndex].x, 1 - mesh->mTextureCoords[0][vertexIndex].y); //mTextureCoords[channel][vertex]

			        glNormal3fv(&mesh->mNormals[vertexIndex].x);
                }//if mNormals

				glVertex3fv(&mesh->mVertices[vertexIndex].x);
			}//for i

			glEnd();
		}//for t
	}//for n

	//### Draw all child nodes ###
	for (unsigned int i = 0; i < currentNode->mNumChildren; i++)
		recursiveRender(currentNode->mChildren[i], scale, glContextID);

	glPopMatrix();
}//recursiveRender

void EG_ModelInt::applyMaterial(const struct aiMaterial *theMaterial,
                              int glContextID)
{
    EG_ModelData *myModelData = contextData.at(glContextID);
    std::map<std::string, GLuint*>::iterator idMapIter;//points to an element in textureIdMap
    map<std::string, GLuint*> *textureIdMap = myModelData->getTextureMap();
    vector<GLfloat> glColor;//contains the r, g, b and a components of the current color to apply
	GLenum fillMode = GL_FILL;//the OpenGL fill mode to use for this material
	struct aiColor4D diffuse;//The OpenGL color types for this material
	struct aiColor4D specular;
	struct aiColor4D ambient;
	struct aiColor4D emission;
	float shininess = 0;//the material's shininess
    float shineStrength = 0;//the strength of the shininess
	int twoSided = 0;//non zero if the material has two sides
	int wireframe = 0;
	unsigned int max = 1;
    unsigned int textureID = 0;//the ID of the texture to bind
	int texIndex = 0;
	aiString texPath;//contains filename of texture
    bool floatArrayOK = true;//false if there was an error getting the material float array

    //### Bind the texture ###
	if (theMaterial->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath) == AI_SUCCESS)
	{
        glEnable(GL_TEXTURE_2D);
        idMapIter = textureIdMap->find(texPath.data);

		textureID = *((*textureIdMap)[texPath.data]);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}//if GetTexture()
    else
        glDisable(GL_TEXTURE_2D);

    //### Set the diffuse color ###
	if (aiGetMaterialColor(theMaterial, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == AI_SUCCESS)
        glColor = setGLColor(&diffuse);
    else
        glColor = setGLColor(0.8f, 0.8f, 0.8f, 1.0f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &glColor.at(0));

    //### Set the specular color ###
	if (aiGetMaterialColor(theMaterial, AI_MATKEY_COLOR_SPECULAR, &specular) == AI_SUCCESS)
		glColor = setGLColor(&specular);
    else
        glColor = setGLColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &glColor.at(0));

    //### Set the ambient color ###
	if (aiGetMaterialColor(theMaterial, AI_MATKEY_COLOR_AMBIENT, &ambient) == AI_SUCCESS)
		glColor = setGLColor(&ambient);
    else
        glColor = setGLColor(0.2f, 0.2f, 0.2f, 1.0f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &glColor.at(0));

    //### Set the emissive color ###
	if (aiGetMaterialColor(theMaterial, AI_MATKEY_COLOR_EMISSIVE, &emission) == AI_SUCCESS)
		glColor = setGLColor(&emission);
    else
        glColor = setGLColor(0.0f, 0.0f, 0.0f, 1.0f);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &glColor.at(0));

    //### Set the shininess ###
	max = 1;
	if (aiGetMaterialFloatArray(theMaterial, AI_MATKEY_SHININESS, &shininess, &max) != AI_SUCCESS)
        floatArrayOK = false;
	max = 1;
	if (aiGetMaterialFloatArray(theMaterial, AI_MATKEY_SHININESS_STRENGTH, &shineStrength, &max) != AI_SUCCESS)
        floatArrayOK = false;

	if (floatArrayOK)
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * shineStrength);
	else
    {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
        glColor = setGLColor(0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &glColor.at(0));
	}//else floatArrayOK

    //### Set the drawing mode ###
	max = 1;
	if (aiGetMaterialIntegerArray(theMaterial, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max) == AI_SUCCESS)
    {
        if (wireframe)
            fillMode = GL_LINE;
        else
            fillMode = GL_FILL;
    }//if aiGetMaterialIntegerArray()
	else
		fillMode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fillMode);

    //### Cull back faces for one-sided models ###
	max = 1;
	if (aiGetMaterialIntegerArray(theMaterial, AI_MATKEY_TWOSIDED, &twoSided, &max) == AI_SUCCESS)
    {
        if (twoSided)
		    glEnable(GL_CULL_FACE);
        else
            glDisable(GL_CULL_FACE);
    }//if aiGetMaterialIntegerArray()
	else
		glDisable(GL_CULL_FACE);
}//applyMaterial

void EG_ModelInt::logInfo(string logString)
{
	Assimp::DefaultLogger::get()->info(logString.c_str());
}//logInfo

void EG_ModelInt::logDebug(const char *logString)
{
	Assimp::DefaultLogger::get()->debug(logString);
}//logDebug

void EG_ModelInt::applyColor(const struct aiColor4D *color)
{
	glColor4f(color->r, color->g, color->b, color->a);
}//applyColor

vector<GLfloat> EG_ModelInt::setGLColor(float red,
                                      float green,
                                      float blue,
                                      float alpha)

{
    vector <GLfloat> result;//the color array to return

	result.push_back(red);
    result.push_back(green);
    result.push_back(blue);
    result.push_back(alpha);

    return result;
}//setGLColor

vector<GLfloat> EG_ModelInt::setGLColor(const struct aiColor4D *theColor)
{
    vector<GLfloat> result;//the color array to return

    result.push_back(theColor->r);
    result.push_back(theColor->g);
    result.push_back(theColor->b);
    result.push_back(theColor->a);

    return result;
}//setGLColor

void EG_ModelInt::drawShadows(EG_Light *light,
                            EG_Object *surfaceObject)
{
}//drawShadows

unsigned int EG_ModelInt::numPolygons(void)
{
    return 0;
}//numPolygons

const EG_Polygon* EG_ModelInt::getPolygon(unsigned int index)
{
    return NULL;
}//getPolygon

void EG_ModelInt::clearTextures(void)
{
    if (!isCopy)
    {
        for (unsigned int i = 0; i < contextData.size(); i++)
            delete contextData.at(i);
    }//if !isCopy

    contextData.clear();
}//clearTextures

void EG_ModelInt::setScale(double newScale)
{
    modelScale = newScale;
}//setScale

string EG_ModelInt::removeSpaces(const string &original)
{
    string result;//the string with trailing whitespace removed
    int spacePos = 0;//the index of the char before the first trailing space
    bool finishedLooking = false;//true if we're done looking for the first trailing space

    spacePos = static_cast<int>(original.size() - 1);

    //### Search for the first trailing space ###
    while ((spacePos >= 0) && (!finishedLooking))
    {
        if (original.at(spacePos) == ' ')
            spacePos--;
        else
            finishedLooking = true;
    }//while spacePos && !finishedLooking

    //### Prune trailing spaces ###
    result.append(original, 0, spacePos + 1);

    return result;
}//removeSpaces
