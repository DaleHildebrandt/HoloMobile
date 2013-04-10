//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       DisplayInfo.h
//  Content:    The DisplayInfo class containing information about all
//              connected display devices.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_DISPLAY_INFO_H__
#define __ZSPACE_DISPLAY_INFO_H__

#include "ZSpace/Common/CommonDefines.h"
#include "ZSpace/Common/Math/Vector3.h"
#include "ZSpace/Common/Math/Matrix4.h"

/**
* @addtogroup Common
* @{
*
*   @addtogroup System
*   @{
*/

namespace zspace 
{ 
  namespace system 
  { 
    class SystemClient; 
  } 
}

ZSPACE_COMMON_NS_BEGIN

/**
* This class provides information about the display.
*
* @remark
*   This class provides information about the current display
*   (e. g., identification number, position, offset, angle, and
*   other attributes) that will be useful in interacting with the
*   device. Getting the display information is an expensive process
*   which should be used minimally and, if possible, only during initialization.
*/
class ZSPACE_COMMON_API DisplayInfo
{
public:
  static const int MAX_NUM_DISPLAYS               = 128;
  static const int MONITOR_DEVICE_NAME_MAX_LENGTH = 32;
  static const int DEVICE_NAME_MAX_LENGTH         = 32;
  static const int DEVICE_STRING_MAX_LENGTH       = 128;
  static const int DEVICE_ID_MAX_LENGTH           = 128;
  static const int DEVICE_KEY_MAX_LENGTH          = 128;

public:

  /**
  * This enum represents the type of data that the EDID year represents
  */
  enum YearType
  {
    YEAR_TYPE_UNKNOWN = -1,
    YEAR_TYPE_MODEL_YEAR = 0,
    YEAR_TYPE_YEAR_OF_MANUFACTURE
  };

  /**
  * This enum represents the type of data that the video input definition represents
  */
  enum VideoSignalInterfaceType
  {
    VIDEO_SIGNAL_INTERFACE_TYPE_UNKNOWN = -1,
    VIDEO_SIGNAL_INTERFACE_TYPE_ANALOG = 0,
    VIDEO_SIGNAL_INTERFACE_TYPE_DIGITAL
  };

  /**
  * This enum represents a part of the digital video input definition
  */
  enum ColorBitDepthType
  {
    COLOR_BIT_DEPTH_TYPE_UNKNOWN = -1,
    COLOR_BIT_DEPTH_TYPE_UNDEFINED = 0,
    COLOR_BIT_DEPTH_TYPE_6_BITS_PER_PRIMARY_COLOR,
    COLOR_BIT_DEPTH_TYPE_8_BITS_PER_PRIMARY_COLOR,
    COLOR_BIT_DEPTH_TYPE_10_BITS_PER_PRIMARY_COLOR,
    COLOR_BIT_DEPTH_TYPE_12_BITS_PER_PRIMARY_COLOR,
    COLOR_BIT_DEPTH_TYPE_14_BITS_PER_PRIMARY_COLOR,
    COLOR_BIT_DEPTH_TYPE_16_BITS_PER_PRIMARY_COLOR
  };

  /**
  * This enum represents a part of the digital video input definition
  */
  enum DigitalVideoInterfaceStandardType
  {
    DIGITAL_VIDEO_INTERFACE_STANDARD_TYPE_UNKNOWN = -1,
    DIGITAL_VIDEO_INTERFACE_STANDARD_TYPE_UNDEFINED = 0,
    DIGITAL_VIDEO_INTERFACE_STANDARD_TYPE_DVI,
    DIGITAL_VIDEO_INTERFACE_STANDARD_TYPE_HDMI_A,
    DIGITAL_VIDEO_INTERFACE_STANDARD_TYPE_HDMI_B,
    DIGITAL_VIDEO_INTERFACE_STANDARD_TYPE_MDDI,
    DIGITAL_VIDEO_INTERFACE_STANDARD_TYPE_DISPLAYPORT
  };

  /**
  * This struct represents the digital video input definition of the EDID
  */
  struct DigitalVideoInterfaceInfo
  {
    ColorBitDepthType colorBitDepthType;
    DigitalVideoInterfaceStandardType digitalVideoInterfaceStandardType;
  };

  /**
  * This struct represents the supported EDID (Extended Display Identification Data)
  */
  struct Edid
  {
    // The ID manufacturer name contains a NULL terminated string representation of the display manufacturer's 3 character code
    char idManufacturerName[4];

    // The ID product code is an integer representation used to differentiate between different models from the same manufacturer
    char idProductCode[5];

    // The ID serial number is an integer representation of a 32-bit number used to differentiate between individual instances of the same display model
    unsigned int idSerialNumber;

    // If valid, the week of manufacture is set to a value in the range of 1-54 weeks
    bool validWeekOfManufacture;
    unsigned int weekOfManufacture;

    // The year of manufacturer is used to represent the year of the display's manufacture or the model year
    YearType yearOfManufactureType;
    unsigned int yearOfManufacture;

    // The EDID version and revision information
    unsigned int version;
    unsigned int revision;

    // Video Input Definition
    // Note, the valid information below depends on the EDID video signal interface type (digital or analog)
    VideoSignalInterfaceType videoSignalInterfaceType;
    DigitalVideoInterfaceInfo digitalVideoInterfaceInfo;

    // (Optional) The display product serial number as a NULL terminated alphanumeric string
    bool isDisplayProductSerialNumberValid;
    char displayProductSerialNumber[14];

    // (Optional) The alphanumeric NULL terminated string
    bool isAlphanumericDataStringValid;
    char alphanumericDataString[14];

    // (Optional) The model name of the display product as a NULL terminated alphanumeric string
    bool isDisplayProductNameValid;
    char displayProductName[14];
  };

  /**
  * This struct represents the display.
  */
  struct Display 
  {
    // From the EDID, a flag to specify if the display is a zSpace display
    bool  isZSpaceDisplay;

    // Extended display identification data
    Edid edid;

    // Display Properties > Setting counts unattached monitors so this number could differ from adapterIndex + 1.
    int   displayNumber;		    ///< The number displayed in "Display Properties" > "Settings"

    // From DEVMODE returned by EnumDisplaySettingsEx.
    int   displayPosition[2];		///< The display [x, y] pixel location on virtual desktop
    int   displayResolution[2];	///< The display [width, height] in pixels
    
    // From HORZSIZE and VERTSIZE returned by GetDeviceCaps.
    float displaySize[2];			          ///< The horizontal and vertical size (in meters)
    int   displayVerticalRefreshRate;       ///< Current vertical refresh rate.
    
    // From registry: zSpace/Settings/Display/Profile.
    float displayOffset[3];		  ///< The offset vector from origin of universal coordinate system (in meters)
    float displayAngle[2];		  ///< The horizontal and vertical angle relative to UCS (left-hand rotation)
    
    // From DISPLAY_DEVICE structure returned by EnumDisplayDevices for adapters.
    int   adapterIndex;
    char  adapterName[DEVICE_NAME_MAX_LENGTH];
    char  adapterString[DEVICE_STRING_MAX_LENGTH];
    char  adapterId[DEVICE_ID_MAX_LENGTH];
    char  adapterKey[DEVICE_KEY_MAX_LENGTH];
    
    // From DISPLAY_DEVICE structure returned by EnumDisplayDevices for monitors.
    int   monitorIndex;
    char  monitorName[DEVICE_NAME_MAX_LENGTH];
    char  monitorString[DEVICE_STRING_MAX_LENGTH];
    char  monitorId[DEVICE_ID_MAX_LENGTH];
    char  monitorKey[DEVICE_KEY_MAX_LENGTH];
    
    // The generic monitor device name.
    char  deviceName[MONITOR_DEVICE_NAME_MAX_LENGTH];
  };
 
public:
  /**
  * The default constructor.
  */
  DisplayInfo();
  
  /**
  * The destructor.
  */
  ~DisplayInfo();
	
  /**
  * Find the index of the display at a particular set of virtual desktop coordinates.
  *
  * @param[in] x
  *   Virtual desktop x coordinate.
  * @param[in] y
  *   Virtual desktop coordinate.
  *
  * @return
  *   The index of the display at that particular virtual (x, y) position.  If the
  *   display was not found, return -1.
  */
  int getDisplayIndex(int x, int y) const;

  /**
  * Get the display at a specified index.
  */
  const Display* getDisplay(int index) const;
	
  /**
  * Get the display for given a set of virtual desktop coordinates.
  *
  * @param[in] x
  *   Virtual desktop x coordinate.
  * @param[in] y
  *   Virtual desktop coordinate.
  *
  * @return
  *   A reference to the display at that particular virtual (x, y) position.
  */
  const Display* getDisplay(int x, int y) const;

  /**
  * Get the number of displays.
  *
  * @return
  *   The number of displays in the system.
  */
  int getNumDisplays() const;

  /**
  * Check if the zSpace hardware is present (USB connected).
  */
  bool isZSpaceHardwarePresent() const;

  /**
  * Get the relative offset to apply to a tracking device when the viewport is
  * not the full size of the display.
  *
  * @param[in] display
  *   The Display that contains the viewport
  * @param[in] viewportX
  *   The viewport's x position.
  * @param[in] viewportY
  *   The viewport's y position.
  * @param[in] viewportWidth
  *   The viewport's width.
  * @param[in] viewportHeight
  *   The viewport's hight.
  *
  * @return
  *   A relative offset to apply to tracker information.
  */
  static zspace::common::Vector3 getViewportOffset(const Display* display, int viewportX, int viewportY, int viewportWidth, int viewportHeight);

   /**
  * Get the transform matrix that represents the transform from tracker space to
  * camera space.
  *
  * @param[in] display
  *   The Display that contains the viewport.
  *
  * @return
  *   The Matrix representing the transform from tracker space to camera space.
  */
  static zspace::common::Matrix4 getTrackerToCameraSpaceTransform(const Display* display);

private:
  Display                       m_displays[MAX_NUM_DISPLAYS];
  int                           m_numDisplays;
  zspace::system::SystemClient* m_systemClient;
};

ZSPACE_COMMON_NS_END

/** @} */   // End of group: System
/** @} */   // End of group: Common

#endif // __ZSPACE_DISPLAY_INFO_H__