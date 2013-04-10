//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2010 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ZSProfile.inl
//  Content:    Real-Time Hierarchical Profiling taken from Bullet's btQuickprof.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "ZSpace/Common/Reporting/Profile.h"

ZSPACE_COMMON_NS_BEGIN

inline
ProfileNode* ProfileNode::getParent()
{
  return m_parent;
}


inline
ProfileNode* ProfileNode::getSibling()
{
  return m_sibling;
}


inline
ProfileNode* ProfileNode::getChild()
{
  return m_child;
}


inline
const char* ProfileNode::getName()
{
  return m_name;
}


inline
int ProfileNode::getTotalCalls()
{
  return m_totalCalls;
}


inline
double ProfileNode::getTotalTime()
{
  return m_totalTime;
}


inline
bool ProfileIterator::isRoot()
{
  return (m_currentParent->getParent() == 0);
}


inline
const char* ProfileIterator::getCurrentName()
{
  return m_currentChild->getName();
}


inline
int ProfileIterator::getCurrentTotalCalls()
{
  return m_currentChild->getTotalCalls();
}


inline
double ProfileIterator::getCurrentTotalTime()
{
  return m_currentChild->getTotalTime();
}


inline
const char* ProfileIterator::getCurrentParentName()
{
  return m_currentParent->getName();
}


inline
int ProfileIterator::getCurrentParentTotalCalls()
{
  return m_currentParent->getTotalCalls();
}


inline
double ProfileIterator::getCurrentParentTotalTime()
{
  return m_currentParent->getTotalTime();
}


inline
ProfileSample::ProfileSample(const char* name)
{
  ProfileManager::startProfile(name);
}


inline
ProfileSample::~ProfileSample()
{
  ProfileManager::stopProfile(); 
}


inline
ProfileNode& ProfileManager::getRoot(unsigned int tid)
{
  return s_roots[tid];
}


inline
ProfileNode*& ProfileManager::getCurrentNode(unsigned int tid)
{
  ProfileNode*& currentNode = s_currentNodes[tid];
  if (currentNode == NULL)
    currentNode = &getRoot(tid);
  return currentNode;
}

ZSPACE_COMMON_NS_END