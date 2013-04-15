//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       CommonDefines.h
//  Content:    Definitions for the ZSpace Common Library.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_COMMON_DEFINES_H__
#define __ZSPACE_COMMON_DEFINES_H__

// dll import/exports or static API
#if (defined(_LIB) || defined(ZSPACE_STATIC) || defined(ZSPACE_COMMON_STATIC))
#define ZSPACE_COMMON_API
#elif defined(ZSPACE_COMMON_EXPORTS)
#define ZSPACE_COMMON_API __declspec(dllexport)
#else
#define ZSPACE_COMMON_API __declspec(dllimport)
#endif

// Compiler warning enable/disable macros.
#ifdef _WIN32
#define ZSPACE_COMMON_DISABLE_WARNINGS \
  __pragma(warning( push ))            \
  __pragma(warning( disable : 4251 ))

#define ZSPACE_COMMON_ENABLE_WARNINGS \
  __pragma(warning( pop ))
#else
#define ZSPACE_COMMON_DISABLE_WARNINGS
#define ZSPACE_COMMON_ENABLE_WARNINGS
#endif

// Namespace begin/end macros.
#define ZSPACE_COMMON_NS_BEGIN namespace zspace { namespace common { \
  ZSPACE_COMMON_DISABLE_WARNINGS

#define ZSPACE_COMMON_NS_END   } } \
  ZSPACE_COMMON_ENABLE_WARNINGS

#define MAX_STRING 2048

#endif // __ZSPACE_COMMON_DEFINES_H__