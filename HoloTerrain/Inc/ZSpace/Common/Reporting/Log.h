//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Log.h
//  Content:    Log - Generalized class and utilities for logging messages.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_LOG_H__
#define __ZSPACE_LOG_H__

#include "ZSpace/Common/CommonDefines.h"

ZSPACE_COMMON_NS_BEGIN

class LogFile;

/**
* @addtogroup Common
* @{
*
*   @addtogroup Reporting
*   @{
*/

/**
* The Log class is a singleton that allows for verbose logging of errors, warnings,
* and general information.
*
* The following is an example of how to use the Log class:
*
* Application Main:
*
*   // In the main function of your application create the Log singleton to log\n
*   // all messages of severity INFO and above (this includes ERR and WARN).\n
*   zspace::common::Log::create("application.log", zspace::common::Log::INFO);
*
*   // Close the Log.\n
*   zspace::common::Log::close();
*
*
* Module/DLL:
*
*   // In any module/DLL, use the following macro to write to the log file.\n
*   // This will write: "[ERR ] Cannot open file.txt."\n
*   ZS_LOG(ERR, "Cannot open %s.\n", "file.txt");
*/
class ZSPACE_COMMON_API Log
{
public:
  /**
  * The type of log entry.
  */
  enum EntryType
  {
    ERR,    /**< Critical error messages.  
                 Setting the log's level to this mode will enforce that only messages of type ERR will get logged. */

    WARN,   /**< Non-critical warning messages.  
                 Setting the log's level to this mode will enforce that only messages of types ERR and WARN will get logged. */
    
    INFO    /**< General information messages.  
                 Setting the log's level to this mode will enforce that messages of all types will get logged. */
  };

	/**
  * Create a Log singleton. 
  * 
  * @remark
  *   This method is designed to control the name of the output log file as well
  *   as define the logging level (ERR, WARN, or INFO) for messages. 
  *
  * @param[in] filename
  *   The name of the log file to open.
  * @param[in] level
  *   The level of the EntryType that is allowed to get written to the log file.\n
  *   For example, if level == WARN, only log messages of type ERR and WARN will\n
  *   get written to the log file.
  * @param[in] useDefaultLogDirectory
  *   This will use the default log directory specified in Settings.xml and prepend
  *   it to the specified log filename.
	*
	* @return 
  *   A pointer to the Log singleton.
  */
  static Log* create(const char* filename, EntryType level, bool useDefaultLogDirectory = true);
	
  /**
  * Get the Log singleton.
	*
	* @return 
  *   A pointer to the Log singleton.  This returns NULL of the Log was not created.
  */
  static Log* get();

  /**
  * Whether error messages also appear onscreen in a messagebox.
  */
  bool useMessageBox() const;

  /**
  * Set whether error messages also appear onscreen in a messagebox.
  */
  void setUseMessageBox(bool usePopups);

  /**
  * Write a message of given type to the log file if its entry type is less or equal than a predefined level. 
	*
  * @param[in] type
  *   The EntryType of the log message (ERR, WARN, INFO).
  * @param[in] message
  *   The log message.
  * @param[in] ...
  *   The argument list associated with the log message.
  * 
	* @return 
  *   The number of characters logged.
  */
  int log(EntryType type, const char* message, ...);

  /**
  * Close the Log.
  *
  * @remark
  *   This closes the log file and releases the Log singleton.
  */
  static void close();

protected:
  /**
  * The default constructor.
  *
  * @param[in] filename
  *   The name of the log file to open.
  * @param[in] level
  *   The level of the EntryType that is allowed to get written to the log file.\n
  *   For example, if level == WARN, only log messages of type ERR and WARN will\n
  *   get written to the log file.
  */
	Log(const char* filename, EntryType level);

	virtual ~Log();

private:

  LogFile*  m_file;
  EntryType m_level;
  bool      m_useMessageBox;
};

ZSPACE_COMMON_NS_END

/** 
* A macro to check the singleton and log the message if the singleton is created.  Otherwise no logging.
*
* Example: ZS_LOG(ERR, "Cannot open file %s", fileName); 
*/
#define ZS_LOG(__TYPE__, __MESSAGE__, ...) \
  zspace::common::Log::get() && zspace::common::Log::get()->log(zspace::common::Log::__TYPE__, __MESSAGE__, __VA_ARGS__)

/** @} */   // End of group: Reporting
/** @} */   // End of group: Common

#endif // __ZSPACE_LOG_H__