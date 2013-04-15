//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       PluginLoader.h
//  Content:    The generic Plugin Loader class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_PLUGIN_LOADER_H__
#define __ZSPACE_PLUGIN_LOADER_H__

#include <map>
#include <string>

#include "ZSpace/Common/CommonDefines.h"
#include "ZSpace/Common/Resource/Plugin.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Resource
*   @{
*/

/**
* A plugin loading class.
*
* @remark
*   This class loads plugins. Under Windows, the name of the plugin
*   corresponds to the name of the dll in which the plugin resides. By default,
*   the dll will get loaded and an instance will get created in memory. Since
*   the plugin has the same name as the dll, the argument to the constructor
*   is the dll filename (e.g., "plugin.dll").
*/
template <class T>
class PluginLoader
{
public:
  /**
  * The default constructor.
  */
  PluginLoader();

  /**
  * The destructor.
  */
  ~PluginLoader();

  /**
  * Optional method to set the resource path for the plugin loader to locate plugin dlls.
  */
  void setResourcePath(const char* resourcePath);

  /**
  * Load a plugin.
  *
  * @remark
  *   If the plugin has been previously loaded (without being unloaded), the instance
  *   of the plugin that was originally loaded will be returned.
  *
  * @param[in] filename
  *   The name of the plugin. On Windows platforms, this corresponds to the name of the dll.
  *
  * @return
  *   An instance of the loaded plugin.
  */
  T load(const char* filename);

private:
  typedef std::map<std::string, Plugin<T>*>  PluginMap;
  typedef std::pair<std::string, Plugin<T>*> PluginPair;

private:
  std::string m_resourcePath;
  PluginMap   m_loadedPlugins;

private:
  /**
  * Unload all plugins that have been previously loaded.
  */
  void unloadAll();
};

/** @} */   // End of group: Resource
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#include "ZSpace/Common/Resource/PluginLoader.inl"

#endif // __ZSPACE_PLUGIN_LOADER_H__