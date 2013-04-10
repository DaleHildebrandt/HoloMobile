//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       TrackerSystem.h
//  Content:    The TrackerSystem class. This loads and manages all known tracker
//              systems and targets.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_TRACKER_SYSTEM_H__
#define __ZSPACE_TRACKER_SYSTEM_H__

#include "ZSpace/Tracker/TrackerDefines.h"

#include "ZSpace/Tracker/TrackerDevice.h"
#include "ZSpace/Tracker/TrackerTarget.h"
#include "Zspace/Tracker/TrackerMouseSimulator.h"

#include "ZSpace/Common/Math/Matrix4.h"
#include "ZSpace/Common/Memory/List.h"
#include "ZSpace/Common/Memory/ReferenceObject.h"

ZSPACE_TRACKER_NS_BEGIN

/**
* @addtogroup Tracker
* @{
*/

/**
* The tracker system class.
*
* @warning
*   This class is not thread-safe.
*
* @remark
*   This class is responsible for loading up all of the pluggable tracking devices
*   in the default DLL directory or a user specified location. 
*   All of the loaded TrackerDevices along with the default head, primary and secondary
*   TrackerTargets are accessible from the TrackerSystem interface. For convenience, the
*   TrackerSystem::captureTargets() method is available for use in order to update all 
*   of the pose and button state information of the active TrackerTargets.
*
* @code
*   // The following is a quick example for how to create a TrackerSystem object,
*   // grab the default head TrackerTarget, update active target information,
*   // and destroy the TrackerSystem object:
*
*   // Create the TrackerSystem object and get the default head TrackerTarget.
*   zspace::tracker::TrackerSystem* trackerSystem = new zspace::tracker::TrackerSystem();
*   zspace::tracker::TrackerTarget* headTarget    = tracker->getDefaultTrackerTarget(TrackerTarget::TYPE_HEAD);
*
*   // MAIN LOOP BEGIN
*   // Capture all active TrackerTarget information.
*   trackerSystem->captureTargets();
*
*   // Put TrackerTarget query code here...
*   // MAIN LOOP END
*
*   // Destroy the TrackerSystem object.
*   trackerSystem->removeReference();
* @endcode
*/
class ZSPACE_TRACKER_API TrackerSystem: public zspace::common::ReferenceObject
{
public:
  /**
  * The default constructor.
  */
  TrackerSystem();

  /**
  * The destructor.
  */
  ~TrackerSystem();

  /**
  * Get a TrackerDevice based on a specified name.
  *
  * @param[in] deviceName
  *   The name of the TrackerDevice specified in its associated configuration file.
  *
  * @return
  *   A pointer to the TrackerDevice. If the device could not be found, NULL
  *   is returned.
  */
  TrackerDevice* getTrackerDevice(const char* deviceName);

  /**
  * Get a TrackerDevice based on a specified index.
  *
  * @param[in] index
  *   The index of the TrackerDevice.
  */
  TrackerDevice* getTrackerDeviceByIndex(unsigned int index);

  /**
  * Get the number of TrackerDevices.
  */
  unsigned int getNumTrackerDevices() const;
  
  /**
  * Get the default TrackerTarget of a specified type.
  *
  * @remark
  *   If the TrackerDevice corresponding to the requested TrackerTarget is
  *   not enabled, calling this method will enable it.
  *
  * @param[in] type
  *   The type of TrackerTarget.
  *
  * @return
  *   A pointer to the default TrackerTarget.  If no TrackerTarget exists
  *   corresponding to the specified type, NULL is returned.
  */
  TrackerTarget* getDefaultTrackerTarget(TrackerTarget::Type type) const;

  /**
  * Get the TrackerMouseSimulator.
  */
  TrackerMouseSimulator* getTrackerMouseSimulator();

  /**
  * Capture the latest target data.
  *
  * @remark
  *   For each TrackerDevice that is enabled, capture information for their
  *   respective TrackerTargets.
  */
  void captureTargets();

private:
  typedef zspace::common::List<TrackerDevice*> TrackerDeviceList;

private:
  TrackerDeviceList           m_trackerDevices;
  TrackerTarget*              m_defaultTrackerTargets[TrackerTarget::NUM_TYPES];
  TrackerMouseSimulator       m_trackerMouseSimulator;

private:
  void load();
};

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_TRACKER_SYSTEM_H__
