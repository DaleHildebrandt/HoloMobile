//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       TrackerDefines.h
//  Content:    Definitions for the ZSpace Tracker Library.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_TRACKER_DEFINES_H__
#define __ZSPACE_TRACKER_DEFINES_H__

// dll import/exports or static API
#if (defined(_LIB) || defined(ZSPACE_STATIC) || defined(ZSPACE_TRACKER_STATIC))
#define ZSPACE_TRACKER_API
#elif defined(ZSPACE_TRACKER_EXPORTS)
#define ZSPACE_TRACKER_API __declspec(dllexport)
#else
#define ZSPACE_TRACKER_API __declspec(dllimport)
#endif

// Compiler warning enable/disable macros.
#ifdef _WIN32
#define ZSPACE_TRACKER_DISABLE_WARNINGS \
  __pragma(warning( push ))             \
  __pragma(warning( disable : 4251 ))

#define ZSPACE_TRACKER_ENABLE_WARNINGS \
  __pragma(warning( pop ))
#else
#define ZSPACE_TRACKER_DISABLE_WARNINGS
#define ZSPACE_TRACKER_ENABLE_WARNINGS
#endif

// Namespace begin/end macros.
#define ZSPACE_TRACKER_NS_BEGIN namespace zspace { namespace tracker { \
  ZSPACE_TRACKER_DISABLE_WARNINGS

#define ZSPACE_TRACKER_NS_END   } } \
  ZSPACE_TRACKER_ENABLE_WARNINGS

#endif // __ZSPACE_TRACKER_DEFINES_H__