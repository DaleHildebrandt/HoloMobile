//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Error.h
//  Content:    The ZSpace custom error macros.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_ERROR_H__
#define __ZSPACE_ERROR_H__

#ifdef _WIN32
#include <windows.h>
#endif

#include <map>

#include "ZSpace/Common/CommonDefines.h"
#include "ZSpace/Common/Reporting/Log.h"

ZSPACE_COMMON_NS_BEGIN

/**
* There are currently two custom error macros provided in the ZSpace SDK:
*
* ZS_WARN   - non-fatal warnings that trigger a debug breakpoint if the debugger is attached.\n
* ZS_ASSERT - fatal errors that will abort application execution.\n
*
* IMPORTANT:
*   Each of these macros accepts an id as a first parameter. When using these macros, make sure
*   to pass in an id of value "0x0". A pre-commit python script will make sure to convert 
*   the "0x0" id to a unique id in hex format (i.e. 0x00000235). DO NOT assign a unique id 
*   to error macros you use in your code.
*/

/**
* @addtogroup Common
* @{
*
*   @addtogroup Error
*   @{
*/

/**
* The main error reporting class.
*
* @remark
*   This class reports errors. You can specify a custom message. You can query that the error was reported.
*/
class ZSPACE_COMMON_API Error
{
public:
  /**
  * The types of errors.
  */
  enum Type
  {
    TYPE_WARN = 0,    /**< This is a warning (non-fatal and will halt application execution) */
    TYPE_ASSERT,      /**< This is an assertion (fatal and will terminate application execution) */
    NUM_TYPES
  };

public:
  typedef void (*MessageHandler)(int id, const char* file, int line, const char* message);

public:
  /**
  * Report a failure.
  *
  * @param[in] type
  *   The type of error (TYPE_WARN, TYPE_ASSERT, etc).
  * @param[in] id
  *   The unique integer id of the error.
  * @param[in] file
  *   The file name that the error was found.
  * @param[in] line
  *   The line number of the error.
  * @param[in] message
  *   The custom message.
  * @param[in] ...
  *   Any arguments corresponding to the message.
  *
  * @return
  *   True if a failure was reported, false if not.
  */
  static bool reportFailure(Type type, int id, const char* file, int line, const char* message, ...);

  /**
  * Set the message handler for a specified type of error macro.
  *
  * @param[in] type
  *   The type of message handler.
  * @param[in] messageHandler
  *   A function pointer to a custom message handler. Currently, the default
  *   message handlers will print warning and assertion messages to standard
  *   output as well as Visual Studio's debug output window.
  */
  static void setMessageHandler(Type type, MessageHandler messageHandler);

  /**
  * Resets the message handler back to the default for a specified type of error macro.
  *
  * @param[in] type
  *   The type of message handler.
  */
  static void resetMessageHandler(Type type);

  /**
  * Set whether or not all error macros are enabled.
  *
  * @param[in] isEnabled
  *   Boolean specifying whether or not all error macros are enabled.
  */
  static void setAllEnabled(bool isEnabled);

  /**
  * Set whether or not a specific error macro is enabled.
  *
  * @param[in] id
  *   The unique integer id of the error macro.
  * @param[in] isEnabled
  *   Whether or not the error macro is enabled.
  */
  static void setEnabled(int id, bool isEnabled);

private:
  static MessageHandler     m_messageHandlers[NUM_TYPES];
  static bool               m_areAllEnabled;
  static std::map<int, int> m_disabledIds;

private:
  static bool isEnabled(int id);
};

ZSPACE_COMMON_NS_END

#ifdef _DEBUG	// Debug

#ifdef _WIN32
/**
* Halt execution of the application.
*
* @remark
*   If the debugger is currently attached, a debug breakpoint will get triggered.
*/
#define ZS_HALT() ((void)((IsDebuggerPresent()) && (__debugbreak(), 1)))

/**
* Exit the application.
*/
#if _HAS_EXCEPTIONS
#define ZS_ABORT() throw "Assertion failure."
#else
#define ZS_ABORT() std::abort()
#endif // _HAS_EXCEPTIONS
#else
#define ZS_HALT()
#define ZS_ABORT()
#endif

/**
* Set the error handler for a specified type of error macro.
*
* @param[in] type
*   The type of error macro (TYPE_WARN or TYPE_ASSERT).
* @param[in] handler
*   A function pointer based on the zspace::common::Error::MessageHandler.
*/
#define ZS_ERROR_MESSAGE_HANDLER(type, messageHandler) zspace::common::Error::setMessageHandler(type, messageHandler)

/**
* Enable all error macros.
*/
#define ZS_ENABLE_ALL_ERRORS() zspace::common::Error::setAllEnabled(true)

/**
* Disable all error macros.
*/
#define ZS_DISABLE_ALL_ERRORS() zspace::common::Error::setAllEnabled(false)

/**
* Enable an error macro based on a specified integer id.
*
* @param[in] id
*   The error macro's (TYPE_WARN, TYPE_ASSERT, etc.) unique integer id.
*/
#define ZS_ENABLE_ERROR(id) zspace::common::Error::setEnabled(id, true)

/**
* Disable an error macro based on a specified integer id.
*
* @param[in] id
*   The error macro's (TYPE_WARN, TYPE_ASSERT, etc.) unique integer id.
*/
#define ZS_DISABLE_ERROR(id) zspace::common::Error::setEnabled(id, false)

/**
* Custom macro for generating warnings.
*
* @param[in] id
*   The warning's unique integer id.
* @param[in] expr
*   The boolean expression to be evaluated.
* @param[in] msg
*   The custom message for the warning.
* @param[in] ...
*   Arguments associated with the custom message.
*/
#define ZS_WARN(id, expr, msg, ...) \
  ((void)(!(expr) && \
          zspace::common::Error::reportFailure(zspace::common::Error::TYPE_WARN, id, __FILE__, __LINE__, msg, ##__VA_ARGS__) && \
          (ZS_HALT(), 1)))

/**
* Custom macro for generating assertions.
*
* @param[in] id
*   The warning's unique integer id. Use 0x0 in function call and unique value will get filled in automatically by svn
* @param[in] expr
*   The boolean expression to be evaluated.
* @param[in] msg
*   The custom message for the warning.
* @param[in] ...
*   Arguments associated with the custom message.
*/
#define ZS_ASSERT(id, expr, msg, ...) \
  ((void)(!(expr) && \
          zspace::common::Error::reportFailure(zspace::common::Error::TYPE_ASSERT, id, __FILE__, __LINE__, msg, ##__VA_ARGS__) && \
          (ZS_ABORT(), 1)))

#else			    // Release

#define ZS_HALT()
#define ZS_ABORT()

#define ZS_ERROR_MESSAGE_HANDLER(type, messageHandler)
#define ZS_ENABLE_ALL_ERRORS()
#define ZS_DISABLE_ALL_ERRORS()
#define ZS_ENABLE_ERROR(id)
#define ZS_DISABLE_ERROR(id)

#define ZS_WARN(id, expr, msg, ...)
#define ZS_ASSERT(id, expr, msg, ...)

#endif // Debug/Release Macros

/** @} */   // End of group: Error
/** @} */   // End of group: Common

#endif // __ZSPACE_ERROR_H__