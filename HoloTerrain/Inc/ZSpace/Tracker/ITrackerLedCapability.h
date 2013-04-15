//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ITrackerLedCapability.h
//  Content:    Pure virtual interface for accessing LED capabilities on 
//              tracker targets.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_I_TRACKER_LED_CAPABILITY_H__
#define __ZSPACE_I_TRACKER_LED_CAPABILITY_H__

#include "ZSpace/Tracker/TrackerDefines.h"
#include "ZSpace/Tracker/ITrackerCapability.h"

#include "ZSpace/Common/Math/Vector3.h"

ZSPACE_TRACKER_NS_BEGIN

/**
* @addtogroup Tracker
* @{
*/

/**
* Access LED's on tracker devices.
*
* @remarks
*    This class allows one to enable a particular LED and specify its
*    color in the RGB color space using floating point numbers from 0.0
*    to 1.0 inclusive for each color (Red, Green, Blue). The state and
*    color of the LED can also be queried.
*/
class ITrackerLedCapability: public ITrackerCapability
{
public:
  virtual ~ITrackerLedCapability() = 0;

  /**
  * Set whether or not the LED is enabled.
  *
  * @param[in] isEnabled
  *   Flag denoting whether or not the LED is enabled.
  *
  * @return
  *   True if enabling/disabling the LED succeeded.  False if failed.
  */
  virtual bool setEnabled(bool isEnabled) = 0;

  /**
  * Check whether or not the LED is enabled.
  */
  virtual bool isEnabled() = 0;

  /**
  * Set the RGB color of the LED.
  *
  * @param[in] color
  *   A Vector3 with x, y, z values corresponding to r, g, b 
  *   respectively. Each component should in the 0.0f to 1.0f
  *   range (inclusive).
  *
  * @return
  *   True if setting the color succeeded. False if failed.
  */
  virtual bool setColor(const zspace::common::Vector3& color) = 0;

  /**
  * Get the RGB color of the LED.
  *
  * @return
  *   A Vector3 corresponding the current RGB color of the LED.
  */
  virtual const zspace::common::Vector3& getColor() = 0;
};


inline ITrackerLedCapability::~ITrackerLedCapability()
{
}

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_I_TRACKER_LED_CAPABILITY_H__