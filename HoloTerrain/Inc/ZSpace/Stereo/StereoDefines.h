//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       StereoDefines.h
//  Content:    Definitions for the ZSpace Stereo Library.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_STEREO_DEFINES_H__
#define __ZSPACE_STEREO_DEFINES_H__

// dll import/exports or static API
#if (defined(_LIB) || defined(ZSPACE_STATIC) || defined(ZSPACE_STEREO_STATIC))
#define ZSPACE_STEREO_API
#elif defined(ZSPACE_STEREO_EXPORTS)
#define ZSPACE_STEREO_API __declspec(dllexport)
#else
#define ZSPACE_STEREO_API __declspec(dllimport)
#endif

// Compiler warning enable/disable macros.
#ifdef _WIN32
#define ZSPACE_STEREO_DISABLE_WARNINGS \
  __pragma(warning( push ))            \
  __pragma(warning( disable : 4251 ))

#define ZSPACE_STEREO_ENABLE_WARNINGS \
  __pragma(warning( pop ))
#else
#define ZSPACE_STEREO_DISABLE_WARNINGS
#define ZSPACE_STEREO_ENABLE_WARNINGS
#endif

// Namespace begin/end macros.
#define ZSPACE_STEREO_NS_BEGIN namespace zspace { namespace stereo { \
  ZSPACE_STEREO_DISABLE_WARNINGS

#define ZSPACE_STEREO_NS_END   } } \
  ZSPACE_STEREO_ENABLE_WARNINGS

#endif // __ZSPACE_STEREO_DEFINES_H__