//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2010 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       ZSUnitUtil.h
//  Content:    ZSUnitUtil - Unit conversion
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_UNIT_UTIL_H__
#define __ZSPACE_UNIT_UTIL_H__

#include "ZSpace/Common/CommonDefines.h"

ZSPACE_COMMON_NS_BEGIN

/**
* @addtogroup Common
* @{
*
*   @addtogroup Utility
*   @{
*/

/** Conversion between different unit types.
 */
class ZSPACE_COMMON_API Unit 
{
public:
  enum Type
  {
    TYPE_METERS = 0,
    TYPE_INCHES,
    TYPE_FEET,
  };

  /**
  * Get the scale conversion factor from any unit type to meters.
  *
  * @param[in] unitType
  *   The type of unit.
  *
  * @return
  *   The scale factor from the specified unit type to meters.
  */
  static float getUnitToMeterScale(Unit::Type unitType);

  /**
  * Get the scale conversion factor from meters to any unit type.
  *
  * @param[in] unitType
  *   The type of unit.
  *
  * @return
  *   The scale factor from meters to the specified unit type.
  */
  static float getMeterToUnitScale(Unit::Type unitType);
};

/** @} */   // End of group: Utility
/** @} */   // End of group: Common

ZSPACE_COMMON_NS_END

#endif // __ZSPACE_UNIT_UTIL_H__