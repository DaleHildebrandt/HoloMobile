//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Timer.h
//  Content:    The high resolution timer class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_TIMER_H__
#define __ZSPACE_TIMER_H__

#include "ZSpace/Common/CommonDefines.h"
#include "ZSpace/Common/Memory/AtomicIntegral.h"

#ifdef _WIN32
  #include <Windows.h>
#else
  #error LARGE_INTEGER is not supported for the current platform
#endif

/**
* @addtogroup Common
* @{
*
*   @addtogroup System
*   @{
*/

ZSPACE_COMMON_NS_BEGIN

/**
* The high resolution timer class.
*
* @remark
*   This class provides high-resolution timing capability.
*/
class ZSPACE_COMMON_API Timer
{
public:
  /**
  * The default constructor.
  */
  Timer();

  /**
  * Start the timer.
  */
  void start();
  
  /**
  * Stop the timer.
  *
  * @remark
  *   Stopping the timer pauses the timer and accumulates the amount of time elapsed while it was running.\n
  *   The start function will resume the accumulation of elapsed time.
  */
  double stop();
  
  /**
  * Restart the timer. 
  *
  * @remark
  *   This will reset the accumulation of time back to zero and start the timer again.
  */
  void restart();
  
  /**
  * Reset the timer.
  *
  * @remark
  *   This will reset the accumulation of time back to zero and leave the timer stopped.
  */
  void reset();
  
  /**
  * Check whether or not the timer is currently running.
  */
  bool isRunning() const;
  
  /**
  * Get the elapsed time in seconds.
  *
  * @return
  *   The elapsed time as floating point approximation.
  */
  double getElapsedSeconds() const;
  
  /**
  * Get the number of elapsed ticks.
  *
  * @return
  *   The precise count of elapsed system clock timer ticks.
  */
  LARGE_INTEGER getRawElapsedTicks() const;
  
  /**
  * Get the frequency of the number of ticks per second.
  */
  LARGE_INTEGER getFrequencyTicksPerSecond() const;

private:  
  zspace::common::AtomicBool      m_isRunning;
  LARGE_INTEGER                   m_frequency;
  LARGE_INTEGER                   m_startTimeStamp;
  LARGE_INTEGER                   m_elapsed;
  double                          m_fFrequency;

private:
  /**
  * Sample the high-resolution performance counter.
  */
	LARGE_INTEGER getTimestamp() const;
};

ZSPACE_COMMON_NS_END

/** @} */   // End of group: System
/** @} */   // End of group: Common

#endif __ZSPACE_TIMER_H__