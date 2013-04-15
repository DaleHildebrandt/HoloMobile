//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       TrackerDisplayIntersection.h
//  Content:    Test intersection of the tracker pose against displays.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_TRACKER_DISPLAY_INTERSECTION_H__
#define __ZSPACE_TRACKER_DISPLAY_INTERSECTION_H__

#include "ZSpace/Tracker/TrackerDefines.h"

#include "ZSpace/Common/Math/Matrix3.h"
#include "ZSpace/Common/Math/Matrix4.h"
#include "ZSpace/Common/Math/Vector2.h"
#include "ZSpace/Common/Math/Vector3.h"

#include "ZSpace/Common/Reporting/Log.h"
#include "ZSpace/Common/System/DisplayInfo.h"

ZSPACE_TRACKER_NS_BEGIN

/**
* @addtogroup Tracker
* @{
*/

/**
* This class provides the ability to determine the virtual x and y screen
* at which a virtual ray intersects a physical display.  By default, only
* intersections with the zSpace display are enabled.
*/
class ZSPACE_TRACKER_API TrackerDisplayIntersection
{
public:
  /**
  * The main struct associated with TrackerDisplayIntersection.
  */
  struct IntersectionInfo
  {
    /**
    * The default constructor.
    */
    IntersectionInfo()
    : displayId(-1),
      virtualScreenX(-1),
      virtualScreenY(-1),
      normalizedX(0),
      normalizedY(0),
      distance(0)
    {}

    int   displayId;      /**< The id of the display that was intersected. -1 denotes no displays intersected. */
    int   virtualScreenX; /**< Virtual X coordinate of the screen. */
    int   virtualScreenY; /**< Virtual Y coordinate of the screen. */
    int   normalizedX;    /**< Normalized X screen coordinate (0 - 65535, currently only Windows is supported). */
    int   normalizedY;    /**< Normalized Y screen coordinate (0 - 65535, currently only Windows is supported). */
    float distance;       /**< The intersection distance from the ray origin to the display. */
  };

public:
  /** 
  * Test whether a ray of given pose intersects with any displays.
	*
  * @warning
  *   This is not thread-safe.
  *
  * @param[in] rayTransform
  *   The transform of a ray in GL format but row major (rotation and translation). 
  * @param[out] intersectionInfo
  *   A struct containing all the information pertaining to the intersection test.
  */
  static void intersect(const zspace::common::Matrix4& rayTransform, 
                        TrackerDisplayIntersection::IntersectionInfo& intersectionInfo);

  /** 
  * Test whether a ray of given position and orientation intersects with any displays.
  *
  * @warning
  *   This is not thread-safe.
  *
  * @param[in] rayPosition
  *   The position of the ray.
  * @param[in] rayOrientation
  *   The orientation of the ray.
  * @param[out] intersectionInfo
  *   A struct containing all the information pertaining to the intersection test.
  */
  static void intersect(const zspace::common::Vector3& rayPosition, const zspace::common::Matrix3& rayOrientation, 
                        TrackerDisplayIntersection::IntersectionInfo& intersectionInfo);

  /** 
  * Set whether the specified display will be tested for ray intersections.
  *
  * @warning
  *   This is not thread-safe.
  *
  * @param[in] displayNumber
  *   The number displayed in "Display Properties" > "Settings" for the desired display.
  * @param[in] isIntersectable
  *   Pass true for yes, false for no.
  */
  static void setDisplayIntersectable(int displayNumber, bool isIntersectable);

  /** 
  * Returns whether the specified display will be tested for ray intersections.
  *
  * @warning
  *   This is not thread-safe.
  *
  * @param[in] displayNumber
  *   The number displayed in "Display Properties" > "Settings" for the desired display.
  *   
  * @return
  *   True if the display will be tested for intersections, false otherwise.
  */
  static bool isDisplayIntersectable(int displayNumber);

private:
  static bool   m_isInitialized;
  static int    m_numDisplays;

  static int    m_topLeft[2];
  static int    m_virtualDisplaySize[2];

private:
  static void initialize();
  static int getMaximalSizeOfNormalizedWindowCoordinate();
};

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_TRACKER_DISPLAY_INTERSECTION_H__