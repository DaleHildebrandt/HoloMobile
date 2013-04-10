//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       DualQueue.h
//  Content:    The double buffered, reference-counting, queue class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_DUAL_QUEUE_H__
#define __ZSPACE_DUAL_QUEUE_H__

#include "ZSpace/Common/CommonDefines.h"

#include "ZSpace/Common/Error/Error.h"
#include "ZSpace/Common/Memory/Queue.h"
#include "ZSpace/Common/Memory/ReferenceObject.h"
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
* The generic, reference counting, double buffered queue class.
*
* @remark
*   This class wraps our generic Queue class. When pushing objects, they get
*   added to the end of the writer queue (back buffer queue). When popping objects, they
*   get removed from the front of the reader queue (front buffer queue). The reader and
*   writer queues can be swapped by calling the DualQueue::swap() method.
*/
template <class T>
class DualQueue : public zspace::common::Lockable<DualQueue<T> >
{
public:
	/**
	* The default constructor.
	*/
  DualQueue();

	/**
	* The destructor.
	*/
  ~DualQueue();

	/**
	* Push a ReferenceObject onto the end of the writer queue.
  *
  * @param[in] referenceObject
  *   A pointer to a ReferenceObject.
	*/
  void push(T referenceObject);

	/**
	* Pop a reference object from the front of the reader queue.
	*/
  void pop();

	/**
	* Clear both reader and writer queues.
	*/
  void clear();

	/**
	* Swap the reader and writer queues.
	*/
  void swap();

	/**
	* Get the ReferenceObject at the front of the reader queue.
	*/
  T getFront();

	/**
	* Check whether or not the reader queue is empty.
	*/
  bool isEmpty();

	/**
	* Get the current number of ReferenceObjects in the reader queue.
	*/
  unsigned int getCount();

private:
  Queue<T>*     m_writerQueue;
  Queue<T>*     m_readerQueue;
};

/** @} */   // End of group: Memory
/** @} */   // End of group: Common

template <class T>
DualQueue<T>::DualQueue()
{
  m_writerQueue = new Queue<T>();
  ZS_ASSERT(0x000000df, m_writerQueue != NULL, "DualQueue writer queue allocation failed.");
  m_readerQueue = new Queue<T>();
  ZS_ASSERT(0x000000e0, m_readerQueue != NULL, "DualQueue reader queue allocation failed.");
}


template <class T>
DualQueue<T>::~DualQueue()
{
  // Clear the writer and reader queues.
  clear();

  // Free up memory.
  delete m_writerQueue;
  delete m_readerQueue;
}


template <class T>
void DualQueue<T>::push(T referenceObject)
{
  m_writerQueue->push(referenceObject);
}
  

template <class T>
void DualQueue<T>::pop()
{
  m_readerQueue->pop();
}


template <class T>
void DualQueue<T>::clear()
{
  m_writerQueue->clear();
  m_readerQueue->clear();
}


template <class T>
void DualQueue<T>::swap()
{
  if (!m_writerQueue->isEmpty() && m_readerQueue->isEmpty())
  {
    Queue<T>* temp = m_readerQueue;
    m_readerQueue = m_writerQueue;
    m_writerQueue = temp;
  }
}


template <class T>
bool DualQueue<T>::isEmpty()
{
  return m_readerQueue->isEmpty();
}


template <class T>
T DualQueue<T>::getFront()
{
  ZS_ASSERT(0x000001b4, !m_readerQueue->isEmpty(), "Do not call getFront() on an empty reader Queue");
  return m_readerQueue->getFront();
}


template <class T>
unsigned int DualQueue<T>::getCount()
{
  return m_readerQueue->getCount();
}


ZSPACE_COMMON_NS_END

#endif // __ZSPACE_DUAL_QUEUE_H__
