#include "EG_Utils.hpp"
#include "utilities/EG_LogWriter.hpp"
#include "utilities/EG_String.hpp"
#include "sceneobjects/EG_Model.hpp"
#include <stdlib.h>
#include "atlbase.h"
#include "atlstr.h"

using namespace EbonGL;
using namespace std;
using namespace MHTypes;

EG_LogWriter* EG_Utils::logHandler = NULL;
volatile unsigned int EG_Utils::numViewports = 0;

EG_Utils::EG_Utils(void)
{
}//constructor

void EG_Utils::initEbonGL(unsigned int newViewports)
{
    //### Verify that we haven't already called initEbonGL ###
    if (logHandler != NULL)
    {
        *logHandler << "EG_Utils::initEbonGL(): already set number of viewports to "
                    << static_cast<int>(numViewports) << endl;
        fatalError();
    }//if logHandler

    //### Open the logfile for writing ###
    logHandler = new EG_LogWriter("EbonGL_log.txt");

    //### Initialize EG_Model ###
    if (!EG_Model::initialize())
        fatalError();

    //### Set the number of viewports ###
    if (newViewports > 0)
        numViewports = newViewports;
    else
    {
        *logHandler << "EG_Utils::initEbonGL(): invalid number of viewports: 0" << endl;
        fatalError();
    }//else newViewports
}//initEbonGL

EG_Utils::~EG_Utils(void)
{
    *logHandler << "Exiting normally." << endl;

    delete logHandler;
    logHandler = NULL;

    EG_Model::shutdown();
}//destructor

void EG_Utils::setColor(vector<GLfloat> &rgbaCol,
                        MHFloat red,
                        MHFloat green,
                        MHFloat blue,
                        MHFloat alpha)
{
    rgbaCol.clear();
    rgbaCol.push_back(static_cast<GLfloat>(red));
    rgbaCol.push_back(static_cast<GLfloat>(green));
    rgbaCol.push_back(static_cast<GLfloat>(blue));
    rgbaCol.push_back(static_cast<GLfloat>(alpha));
}//setColor

void EG_Utils::addVertex(vector<GLdouble> &vertices,
                         GLdouble vertexX,
                         GLdouble vertexY,
                         GLdouble vertexZ)
{
    vertices.push_back(vertexX);
    vertices.push_back(vertexY);
    vertices.push_back(vertexZ);
}//addVertex

Point3D EG_Utils::getVertex(const vector<GLdouble> &vertices,
                            unsigned int index)
{
    Point3D result;//the vertex to return
    unsigned int position = index * 3;//the vertex's position in vertices

    result.x = vertices.at(position);
    result.y = vertices.at(position + 1);
    result.z = vertices.at(position + 2);

    return result;
}//getVertex

string EG_Utils::getErrMsg(void)
{
    checkInitialized();

    return logHandler->getErrMsg();
}//getErrMsg

unsigned int EG_Utils::getNumViewports(void)
{
    if (numViewports == 0)
    {
        cout << "Call EG_Utils::initEbonGL() before using EbonGL!" << endl;
        fatalError();
    }//if numViewports

    return numViewports;
}//getNumViewports

void EG_Utils::addLog(const EG_String &newEntry)
{
    string strEntry;//string version of newEntry

    checkInitialized();

    strEntry.append(newEntry.toCString());
    *logHandler << strEntry;
}//addLog

void EG_Utils::writeLog(const EG_String &newEntry)
{
    string strEntry;//string version of newEntry

    checkInitialized();

    strEntry.append(newEntry.toCString());
    *logHandler << strEntry << endl;
}//writeLog

void EG_Utils::checkInitialized(void)
{
    if (logHandler == NULL)
    {
        logHandler = new EG_LogWriter("EbonGL_log.txt");
        *logHandler << "EG_Utils::checkInitialized(): initEbonGL must be called before using"
                    << " EbonGL" << endl;
        fatalError();
    }//if logHandler
}//checkInitialized

void EG_Utils::fatalError(void)
{
    throw 1;
    exit(1);
}//fatalError

wchar_t* EG_Utils::charToWchar(const char* narrowString)
{
    // newsize describes the length of the 
    // wchar_t string called wcstring in terms of the number 
    // of wide characters, not the number of bytes.
    size_t newsize = strlen(narrowString) + 1;

    // The following creates a buffer large enough to contain 
    // the exact number of characters in the original string
    // in the new format. If you want to add more characters
    // to the end of the string, increase the value of newsize
    // to increase the size of the buffer.
    wchar_t *result = new wchar_t[newsize];

    // Convert char* string to a wchar_t* string.
    size_t convertedChars = 0;
    mbstowcs_s(&convertedChars, result, newsize, narrowString, _TRUNCATE);
    
    return result;
}//charToWchar