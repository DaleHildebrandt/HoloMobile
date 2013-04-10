//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       BoundingBox.h
//  Content:    The BoundingBox class
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_BOUNDING_BOX_H__
#define __ZSPACE_BOUNDING_BOX_H__

#include "ZSpace/Common/Math/Math.h"
#include "ZSpace/Common/Math/Vector3.h"
#include "ZSpace/Common/Math/Bounds.h"
#include "ZSpace/Common/Math/Transform.h"
#include "ZSpace/Common/Math/Ray.h"
#include "ZSpace/Common/Math/BoundingSphere.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

class BoundingSphere;

/**
* This is the BoundingBox class.
*/
class ZSPACE_COMMON_API BoundingBox: public Bounds
{

public:
  /**
	* @brief
  *   The default constructor for the BoundingBox.
	*/
	BoundingBox();

  /**
	* @brief
  *   Construct a BoundingBox with the given lower and upper corners.
	*/
	BoundingBox(const Vector3& lower, const Vector3& upper);

  /**
	* @brief
  *   The default destructor for the BoundingBox.
	*/
	~BoundingBox();
  
  /**
	* @brief
  *   Set the lower corner for this bounding box.
  *
  * @param[in] lower  The lower corner for this bounding box.
	*/
  void setLower(const Vector3& lower);

  /**
	* @brief
  *   Get the lower corner for this bounding box.
  *
  * @return The lower corner of the bounding box.
	*/
  const Vector3& getLower() const;
  
  /**
	* @brief
  *   Set the upper corner for this bounding box.
  *
  * @param[in] upper  The upper corner for this bounding box.
	*/
  void setUpper(const Vector3& upper);

  /**
	* @brief
  *   Get the upper corner for this bounding box.
  *
  * @return The upper corner of the bounding box.
	*/
  const Vector3& getUpper() const;

  /**
	* @brief
  *   Get the center of this bounding box.
  *
  * @return The center of the bounding box.
	*/
  Vector3 getCenter() const;

  /**
	* @brief
  *   Set this bounds to the one given.
  *
  * @param[in] bounds  The bounds to set this bounds from.
	*/
  virtual void operator = (const BoundingBox& bounds);
  virtual void operator = (const BoundingSphere& bounds);

  /**
	* @brief
  *   Check the equality of the given bounds to this one.
  *
  * @param[in] bounds  The bounds to check.
  * @return A boolean indicating whether or not the bounds are equal.
	*/
  virtual bool operator == (const BoundingBox& bounds) const;
  virtual bool operator == (const BoundingSphere& bounds) const;

  /**
	* @brief
  *   Check the inequality of the given bounds to this one.
  *
  * @param[in] bounds  The bounds to check.
  * @return A boolean indicating whether or not the bounds are unequal.
	*/
  virtual bool operator != (const BoundingBox& bounds) const;
  virtual bool operator != (const BoundingSphere& bounds) const;

  /**
	* @brief
  *   Combine the given bounds with this one.
  *
  * @param[in] bounds  The bounds to combine with this one.
	*/
  virtual void combine(const BoundingBox &bounds);
  virtual void combine(const BoundingSphere &bounds);

  /**
	* @brief
  *   Transform this bounds by the given transform.
  *
  * @param[in] transform  The transform to apply to this bounds.
	*/
  void transform(const Transform& transform);

  /**
	* @brief
  *   Intersect the given ray with this Bounds.
  *
  * @param[in] ray  The ray to check against.
  * @return A boolean indicating whether or not the ray intersects this bounds.
	*/
  bool intersect(const Ray& ray) const;

  /**
	* @brief
  *   Intersect the given bounds with this Bounds.
  *
  * @param[in] bounds  The bounds to check against.
  * @return A boolean indicating whether or not the bounds intersects this bounds.
	*/
  virtual bool intersect(const BoundingBox& bounds) const;
  virtual bool intersect(const BoundingSphere& bounds) const;

private:
  Vector3 m_lower;
  Vector3 m_upper;
};

/** @} */   // End of group: Math
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#endif