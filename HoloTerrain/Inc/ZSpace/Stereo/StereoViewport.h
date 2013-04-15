//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       StereoViewport.h
//  Content:    The StereoViewport class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_STEREO_VIEWPORT_H__
#define __ZSPACE_STEREO_VIEWPORT_H__

#include "ZSpace/Stereo/StereoDefines.h"
#include "ZSpace/Common/Memory/ReferenceObject.h"

ZSPACE_STEREO_NS_BEGIN

class StereoFrustum;
class StereoWindow;

/**
* @addtogroup Stereo
* @{
*/

/**
* An important class for performing actions inside a stereo window.
*
* @remark
*   The StereoViewport represents a sub-window within the StereoWindow
*   for stereo rendering. In stereoscopic imaging, there are two frusta
*   (see the class StereoFrustum), one for each eye. Frusta provide
*   clipping volumes to render images for the left and right eye. The
*   images are resolved by the viewer to appear as one stereoscopic 3D
*   image. An application can have multiple StereoViewports for a single
*   StereoWindow. The StereoViewport specifies the rectangular region of
*   the StereoWindow into which it will render and its position represents
*   its upper-left corner (starting at 0, 0). The most common case is
*   a 1-1 mapping between StereoWindow and StereoViewport. 1-many relationships 
*   are also supported.
*/
class ZSPACE_STEREO_API StereoViewport: public zspace::common::ReferenceObject
{
  friend class StereoWindow;

public:
  StereoViewport();

  /**
  * Default Constructor.
  *
  * @param[in] x
  *   The virtual x screen coordinate relative to its parent StereoWindow.
  *   This aligns with the viewport's left side.
  * @param[in] y
  *   The virtual y screen coordinate relative to its parent StereoWindow.
  *   This aligns with the viewport's top side.
  * @param[in] width
  *   The width of the viewport.
  * @param[in] height
  *   The height of the viewport.
  */
  StereoViewport(int x, int y, unsigned int width, unsigned int height);

  /**
  * The destructor.
  */
  ~StereoViewport();

  /**
  * Get the viewports parent StereoWindow.
  *
  * @return
  *   A pointer to the parent StereoWindow. If the viewport has not been added to
  *   a window, returns NULL.
  */
  StereoWindow* getStereoWindow() const;

  /**
  * Get the StereoFrustum of the viewport.
  */
  StereoFrustum* getStereoFrustum() const;

  /**
  * Move the the viewport to a specified [x, y] screen coordinate relative to its parent window.
  *
  * @param[in] x
  *   The virtual x screen coordinate that the viewport's left side will align with.
  * @param[in] y
  *   The virtual y screen coordinate that the viewport's top side will align with.
  */
  void move(int x, int y);

  /**
  * Set the horizontal x position of the StereoViewport.
  *
  * @param[in] x
  *   The virtual x screen coordinate that the viewport's left side will align with.
  *   This x screen coordinate is relative to the viewport's parent window.
  */
  void setX(int x);

  /**
  * Get the horizontal x position of the StereoViewport.
  *
  * @remark
  *   The x screen coordinate is relative to the viewport's parent window.
  */
  int getX() const;

  /**
  * Set the vertical y position of the StereoViewport.
  *
  * @param[in] y
  *   The virtual y screen coordinate that the viewport's top side will align with.
  *   This y screen coordinate is relative to the viewport's parent window.
  */
  void setY(int y);
  
  /**
  * Get the vertical y position of the StereoViewport.
  *
  * @remark
  *   The y screen coordinate is relative to the viewport's parent window.
  */
  int getY() const;

  /**
  * Resize the viewport to a specified width and height.
  *
  * @param[in] width
  *   The pixel width of the viewport.
  * @param[in] height
  *   The pixel height of the viewport.
  */
  void resize(unsigned int width, unsigned int height);

  /**
  * Set the width of the StereoViewport.
  *
  * @param[in] width
  *   The pixel width of the StereoViewport.
  */
  void setWidth(unsigned int width);

  /**
  * Get the pixel width of the StereoViewport.
  */
  unsigned int getWidth() const;

  /**
  * Set the height of the StereoViewport.
  *
  * @param[in] height
  *   The pixel height of the StereoViewport.
  */
  void setHeight(unsigned int height);

  /**
  * Get the pixel height of the StereoViewport.
  */
  unsigned int getHeight() const;

  /**
  * Sets whether or not the viewport will use the same width and height as its parent StereoWindow.
  *
  * @remark
  *   If set to true, whenever the StereoWindow is resized, the viewport will automatically get resized.
  */
  void setUsingWindowSize(bool isUsingWindowSize);

  /**
  * Checks whether or not the viewport will use the same width and height as its parent StereoWindow.
  */
  bool isUsingWindowSize() const;

private:
  StereoWindow*     m_stereoWindow;
  StereoFrustum*    m_stereoFrustum;
  int               m_x;
  int               m_y;
  unsigned int      m_width;
  unsigned int      m_height;
  bool              m_isUsingWindowSize;

private:
  // For best practices, the copy constructor and assignment operator are disabled for this class
  StereoViewport(const StereoViewport&);
  StereoViewport& operator=(const StereoViewport&);

  void setStereoWindow(StereoWindow* stereoWindow);

  void onMove();
  void onResize();
};

/** @} */   // End of group: Stereo

ZSPACE_STEREO_NS_END

#endif // __ZSPACE_STEREO_VIEWPORT_H__