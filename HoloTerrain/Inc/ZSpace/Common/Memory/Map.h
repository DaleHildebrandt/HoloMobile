//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       Map.h
//  Content:    The reference-counting, Map class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_MAP_H__
#define __ZSPACE_MAP_H__

#include <map>

#include "ZSpace/Common/CommonDefines.h"

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
* The generic map class for key/value pairs.
*
* @remark
*   This class provides a map of key/value pairs. There are utility
*   methods to add and remove values based on specific keys. You can replace
*   keys. You can remove a key/value pair at a specific position based on an
*   integer iterator.
*/
template <class K, class V>
class Map : public Lockable<Map<K,V> >
{
public:
  typedef typename std::map<K, V>::iterator Iterator;

public:
	/**
	* The default constructor.
	*/
  Map();

	/**
	* The destructor.
	*/
  ~Map();

  /**
	* Add a key-value pair to the map.
  *
  * @remark
  *   If the key already exists in the map, the add will be ignored.
  *
  * @param[in] key
  *   A key of any type.
  * @param[in] referenceObject
  *   A ReferenceObject.
	*/
  void add(const K& key, const V& referenceObject);

  /**
	* Replace a ReferenceObject at a specified key.
  * 
  * @remark
  *   If the key does not exist, a new entry will be created.
  *
  * @param[in] key
  *   A key of any type.
  * @param[in] referenceObject
  *   A ReferenceObject.
	*/
  void replace(const K& key, const V& referenceObject);

  /**
	* Remove a key-value pair from the map.
  *
  * @param[in] i
  *   An Iterator corresponding to the key-value pair.
	*/
  void remove(Iterator i);

  /**
	* Remove a key-value pair from the map.
  *
  * @param[in] key
  *   The key of the entry to be removed.
	*/
  void removeKey(const K& key);

  /**
	* Remove all key-value pairs from the map.
	*/
  void clear();

  /**
	* Check if the map contains a specified key.
  *
  * @return
  *   Whether or not the key exists in the map.
	*/
  bool containsKey(const K& key);

  /**
	* Check if the map contains a specified ReferenceObject.
  *
  * @return
  *   Whether or not the value exists in the map.
	*/
  bool containsValue(const V& referenceObject);

  /**
	* Get the ReferenceObject with a specified key.
  *
  * @param[in] key
  *   The key corresponding to the ReferenceObject.
  *
  * @return
  *   A pointer to the ReferenceObject.  If the key does not exist, NULL is returned.
	*/
  V get(const K& key);

  /**
	* Get the ReferenceObject with a specified key.
  *
  * @param[in] key
  *   The key corresponding to the ReferenceObject.
  *
  * @return
  *   A pointer to the ReferenceObject.  If the key does not exist, NULL is returned.
	*/
  V operator[] (const K& key);

	/**
	* Get the number of key-value pairs in the map.
	*/
  unsigned int getCount() const;

	/**
	* Get an iterator based on a specified key.
  *
  * @param[in] key
  *   A key corresponding to a key-value pair.
  *
  * @return
  *   An Iterator corresponding to the key-value pair if the key exists.\n
  *   If the key does not exist, returns an Iterator equal to Map::end().
	*/
  Iterator find(const K& key);

	/**
	* Get an iterator corresponding to the beginning of the map.
	*/
  Iterator begin();

  /**
	* Get an iterator corresponding to the end of the map.
	*/
  Iterator end();

private:
  std::map<K, V>  m_referenceObjects;
};

/** @} */   // End of group: Memory
/** @} */   // End of group: Common

template <class K, class V>
Map<K, V>::Map()
{
}


template <class K, class V>
Map<K, V>::~Map()
{
  clear();
}


template <class K, class V>
void Map<K, V>::add(const K& key, const V& referenceObject)
{
  //Iterator i = m_referenceObjects.find(key);

  //if (i == m_referenceObjects.end())
  //{
  //  static_cast<ReferenceObject*>(referenceObject)->addReference();
  //  m_referenceObjects.insert(std::pair<K, V>(key, referenceObject));
  //}

  Iterator i = m_referenceObjects.lower_bound(key);

  if (i != m_referenceObjects.end() && !(m_referenceObjects.key_comp()(key, i->first)))
  {
    // Key already exists.
  }
  else
  {
    // The key does not exist, so add it to the map.
    static_cast<ReferenceObject*>(referenceObject)->addReference();
    m_referenceObjects.insert(i, std::map<K, V>::value_type(key, referenceObject));
  }
}


template <class K, class V>
void Map<K, V>::replace(const K& key, const V& referenceObject)
{
  Iterator i = find(key);

  if (i != end())
    static_cast<ReferenceObject*>(i->second)->removeReference();

  static_cast<ReferenceObject*>(referenceObject)->addReference();
  m_referenceObjects[key] = referenceObject;
}


template <class K, class V>
void Map<K, V>::remove(typename Map<K, V>::Iterator i)
{
  static_cast<ReferenceObject*>(i->second)->removeReference();
  m_referenceObjects.erase(i);
}


template <class K, class V>
void Map<K, V>::removeKey(const K& key)
{
  Iterator i = find(key);

  if (i != end())
    remove(find(key));
}


template <class K, class V>
void Map<K, V>::clear()
{
  for (Iterator i = begin(); i != end(); i++)
    static_cast<ReferenceObject*>(i->second)->removeReference();

  m_referenceObjects.clear();
}


template <class K, class V>
bool Map<K, V>::containsKey(const K& key)
{
  Iterator i = find(key);

  if (i != m_referenceObjects.end())
    return true;

  return false;
}


template <class K, class V>
bool Map<K, V>::containsValue(const V& referenceObject)
{
  for (Iterator i = m_referenceObjects.begin(); i != m_referenceObjects.end(); i++)
  {
    if (i->second == referenceObject)
      return true;
  }

  return false;
}


template <class K, class V>
V Map<K, V>::get(const K& key)
{
  Iterator i = find(key);

  if (i != m_referenceObjects.end())
    return i->second;

  return NULL;
}


template <class K, class V>
V Map<K, V>::operator[] (const K& key)
{
  return get(key);
}


template <class K, class V>
unsigned int Map<K, V>::getCount() const
{
  return (unsigned int)m_referenceObjects.size();
}


template <class K, class V>
typename Map<K, V>::Iterator Map<K, V>::find(const K& key)
{
  return m_referenceObjects.find(key);
}


template <class K, class V>
typename Map<K, V>::Iterator Map<K, V>::begin()
{
  return m_referenceObjects.begin();
}


template <class K, class V>
typename Map<K, V>::Iterator Map<K, V>::end()
{
  return m_referenceObjects.end();
}


ZSPACE_COMMON_NS_END

#endif // __ZSPACE_MAP_H__
