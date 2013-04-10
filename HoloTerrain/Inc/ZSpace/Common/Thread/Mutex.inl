//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Mutex.inl
//  Content:    Simple mutex for locking reentrant code.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "ZSpace/Common/Thread/Mutex.h"

inline
Mutex::Mutex()
{
#ifdef _DEBUG
  m_owningThread = 0;
#endif
#ifdef _WIN32
  m_mutexHandle = CreateMutex(NULL, FALSE, NULL);
#else
  pthread_mutex_init(&m_mutex, 0);
#endif
}


inline
Mutex::~Mutex()
{
#ifdef _WIN32
  CloseHandle(m_mutexHandle);
#else
  pthread_mutex_destroy(&m_mutex);
#endif
}


inline
void Mutex::lock()
{
#ifdef _WIN32
  WaitForSingleObject(m_mutexHandle, INFINITE);
#else
  pthread_mutex_lock(&m_mutex);
#endif
#ifdef _DEBUG
  m_owningThread = getThreadId();
#endif
}


inline
void Mutex::unlock()
{
#ifdef _DEBUG
  m_owningThread = 0;
#endif
#ifdef _WIN32
  ReleaseMutex(m_mutexHandle);
#else
  pthread_mutex_unlock(&m_mutex);
#endif
}


inline
unsigned int Mutex::getThreadId()
{
#ifdef _WIN32
  return GetCurrentThreadId();
#else
  return reinterpret_cast<unsigned int>(pthread_self());
#endif
}