//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       PluginLoader.h
//  Content:    The generic Plugin Loader class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "ZSpace/Common/Error/Error.h"
#include "ZSpace/Common/Reporting/Log.h"
#include "ZSpace/Common/Resource/PluginLoader.h"

ZSPACE_COMMON_NS_BEGIN

template <class T>
PluginLoader<T>::PluginLoader()
: m_resourcePath("")
{
}


template <class T>
PluginLoader<T>::~PluginLoader()
{
  unloadAll();
}


template <class T>
void PluginLoader<T>::setResourcePath(const char* resourcePath)
{
  ZS_ASSERT(0x000001aa, resourcePath != NULL, "resourcePath was NULL when calling PluginLoader::setResourcePath.");
  m_resourcePath = resourcePath;
}


template <class T>
T PluginLoader<T>::load(const char* filename)
{
  ZS_ASSERT(0x000001ab, filename != NULL, "filename was NULL when calling PluginLoader::load.");
  PluginMap::iterator i = m_loadedPlugins.find(filename);

  // Check to make sure the plugin has not been loaded already.
  if (i == m_loadedPlugins.end())
  {
    std::string path  = m_resourcePath + filename;
    Plugin<T>* plugin = new Plugin<T>(path.c_str());
    ZS_ASSERT(0x000000de, plugin != NULL, "Failed to allocate plugin.");
    m_loadedPlugins.insert(PluginPair(filename, plugin));
    return plugin->getInstance();
  }
  else
    ZS_LOG(INFO, "Plugin is already loaded: %s.\n", filename);

  return i->second->getInstance();
}


template <class T>
void PluginLoader<T>::unloadAll()
{
  for (PluginMap::iterator i = m_loadedPlugins.begin(); i != m_loadedPlugins.end(); ++i)
    delete i->second;

  m_loadedPlugins.clear();
}

ZSPACE_COMMON_NS_END
