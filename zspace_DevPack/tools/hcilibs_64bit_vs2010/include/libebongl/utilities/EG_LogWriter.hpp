/* DESCRIPTION
    Output that would normally be written to stdout is sent to the log file,
    since Windows doesn't display stdout, even if the program is run from a console window.
    The write and add functions are thread-safe, though calls to add() may get mixed up in the
    log file.
*/

#ifndef EG_LOGWRITER_HPP_INCLUDED
#define EG_LOGWRITER_HPP_INCLUDED

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "../EG_Utils.hpp"

namespace EbonGL
{
	class EG_Mutex;

	class EG_LogWriter
	{
		public:
			//constructor
			EG_LogWriter(const std::string &newFilename);

			//copy constructor
			EG_LogWriter(const EG_LogWriter &source);

			//destructor
			~EG_LogWriter(void);

			//Overloaded = operator
            EG_LogWriter& operator= (const EG_LogWriter &source);

			//Append data to the log message
			EG_LogWriter& operator << (const std::string &text);
			EG_LogWriter& operator << (const int &number);
			EG_LogWriter& operator << (const unsigned int &number);
			EG_LogWriter& operator << (const float &number);
			EG_LogWriter& operator << (const double &number);

			//intended to catch endl, outputs the log message
			EG_LogWriter& operator << (std::ostream& ( *pf )(std::ostream&));

            //Returns the most recently written log entry, or an empty string if there
            //aren't any
            std::string getErrMsg(void);

			//Limit the number of log entries to prevent it from growing out of control.
			static const int MAX_ENTRIES = 100000;

		private:
			//The file to write to
			std::string fileName;

			//A line of output to write to the log file
			std::string outputLine;

			//Controls access to the log file in case multiple threads are trying
			//to write at once.
			EbonGL::EG_Mutex *logMutex;

            //The most recently written log entry
            std::string errMsg;

			//Count the number of entries made to the log file so far
			int numEntries;

			//Copy the member variables from source into this instance
			void copyFrom(const EG_LogWriter &source);

	};//EG_LogWriter
}//namespace EbonGL

#endif
