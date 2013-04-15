//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Plugin.h
//  Content:    The generic Plugin class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_PLUGIN_H__
#define __ZSPACE_PLUGIN_H__

// If using Windows OS
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif
#include <string>

#include "ZSpace/Common/CommonDefines.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Resource
*   @{
*/

/**
* A plugin management class.
*
* @remark
*   This class manages plugins. Under Windows, the name of the plugin
*   corresponds to the name of the dll in which the plugin resides. By default,
*   the dll will get loaded and an instance will get created in memory. Since
*   the plugin has the same name as the dll, the argument to the constructor
*   is the dll filename (e.g., "plugin.dll"). The client of the plugin simply
*   needs to call getInstance() to get a reference to the API the plugin implements.
*/
template <class T>
class Plugin
{
public:
  /**
  * The default constructor.
  *
  * @remark
  *   The constructor is responsible for loading the dll and locating
  *   the createInstance function.
  *
  * @param[in] filename
  *   The name of plugin. On Windows platforms, this corresponds to the name of the dll.
  */
  Plugin(const char* filename);

  /**
  * The destructor.
  */
  ~Plugin();

  /**
  * Get the plugin's instance pointer.
  */
  T getInstance() const;

private:
  typedef T fnCreateInstance();
  typedef void fnDestroyInstance();

private:
  T                   m_instance;
  fnCreateInstance*   m_fnCreateInstance;
  fnDestroyInstance*  m_fnDestroyInstance;

#ifdef _WIN32
  HMODULE           m_dllHandle;
#endif
};

/** @} */   // End of group: Resource
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#include "ZSpace/Common/Resource/Plugin.inl"

#endif // __ZSPACE_PLUGIN_H__