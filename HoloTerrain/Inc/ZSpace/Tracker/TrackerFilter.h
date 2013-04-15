//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       TrackerFilter.h
//  Content:    The Tracker Filter class - responsible for smoothing tracker
//              pose data.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_TRACKER_FILTER_H__
#define __ZSPACE_TRACKER_FILTER_H__

#include "ZSpace/Tracker/TrackerDefines.h"

#include "ZSpace/Common/Math/Matrix4.h"
#include "ZSpace/Common/Math/Transform.h"
#include "ZSpace/Common/System/Timer.h"
#include "ZSpace/Common/Memory/ReferenceObject.h"

ZSPACE_TRACKER_NS_BEGIN

/**
* @addtogroup Tracker
* @{
*/

/**
* Buffers and smooths data coming in from the tracker.
*
* @remark
*   Buffers the incoming data to smooth over errors that can occur
*   as a side effect of timing, sudden movements, lack of movement,
*   etc. Takes the actual pose and creates a virtual equivalent with
*   anomalies removed. Improves the overall quality of tracking.
*/
class ZSPACE_TRACKER_API TrackerFilter: public zspace::common::ReferenceObject
{
public:
  TrackerFilter();
  virtual ~TrackerFilter();

  virtual void setEnabled(bool isEnabled);
  virtual bool isEnabled();

  virtual void setThreshold(float threshold);
  virtual float getThreshold() const;

  virtual void setLevel(float level);
  virtual float getLevel() const;

  virtual void setStartRate(float startRate);
  virtual float getStartRate() const;

  virtual void setStopRate(float stopRate);
  virtual float getStopRate() const;

  virtual void filterPose(const zspace::common::Matrix4& currentPose, zspace::common::Matrix4& filteredPose);

private:
  struct Pose
  {
    zspace::common::Transform transform;
    double                    time;
  };

private:
  static const int  POSE_BUFFER_SIZE = 256;

  bool              m_isEnabled;

  Pose              m_poseBuffer[POSE_BUFFER_SIZE];
  int               m_poseBufferIndex;

  // Filter controls.
  float             m_threshold;
  float             m_level;
  float             m_startRate;
  float             m_stopRate;

  // Internal parameters derived from controls.
  double            m_velocityThreshold;
  double            m_minIntervalForAverage;
  double            m_maxIntervalForAverage;
  double            m_intervalForAverage;
  double            m_intervalIncreaseRate;
  double            m_intervalDecreaseRate;

  zspace::common::Timer m_timer;

private:
  void insertPose(const zspace::common::Transform& transform, double time);
  void updateIntervalForAverage();
};

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif //__ZSPACE_TRACKER_FILTER_H__
