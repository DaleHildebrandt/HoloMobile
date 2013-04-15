//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ITrackerVibrateCapability.h
//  Content:    Pure virtual interface for accessing vibration capabilities 
//              on tracker targets.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_I_TRACKER_VIBRATE_CAPABILITY_H__
#define __ZSPACE_I_TRACKER_VIBRATE_CAPABILITY_H__

#include "ZSpace/Tracker/TrackerDefines.h"
#include "ZSpace/Tracker/ITrackerCapability.h"

ZSPACE_TRACKER_NS_BEGIN

/**
* @addtogroup Tracker
* @{
*/

/**
* Used for vibration control of the tracker target.
*
* @remark
*   The vibration capability is an abstract API over the
*   actual hardware implementation of the tracker target so
*   that different hardware implementations share a common
*   interface that can be understood by the application.
*/
class ITrackerVibrateCapability : public ITrackerCapability
{
public:
  virtual ~ITrackerVibrateCapability() = 0;

  /**
  * Set whether or not vibration is enabled.  This only determines
  * whether the appropriate command is sent to the hardware
  * if startVibration() is called.
  *
  * @remark
  *   If the device is already vibrating, stopVibration() should be
  *   called to stop the current vibration.
  *
  * @return
  *   True if enabling/disabling the vibration succeeded.
  *   False for failure.
  */
  virtual bool setEnabled(bool isEnabled) = 0;

  /**
  * Check whether or not vibration is enabled.
  *
  * @return
  *   True if the vibration device is enabled.  False for disabled.
  */
  virtual bool isEnabled() = 0;

  /**
  * Set the period for how long the device should not vibrate.
  *
  * @remark
  *   The actual period set will depend on the resolution of the
  *     motor in the device.
  *
  * @param[in] seconds
  *   The period in seconds.
  *
  * @return
  *   True if setting the period succeeded. False for failure.
  */
  virtual bool setOffPeriod(float seconds) = 0;

  /**
  * Get the off period of the vibration.
  *
  * @return
  *   The period of the vibration in seconds.
  */
  virtual float getOffPeriod() = 0;

  /**
  * Set the period for how long the device should vibrate.
  *
  * @remark
  *   The actual period set will depend on the resolution of the
  *     motor in the device.
  *
  * @param[in] seconds
  *   The period in seconds.
  *
  * @return
  *   True if setting the period succeeded. False for failure.
  */
  virtual bool setOnPeriod(float seconds) = 0;

  /**
  * Get the on period of the vibration.
  *
  * @return
  *   The period of the vibration in seconds.
  */
  virtual float getOnPeriod() = 0;

  /**
  * Set the repeat count of the vibration.
  *
  *   "repeat count" is the number of vibration cycles that occur after the initial vibration.
  *    If the value passed in is non-negative, one period of "on" then "off" will occur,
  *    followed by [repeatCount] additional periods. If the value is negative, it will continue
  *    to vibrate indefinitely or until stopVibration() is called.
  *
  *     (repeatCount = 0: 1 vibration + 0 additional = 1 total)
  *
  *     (repeatCount = 1: 1 vibration + 1 additional = 2 total)
  *
  *     (repeatCount = -1: infinite)
  *
  * @param[in] repeatCount
  *   The number of times the vibration on/off pattern should repeat after the initial vibration.
  *   A negative value means infinite repetition.
  *
  * @return
  *   True if setting the repeat count succeeded. False for failure (this should reflect the
  *    actual capability of a device, e.g. it does not support infinite repetition, or maybe
  *    it can only support one vibration and no repeats).
  */
  virtual bool setRepeatCount(int repeatCount) = 0;

  /**
  * Get the repeat count of the vibration.
  *
  * "repeat count" is the number of vibration cycles that occur after the initial vibration.
  *
  * @return
  *   The repeat count of the vibration.
  */
  virtual int getRepeatCount() = 0;

  /**
  * Starts vibrating the device by repeating the specified on and off periods.
  *
  * @return
  *   True if the vibration was successfully done.  False for failure.
  */
  virtual bool startVibration() = 0;

  /**
  * If the device is currently vibrating, stops vibrating the device.
  *
  * @remark
  *   If startVibration() is called again, the device will start vibrating the
  *   full sequence of on and off periods.
  *
  * @return
  *   True if the vibration was successfully stopped.  False for failure.
  */
  virtual bool stopVibration() = 0;
};


inline ITrackerVibrateCapability::~ITrackerVibrateCapability()
{
}

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_I_TRACKER_VIBRATE_CAPABILITY_H__