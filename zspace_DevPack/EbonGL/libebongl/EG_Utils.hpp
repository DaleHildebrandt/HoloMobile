/* DESCRIPTION
    Graphics functions that are generally useful to multiple EbonGL classes. Only one instance of EbonGL
    should be created and initialized - it contains static member variables that are used by the rest of
    the libraries.
*/

#ifndef EG_UTILS_HPP_INCLUDED
#define EG_UTILS_HPP_INCLUDED

#include "EG_Settings.hpp"

namespace EbonGL
{
    class EG_LogWriter;
    class EG_String;

    class MH_INTERFACE EG_Utils
    {
        public:
            //constructor
            EG_Utils(void);

            //destructor
            ~EG_Utils(void);

            //Initialze the EbonGL graphics library. Should only be called once.
            //newViewports is the number of OpenGL contexts we'll create.
            void initEbonGL(unsigned int newViewports);

            //Handle an unrecoverable error, throws an exception and exits
            static void fatalError(void);

            //Converts an 8-bit ASCII C-string to a unicode wchar_t C-string. The caller is responsible
            //for deleting the wchar_t* when finished.
            static wchar_t* charToWchar(const char* narrowString);

            //Add a vertex to the specifed vector. We get three new elements, one for each
            //vertex coordinate
            static void addVertex(std::vector<GLdouble> &vertices,
                                  GLdouble vertexX,
                                  GLdouble vertexY,
                                  GLdouble vertexZ);

            //Set rgbaCol to contain the specified values, one per element.
            //Clears any existing contents. Values should be between 0 and 1
            //inclusive.
            static void setColor(std::vector<GLfloat> &rgbaCol,
                                 MHTypes::MHFloat red,
                                 MHTypes::MHFloat green,
                                 MHTypes::MHFloat blue,
                                 MHTypes::MHFloat alpha);

            //Returns the vertex at the specified index in vertices
            static MHTypes::Point3D getVertex(const std::vector<GLdouble> &vertices,
                                              unsigned int index);

            //Adds newEntry to the current line in the log file
			static void addLog(const EG_String &newEntry);

            //Adds newEntry to the current line, and starts a new line.
            static void writeLog(const EG_String &newEntry);

            //Returns the most recent entry written to file
            static std::string getErrMsg(void);

            //Return the number of OpenGL viewports being drawn to.
            static unsigned int getNumViewports(void);

		private:
			//saves errors and events of interest to log file
			static EG_LogWriter *logHandler;

            //The number of OpenGL contexts being drawn to by EbonGL. With passive stereo
            //and VRJuggler, this will generally be one per surface (floor, wall, etc.) and one per
            //eye. A system with a floor and screen will therefore have 4.
            static volatile unsigned int numViewports;

            //Returns false if initEbonGL wasn't called, and throws an exception
            static void checkInitialized(void);

    };//EG_Utils
}//namespace EbonGL

#endif
