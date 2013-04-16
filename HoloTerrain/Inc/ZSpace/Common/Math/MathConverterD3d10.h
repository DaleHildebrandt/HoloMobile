//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       MathConverterD3d10.h
//  Content:    The math conversion class for converting to and from
//              D3d10 based math objects.
//  $Id:        
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_MATH_CONVERTER_D3D10_H__
#define __ZSPACE_MATH_CONVERTER_D3D10_H__

#include "ZSpace/Common/CommonDefines.h"

#include "ZSpace/Common/Math/Matrix4.h"

#include <d3d10.h>

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

/** Class which converts various zSpace structures into D3d10 format.
*/
class MathConverterD3d10
{
public:
  /**
  * Convert a zSpace Matrix4 to a matrix in D3d10 format.
  *
  * @param[in] src
  *   A matrix in zSpace Matrix4 format.
  * @param[out] dest
  *   A matrix in D3d10 format.
  */
  static void convertMatrix4ToMatrixD3d10(const zspace::common::Matrix4& src, D3DMATRIX* dest);

  /**
  * Convert a matrix in D3d10 format to a zSpace Matrix4.
  *
  * @param[in] src
  *   A matrix in D3d10 format.
  * @param[out] dest
  *   A matrix in zSpace Matrix4 format.
  */
  static void convertMatrixD3d10ToMatrix4(const D3DMATRIX* src, zspace::common::Matrix4& dest);
};

/** @} */   // End of group: Math
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#include "ZSpace/Common/Math/MathConverterD3d.inl"

ZSPACE_COMMON_NS_BEGIN

inline void MathConverterD3d10::convertMatrix4ToMatrixD3d10(const zspace::common::Matrix4& src, D3DMATRIX* dest)
{
  convertMatrix4ToMatrixD3d(src, dest);
}


inline void MathConverterD3d10::convertMatrixD3d10ToMatrix4(const D3DMATRIX* src, zspace::common::Matrix4& dest)
{
  convertMatrixD3dToMatrix4(src, dest);
}

ZSPACE_COMMON_NS_END

#endif // __ZSPACE_MATH_CONVERTER_D3D10_H__