//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       StereoWindow.h
//  Content:    The StereoWindow class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_STEREO_WINDOW_H__
#define __ZSPACE_STEREO_WINDOW_H__

#include "ZSpace/Stereo/StereoDefines.h"
#include "ZSpace/Stereo/StereoViewport.h"

#include "ZSpace/Common/Memory/List.h"
#include "ZSpace/Common/Memory/ReferenceObject.h"
#include "ZSpace/Common/System/DisplayInfo.h"

ZSPACE_STEREO_NS_BEGIN

/**
* @addtogroup Stereo
* @{
*/

/**
* The main class in the windowing hierarchy.
*
* @remark
*   The StereoWindow contains a collection of StereoViewport objects. 
*   Normally, the windowing measurements work in meters with the
*   0, 0, 0 position being in the center of the screen with +x to
*   the right, +y up and +z coming out of the screen; however, in
*   the case of these lower level classes, all measurements are in
*   pixels with the 0, 0 position being the upper left hand corner
*   of the screen.
*/
class ZSPACE_STEREO_API StereoWindow: public zspace::common::ReferenceObject
{
public:
  StereoWindow();
  StereoWindow(int x, int y, unsigned int width, unsigned int height);

  ~StereoWindow();
  
  /**
  * Add a StereoViewport to the window.
  *
  * @param[in] stereoViewport
  *   A pointer to a StereoViewport.
  * @param[in] index
  *   An optional index to determine where to insert the StereoViewport.
  *
  * @remark
  *   If index == UINT_MAX, the StereoViewport will get added to the end.
  */
  void addStereoViewport(StereoViewport* stereoViewport, unsigned int index = UINT_MAX);

  /**
  * Remove a StereoViewport from the window.
  *
  * @param[in] stereoViewport
  *   A pointer to the StereoViewport to be removed.
  */
  void removeStereoViewport(StereoViewport* stereoViewport);

  /**
  * Remove a StereoViewport from the window at a specified index.
  *
  * @param[in] index
  *   The index at which the StereoViewport will be removed.
  */
  void removeStereoViewportAtIndex(unsigned int index);

  /**
  * Remove all StereoViewports from the window.
  */
  void removeAllStereoViewports();

  /**
  * Get a StereoViewport at a specified index.
  */
  StereoViewport* getStereoViewport(unsigned int index) const;

  /**
  * Get the number of StereoViewports that the window currently has.
  */
  unsigned int getNumStereoViewports() const;

  /**
  * Move the the window to a specified [x, y] screen coordinate.
  *
  * @remark
  *   This will move the top left corner of the window to match the specified screen coordinates.
  *
  * @param[in] x
  *   The virtual x screen coordinate that the window's left side will align with.
  * @param[in] y
  *   The virtual y screen coordinate that the window's top side will align with.
  */
  void move(int x, int y);

  /**
  * Set the horizontal x position of the window.
  *
  * @param[in] x
  *   The virtual x screen coordinate that the window's left side will align with.
  */
  void setX(int x);

  /**
  * Get the horizontal x position of the window.
  */
  int getX() const;

  /**
  * Set the vertical y position of the window.
  *
  * @param[in] y
  *   The virtual y screen coordinate that the window's top side will align with.
  */
  void setY(int y);
  
  /**
  * Get the vertical y position of the window.
  */
  int getY() const;

  /**
  * Resize the window to a specified width and height.
  *
  * @param[in] width
  *   The pixel width of the window.
  * @param[in] height
  *   The pixel height of the window.
  */
  void resize(unsigned int width, unsigned int height);

  /**
  * Set the width of the window.
  *
  * @param[in] width
  *   The pixel width of the window.
  */
  void setWidth(unsigned int width);

  /**
  * Get the pixel width of the window.
  */
  unsigned int getWidth() const;

  /**
  * Set the height of the window.
  *
  * @param[in] height
  *   The pixel height of the window.
  */
  void setHeight(unsigned int height);

  /**
  * Get the pixel height of the window.
  */
  unsigned int getHeight() const;

  /**
  * Get the index of the display that the window is currently on.
  *
  * @return
  *   The index of the display. If the window is on an invalid display, -1 is returned.
  */
  int getCurrentDisplayIndex() const;

  /**
  * Get the display that the window is currently on.
  *
  * @return
  *   The display. If the window is on an invalid display, NULL is returned.
  */
  const zspace::common::DisplayInfo::Display* getCurrentDisplay();

private:
  // For best practices, the copy constructor and assignment operator are disabled for this class
  StereoWindow(const StereoWindow&);
  StereoWindow& operator=(const StereoWindow&);

  // Type definitions.
  typedef zspace::common::List<StereoViewport*> StereoViewportList;

private:
  StereoViewportList            m_stereoViewports;
  int                           m_x;
  int                           m_y;
  unsigned int                  m_width;
  unsigned int                  m_height;
  zspace::common::DisplayInfo   m_displayInfo;
  int                           m_currentDisplayIndex;

private:
  void onMove();
  void onResize();

  void moveStereoViewport(StereoViewport* stereoViewport);
  void resizeStereoViewport(StereoViewport* stereoViewport);
  void calculateDisplayIndex();
};

/** @} */   // End of group: Stereo

ZSPACE_STEREO_NS_END

#endif // __ZSPACE_STEREO_WINDOW_H__