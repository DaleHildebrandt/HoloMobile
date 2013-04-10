//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Plugin.inl
//  Content:    The generic Plugin class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "ZSpace/Common/Resource/Plugin.h"
#include "ZSpace/Common/Error/Error.h"

ZSPACE_COMMON_NS_BEGIN

template <class T>
Plugin<T>::Plugin(const char* filename)
: m_instance(NULL),
  m_fnCreateInstance(NULL),
  m_fnDestroyInstance(NULL)
{
  ZS_ASSERT(0x000001a9, filename != NULL, "filename was NULL when calling Plugin::Plugin.");

#ifdef _WIN32
  m_dllHandle = NULL;

  // Try to load the plugin as a dynamic library.
  m_dllHandle = ::LoadLibrary(filename); 
  if (m_dllHandle == NULL)
    ZS_LOG(ERR, "Could not load plugin: \"%s\".", filename);

  if (m_dllHandle)
  {
    // Locate the plugin's exported functions.
    m_fnCreateInstance = reinterpret_cast<fnCreateInstance*>(::GetProcAddress(m_dllHandle, "createInstance"));
    ZS_WARN(0x00000002, m_fnCreateInstance, "Could not find exported createInstance function in: \"%s\".", filename);

    m_fnDestroyInstance = reinterpret_cast<fnDestroyInstance*>(::GetProcAddress(m_dllHandle, "destroyInstance"));
    ZS_WARN(0x00000003, m_fnDestroyInstance, "Could not find exported destroyInstance function in: \"%s\".", filename);

    // If the plugin's exported function could not be found, unload the dll.
    if (!m_fnCreateInstance || !m_fnDestroyInstance)
    {
      ::FreeLibrary(m_dllHandle);
      m_dllHandle = NULL;
    }
    else
      m_instance = m_fnCreateInstance();
  }
#endif
}


template <class T>
Plugin<T>::~Plugin()
{

  if (m_instance)
  {
    m_fnDestroyInstance();
    m_instance = NULL;
  }

#ifdef _WIN32
  // Unload the dll. 
  if (m_dllHandle)
  {
    ::FreeLibrary(m_dllHandle);
    m_dllHandle = NULL;
  }

#endif
}


template <class T>
T Plugin<T>::getInstance() const
{
  return m_instance;
}

ZSPACE_COMMON_NS_END