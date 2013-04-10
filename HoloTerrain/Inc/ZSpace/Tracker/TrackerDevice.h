//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       TrackerDevice.h
//  Content:    The Tracker Device class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_TRACKER_DEVICE_H__
#define __ZSPACE_TRACKER_DEVICE_H__

#include <string>

#include "ZSpace/Tracker/TrackerDefines.h"
#include "ZSpace/Tracker/TrackerTarget.h"

#include "ZSpace/Common/Memory/List.h"
#include "ZSpace/Common/Memory/ReferenceObject.h"
#include "ZSpace/Common/Resource/PluginLoader.h"

ZSPACE_TRACKER_NS_BEGIN

class ITrackerPhysicalDevice;

/**
* @addtogroup Tracker
* @{
*/

/**
* The tracker device class, manages TrackerTargets (see TrackerTarget).
*
* @warning
*   This class is not thread-safe.
*
* @remark
*   Responsible for managing a set of TrackerTargets (see TrackerTarget).
*   The TrackerDevice can update its set of active targets via the
*   TrackerDevice::captureTargets() method. Never create or destroy
*   a TrackerDevice directly. The base TrackerSystem class is responsible
*   for loading, creating and destroying TrackerDevices.
*/
class ZSPACE_TRACKER_API TrackerDevice: public zspace::common::ReferenceObject
{
public:
  /**
  * The default constructor.
  *
  * @param[in] name
  *   The name of the tracker device.
  * @param[in] pluginName
  *   The name of the device's associated plugin.
  */
  TrackerDevice(const char* name, const char* pluginName);

  /**
  * The destructor.
  */
  virtual ~TrackerDevice();

  /**
  * Get the device's name.
  */
  const char* getName() const;

  /**
  * Get the device's plugin name.
  */
  const char* getPluginName() const;

  /**
  * Add a TrackerTarget to the device.
  *
  * @remark
  *   If the device contains a TrackerTarget with the same name as
  *   the one to be added, the add will be ignored.
  *
  * @param[in] trackerTarget
  *   A pointer to the TrackerTarget.
  */
  void addTarget(TrackerTarget* trackerTarget);

  /**
  * Remove a TrackerTarget from the device.
  *
  * @param[in] trackerTarget
  *   A point to the TrackerTarget to be removed.
  */
  void removeTarget(TrackerTarget* trackerTarget);

  /**
  * Remove a TrackerTarget from the device by its name.
  *
  * @param[in] targetName
  *   The name of the TrackerTarget.
  */
  void removeTargetByName(const char* targetName);

  /**
  * Remove all TrackerTargets (see TrackerTarget) from the device.
  */
  void removeAllTargets();

  /**
  * Get the number of TrackerTargets (see TrackerTarget) contained by the device.
  */
  unsigned int getNumTargets() const;

  /**
  * Get a TrackerTarget by name.
  *
  * @param[in] targetName
  *   The name of the TrackerTarget.
  *
  * @return
  *   A pointer to the TrackerTarget.
  */
  TrackerTarget* getTarget(const char* targetName);

  /**
  * Get a TrackerTarget by index.
  *
  * @param[in] index
  *   The index of the TrackerTarget.
  */
  TrackerTarget* getTargetByIndex(unsigned int index);

  /**
  * Capture the pose and any other information for TrackerTargets (see TrackerTarget)
  * associated with the device.
  */
  void captureTargets();

  /**
  * Set the base transform of the device.
  *
  * @param[in] baseTransform
  *   The base transform of the system. By default, the tracker device's calibration
  *   transform will be used.
  */
  void setBaseTransform(const zspace::common::Matrix4& baseTransform);

  /**
  * Get the base transform of the device.
  */
  const zspace::common::Matrix4& getBaseTransform() const;

  /**
  * Set whether or not the device is enabled.
  *
  * @remark
  *   If it is not physically connected, setting the device to enabled will fail.
  */
  void setEnabled(bool isEnabled);

  /**
  * Check whether or not the device is enabled.
  */
  bool isEnabled();

private:
  // Type definitions.
  typedef zspace::common::List<TrackerTarget*>                   TargetList;
  typedef zspace::common::PluginLoader<ITrackerPhysicalDevice*>  TrackerDevicePluginLoader;

private:
  std::string               m_name;
  std::string               m_pluginName;

  TrackerDevicePluginLoader m_trackerDevicePluginLoader;
  ITrackerPhysicalDevice*   m_iTrackerPhysicalDevice;
  bool                      m_isEnabled;
  
  TargetList                m_targets;
  zspace::common::Matrix4   m_baseTransform;

  double                    m_currentTime;
  double                    m_previousTime;
  double                    m_deltaTime;  
  double                    m_elapsedTime;
  double                    m_sampleRate;

private:
  void loadITrackerPhysicalDevice();
  TrackerTarget* getTargetById(int trackerTargetId);
  bool isTargetAssigned(TrackerTarget* trackerTarget);
};

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_TRACKER_DEVICE_H__
