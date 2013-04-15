//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Bounds.h
//  Content:    The Bounds class
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_BOUNDS_H__
#define __ZSPACE_BOUNDS_H__

#include "ZSpace/Common/Math/Math.h"
#include "ZSpace/Common/Math/Transform.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Math
*   @{
*/

/**
* This is the base Bounds class.
*/
class ZSPACE_COMMON_API Bounds
{

public:
  /**
	* @brief
  *   The default constructor for the Bounds.
	*/
	Bounds();

  /**
	* @brief
  *   The default destructor for the Bounds.
	*/
	virtual ~Bounds();

  /**
	* @brief
  *   Set the empty flag for this bounds.  Making a bounds empty
  *   automatically sets its infinite flag to false.
  *
  * @param[in] empty  The empty value for this bounds.
	*/
  void setEmpty(bool empty);

  /**
	* @brief
  *   Get the empty flag for this bounds.
  *
  * @return A boolean indicating whether or not the bounds is empty.
	*/
  bool isEmpty() const;

  /**
	* @brief
  *   Set the infinite flag for this bounds.  Making a bounds infinite
  *   automatically sets its empty flag to false.
  *
  * @param[in] infinite  The infinite value for this bounds.
	*/
  void setInfinite(bool infinite);

  /**
	* @brief
  *   Get the infinite flag for this bounds.
  *
  * @return A boolean indicating whether or not the bounds is infinite.
	*/
  bool isInfinite() const;

  /**
	* @brief
  *   Set this bounds to the one given.
  *
  * @param[in] bounds  The bounds to set this bounds from.
	*/
  virtual void operator = (const Bounds& bounds);

  /**
	* @brief
  *   Check the equality of the given bounds to this one.
  *
  * @param[in] bounds  The bounds to check.
  * @return A boolean indicating whether or not the bounds are equal.
	*/
  virtual bool operator == (const Bounds& bounds) const;

  /**
	* @brief
  *   Check the inequality of the given bounds to this one.
  *
  * @param[in] bounds  The bounds to check.
  * @return A boolean indicating whether or not the bounds are unequal.
	*/
  virtual bool operator != (const Bounds& bounds) const;

  /**
	* @brief
  *   Combine the given bounds with this one.
  *
  * @param[in] bounds  The bounds to combine with this one.
	*/
  virtual void combine(const Bounds& bounds);

  /**
	* @brief
  *   Transform this bounds by the given transform.
  *
  * @param[in] transform  The transform to apply to this bounds.
	*/
  virtual void transform(const Transform& transform);

  /**
	* @brief
  *   Intersect the given ray with this Bounds.
  *
  * @param[in] ray  The ray to check against.
  * @return A boolean indicating whether or not the ray intersects this bounds.
	*/
  virtual bool intersect(const Ray& ray) const;

  /**
	* @brief
  *   Intersect the given bounds with this Bounds.
  *
  * @param[in] bounds  The bounds to check against.
  * @return A boolean indicating whether or not the bounds intersects this bounds.
	*/
  virtual bool intersect(const Bounds& bounds) const;

protected:
  bool m_isEmpty;
  bool m_isInfinite;
};

/** @} */   // End of group: Math
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#endif