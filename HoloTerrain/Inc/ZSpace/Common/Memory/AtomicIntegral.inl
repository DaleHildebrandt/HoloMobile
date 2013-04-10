//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       AtomicIntegral.inl
//  Content:    The Atomic Integral template
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#include "ZSpace/Common/Memory/AtomicIntegral.h"

ZSPACE_COMMON_NS_BEGIN;

#ifdef _WIN32

template <class T>
inline AtomicIntegral<T>::AtomicIntegral() { }


template <class T>
inline AtomicIntegral<T>::AtomicIntegral(T i) : m_i(i) { }


template <class T>
inline AtomicIntegral<T>::AtomicIntegral(const AtomicIntegral<T> &a) : m_i(T(a.m_i)) { } // use T() to force atomic read


template <class T>
inline AtomicIntegral<T>::operator T() const
{
  return const_cast<AtomicIntegral<T>*>(this)->cmpxchg(0,0); // this is a no-op, but returns the existing value
}


template <class T>
inline T AtomicIntegral<T>::operator=(T i)
{
  return xchg(i);
}


// note: The code sequence 'if (sizeof(T) == sizeof(...))' will be resolved at compile time.
// It is used here to reduce code duplication.


template <class T>
inline T AtomicIntegral<T>::xchg(T i)
{
  if (sizeof(T) == sizeof(LONGLONG))
    return T(InterlockedExchange64(reinterpret_cast<volatile LONGLONG*>(&m_i), LONGLONG(i)));
  else
    return T(InterlockedExchange(reinterpret_cast<volatile LONG*>(&m_i), LONG(i)));
}


template <class T>
inline T AtomicIntegral<T>::cmpxchg(T cmp, T xchg)
{
  if (sizeof(T) == sizeof(LONGLONG))
    return T(InterlockedCompareExchange64(reinterpret_cast<volatile LONGLONG*>(&m_i), LONGLONG(xchg), LONGLONG(cmp)));
  else
    return T(InterlockedCompareExchange(reinterpret_cast<volatile LONG*>(&m_i), LONG(xchg), LONG(cmp)));
}


template <class T>
inline T AtomicIntegral<T>::operator&=(T i)
{
  if (sizeof(T) == sizeof(LONGLONG))
    return T(InterlockedAnd64(reinterpret_cast<volatile LONGLONG*>(&m_i), LONGLONG(i)));
  else
    return T(InterlockedAnd(reinterpret_cast<volatile LONG*>(&m_i), LONG(i)));
}


template <class T>
inline T AtomicIntegral<T>::operator|=(T i)
{
  if (sizeof(T) == sizeof(LONGLONG))
    return T(InterlockedOr64(reinterpret_cast<volatile LONGLONG*>(&m_i), LONGLONG(i)));
  else
    return T(InterlockedOr(reinterpret_cast<volatile LONG*>(&m_i), LONG(i)));
}


template <class T>
inline T AtomicIntegral<T>::operator^=(T i)
{
  if (sizeof(T) == sizeof(LONGLONG))
    return T(InterlockedXor64(reinterpret_cast<volatile LONGLONG*>(&m_i), LONGLONG(i)));
  else
    return T(InterlockedXor(reinterpret_cast<volatile LONG*>(&m_i), LONG(i)));
}


template <class T>
inline T AtomicIntegral<T>::operator+=(T i)
{
  if (sizeof(T) == sizeof(LONGLONG))
    return T(InterlockedAdd64(reinterpret_cast<volatile LONGLONG*>(&m_i), LONGLONG(i)));
  else
    return T(InterlockedAdd(reinterpret_cast<volatile LONG*>(&m_i), LONG(i)));
}


template <class T>
inline T AtomicIntegral<T>::operator-=(T i)
{
  if (sizeof(T) == sizeof(LONGLONG))
    return T(InterlockedAdd64(reinterpret_cast<volatile LONGLONG*>(&m_i), LONGLONG(-i)));
  else
    return T(InterlockedAdd(reinterpret_cast<volatile LONG*>(&m_i), LONG(-i)));
}


template <class T>
inline T AtomicIntegral<T>::operator++()
{
  if (sizeof(T) == sizeof(LONGLONG))
    return T(InterlockedIncrement64(reinterpret_cast<volatile LONGLONG*>(&m_i)));
  else
    return T(InterlockedIncrement(reinterpret_cast<volatile LONG*>(&m_i)));
}


template <class T>
inline T AtomicIntegral<T>::operator--()
{
  if (sizeof(T) == sizeof(LONGLONG))
    return T(InterlockedDecrement64(reinterpret_cast<volatile LONGLONG*>(&m_i)));
  else
    return T(InterlockedDecrement(reinterpret_cast<volatile LONG*>(&m_i)));
}


template <class T>
inline T AtomicIntegral<T>::operator++(int)
{
  return (++*this)-1; // recompute the old value after update, to preserve atomicness
}


template <class T>
inline T AtomicIntegral<T>::operator--(int)
{
  return (--*this)+1; // recompute the old value after update. to preserve atomicness
}


#else // _WIN32
// put other platform implementations here

#error "AtomicIntegral<T> is not implemented for this platform."

#endif

ZSPACE_COMMON_NS_END;
