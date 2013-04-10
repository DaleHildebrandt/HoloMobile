//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       List.h
//  Content:    The reference-counting, List class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_LIST_H__
#define __ZSPACE_LIST_H__

#include <algorithm>
#include <vector>

#include "ZSpace/Common/CommonDefines.h"

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
* The generic list class.
*
* @remark
*   This class mimics an array. It allows you to add and remove
*   objects at the end of the list or at a specific index in the list.
*   You can also access a given object at its specific index. You can
*   check to see if the list contains a specific object.
*/
template <class T>
class List : public zspace::common::Lockable<List<T> >
{
public:
	/**
	* The default constructor.
	*/
  List();

	/**
	* Copy constructor.
	*/
  List(const List& list);

	/**
	* The destructor.
	*/
  ~List();

	/**
	* Add a ReferenceObject to the list.
  *
  * @param[in] referenceObject
  *   A pointer to any class derived from ReferenceObject.
  * @param[in] index
  *   An optional index to determine where to insert the value into the list.
  *
  * @remark
  *   If index == UINT_MAX, the ReferenceObject will get added to the end of the list.
	*/
  void add(T referenceObject, unsigned int index = UINT_MAX);

	/**
	* Remove a ReferenceObject from the list.
  *
  * @param[in] referenceObject
  *   A pointer to the ReferenceObject to be removed.
	*/
  void remove(T referenceObject);

	/**
	* Remove a ReferenceObject from the list at a specified index.
  *
  * @param[in] index
  *   The index of the ReferenceObject to be removed.
	*/
  void removeAtIndex(unsigned int index);

	/**
	* Clear all ReferenceObjects from the list.
	*/
  void clear();

	/**
	* Check if the list contains specified ReferenceObject.
  *
  * @param[in] referenceObject
  *   A pointer to a ReferenceObject.
  *
  * @return
  *   Whether or not the ReferenceObject exists in the list.
	*/
  bool contains(T referenceObject);

	/**
	* Get the referenceObject at a specified index.  
	*/
  T get(unsigned int index) const;

  /**
	* Returns the index of the specified ReferenceObject.
  *
  * @param[in] referenceObject
  *   Pointer to a ReferenceObject.
  *
  * @return
  *   The index of the specifed ReferenceObject.\n
  *   If the ReferenceObject was not found, UINT_MAX is returned.
	*/
  unsigned int getIndex(T referenceObject);

	/**
	* Get the reference object at a specified index.
	*/
  T operator[] (unsigned int index) const;

  /**
	* Assignment operator.
	*/
  List& operator= (const List& list);

	/**
	* Get the number of ReferenceObjects in the list.
	*/
  unsigned int getCount() const;

private:
  std::vector<T>  m_referenceObjects;
};

/** @} */   // End of group: Memory
/** @} */   // End of group: Common

template <class T>
List<T>::List()
{
}


template <class T>
List<T>::List(const List<T>& list)
{
  for (unsigned int i = 0; i < list.getCount(); i++)
    add(list[i]);
}


template <class T>
List<T>::~List()
{
  clear();
}


template <class T>
void List<T>::add(T referenceObject, unsigned int index)
{  
  static_cast<ReferenceObject*>(referenceObject)->addReference();

  if (index == UINT_MAX)
    m_referenceObjects.push_back(referenceObject);
  else
    m_referenceObjects.insert(m_referenceObjects.begin() + index, referenceObject);
}


template <class T>
void List<T>::remove(T referenceObject)
{
  std::vector<T>::iterator i = find(m_referenceObjects.begin(), m_referenceObjects.end(), referenceObject);

  if (i != m_referenceObjects.end())
  {
    static_cast<ReferenceObject*>(referenceObject)->removeReference();
    m_referenceObjects.erase(i);
  }
}


template <class T>
void List<T>::removeAtIndex(unsigned int index)
{
  static_cast<ReferenceObject*>(m_referenceObjects[index])->removeReference();
  m_referenceObjects.erase(m_referenceObjects.begin() + index);
}


template <class T>
void List<T>::clear()
{
  for (unsigned int i = 0; i < getCount(); i++)
    static_cast<ReferenceObject*>(m_referenceObjects[i])->removeReference();

  m_referenceObjects.clear();
}


template <class T>
bool List<T>::contains(T referenceObject)
{
  std::vector<T>::iterator i = find(m_referenceObjects.begin(), m_referenceObjects.end(), referenceObject);

  if (i != m_referenceObjects.end())
    return true;

  return false;
}


template <class T>
T List<T>::get(unsigned int index) const
{
  return m_referenceObjects[index];
}


template <class T>
unsigned int List<T>::getIndex(T referenceObject)
{
  for (unsigned int i = 0; i < getCount(); i++)
  {
    if (m_referenceObjects[i] == referenceObject)
      return i;
  }

  return UINT_MAX;
}


template <class T>
T List<T>::operator[] (unsigned int index) const
{
  return m_referenceObjects[index];
}


template <class T>
List<T>& List<T>::operator= (const List<T>& list)
{
  clear();

  for (unsigned int i = 0; i < list.getCount(); i++)
    add(list[i]);

  return *this;
}


template <class T>
unsigned int List<T>::getCount() const
{
  return (unsigned int)m_referenceObjects.size();
}


ZSPACE_COMMON_NS_END

#endif // __ZSPACE_LIST_H__
