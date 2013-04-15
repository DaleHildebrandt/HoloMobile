//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       DoubleBuffer.h
//  Content:    The double buffer class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_DOUBLE_BUFFER_H__
#define __ZSPACE_DOUBLE_BUFFER_H__

#include "ZSpace/Common/CommonDefines.h"
#include "ZSpace/Common/Memory/Lockable.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Memory
*   @{
*/

/**
* This is the generic, templated double buffer class.
*
* @remark
*   This currently only supports types that have constructors which accept no parameters.
*   It also maintains an internal mutex for thread synchronization.
*
* @code
*   DoubleBuffer<int> intBuffer;
*
*   // Set front buffer to 10 and back buffer to -10.
*   *intBuffer.getFront() = 10;
*   *intBuffer.getBack()  = -10;
*
*   // Swap the buffers. Now getFront() returns -10 and getBack() returns 10.
*   intBuffer.swap();
* @endcode
*/
template <class T>
class DoubleBuffer : public zspace::common::Lockable<DoubleBuffer<T> >
{
public:
  /**
  * The default constructor.
  */
  DoubleBuffer();

  /**
  * The destructor.
  */
  ~DoubleBuffer();

  /**
  * Get the front buffer.
  *
  * @remark
  *   This is usually used for reading.
  *
  * @return
  *   A pointer to the front buffer.
  */
  T* getFront();

  /**
  * Get the back buffer.
  * 
  * @remark
  *   This is usually used for writing.
  *
  * @return
  *   A pointer to the back buffer.
  */
  T* getBack();

  /**
  * Swap the front and back buffers.
  */
  void swap();

private:
  T                     m_frontBuffer;
  T                     m_backBuffer;
  T*                    m_front;
  T*                    m_back;
};

/** @} */   // End of group: Memory
/** @} */   // End of group: Common

template <class T>
DoubleBuffer<T>::DoubleBuffer()
{
  m_front = &m_frontBuffer;
  m_back  = &m_backBuffer;
}


template <class T>
DoubleBuffer<T>::~DoubleBuffer()
{
}


template <class T>
T* DoubleBuffer<T>::getFront()
{
  return m_front;
}


template <class T>
T* DoubleBuffer<T>::getBack()
{
  return m_back;
}


template <class T>
void DoubleBuffer<T>::swap()
{
  T* temp = m_front;
  m_front = m_back;
  m_back  = temp;
}


ZSPACE_COMMON_NS_END

#endif // __ZSPACE_DOUBLE_BUFFER_H__
