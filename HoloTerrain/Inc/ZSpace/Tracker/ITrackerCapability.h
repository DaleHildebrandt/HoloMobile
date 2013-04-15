//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ITrackerCapability.h
//  Content:    
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_I_TRACKER_CAPABILITY_H__
#define __ZSPACE_I_TRACKER_CAPABILITY_H__

#include "ZSpace/Tracker/TrackerDefines.h"

ZSPACE_TRACKER_NS_BEGIN

/**
* @addtogroup Tracker
* @{
*/

/**
* Defines type(s) and number of capabilities a tracker target can have.
*
* @remarks
*    This class indicates the number and types of capabilities a
*    tracker can have, such as buttons, light emitting diodes, the
*    ability to vibrate, etc.
*/
class ITrackerCapability
{
public:
  enum Type
  {
    TYPE_UNKNOWN  = -1,
    TYPE_BUTTON   = 0,
    TYPE_LED,
    TYPE_VIBRATE,
    NUM_TYPES
  };

public:
  virtual ~ITrackerCapability() = 0;
};


inline ITrackerCapability::~ITrackerCapability()
{
}

/** @} */   // End of group: Tracker

ZSPACE_TRACKER_NS_END

#endif // __ZSPACE_I_TRACKER_CAPABILITY_H__