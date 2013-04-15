//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Mutex.h
//  Content:    Simple mutex for locking reentrant code.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_MUTEX_H__
#define __ZSPACE_MUTEX_H__

#ifdef _WIN32
#include <windows.h>
#define ZS_TLS __declspec( thread )
#else
#include "pthread.h"
#define ZS_TLS __thread
#endif

#include "ZSpace/Common/CommonDefines.h"

/**
* @addtogroup Common
* @{
*
*   @addtogroup Thread
*   @{
*/

ZSPACE_COMMON_NS_BEGIN

/**
* Simple inline mutex interface which calls through to the native implementation.
*/
class ZSPACE_COMMON_API Mutex
{
public:

  /**
  * The default constructor.
  */
  Mutex();
  
  /**
  * The destructor.
  */
  ~Mutex();

   /**
  * Acquire the mutex.
  */
  void lock();
  
  /**
  * Release the mutex.
  */
  void unlock();

  static unsigned int getThreadId();

private:
#ifdef _WIN32
  HANDLE m_mutexHandle;
#else
  pthread_mutex_t m_mutex;
#endif
#ifdef _DEBUG
  unsigned int m_owningThread;
#endif
};

/** @} */   // End of group: Thread
/** @} */   // End of group: Common

#include "ZSpace/Common/Thread/Mutex.inl"

ZSPACE_COMMON_NS_END

#endif //__ZSPACE_MUTEX_H__