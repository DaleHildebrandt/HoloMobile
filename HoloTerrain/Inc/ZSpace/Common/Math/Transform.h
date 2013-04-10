//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Transform.h
//  Content:    The Transform class
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_TRANSFORM_H__
#define __ZSPACE_TRANSFORM_H__

#include "ZSpace/Common/Math/Math.h"
#include "ZSpace/Common/Math/Quaternion.h"
#include "ZSpace/Common/Math/Vector3.h"
#include "ZSpace/Common/Math/Matrix4.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

/**
* This is the container class for Transforms.
*/
class ZSPACE_COMMON_API Transform
{

public:
  /**
	* @brief
  *   The default constructor for the Transform.
	*/
	Transform();
	
  /**
	* @brief
  *   Construct a transform with the given parameters.
  *
  * @param[in] position  The translation component of this transform.
  * @param[in] scale  The scale component of this transform.
  * @param[in] orientation  The orientation component of this transform.
	*/
  Transform(const Vector3& position, const Vector3& scale, const Quaternion& orientation,
    const common::Vector3& linearVelocity = common::Vector3::ZERO(),
    const common::Quaternion& angularVelocity = common::Quaternion::IDENTITY());

  /**
  * Construct a transform from a matrix.
  */
  Transform(const Matrix4& matrix, const Matrix4& velocityMatrix = Matrix4::IDENTITY());

  /**
  *   The default destructor for the Transform.
	*/
	virtual ~Transform();

  /**
	* @brief
  *   Set the position for this transform.
  *
  * @param[in] position  The position for this transform.
	*/
  void setPosition(const Vector3& position);

  /**
	* @brief
  *   Get the position for this transform.
  *
  * @return A copy of the position for this Transform.
	*/
  const Vector3& getPosition() const;

  /**
	* @brief
  *   Set the scale for this transform.
  *
  * @param[in] scale  The scale for this transform.
	*/
  void setScale(const Vector3& scale);

  /**
	* @brief
  *   Get the scale for this transform.
  *
  * @return A copy of the scale for this Transform.
	*/
  const Vector3& getScale() const;

  /**
	* @brief
  *   Set the orientation for this transform.
  *
  * @param[in] orientation  The orientation for this transform.
	*/
  void setOrientation(const Quaternion& orientation);

  /**
	* @brief
  *   Get the orientation for this transform.
  *
  * @return The orientation for this Transform.
	*/
  const Quaternion& getOrientation() const;

  /**
  *   Set the linear velocity for this transform.
  *
  * @param[in] linearVelocity  The linear velocity for this transform.
	*/
  void setLinearVelocity(const Vector3& linearVelocity);

  /**
  *   Get the linear velocity for this transform.
  *
  * @return The linear velocity for this Transform.
	*/
  const Vector3& getLinearVelocity() const;

  /**
  *   Set the angular velocity for this transform.
  *
  * @param[in] angularVelocity  The angular velocity for this transform.
	*/
  void setAngularVelocity(const Quaternion& angularVelocity);

  /**
  *   Get the angular velocity for this transform.
  *
  * @return The angular velocity for this Transform.
	*/
  const Quaternion& getAngularVelocity() const;

  /**
  *   Append a translation to this transform.
  *
  * @param[in] translation  The translation to append to this transform.
	*/
  void translate(const Vector3& translation);

  /**
	* @brief
  *   Append a rotation to this transform.
  *
  * @param[in] rotation  The rotation to append to this transform.
	*/
  void rotate(const Quaternion& rotation);

  /**
	* @brief
  *   Append a scale to this transform.
  *
  * @param[in] scale  The scale to append to this transform.
	*/
  void scale(const float scaleFactor);

  /**
  *   Returns the inverse of this Transform.
  */
  Transform inverse() const;

  /**
	* @brief
  *   The multiply operator for the Transform object.
	*/
  Transform operator* (const Transform &t1) const;

  /**
  * Report whether two transforms are equal.
  */
  bool operator== (const Transform& t1) const;

  /**
  * Report whether two transforms are approximately equal.
  */
  bool equals(const Transform& t1) const;

  /**
  * Report whether two transforms are inequal.
  */
  bool operator!= (const Transform& t1) const;

  /**
  * Compute a 4x4 matrix corresponding to this transform.
  */
  Matrix4 getMatrix() const;

  /**
  * Compute a 4x4 matrix corresponding to the velocity component of this transform.
  */
  Matrix4 getVelocityMatrix() const;

  /**
  * The identity transform.
  */
  static const Transform& IDENTITY();

private:
  Vector3             m_position;
  Vector3             m_scale;
  Quaternion          m_orientation;
  Vector3             m_linearVelocity;
  Quaternion          m_angularVelocity;
  //TODO: If we move to a matrix-based transform, cache it here.
  //Matrix4             m_transform;
};

ZSPACE_COMMON_NS_END

#endif