//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       StereoLeftRightDetect.h
//  Content:    The L/R frame detection and synchronization interface.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_STEREO_LEFT_RIGHT_DETECT_H__
#define __ZSPACE_STEREO_LEFT_RIGHT_DETECT_H__

#include "ZSpace/Stereo/StereoDefines.h"
#include <cstdio>

ZSPACE_STEREO_NS_BEGIN

/**
* @addtogroup Stereo
* @{
*/

class StereoWindow;
class StereoLeftRightDetectImpl;

class ZSPACE_STEREO_API StereoLeftRightDetect
{
public:
  enum WindowType
  {
    WINDOW_TYPE_UNKNOWN = -1,
    WINDOW_TYPE_GL      = 0,
    WINDOW_TYPE_DIRECT3D9,
    WINDOW_TYPE_DIRECT3D10,
    WINDOW_TYPE_DIRECT3D11
  };

public:
  /**
  * Initialize a specified StereoWindow for L/R synchronization.
  *
  * @remark
  *   This should be called after StereoWindow creation.
  *
  * @param[in] stereoWindow
  *   A pointer to the StereoWindow.
  * @param[in] windowType
  *   The window's rendering type: WINDOW_TYPE_GL, WINDOW_TYPE_DIRECT3D9, etc.
  * @param[in] reserved
  *   A reserved argument depending on the windowType:
  *     WINDOW_TYPE_GL - NULL
  *     WINDOW_TYPE_DIRECT3D9 - The user should pass in a IDirect3DDevice9*
  *     WINDOW_TYPE_DIRECT3D10 - The user should pass in a IDXGISwapChain*
  *     WINDOW_TYPE_DIRECT3D11 - The user should pass in a IDXGISwapChain*
  */
  static void initialize(StereoWindow* stereoWindow, WindowType windowType, void* reserved = NULL);

  /**
  * Update any windows that have received an automatic L/R synchronization request.
  *
  * @remark
  *   This method should be called every frame on the application's rendering thread.
  */
  static void update();

  /**
  * Shutdown L/R synchronization.
  */
  static void shutdown();

private:
  static StereoLeftRightDetectImpl* m_stereoLeftRightDetectImpl;

private:
  /**
  * Default Constructor.
  *
  * @remark
  *   This class should never be instantiated.  
  */
  StereoLeftRightDetect();
};

/** @} */   // End of group: Stereo

ZSPACE_STEREO_NS_END

#endif // __ZSPACE_STEREO_LEFT_RIGHT_DETECT_H__
