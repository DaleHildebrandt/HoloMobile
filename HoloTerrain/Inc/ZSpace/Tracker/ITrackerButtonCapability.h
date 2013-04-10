//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ITrackerButtonCapability.h
//  Content:    Pure virtual interface for accessing button capabilities on 
//              tracker targets.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_I_TRACKER_BUTTON_CAPABILITY_H__
#define __ZSPACE_I_TRACKER_BUTTON_CAPABILITY_H__

#include "ZSpace/Tracker/TrackerDefines.h"
#include "ZSpace/Tracker/ITrackerCapability.h"

ZSPACE_TRACKER_NS_BEGIN

/**
* @addtogroup Tracker
* @{
*/

/**
* Determine number of tracker buttons and their state.
*
* @remarks
*    This class indicates the number of buttons on the tracking
*    device and if a given button has been pressed as described
*    by the integer ID of the button, which generally starts with
*    0 but could be implementation-dependent.
*/
class ITrackerButtonCapability: public ITrackerCapability
{
public:
  virtual ~ITrackerButtonCapability() = 0;

  /**
  * Get the number of buttons supported.
  */
  virtual unsigned int getNumButtons() = 0;

  /**
  * Check whether or not a specified button is pressed.
  *
  * @param[in] buttonId
  *   The integer id of the button.
  *
  * @return
  *   True if the button is pressed, false if not.
  */
  virtual bool isButtonPressed(unsigned int buttonId) = 0;
};


inline ITrackerButtonCapability::~ITrackerButtonCapability()
{
}

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_I_TRACKER_BUTTON_CAPABILITY_H__