//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Queue.h
//  Content:    The reference-counting, queue class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_QUEUE_H__
#define __ZSPACE_QUEUE_H__

#include <queue>

#include "ZSpace/Common/CommonDefines.h"
#include "ZSpace/Common/Error/Error.h"

#include "ZSpace/Common/Memory/ReferenceObject.h"
#include "ZSpace/Common/Memory/Lockable.h"

/**
* @addtogroup Common
* @{
*
*   @addtogroup Memory
*   @{
*/

ZSPACE_COMMON_NS_BEGIN

/**
* The generic queue class.
*
* @remark
*   This class allows you to add objects to the end of a queue. You can access objects
*   at the front of the queue. You can check to see whether the queue is empty. You can
*   get the count of objects in the queue. Please see http://en.wikipedia.org/wiki/Queue_%28data_structure%29
*   for a more detailed discussion of queue functionality.
*/
template <class T>
class Queue : public zspace::common::Lockable<Queue<T> >
{
public:
	/**
	* The default constructor.
	*/
  Queue();

	/**
	* The destructor.
	*/
  ~Queue();

	/**
	* Push a ReferenceObject onto the end of the queue.
  *
  * @param[in] referenceObject
  *   A pointer to a ReferenceObject.
	*/
  void push(T referenceObject);

	/**
	* Remove a ReferenceObject from the front of the queue.
	*/
  void pop();

	/**
	* Clear the queue.
	*/
  void clear();

	/**
	* Grab the ReferenceObject at the front of the queue.
  *
  * @return
  *   A pointer to a ReferenceObject.
	*/
  T getFront();

	/**
	* Check whether or not the queue is empty.
	*/
  bool isEmpty();

	/**
	* Get the current number of ReferenceObjects in the queue.
	*/
  unsigned int getCount();

private:
  std::queue<T> m_referenceObjects;
};

/** @} */   // End of group: Memory
/** @} */   // End of group: Common

template <class T>
Queue<T>::Queue()
{
}


template <class T>
Queue<T>::~Queue()
{
  clear();
}


template <class T>
void Queue<T>::push(T referenceObject)
{
  static_cast<ReferenceObject*>(referenceObject)->addReference();
  m_referenceObjects.push(referenceObject);
}


template <class T>
void Queue<T>::pop()
{
  if (!m_referenceObjects.empty())
  {
    static_cast<ReferenceObject*>(m_referenceObjects.front())->removeReference();
    m_referenceObjects.pop();
  }
}


template <class T>
void Queue<T>::clear()
{
  while (!isEmpty())
    pop();
}


template <class T>
T Queue<T>::getFront()
{
  ZS_ASSERT(0x000001b5, !isEmpty(), "Do not call getFront() on an empty Queue");
  return m_referenceObjects.front();
}


template <class T>
bool Queue<T>::isEmpty()
{
  return m_referenceObjects.empty();
}


template <class T>
unsigned int Queue<T>::getCount()
{
  return (unsigned int)m_referenceObjects.size();
}

ZSPACE_COMMON_NS_END

#endif // __ZSPACE_QUEUE_H__
