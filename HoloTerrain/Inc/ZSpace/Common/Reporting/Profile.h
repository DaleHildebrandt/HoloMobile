//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ZSProfile.h
//  Content:    Real-Time Hierarchical Profiling taken from Bullet's btQuickprof.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_PROFILE_H__
#define __ZSPACE_PROFILE_H__

//! To disable built-in profiling, define ZSPACE_NO_PROFILE.
#ifndef ZSPACE_NO_PROFILE

/**
* Use the ZS_PROFILE_SCOPE macro at the start of scope to time the whole scope.
*/
#define	ZS_PROFILE_SCOPE(name) zspace::common::ProfileSample __profile(name)

/**
* Use the ZS_PROFILE_BEGIN macro at the start of a region to start timing.
*/
#define	ZS_PROFILE_BEGIN(name) zspace::common::ProfileManager::startProfile(name)

/**
* Use the ZS_PROFILE_END macro at the end of a region to stop timing.
*/
#define	ZS_PROFILE_END() zspace::common::ProfileManager::stopProfile()

/**
* Use the ZS_PROFILE_FRAME macro to inform ZSProfile of a new frame for statistics purposes.
*/
#define ZS_PROFILE_FRAME() zspace::common::ProfileManager::incrementFrameCounter()

/**
* Use the ZS_PROFILE_DUMP macro to dump all performance timings to the active Log.
*/
#define ZS_PROFILE_DUMP() zspace::common::ProfileManager::dumpAll()

/** 
* Use the ZS_PROFILE_RESET macro to throw out performance timings gathered so far.
*/
#define ZS_PROFILE_RESET() zspace::common::ProfileManager::reset()

#else

#define	ZS_PROFILE_SCOPE( name )
#define	ZS_PROFILE_BEGIN( name )
#define	ZS_PROFILE_END()
#define ZS_PROFILE_FRAME()
#define ZS_PROFILE_DUMP()
#define ZS_PROFILE_RESET()

#endif //#ifdef ZSPACE_NO_PROFILE

#include "ZSpace/Common/CommonDefines.h"

#include "ZSpace/Common/System/Timer.h"
#include "ZSpace/Common/Thread/Mutex.h"

#include <hash_map>
#include <map>
#include <string>

#ifndef _WIN32
namespace stdext
{
  using namespace __gnu_cxx;
}
#endif

ZSPACE_COMMON_NS_BEGIN

class Log;

/**
* @addtogroup Common
* @{
*
*   @addtogroup Reporting
*   @{
*/

//! A node in the Profile Hierarchy Tree
class	ZSPACE_COMMON_API ProfileNode
{
  friend class ProfileManager;

public:
  //! @param[in] name Pointer to a static string which is the name of this profile node.
  //! @param[in] parent Parent pointer.
  //!                                                                                             
  //! @warning
  //! The name is assumed to be a static pointer, only the pointer is stored and compared for     
  //! efficiency reasons.                                                                         
  ProfileNode(const char * name = "Root", ProfileNode * parent = NULL);
  ~ProfileNode();

  //! @param[in] name Static string pointer to the name of the node we are searching for.
  //!                                                                                             
  //! @warning
  //! All profile names are assumed to be static strings so this function uses pointer compares   
  //! to find the named node.                                                                     
  ProfileNode* getSubNode(const char* name);

  ProfileNode* getParent();
  ProfileNode* getSibling();
  ProfileNode* getChild();

  void cleanupMemory();
  void reset();
  void start();
  bool finish();

  const char* getName();
  int getTotalCalls();
  double getTotalTime();

  void sortChildren();

protected:
  const char*	m_name;
  int m_totalCalls;
  double m_totalTime;
  unsigned long long int m_startTime;
  int m_recursionCounter;

  ProfileNode*	m_parent;
  ProfileNode*	m_child;
  ProfileNode*	m_sibling;
};

//! An iterator to navigate through the Profile Hierarchy Tree
class ZSPACE_COMMON_API ProfileIterator
{
protected:
  friend class ProfileManager;
  ProfileIterator(ProfileNode* start);

public:
  //! Access the first child of the current parent
  void first();
  //! Access the next child of the current parent
  void next();
  //! Returns true if the current child is the last one.
  bool isDone();
  //! Returns true if the current parent is the root.
  bool isRoot();

  bool isNephew();

  //! Make the given child the new parent
  void enterChild( int index );
  //! Make the largest child the new parent
  void enterLargestChild();
  //! Make the current parent's parent the new parent
  void enterParent();

  //! Name of the current child
  const char* getCurrentName();
  //! Number of calls to the current child
  int getCurrentTotalCalls();
  //! Total time spent in the current child
  double getCurrentTotalTime();

  //! Name of the current parent
  const char* getCurrentParentName();
  //! Number of calls to the current parent
  int getCurrentParentTotalCalls();
  //! Total time spent in the current parent
  double getCurrentParentTotalTime();

protected:
  ProfileNode*	m_currentParent;
  ProfileNode*	m_currentChild;
};


//! The Manager for the Profile system taken from Bullet's btQuickprof
class	ZSPACE_COMMON_API ProfileManager
{
public:
  //! Changes where the profiling statistics will be dumped.
  //!
  //! By default, all statistics go to STDOUT.  To send them elsewhere, instantiate a class derived from ZSLog and provide it
  //! here at startup.
  static void setLog(Log* zsLog); 

  //! Begins timing the section "name" and creates it it if necessary.
  //!                                                                                             
  //! Steps one level deeper into the tree, if a child already exists with the specified name     
  //! then it accumulates the profiling; otherwise a new child node is added to the profile tree. 
  //!                                                                                             
  //! @param[in] name The name of this profiling record.
  //!                                                                                             
  //! @warning Do not use this!  Use ZS_PROFILE_SCOPE(name) or ZS_PROFILE_BEGIN(name), which can be compiled out.
  //! @warning                                                                                   
  //! The string used is assumed to be a static string; pointer compares are used throughout      
  //! the profiling code for efficiency.   static void startProfile(const char* name);
  static void startProfile(const char* name);

  //! Stops timing the section "name" and record results.
  //! @warning Do not use this function directly.  Use ZS_PROFILE_END, which can be compiled out.
  static void stopProfile();

  //! Delete the performance tree to free up memory.
  static void cleanupMemory();

  //! Reset the contents of the profiling system.
  //!
  //! This resets everything except for the tree structure.  All of the timing data is reset.
  static void reset();

  //! Signals a new frame for per-frame statistics.
  //! @warning Do not use this function directly.  Use ZS_PROFILE_FRAME, which can be compiled out.
  static void incrementFrameCounter();
  static int getFrameCountSinceReset();
  //! Returns the elapsed time since last reset.
  static double getTimeSinceReset();

  static ProfileIterator*	getIterator(unsigned int tid = Mutex::getThreadId());
  static void releaseIterator(ProfileIterator* iterator);

  //! Recursively dump all profiler statistics below the given node to a static logger.
  static void dumpRecursive(ProfileIterator* profileIterator, std::string &spacing);
  //! Dump all profiler statistics to a static logger.
  static void dumpAll();
  static void profileGetTicks(unsigned long long int* ticks);
  static double profileGetTickRate();
  static void scheduleDeletion();

private:
  //static void deleteLog();
  static ProfileNode& getRoot(unsigned int tid);
  static ProfileNode*& getCurrentNode(unsigned int tid);
  //static ZSLog* s_log;
  //static ZSLogDebugger s_defaultLog;
  static Timer s_profileClock;
  //NOTE: Hash_map handles invalidation like std::map, so this is safe.
  static stdext::hash_map<unsigned int, ProfileNode> s_roots;
  static stdext::hash_map<unsigned int, ProfileNode*> s_currentNodes;
  static std::map<std::string, ProfileNode> s_totals;
  static int s_frameCounter;
  static unsigned long long int s_resetTime;
  static bool s_isDeletionScheduled;
};


//! Simple way to profile a function's scope.
class ZSPACE_COMMON_API ProfileSample
{
public:
  ProfileSample(const char* name);
  ~ProfileSample();
};

ZSPACE_COMMON_NS_END

/** @} */   // End of group: Reporting
/** @} */   // End of group: Common

#include "ZSpace/Common/Reporting/Profile.inl"

#endif //__ZSPACE_PROFILE_H__