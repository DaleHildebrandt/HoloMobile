//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       MathConverterGl.h
//  Content:    The math conversion class for converting to and from
//              OpenGl based math objects.
//  $Id:        
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_MATH_CONVERTER_GL_H__
#define __ZSPACE_MATH_CONVERTER_GL_H__

#include "ZSpace/Common/CommonDefines.h"

#include "ZSpace/Common/Math/Matrix3.h"
#include "ZSpace/Common/Math/Matrix4.h"
#include "ZSpace/Common/Math/Vector3.h"

#include <GL/gl.h>

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

/** Class which converts various zSpace structures into Open GL format.
*/
class MathConverterGl
{
public:
  /**
  * Convert a zSpace Matrix3 to a matrix in the form of an array of GLfloats.
  *
  * @param[in] src
  *   A matrix in zSpace Matrix3 format.
  * @param[out] dest
  *   A matrix in OpenGl format (9 element array of GLfloats).
  */
  static void convertMatrix3ToMatrixGl(const zspace::common::Matrix3& src, GLfloat* dest /*[9]*/);

  /**
  * Convert a matrix in the form of an array of GLfloats to a zSpace Matrix3.
  *
  * @param[in] src
  *   A matrix in OpenGl format (9 element array of GLfloats).
  * @param[out] dest
  *   A matrix in zSpace Matrix3 format.
  */
  static void convertMatrixGlToMatrix3(const GLfloat* src /*[9]*/, zspace::common::Matrix3& dest);

  /**
  * Convert a zSpace Matrix4 to a matrix in the form of an array of GLfloats.
  *
  * @param[in] src
  *   A matrix in zSpace Matrix4 format.
  * @param[out] dest
  *   A matrix in OpenGl format (16 element array of GLfloats).
  */
  static void convertMatrix4ToMatrixGl(const zspace::common::Matrix4& src, GLfloat* dest /*[16]*/);

  /**
  * Convert a matrix in the form of an array of GLfloats to a zSpace Matrix4.
  *
  * @param[in] src
  *   A matrix in OpenGl format (16 element array of GLfloats).
  * @param[out] dest
  *   A matrix in zSpace Matrix4 format.
  */
  static void convertMatrixGlToMatrix4(const GLfloat* src /*[16]*/, zspace::common::Matrix4& dest);

  /**
  * Convert a zSpace Vector3 to a 3 component vector in the form of an array of GLfloats.
  *
  * @param[in] src
  *   A vector in zSpace Vector3 format.
  * @param[out] dest
  *   A vector in OpenGl format (3 element array of GLfloats).
  */
  static void convertVector3ToVectorGl(const zspace::common::Vector3& src, GLfloat* dest /*[3]*/);

  /**
  * Convert a 3-component vector in the form of an array of GLfloats to a zSpace Vector3.
  *
  * @param[in] src
  *   A vector in OpenGl format (3 element array of GLfloats).
  * @param[out] dest
  *   A vector in zSpace Vector3 format.
  */
  static void convertVectorGlToVector3(const GLfloat* src /*[3]*/, zspace::common::Vector3& dest);
};

/** @} */   // End of group: Math
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#include "ZSpace/Common/Math/MathConverterGl.inl"

#endif // __ZSPACE_MATH_CONVERTER_GL_H__