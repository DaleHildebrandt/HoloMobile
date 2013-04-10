//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       TrackerTarget.h
//  Content:    The Tracker Target class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_TRACKER_TARGET_H__
#define __ZSPACE_TRACKER_TARGET_H__

#include "ZSpace/Tracker/TrackerDefines.h"
#include "ZSpace/Tracker/ITrackerCapability.h"

#include "ZSpace/Common/Math/Matrix4.h"
#include "ZSpace/Common/Memory/ReferenceObject.h"

#include <string>

ZSPACE_TRACKER_NS_BEGIN

class ITrackerPhysicalTarget;
class TrackerFilter;

/**
* @addtogroup Tracker
* @{
*/

/**
* The tracker target class.
*
* @warning
*   This class is not thread-safe.
*
* @remark
*   This class represents any 6 degree of freedom target that can be tracked by
*   a TrackerDevice. For example, a person's head, stylus, etc. could all be considered
*   targets that get tracked. Currently, the basic information that can be queried
*   from a TrackerTarget is its pose (transform: position, orientation and scale) as 
*   well as its specialized set of capabilities.
*/
class ZSPACE_TRACKER_API TrackerTarget: public zspace::common::ReferenceObject
{
  friend class TrackerDevice;

public:
  enum Type
  {
    TYPE_UNKNOWN = -1,
    TYPE_HEAD = 0,      /**< Corresponds to head targets. */
    TYPE_PRIMARY,       /**< Corresponds to primary hand targets (currently stylus). */
    TYPE_SECONDARY,     /**< Corresponds to secondary hand targets. */
    NUM_TYPES
  };

public:
  struct PoseData
  {
    PoseData()
    : time(-1.0),
      pose(zspace::common::Matrix4::IDENTITY())
    {
    }

    double                  time;
    zspace::common::Matrix4 pose;
  };

public:
  /**
  * The default constructor.
  *
  * @param[in] name
  *   The name of the target.
  * @param[in] id
  *   The target's id - this is used to directly communicate with the tracker device.
  * @param[in] type
  *   The type of the target. Currently TYPE_HEAD, TYPE_PRIMARY, and TYPE_SECONDARY
  *   are supported.  For any other types, use TYPE_OTHER.
  */
  TrackerTarget(const char* name, int id, Type type);
  
  /**
  * The destructor.
  */
  virtual ~TrackerTarget();

  /**
  * Get the target's name.
  */
  const char* getName() const;

  /**
  * Get the target's id.
  */
  int getId() const;

  /**
  * Get the target's type.
  */
  Type getType() const;

  /**
  * Get the TrackerSystem associated with the target.
  *
  * @return
  *   A pointer to the TrackerSystem. If the target has not been
  *   added to a TrackerSystem, NULL is returned.
  */
  TrackerDevice* getTrackerDevice() const;

  /**
  * Set the target's TrackerFilter.
  */
  void setTrackerFilter(TrackerFilter* trackerFilter);

  /**
  * Get the target's TrackerFilter.
  */
  TrackerFilter* getTrackerFilter() const;

  /**
  * Set the base transform of the target.
  *
  * @param[in] baseTransform
  *   The base transform of the target. By default, the tracker target's calibration
  *   transform will be used.
  */
  void setBaseTransform(const zspace::common::Matrix4& baseTransform);

  /**
  * Get the base transform of the target.
  */
  const zspace::common::Matrix4& getBaseTransform() const;

  /**
  * Set the initial pose of the target.
  *
  * @remark
  *   This is only applicable for targets that support relative poses.
  *   For example: The space navigator only generates relative positions
  *   and orientations, so the initial pose will act effectively as an
  *   initial offset.
  *
  * @param[in] initialPose
  *   The initial pose of the target in Matrix4 format.
  */
  void setInitialPose(const zspace::common::Matrix4& initialPose);

  /**
  * Get the target's current local pose (translation and rotation).
  *
  * @remark
  *   The local pose does not have any calibration applied to it (i.e. it does
  *   not take into account the base transforms of the tracker target and system.
  *
  * @param[out] localPose
  *   The local (raw) pose of the target. The pose will default to the
  *   identity if no valid poses have been captured. If the current pose is invalid, 
  *   but valid poses have previously been captured, the pose will default to the 
  *   last valid pose. 
  *
  * @return
  *   True if the pose is valid.  False otherwise.
  */
  bool getLocalPose(zspace::common::Matrix4& localPose);

  /**
  * Get the target's current pose in tracker space (calibration applied).
  *
  * @remark
  *   If filtering is enabled, the pose will also have filtering applied to it.
  *
  * @param[out] pose
  *   The target's pose in tracker space. The pose will default to the
  *   identity if no valid poses have been captured. If the current pose is 
  *   invalid, but valid poses have previously been captured, the pose will 
  *   default to the last valid pose. 
  *
  * @return
  *   True if the pose is valid.  False otherwise.
  */
  bool getPose(zspace::common::Matrix4& pose);

  /**
  * Set whether or not pose buffering is enabled.
  */
  void setPoseBufferingEnabled(bool isPoseBufferingEnabled);

  /**
  * Check whether or not pose buffering is enabled.
  */
  bool isPoseBufferingEnabled() const;

  /**
  * Resize the TrackerTarget's internal pose buffer.
  *
  * @warning
  *   Currently this method does not copy pose data from
  *   the previous pose buffer.
  *
  * @remark
  *   The internal pose buffer size is currently 100.
  */
  void resizePoseBuffer(unsigned int size);

  /**
  * Get the target's current pose buffer.
  *
  * @remark
  *   If filtering is enabled, the poses will also have filtering applied to them.
  *
  * @param[in] size
  *   The number of elements in the pose buffer.
  * @param[out] poseBuffer
  *   A pre-allocated TrackerTarget::PoseData array for time-stamped pose information
  *   to be copied to.  Poses are copied into the buffer starting with the most recent
  *   and going backwards in time.
  *
  * @return
  *   The number of valid poses in the pose buffer.
  */
  int getPoseBuffer(unsigned int size, PoseData* poseBuffer);

  /**
  * Get a range of the target's current pose buffer.  Only samples that fall within a delta of
  * (minThreshold..maxThreshold) of the most recent pose will be added to the buffer.
  *
  * @remark
  *   If filtering is enabled, the poses will also have filtering applied to them.
  *
  * @param[in] minDelta
  *   The delta of sample time at which to start copying.  That is, the most recent acceptable sample.
  * @param[in] maxDelta
  *   The delta of the sample time at which to stop copying.  That is, the least recent acceptable sample.
  *   If maxDelta <= minDelta, then all the entire buffer prior to minDelta (up to the size limit) is
  *   copied.
  * @param[in] size
  *   The number of elements in the pose buffer.
  * @param[out] poseBuffer
  *   A pre-allocated TrackerTarget::PoseData array for time-stamped pose information
  *   to be copied to.  Poses are copied into the buffer starting with the most recent and 
  *   going backwards in time.
  *
  * @return
  *   The number of valid poses in the pose buffer.
  */
  int getPoseBufferRange(double minDelta, double maxDelta, unsigned int size, PoseData* poseBuffer);

  /** 
  * Get a historical pose from the pose buffer.
  * 
  * @remark
  *   If filtering is enabled, the pose will also have filtering applied to it.
  *
  * @param[in] minDelta
  *   The delta sample time of the most recent acceptable sample.
  * @param[in] maxDelta
  *   The delta sample time of the least recent acceptable sample.
  *   Pass maxDelta <= minDelta if you don't care how old it is as long as it's older than minDelta.
  *
  * @param[out] pose
  *   The target's zspace pose in Matrix4 format. If the target is currently not 
  *   connected to a TrackerDevice, the pose defaults to the identity matrix.  Returns the 
  *   most recent acceptable pose.
 */
  bool getBufferedPose(double minDelta, double maxDelta, zspace::common::Matrix4& pose);

  /**
  * Check whether or not the target is visible.
  */
  bool isVisible();

  /**
  * Check whether or not the target is enabled.
  */
  bool isEnabled();

  /**
  * Get a specified capability from the target.
  *
  * @param[in] type
  *   The type of capability, i.e. TYPE_BUTTON, TYPE_FEEDBACK, etc.
  *
  * @return
  *   A pointer to the base ITrackerCapability class. If the specified capability was not found,
  *   NULL is returned.
  *
  * @code
  *   // The following is an example for how to grab a button capability.
  *   TrackerTarget* target;
  *   ...
  *
  *   ITrackerButtonCapability* buttonCapability = 
  *     static_cast<ITrackerButtonCapability*>(target->getCapability(ITrackerCapability::TYPE_BUTTON));
  *
  *   if (buttonCapability)
  *   {
  *     int numButtons = buttonCapability->getNumButtons();
  *     printf("The target supports %d buttons.\n", numButtons);
  *   }
  * @endcode
  */
  ITrackerCapability* getCapability(ITrackerCapability::Type type);

private:
  std::string               m_name;
  int                       m_id;
  Type                      m_type;

  ITrackerPhysicalTarget*   m_iTrackerPhysicalTarget;
  TrackerDevice*            m_trackerDevice;
  TrackerFilter*            m_trackerFilter;

  zspace::common::Matrix4   m_baseTransform;
  zspace::common::Matrix4   m_lastValidPose;
  bool                      m_hasCapturedValidPose;

  PoseData*                 m_poseBuffer;
  unsigned int              m_poseBufferSize;
  unsigned int              m_poseBufferIndex;
  bool                      m_isPoseBufferingEnabled;

private:
  void setITrackerPhysicalTarget(ITrackerPhysicalTarget* iTrackerPhysicalTarget);
  void setTrackerDevice(TrackerDevice* trackerDevice);
  void bufferPose(double time);
};

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif //__ZSPACE_TRACKER_TARGET_H__
