//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Lockable.h
//  Content:    Template for adding lockability to objects
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_LOCKABLE_H__
#define __ZSPACE_LOCKABLE_H__

#include "ZSpace/Common/Thread/Mutex.h"

/**
* @addtogroup Common
* @{
*
*   @addtogroup Memory
*   @{
*/

ZSPACE_COMMON_NS_BEGIN


/**
 * Template mixin for adding lockability to your class.  Use the Curiously Recurring Template Pattern
 * in your code:
 * @code
 *     class X : public Lockable<X> { ... };
 * @endcode
 * Since Mutex is a base class, this adds lock() and unlock() methods to your class.
 * @note the class is derived virtual from Mutex, so that in case of the hopefully rare circumstance
 *       where you would need to derive from two base Lockable's, there will only be one mutex on the
 *       object as a whole.
 *
 * It also defines a smart pointer, LockedPtr, in your class' scope, which manages locking via RAII.
 * To get a lock, you can say:
 * @code
 *     X::LockedPtr lockedX = getX();
 *     lockedX->frobnicate();
 *     // the lock remains in effect until lockedX goes out of scope
 * @endcode
 *
 * A crucial aspect of this design is to enable self-documentation of locking requirements in
 * function signatures.  For example, a function with a signature like this:
 * @code
 *     void foo(X::LockedPtr& lockedX);
 * @endcode
 * This says that foo() expects to act on an already locked X.  In contrast, this signature:
 * @code
 *     void foo(const X::LockedPtr& lockedX);
 * @endcode
 * This function will accept a prelocked X.  Or, it will accept a raw X* pointer, lock the object
 * on function entry, and unlock it on exit.  With these two patterns in place, then the remaining
 * pattern is a function that takes a raw pointer, like this:
 * @code
 *     void foo(const X* x);
 * @endcode
 * This signature can now imply that the function will not lock the object at all.
 */
template <class T>
class Lockable : public virtual zspace::common::Mutex
{
public:
  /**
   * A smart pointer that locks an object when it starts pointing at it, and unlocks it when
   * it stops pointing at it.
   * @note Because locking is not recursive, the copy constructor and assignment operator actually implement
   *       move semantics: the LockedPtr on the right hand side is cleared.  These functions are implemented
   *       primarily to allow returning LockedPtr's from functions.
   */
  class LockedPtr
  {
  public:
    LockedPtr();                                      /**< Default constructor */
    LockedPtr(Lockable<T>* lockable);                 /**< Raw pointer constructor */
    LockedPtr(const LockedPtr& lockedPtr);            /**< Copy constructor.  To prevent double unlocking, the argument pointer is cleared. */

    ~LockedPtr();                                     /**< Destructor */

    LockedPtr &operator=(Lockable<T>* lockable);      /**< Raw pointer update */
    LockedPtr &operator=(const LockedPtr& lockedPtr); /**< Assignment operator.  To prevent double unlocking, the RHS pointer is cleared. */

    operator T*() const;                              /**< Implicit conversion to raw pointer */
    T& operator*() const;                             /**< Dereference operator */
    T* operator->() const;                            /**< Member access operator */

  private:
    Lockable<T>* m_lockable;
  };

  /**
   * Returns a locked pointer to the object.  Note that the method is const.  This is intended to
   * support the pattern where read-access is granted via a raw const pointer, but which can be converted
   * to write-access through this method.  Doing so locks the object, like so:
   * @code
   *     const X* x = // get a raw const pointer
   *     X::LockedPtr lockedX = x->getLockedPtr();
   *     lockedX->frobnicate();
   *     // the lock remains in effect until lockedX goes out of scope
   * @endcode
   * There is an abbreviated form for single-operation exclusive access:
   * @code
   *     x->getLockedPtr()->frobnicate();
   * @endcode
   * This example locks x, frobnicates it, and then unlocks it.
   * @note You enforce this style by only allowing access to the raw const pointer.
   */
  const LockedPtr getLockedPtr() const;
};

ZSPACE_COMMON_NS_END

/** @} */   // End of group: Memory
/** @} */   // End of group: Common

#include "ZSpace/Common/Memory/Lockable.inl"

#endif // __ZSPACE_LOCKABLE_H__
