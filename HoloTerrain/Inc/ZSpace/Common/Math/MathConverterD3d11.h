//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       MathConverterD3d11.h
//  Content:    The math conversion class for converting to and from
//              D3d11 based math objects.
//  $Id:        
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_MATH_CONVERTER_D3D11_H__
#define __ZSPACE_MATH_CONVERTER_D3D11_H__

#include "ZSpace/Common/CommonDefines.h"

#include "ZSpace/Common/Math/Matrix4.h"

#include <xnamath.h>


ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

/** Class which converts various zSpace structures into D3d11 format.
*/
class MathConverterD3d11
{
public:
  /**
  * Convert a zSpace Matrix4 to a matrix in D3d11 format.
  *
  * @param[in] src
  *   A matrix in zSpace Matrix4 format.
  * @param[out] dest
  *   A matrix in D3d11 format.
  */
  static void convertMatrix4ToMatrixD3d11(const zspace::common::Matrix4& src, XMMATRIX* dest);

  /**
  * Convert a matrix in D3d11 format to a zSpace Matrix4.
  *
  * @param[in] src
  *   A matrix in D3d11 format.
  * @param[out] dest
  *   A matrix in zSpace Matrix4 format.
  */
  static void convertMatrixD3d11ToMatrix4(const XMMATRIX* src, zspace::common::Matrix4& dest);
};

/** @} */   // End of group: Math
/** @} */   // End of group: Common

inline void MathConverterD3d11::convertMatrix4ToMatrixD3d11(const zspace::common::Matrix4& src, XMMATRIX* dest)
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


inline void MathConverterD3d11::convertMatrixD3d11ToMatrix4(const XMMATRIX* src, zspace::common::Matrix4& dest)
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

#endif // __ZSPACE_MATH_CONVERTER_D3D11_H__