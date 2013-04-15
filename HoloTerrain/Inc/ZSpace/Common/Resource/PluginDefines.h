//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       PluginDefines.h
//  Content:    Macro definitions for the generic Plugin architecture.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_PLUGIN_DEFINES_H__
#define __ZSPACE_PLUGIN_DEFINES_H__

#ifdef ZSPACE_PLUGIN_EXPORTS
#define ZSPACE_PLUGIN_API __declspec(dllexport)
#else
#define ZSPACE_PLUGIN_API __declspec(dllimport)
#endif

// Compiler warning enable/disable macros.
#ifdef _WIN32
#define ZSPACE_PLUGIN_DISABLE_WARNINGS \
  __pragma(warning( push ))            \
  __pragma(warning( disable : 4251 ))

#define ZSPACE_PLUGIN_ENABLE_WARNINGS \
  __pragma(warning( pop ))
#else
#define ZSPACE_PLUGIN_DISABLE_WARNINGS
#define ZSPACE_PLUGIN_ENABLE_WARNINGS
#endif

/**
* @addtogroup Common
* @{
*
*   @addtogroup Resource
*   @{
*/

/**
* Register a ZSpace Plugin.
*
* @param[in] plugin
*   The name of the plugin class to be exported.
* @param[in] ...
*   The initializer list for the plugin class if
*   any constructor parameters exist.
*/
#define ZSPACE_REGISTER_PLUGIN(plugin, ...)             \
  plugin* g##plugin = NULL;                             \
                                                        \
  extern "C" ZSPACE_PLUGIN_API plugin* createInstance() \
  {                                                     \
    if (!g##plugin)                                     \
      g##plugin = new plugin(##__VA_ARGS__);            \
                                                        \
    return g##plugin;                                   \
  }                                                     \
                                                        \
  extern "C" ZSPACE_PLUGIN_API void destroyInstance()   \
  {                                                     \
    if (g##plugin)                                      \
      delete g##plugin;                                 \
  }                                                     

/** @} */   // End of group: Resource
/** @} */   // End of group: Common

#endif // __ZSPACE_PLUGIN_DEFINES_H__