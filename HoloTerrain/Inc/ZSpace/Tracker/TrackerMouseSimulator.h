//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       TrackerMouseSimulator.h
//  Content:    Utility that simulates mouse movement and button press/release
//              events given a valid TrackerTarget.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_TRACKER_MOUSE_SIMULATOR_H__
#define __ZSPACE_TRACKER_MOUSE_SIMULATOR_H__

#include "ZSpace/Tracker/TrackerDefines.h"
#include "ZSpace/Tracker/TrackerDisplayIntersection.h"

#include "ZSpace/Common/Math/Matrix4.h"
#include "ZSpace/Common/System/Timer.h"

#include <map>

ZSPACE_TRACKER_NS_BEGIN

class ITrackerButtonCapability;
class TrackerTarget;

/**
* @addtogroup Tracker
* @{
*/

class ZSPACE_TRACKER_API TrackerMouseSimulator
{
  friend class TrackerSystem;

public:
  enum MovementMode
  {
    MOVEMENT_MODE_ABSOLUTE,
    MOVEMENT_MODE_RELATIVE
  };
  
  enum MouseButton
  {
    MOUSE_BUTTON_UNKNOWN  = -2,
    MOUSE_BUTTON_ALL      = -1,
    MOUSE_BUTTON_LEFT     = 0,
    MOUSE_BUTTON_RIGHT,
    MOUSE_BUTTON_MIDDLE,
    NUM_MOUSE_BUTTONS
  };

  enum ButtonState
  {
    BUTTON_STATE_PRESS = 0,
    BUTTON_STATE_RELEASE,
    NUM_BUTTON_STATES
  };

public:
  /**
  * Set the mouse simulator's TrackerTarget.  The simulator
  * will use this TrackerTarget's pose and button state information
  * to drive the mouse.
  *
  * @param[in] trackerTarget
  *   A pointer to a valid TrackerTarget.
  */
  void setTrackerTarget(TrackerTarget* trackerTarget);
  
  /**
  * Get the mouse simulator's current TrackerTarget.
  */
  TrackerTarget* getTrackerTarget() const;

  /**
  * Set whether or not the mouse simulation is enabled.
  *
  * @param[in] isEnabled
  *   Whether or not the capability is enabled.
  */
  void setEnabled(bool isEnabled);
  
  /**
  * Check whether or not mouse simulation is enabled.
  */
  bool isEnabled() const;

  /**
  * Set the maximum distance the TrackerTarget can be from the display 
  * in order to simulate the mouse.
  *
  * @param[in] maxSimulationDistance
  *   The maximum distance (in meters) the TrackerTarget can be from the 
  *   display.  Negative values denote an infinite distance.
  */
  void setMaxSimulationDistance(float maxSimulationDistance);

  /**
  * Get the maximum distance (in meters) the TrackerTarget can be from the 
  * display in order to simulate the mouse.  A negative value denotes an 
  * infinite distance.
  */
  float getMaxSimulationDistance() const;

  /**
  * Set the movement mode of the mouse.
  *
  * @param[in] movementMode
  *   The mouse movement mode.  Currently only two modes
  *   are supported: MOVEMENT_MODE_ABSOLUTE and MOVEMENT_MODE_RELATIVE.
  */
  void setMovementMode(TrackerMouseSimulator::MovementMode movementMode);
  
  /**
  * Get the MovementMode of the capability.
  */
  TrackerMouseSimulator::MovementMode getMovementMode() const;
  
  /**
  * Map a TrackerTarget's button to a specified mouse button.
  *
  * @param[in] buttonId
  *   The buttonId corresponding to the TrackerTarget's buttons. 
  * @param[in] mouseButton
  *   The mouse button.  
  */
  void setButtonMapping(int buttonId, TrackerMouseSimulator::MouseButton mouseButton);
  
  /**
  * Check which mouse button the capability's corresponding TrackerTarget's
  * button is mapped to.
  *
  * @param[in] buttonId
  *   The buttonId corresponding to the TrackerTarget's buttons. 
  * 
  * @return
  *   The mouse button directly mapped to the TrackerTarget's button.
  *   MOUSE_BUTTON_UNKNOWN is returned if no mapping exists.
  */
  TrackerMouseSimulator::MouseButton getButtonMapping(int buttonId);
  
  /**
  * Set the look-back time for a specified button state combined with
  * a mouse button.
  *
  * @remark
  *   This is important since pressing a TrackerTarget
  *   button can potentially cause the TrackerTarget to physically
  *   move.  The look-back time will compensate for that unwanted
  *   movement.
  *
  * @param[in] buttonState
  *   The state of the button.  For now only BUTTON_STATE_PRESS and
  *   BUTTON_STATE_RELEASE are supported.
  * @param[in] mouseButton
  *   The mouse button.
  * @param[in] time
  *   The look-back time in seconds used to query a past mouse position.
  */
  void setPoseLagTime(TrackerMouseSimulator::ButtonState buttonState, 
                      TrackerMouseSimulator::MouseButton mouseButton,
                      double time);

  /**
  * Get the look-back time for a specified button state combined with
  * a mouse button.
  *
  * @param[in] buttonState
  *   The state of the button.  For now only BUTTON_STATE_PRESS and
  *   BUTTON_STATE_RELEASE are supported.
  * @param[in] mouseButton
  *   The mouse button.
  *
  * @return
  *   The look-back time in seconds used to query a past mouse position.
  */
  double getPoseLagTime(TrackerMouseSimulator::ButtonState buttonState, 
                        TrackerMouseSimulator::MouseButton mouseButton);


  /**
  * Get the current intersection information.
  */
  const TrackerDisplayIntersection::IntersectionInfo& getCurrentIntersectionInfo() const;
  
private:
  typedef std::map<int, MouseButton> ButtonMap;
  
private:
  static const unsigned int MAX_NUM_STYLUS_BUTTONS = 3;

  TrackerTarget*            m_trackerTarget;
  ITrackerButtonCapability* m_iTrackerButtonCapability;

  bool                      m_isEnabled;
  bool                      m_forceAbsoluteMovement;

  float                     m_maxSimulationDistance;

  MovementMode              m_movementMode;
  ButtonMap                 m_buttonMap;

  double                    m_poseLagTimes[NUM_BUTTON_STATES][NUM_MOUSE_BUTTONS];  

  zspace::common::Matrix4   m_currentPose;
  zspace::common::Matrix4   m_previousPose;
  zspace::common::Matrix4   m_buttonPressPose;
  zspace::common::Matrix4   m_buttonReleasePose;

  bool                      m_currentButtonStates[MAX_NUM_STYLUS_BUTTONS];
  bool                      m_previousButtonStates[MAX_NUM_STYLUS_BUTTONS];

  TrackerDisplayIntersection::IntersectionInfo m_currentIntersectionInfo;
  TrackerDisplayIntersection::IntersectionInfo m_previousIntersectionInfo;

  zspace::common::Timer     m_timer;
  double                    m_currentTime;
  double                    m_buttonPressTime;
  double                    m_buttonClickDuration;
  
private:
  TrackerMouseSimulator();
  ~TrackerMouseSimulator();

  void update();
  
  void captureCurrentState();
  void capturePreviousState();

  void generateMouseEvents();
  void generateMouseButtonEvents();
  void generateMouseMoveEvents();

  void calculateButtonPressPose(double time, zspace::common::Matrix4& pose);
  void calculateButtonReleasePose(double time, zspace::common::Matrix4& pose);

  bool isWithinSimulationRegion();
};

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_TRACKER_MOUSE_SIMULATOR_H__