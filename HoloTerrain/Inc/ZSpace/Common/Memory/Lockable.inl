//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Lockable.inl
//  Content:    Template for adding lockability to objects
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "ZSpace/Common/Memory/Lockable.h"

ZSPACE_COMMON_NS_BEGIN


template <class T>
const typename Lockable<T>::LockedPtr Lockable<T>::getLockedPtr() const
{
  return LockedPtr(const_cast<Lockable<T>*>(this));
}


template <class T>
Lockable<T>::LockedPtr::LockedPtr() : m_lockable(0) { }


template <class T>
Lockable<T>::LockedPtr::LockedPtr(const LockedPtr& lockedPtr) : m_lockable(lockedPtr.m_lockable)
{
  const_cast<Lockable<T>::LockedPtr&>(lockedPtr).m_lockable = 0;
}


template <class T>
Lockable<T>::LockedPtr::LockedPtr(Lockable<T>* lockable) : m_lockable(lockable)
{
  if (m_lockable)
    m_lockable->lock();
}


template <class T>
Lockable<T>::LockedPtr::~LockedPtr()
{
  if (m_lockable)
    m_lockable->unlock();
}


template <class T>
typename Lockable<T>::LockedPtr& Lockable<T>::LockedPtr::operator=(Lockable<T>* lockable)
{
  if (lockable != m_lockable) {
    if (m_lockable)
      m_lockable->unlock();
    m_lockable = lockable;
    if (m_lockable)
      m_lockable->lock();
  }
  return *this;
}


template <class T>
typename Lockable<T>::LockedPtr& Lockable<T>::LockedPtr::operator=(const LockedPtr& lockedPtr)
{
  if (&lockedPtr != this) {
    if (m_lockable)
      m_lockable->unlock();
    m_lockable = lockedPtr.m_lockable;
    const_cast<Lockable<T>::LockedPtr&>(lockedPtr).m_lockable = 0;
  }
  return *this;
}


template <class T>
Lockable<T>::LockedPtr::operator T*() const
{
  return static_cast<T*>(m_lockable);
}


template <class T>
T& Lockable<T>::LockedPtr::operator*() const
{
  return *static_cast<T*>(m_lockable);
}


template <class T>
T* Lockable<T>::LockedPtr::operator->() const
{
  return static_cast<T*>(m_lockable);
}

ZSPACE_COMMON_NS_END
