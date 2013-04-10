//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ReferenceObject.h
//  Content:    The Reference Object class
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_REFERENCE_OBJECT_H__
#define __ZSPACE_REFERENCE_OBJECT_H__

#include "ZSpace/Common/CommonDefines.h"
#include "ZSpace/Common/Memory/Lockable.h"
#include "ZSpace/Common/Memory/AtomicIntegral.h"

/**
* @addtogroup Common
* @{
*
*   @addtogroup Memory
*   @{
*/

ZSPACE_COMMON_NS_BEGIN

/**
* The generic reference counting object.
*
* @remark
*   This class allows you to create objects with reference counts. You can increment or
*   decrement the reference count. The object gets deleted if the reference count goes to 0.
*/
class ZSPACE_COMMON_API ReferenceObject : protected zspace::common::Lockable<ReferenceObject>
{
public:
  /**
	* The default constructor.
  *
  * @remark
  *   Upon construction, the reference count is initialized to 1.
	*/
  ReferenceObject();
  
  /**
	* The destructor.
	*/
  virtual ~ReferenceObject();

  /**
	* Increment the reference count.
	*/
  virtual void addReference();

  /**
	* Decrement the reference count.  
  *
  * @remark
  *   If reference count becomes zero, the ReferenceObject automatically gets deleted.
	*/
  virtual void removeReference();

  /**
	* Get the reference count.
	*/
  virtual const int getReferenceCount() const;

private:
  AtomicUnsigned32 m_referenceCount;
};

ZSPACE_COMMON_NS_END

/** @} */   // End of group: Memory
/** @} */   // End of group: Common

#endif // __ZSPACE_REFERENCE_OBJECT_H__
