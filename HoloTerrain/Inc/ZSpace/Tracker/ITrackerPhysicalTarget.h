//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ITrackerPhysicalTarget.h
//  Content:    The pure virtual interface for ZSpace tracker target plugins.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_I_TRACKER_PHYSICAL_TARGET_H__
#define __ZSPACE_I_TRACKER_PHYSICAL_TARGET_H__

#include "ZSpace/Tracker/TrackerDefines.h"
#include "ZSpace/Tracker/ITrackerCapability.h"

#include "ZSpace/Common/Math/Matrix4.h"

ZSPACE_TRACKER_NS_BEGIN

/**
* @addtogroup Tracker
* @{
*/

/**
* Used as the basis for setting/getting properties of a target.
*
* @remark
*   Enable, disable and query state of the target. Queries
*   tracker capabilities (see ITrackerCapability). Sets/gets the
*   initial pose. Checks whether a specified target is visible
*   to its corresponding tracker device.
*/
class ITrackerPhysicalTarget
{
public:
  virtual ~ITrackerPhysicalTarget() = 0;

  /**
  * Get a tracker capability based on a specified type.
  *
  * @param[in] type
  *   The type of the tracker capability.
  *
  * @return
  *   A pointer to an ITrackerCapability. If the specified capability does not exist,
  *   NULL is returned.
  */
  virtual ITrackerCapability* getCapability(ITrackerCapability::Type type) = 0;

  /**
  * Set whether or not the physical target is enabled.
  */
  virtual bool setEnabled(bool isEnabled) = 0;

  /**
  * Check whether or not the physical target is enabled.
  */
  virtual bool isEnabled() = 0;

  /**
  * Set the initial pose of a specified target.
  *
  * @remark
  *   This is only applicable for devices that support relative poses.
  *   For example: The space navigator only generates relative positions
  *   and orientations, so the initial pose will act effectively as a
  *   world transform offset.
  *
  * @param[in] pose
  *   The initial pose for the target.
  *
  * @return
  *   True for success, false for failure.
  */
  virtual bool setInitialPose(const zspace::common::Matrix4& initialPose) = 0;

  /**
  * Get the local pose of a specified target.
  *
  * @param[out] localPose
  *   The local pose of the target. This is the raw pose extracted from the
  *   physical device without any calibration applied.
  *
  * @return
  *   True for success, false for failure.
  */
  virtual bool getLocalPose(zspace::common::Matrix4& localPose) = 0;

  /**
  * Check whether or not a specified target is visible to its corresponding 
  * tracker device.
  */
  virtual bool isVisible() = 0;
};


inline ITrackerPhysicalTarget::~ITrackerPhysicalTarget()
{
}

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_I_TRACKER_PHYSICAL_TARGET_H__