//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       AtomicIntegral.h
//  Content:    The Atomic Integral template
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_ATOMICINTEGRAL_H__
#define __ZSPACE_ATOMICINTEGRAL_H__

#include "ZSpace/Common/CommonDefines.h"

#ifdef _WIN32
#include <Windows.h>
#endif

/**
* @addtogroup Common
* @{
*
*   @addtogroup Memory
*   @{
*/

ZSPACE_COMMON_NS_BEGIN

/** Multithread and multicore safe integral value, without needing a mutex do to simple arithmetic
 *  operations.  The implementation uses compiler intrinsics to compile down to opcodes supported
 *  by the CPU, while providing an interface that resembles primitive data types via operator overloading.
 *  @note Use for 32 or 64 bit integral types only.
 *  @note C++0x will have an atomic<T> that should supercede this implementation.
 *  @note This class breaks with C++ convention by returning T instead of *this for all update
 *        operations.  This is because a quite common use case of integral values is code like
 *        'if (--x == 0) ...'.  The C++ convention would lead to unsafe behavior because the two
 *        operations, --x and x == 0, would compile as two separate atomic operations.  But the
 *        combination would not be atomic, which would be surprising to the user and a likely
 *        source of hard to fix bugs.
 */
template <class T>
class AtomicIntegral
{
public:
  AtomicIntegral();                           /**< Default constructor */
  AtomicIntegral(T i);                        /**< Constructor from a given value */
  AtomicIntegral(const AtomicIntegral<T> &a); /**< Copy constructor */

  T xchg(T i);                                /**< Standard exchange operation */
  T cmpxchg(T cmp, T xchg);                   /**< Standard compare-and-exchange operation */

  operator T() const;                         /**< Atomic read */

  T operator= (T i);                          /**< Atomic write */
  T operator&=(T i);                          /**< Atomic update via AND */
  T operator|=(T i);                          /**< Atomic update via OR  */
  T operator^=(T i);                          /**< Atomic update via XOR */
  T operator+=(T i);                          /**< Atomic update via ADD */
  T operator-=(T i);                          /**< Atomic update via SUB */

  T operator++();                             /**< Atomic preincrement */
  T operator--();                             /**< Atomic predecrement */

  T operator++(int);                          /**< Atomic postincrement */
  T operator--(int);                          /**< Atomic postdecrement */

private:
  volatile T m_i;
};

#ifdef _WIN32

typedef AtomicIntegral<INT32>       AtomicInt32;
typedef AtomicIntegral<INT64>       AtomicInt64;

typedef AtomicIntegral<UINT32>      AtomicUnsigned32;
typedef AtomicIntegral<UINT64>      AtomicUnsigned64;

#else
  // Add other platform type definition implementations here
  #error "AtomicIntegral<T> is not implemented for this platform."
#endif

/** Specialization for the boolean type.  The only atomic operations allowed are reading, writing,
 *  ANDing, ORing, and XORing operations.  The implementation uses an AtomicInt32 because the atomic operations
 *  supported for a boolean must be atomic across an entire word boundary.
 *  @Since an integer is being used, 0 == false and 1 == true will explicitly be used in the implementation so
 *        we do not depend on platform or compiler differences.
 */
class ZSPACE_COMMON_API AtomicBool
{
  public:
    AtomicBool();                     /**< Default Constructor */
    AtomicBool(const bool i);         /**< Constructor from a given value */
    AtomicBool(const AtomicBool &a);  /**< Copy constructor */

    operator bool() const;            /**< Atomic read */
    bool operator= (const bool i);    /**< Atomic write */
    
    // Compiler error for 32-bit InterlockedAnd, InterlockedOr, and InterlockedXor
    #if 0
    bool operator&=(const bool i);    /**< Atomic update via AND */
    bool operator|=(const bool i);    /**< Atomic update via OR  */
    bool operator^=(const bool i);    /**< Atomic update via XOR */
    #endif

  private:
    AtomicInt32 m_i;
};

ZSPACE_COMMON_NS_END

/** @} */   // End of group: Memory
/** @} */   // End of group: Common

#include "ZSpace/Common/Memory/AtomicIntegral.inl"

#endif // __ZSPACE_ATOMICINTEGRAL_H__
