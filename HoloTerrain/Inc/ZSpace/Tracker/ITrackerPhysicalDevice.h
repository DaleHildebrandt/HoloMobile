//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ITrackerPhysicalDevice.h
//  Content:    The pure virtual interface for ZSpace tracker device plugins.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_I_TRACKER_PHYSICAL_DEVICE_H__
#define __ZSPACE_I_TRACKER_PHYSICAL_DEVICE_H__

#include "ZSpace/Tracker/TrackerDefines.h"
#include "ZSpace/Tracker/ITrackerPhysicalTarget.h"

#include "ZSpace/Common/Math/Matrix4.h"

ZSPACE_TRACKER_NS_BEGIN

/**
* @addtogroup Tracker
* @{
*/

/**
* The pure virtual interface for pluggable tracker devices.
*
* @remark
*   This pure virtual interface is used for all TrackerDevice plugins
*   such as the IZTrackerDevice, PolhemusTrackerDevice, PstTrackerDevice,
*   etc. Its main functionality is to update and query a specified
*   target's 6DoF (http://en.wikipedia.org/wiki/Six_degrees_of_freedom)
*   pose from the physical tracking device.
*/
class ITrackerPhysicalDevice 
{
public:
  virtual ~ITrackerPhysicalDevice() = 0;

  /**
  * Set whether or not the physical tracker device is enabled.
  *
  * @return
  *   True for success, false for failure.
  */
  virtual bool setEnabled(bool isEnabled) = 0;

  /**
  * Check whether or not the physical tracker device is enabled.
  */
  virtual bool isEnabled() = 0;

  /**
  * Capture and update information for all active targets.
  */
  virtual bool captureTargets() = 0;

  /**
  * Get a physical target based on a specified id.
  */
  virtual ITrackerPhysicalTarget* getTarget(int targetId) = 0;

  /**
  * Get the physical device's current time.
  *
  * @return
  *   The current time in seconds.
  */
  virtual double getCurrentTime() = 0;
};


inline ITrackerPhysicalDevice::~ITrackerPhysicalDevice()
{
}

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_I_TRACKER_PHYSICAL_DEVICE_H__