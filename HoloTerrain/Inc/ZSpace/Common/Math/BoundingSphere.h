//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       BoundingSphere.h
//  Content:    The BoundingSphere class
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_BOUNDING_SPHERE_H__
#define __ZSPACE_BOUNDING_SPHERE_H__

#include "ZSpace/Common/Math/Math.h"
#include "ZSpace/Common/Math/Vector3.h"
#include "ZSpace/Common/Math/Bounds.h"
#include "ZSpace/Common/Math/Transform.h"
#include "ZSpace/Common/Math/Ray.h"
#include "ZSpace/Common/Math/BoundingBox.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

class BoundingBox;

/**
* This is the base class for spherical Bounds.
*/
class ZSPACE_COMMON_API BoundingSphere: public Bounds
{

public:
  /**
	* @brief
  *   The default constructor for the BoundingSphere.
	*/
	BoundingSphere();

  /**
	* @brief
  *   Construct a BoundingSphere with the given center and radius.
	*/
	BoundingSphere(const Vector3& center, const float radius);

  /**
	* @brief
  *   The default destructor for the BoundingSphere.
	*/
	virtual ~BoundingSphere();
  
  /**
	* @brief
  *   Set the center for this bounding sphere.
  *
  * @param[in] center  The center for this bounding sphere.
	*/
  void setCenter(const Vector3& center);

  /**
	* @brief
  *   Get the center for this bounding sphere.
  *
  * @return The center of the bounding sphere.
	*/
  const Vector3& getCenter() const;
  
  /**
	* @brief
  *   Set the radius for this bounding sphere.
  *
  * @param[in] radius  The radius for this bounding sphere.
	*/
  void setRadius(float radius);

  /**
	* @brief
  *   Get the radius for this bounding sphere.
  *
  * @return The radius for this bounding sphere.
	*/
  float getRadius() const;

  /**
	* @brief
  *   Set this bounds to the one given.
  *
  * @param[in] bounds  The bounds to set the bounds for this object.
	*/
  virtual void operator = (const BoundingSphere& bounds);
  virtual void operator = (const BoundingBox& bounds);

  /**
	* @brief
  *   Check the equality of the given bounds to this one.
  *
  * @param[in] bounds  The bounds to check.
  * @return A boolean indicating whether or not the bounds are equal.
	*/
  virtual bool operator == (const BoundingSphere& bounds) const;
  virtual bool operator == (const BoundingBox& bounds) const;

  /**
	* @brief
  *   Check the inequality of the given bounds to this object.
  *
  * @param[in] bounds  The bounds to check.
  * @return A boolean indicating whether or not the bounds are unequal.
	*/
  virtual bool operator != (const BoundingSphere& bounds) const;
  virtual bool operator != (const BoundingBox& bounds) const;

  /**
	* @brief
  *   Combine the given bounds with this one.
  *
  * @param[in] bounds  The bounds to combine with this object.
	*/
  virtual void combine(const BoundingSphere& bounds);
  virtual void combine(const BoundingBox& bounds);

  /**
	* @brief
  *   Transform this bounds by the given transform.
  *
  * @param[in] transform  The transform to apply to the bounds for this object.
	*/
  void transform(const Transform& transform);

  /**
	* @brief
  *   Intersect the given ray with this Bounds.
  *
  * @param[in] ray  The ray to check against.
  * @return A boolean indicating whether or not the ray intersects the bounds for this object.
	*/
  bool intersect(const Ray& ray) const;

  /**
	* @brief
  *   Intersect the given bounds with this Bounds.
  *
  * @param[in] bounds  The bounds to check against.
  * @return A boolean indicating whether or not the bounds intersects the bounds for this object.
	*/
  bool intersect(const BoundingSphere& bounds) const;
  bool intersect(const BoundingBox& bounds) const;

private:
  Vector3 m_center;
  float   m_radius;
};

/** @} */   // End of group: Math
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#endif