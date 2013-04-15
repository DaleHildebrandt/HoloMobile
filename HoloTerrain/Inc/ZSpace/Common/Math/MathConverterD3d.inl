//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       MathConverterD3d.cpp
//  Content:    The math conversion class for converting to and from
//              D3d based math objects.
//  $Id:        
//
//////////////////////////////////////////////////////////////////////////////

ZSPACE_COMMON_NS_BEGIN


inline void convertMatrix4ToMatrixD3d(const zspace::common::Matrix4& src, D3DMATRIX* dest)
{
  if (!dest)
    return;

  // Transpose the matrix.
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      dest->m[i][j] = src[j][i];

  // Negate the z-direction.
  for (int i = 0; i < 4; i++)
    dest->m[2][i] = -dest->m[2][i];

  // Negate the z-components.
  for (int i = 0; i < 4; i++)
    dest->m[i][2] = -dest->m[i][2];
}


inline void convertMatrixD3dToMatrix4(const D3DMATRIX* src, zspace::common::Matrix4& dest)
{
  if (!src)
    return;

  // Transpose the matrix.
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      dest[i][j] = src->m[j][i];

  // Negate the z-direction.
  for (int i = 0; i < 4; i++)
    dest[2][i] = -dest[2][i];

  // Negate the z-components.
  for (int i = 0; i < 4; i++)
    dest[i][2] = -dest[i][2];
}

ZSPACE_COMMON_NS_END