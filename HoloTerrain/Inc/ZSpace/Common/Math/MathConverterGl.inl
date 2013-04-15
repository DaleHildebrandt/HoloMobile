//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       MathConverterGl.inl
//  Content:    The math conversion class for converting to and from
//              OpenGl based math objects.
//  $Id:        
//
//////////////////////////////////////////////////////////////////////////////

#include "ZSpace/Common/Math/MathConverterGl.h"

ZSPACE_COMMON_NS_BEGIN

inline void MathConverterGl::convertMatrix3ToMatrixGl(const zspace::common::Matrix3& src, GLfloat* dest /*[9]*/)
{
  if (!dest)
    return;

  // The OpenGl 3x3 matrix format is just a transpose of our own Matrix3 format.
  for (unsigned int i = 0; i < 3; i++)
    for (unsigned int j = 0; j < 3; j++)
      dest[i*3+j] = src[j][i];
}


inline void MathConverterGl::convertMatrixGlToMatrix3(const GLfloat* src /*[9]*/, zspace::common::Matrix3& dest)
{
  if (!src)
    return;

  // The OpenGl 3x3 matrix format is just a transpose of our own Matrix3 format.
  for (unsigned int i = 0; i < 3; i++)
    for (unsigned int j = 0; j < 3; j++)
      dest[i][j] = src[j*3+i];
}


inline void MathConverterGl::convertMatrix4ToMatrixGl(const zspace::common::Matrix4& src, GLfloat* dest /*[16]*/)
{
  if (!dest)
    return;

  // The OpenGl 4x4 matrix format is just a transpose of our own Matrix4 format.
  for (unsigned int i = 0; i < 4; i++)
    for (unsigned int j = 0; j < 4; j++)
      dest[i*4+j] = src[j][i];
}


inline void MathConverterGl::convertMatrixGlToMatrix4(const GLfloat* src /*[16]*/, zspace::common::Matrix4& dest)
{
  if (!src)
    return;

  // The OpenGl 4x4 matrix format is just a transpose of our own Matrix4 format.
  for (unsigned int i = 0; i < 4; i++)
    for (unsigned int j = 0; j < 4; j++)
      dest[i][j] = src[j*4+i];
}


inline void MathConverterGl::convertVector3ToVectorGl(const zspace::common::Vector3& src, GLfloat* dest /*[3]*/)
{
  if (!dest)
    return;

  for (unsigned int i = 0; i < 3; i++)
    dest[i] = src[i];
}


inline void MathConverterGl::convertVectorGlToVector3(const GLfloat* src /*[3]*/, zspace::common::Vector3& dest)
{
  if (!src)
    return;

  for (unsigned int i = 0; i < 3; i++)
    dest[i] = src[i];
}

ZSPACE_COMMON_NS_END
