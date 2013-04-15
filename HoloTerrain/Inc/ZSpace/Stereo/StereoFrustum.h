//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2011 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       StereoFrustum.h
//  Content:    The StereoFrustum class.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_STEREO_FRUSTUM_H__
#define __ZSPACE_STEREO_FRUSTUM_H__

#include "ZSpace/Stereo/StereoDefines.h"

#include "ZSpace/Common/Math/Math.h"
#include "ZSpace/Common/Math/Vector2.h"
#include "ZSpace/Common/Math/Vector3.h"
#include "ZSpace/Common/Math/Matrix4.h"

#include "ZSpace/Common/Memory/ReferenceObject.h"
#include "ZSpace/Common/Utility/UnitUtil.h"

ZSPACE_STEREO_NS_BEGIN

/**
* @addtogroup Stereo
* @{
*/

/**
* An important class for performing actions inside a stereo viewport.
*
* @remark
*   Each StereoViewport has an associated StereoFrustum. 
*   The StereoFrustum object has all of the functions for
*   getting access to all the coordinate system information.
*   A frustum (http://en.wikipedia.org/wiki/Frustum) is the
*   part of a solid left after cutting off a top portion with
*   a plane perfectly parallel to the base. Our frusta are
*   tetrahedral because they project through a rectangle (the
*   display screen). In stereoscopic imaging, there are two
*   frusta, one for each eye. The frusta, which merge, are
*   formed by the plane of the screen and the plane of the
*   objects seen by the viewer with the originating points of
*   the tetrahedra being the left and right eyes. The object
*   plane can appear to be in front of, parallel to or behind
*   the screen, hence the 3D effect. Since one's head can move,
*   the origins of the frusta need to be tracked to maintain
*   coordination with the left and right eyes.
*/
class ZSPACE_STEREO_API StereoFrustum: public zspace::common::ReferenceObject
{
  friend class StereoViewport;

public:
  enum StereoEye 
  {
    STEREO_EYE_LEFT = 0,
    STEREO_EYE_RIGHT,
    STEREO_EYE_CENTER,
  };

public:
  /**
  * The struct containing the boundaries of the StereoFrustum.
  */
  struct Bounds
  {
    float left;
    float right;
    float top;
    float bottom;
    float nearClip;
    float farClip;
  };

public:

  ~StereoFrustum();

  /**
  * Set the inter-pupillary distance of the frustum.
  *
  * @param[in] interPupillaryDistance
  *   The inter-pupillary distance (physical separation between the eyes).
  * @param[in] unitType
  *   The type of desired units. Defaults to TYPE_METERS. Other types include TYPE_INCHES and TYPE_FEET.
  *
  * @warning
  *   Inter-pupillary distances below 0.0f will result in failure and have 
  *   no effect on the frustum.
  *
  * @return
  *   True for success, false for failure.
  */
  bool setInterPupillaryDistance(float interPupillaryDistance, 
                                 zspace::common::Unit::Type unitType = zspace::common::Unit::TYPE_METERS);

  /**
  * Get the inter-pupillary distance of the frustum.  This is the physical separation between the eyes.
  *
  * @param[in] unitType
  *   The type of desired units. Defaults to TYPE_METERS. Other types include TYPE_INCHES and TYPE_FEET.
  *
  * @return
  *   The inter-pupillary distance.
  */
  float getInterPupillaryDistance(zspace::common::Unit::Type unitType = zspace::common::Unit::TYPE_METERS);

  /**
  * Set the stereo level of the frustum.
  *
  * @param[in] stereoLevel
  *
  * @return
  *   True for success, false for failure.
  */
  bool setStereoLevel(float stereoLevel);

  /**
  * Get the stereo level of the frustum.
  *
  * @return
  *     The stereo level of the frustum
  */
  float getStereoLevel() const;

  /**
  * Set the world scale of the frustum.
  *
  * @param[in] worldScale
  *
  * @return
  *   True for success, false for failure.
  */
  bool setWorldScale(float worldScale);

  /**
  * Get the world scale of the frustum.
  *
  * @return
  *     The world scale of the frustum.
  */
  float getWorldScale() const;

  /**
  * Set the field of view (FOV) scale for the frustum.
  *
  * @param[in] fieldOfViewScale
  *   The field of view scale.
  *
  * @return
  *   True for success, false for failure.
  */
  bool setFieldOfViewScale(float fieldOfViewScale);

  /**
  * Get the field of view (FOV) scale for the frustum.
  *
  * @return
  *   The field of view scale for the frustum.
  */
  float getFieldOfViewScale() const;

  /**
  * Set the zero parallax offset for the frustum.
  *
  * @param[in] zeroParallaxOffset
  *
  * @return
  *   True for success, false for failure.
  */
  bool setZeroParallaxOffset(float zeroParallaxOffset);

  /**
  * Get the zero parallax offset for the frustum.
  *
  * @return
  *   The zero parallax offset for the frustum
  */
  float getZeroParallaxOffset() const;

  /**
  * Get the zero parallax offset that places the given point at zero parallax.
  *
  * @param[in] position The position in ZSpace
  *
  * @return 
  *   The zero parallax offset that places the given point at zero parallax
  */
  float getZeroParallaxOffsetForPosition(const zspace::common::Vector3& position);

  /**
  * Set the near clip plane of the frustum.
  *
  * @param[in] nearClip
  *   The value of the near clip plane.
  * @param[in] unitType
  *   The type of desired units. Defaults to TYPE_METERS. Other types include TYPE_INCHES and TYPE_FEET.
  *
  * @warning
  *   Near clip values less than 0.0f will result in failure and have no effect on the frustum.
  *
  * @return
  *   True for success, false for failure.
  */
  bool setNearClip(float nearClip, zspace::common::Unit::Type unitType = zspace::common::Unit::TYPE_METERS);

  /**
  * Get the near clip plane of the frustum.
  *
  * @param[in] unitType
  *   The type of desired units. Defaults to TYPE_METERS. Other types include TYPE_INCHES and TYPE_FEET.
  *
  * @return
  *   The near clip plane.
  */
  float getNearClip(zspace::common::Unit::Type unitType = zspace::common::Unit::TYPE_METERS);

  /**
  * Set the far clip plane of the frustum.
  *
  * @param[in] farClip
  *   The value of the far clip plane.
  * @param[in] unitType
  *   The type of desired units. Defaults to TYPE_METERS. Other types include TYPE_INCHES and TYPE_FEET.
  *
  * @warning
  *   Far clip values less than 0.0f or less than the current near clip value will result in 
  *   failure and have no effect on the frustum.
  *
  * @return
  *   True for success, false for failure.
  */
  bool setFarClip(float farClip, zspace::common::Unit::Type unitType = zspace::common::Unit::TYPE_METERS);

  /**
  * Get the far clip plane of the frustum.
  *
  * @param[in] unitType
  *   The type of desired units. Defaults to TYPE_METERS. Other types include TYPE_INCHES and TYPE_FEET.
  *
  * @return
  *   The far clip plane.
  */
  float getFarClip(zspace::common::Unit::Type unitType = zspace::common::Unit::TYPE_METERS);

  /**
  * Set the head pose for the frustum.
  *
  * @warning
  *   Setting the head pose on a frustum that has head tracking disabled will result in failure
  *   and have no effect on the frustum.
  *
  * @param[in] headPose
  *   The head pose in zspace::common::Matrix4 format.
  * @param[in] unitType
  *   The type of desired units.  Defaults to TYPE_METERS. Other types include TYPE_INCHES and TYPE_FEET.
  *
  * @return
  *   True for success, false for failure.
  */
  bool setHeadPose(const zspace::common::Matrix4& headPose, 
                   zspace::common::Unit::Type unitType = zspace::common::Unit::TYPE_METERS);

  /**
  * Get the head pose of the frustum.
  *
  * @warning
  *   Getting the head pose from a frustum that has head tracking disabled will result in failure.
  *
  * @param[out] headPose
  *   The current head pose of the frustum in zspace::common::Matrix4 format.
  * @param[in] unitType
  *   The desired type of units. Defaults to TYPE_METERS. Other types include TYPE_INCHES and TYPE_FEET.
  *
  * @return
  *   True for success, false for failure.
  */
  bool getHeadPose(zspace::common::Matrix4& headPose, 
                   zspace::common::Unit::Type unitType = zspace::common::Unit::TYPE_METERS);

  /**
  * Set whether or not head tracking is enabled.
  */
  void setHeadTrackingEnabled(bool isHeadTrackingEnabled);

  /**
  * Check whether or not head tracking is enabled.
  */
  bool isHeadTrackingEnabled() const;

  /**
  * Set whether or not head rotation is taken into account for head tracking.
  *
  * @remark
  *   More specifically, this denotes whether or not the head rotation component of
  *   the frustum's head pose is to be taken into account.
  */
  void setHeadRotationEnabled(bool isHeadRotationEnabled);

  /**
  * Check whether or not head rotation is taken into account for head tracking.
  */
  bool isHeadRotationEnabled() const; 

  /**
  * Get the view matrix for a specified frustum.
  *
  * @remark
  *   The view matrix is calculated from the off-axis projection and head + eye offset.
  *
  * @param[in] eye
  *   The right or left eye.
  * @param[out] viewMatrix
  *   The view matrix for a specified eye's frustum. This is in the zspace::common::Matrix4 format.
  */
  void getViewMatrix(StereoEye eye, zspace::common::Matrix4& viewMatrix);

  /**
  * Get the projection matrix for a specified frustum.
  *
  * @param[in] eye
  *   The right or left eye.
  * @param[out] projectionMatrix
  *   The projection matrix for a specified eye's frustum. This is in the zspace::common::Matrix4 format.
  */
  void getProjectionMatrix(StereoEye eye, zspace::common::Matrix4& projectionMatrix);

  /**
  * Get the position of the eye in the default head coordinate system
  *
  * @param[in] eye
  *   The right or left eye.
  *
  * @return
  *   A const reference to the position corresponding to a specified eye.
  */
  const zspace::common::Vector3& getEyePosition(StereoEye eye);

  /**
  * Get the bounds information for a specified frustum.
  *
  * @param[in] eye
  *   The right or left eye.
  *
  * @return
  *   A const reference to the bounding information corresponding to a specified eye's frustum.
  */
  const Bounds& getBounds(StereoEye eye);

private:
  // Parameters specified by the application.
  float                     m_interPupillaryDistance;   // inter-pupillary distance, in METERS
  float                     m_stereoLevel;              // stereo level: range 0.0 (no stereo) to 1.0 (full ipd accurate stereo)
  float                     m_worldScale;
  float                     m_fieldOfViewScale;
  float                     m_zeroParallaxOffset;

  float                     m_nearClip;                 // distance, in METERS, from Standard Head Position to near clip plane
  float                     m_farClip;                  // distance, in METERS, from Standard Head Position to far clip plane
  
  // Project matrices for left and right eyes.
  zspace::common::Matrix4   m_leftProjectionMatrix;
  zspace::common::Matrix4   m_rightProjectionMatrix;
  zspace::common::Matrix4   m_centerProjectionMatrix;

  // Frustum bounds for application viewport visibility tests.
  Bounds                    m_leftBounds;
  Bounds                    m_rightBounds;
  Bounds                    m_centerBounds;

  zspace::common::Vector2   m_viewportPosition;
  zspace::common::Vector2   m_viewportCenter;           // position, in METERS, of window center relative to Display center [H,V]
  zspace::common::Vector2   m_viewportSize;             // size, in METERS, of window [H,V]
  zspace::common::Vector2   m_viewportResolution;       // resolution, in pixels, of window [H,V]

  zspace::common::Vector2   m_displaySize;              // size, in METERS, of the physical Display [H,V]
  zspace::common::Vector2   m_displayResolution;        // size, in pixels, of the Display [H,V]
  zspace::common::Vector2   m_displayPosition;          // the virtual screen coordinates of the display.
  zspace::common::Vector3   m_displayOffset;            // offset, in METERS, of Display center from Standard Head Position [X,Y,Z]
  zspace::common::Vector2   m_displayAngle;             // angle, in DEGREES, of Display relative to Standard Head Coordinate System [X,Y]

  zspace::common::Vector3   m_headOffset;               // offset, in METERS, of head from Standard Head Position [X,Y,Z]
  zspace::common::Matrix4   m_headRotation;             // rotation of head relative to ZS coordinates
  bool                      m_isHeadTrackingEnabled;
  bool                      m_isHeadRotationEnabled;    // flag to use head rotation (from head pose) into frustum.

  // Set to dirty when any frustum parameter changes.
  bool                      m_isFrustumDirty;

  zspace::common::Matrix4   m_defaultHeadToDisplay;
  zspace::common::Matrix4   m_currentHeadToDefaultHeadLeft;
  zspace::common::Matrix4   m_currentHeadToDefaultHeadRight;
  zspace::common::Matrix4   m_currentHeadToDefaultHeadCenter;
  zspace::common::Vector3   m_leftEye;
  zspace::common::Vector3   m_rightEye;
  zspace::common::Vector3   m_centerEye;

private:
  // Only a StereoViewport object should create an object of this class, therefore
  // the constructor, copy constructor, and assignment operator are not supported for this API
  StereoFrustum();
  StereoFrustum(const StereoFrustum&);
  StereoFrustum& operator=(const StereoFrustum&);


  /**
  * Callback for when the frustum's viewport has been moved.
  *
  * @param[in] displaySizeX
  *   The width of the display that the viewport is currently on (in meters).
  * @param[in] displaySizeY
  *   The height of the display that the viewport is currently on (in meters).
  * @param[in] displayResX
  *   The horizontal resolution of the display (in pixels).
  * @param[in] displayResY
  *   The vertical resolution of the display (in pixels).
  */
  bool onViewportMove(float displaySizeX, float displaySizeY, int displayResX, int displayResY);

  /**
  * Callback for when the frustum's viewport has been resized.
  *
  * @param[in] width
  *   The viewport's width in pixels.
  * @param[in] height
  *   The viewport's height in pixels.
  */
  bool onViewportResize(int width, int height);

  /**
  * Update the position of the viewport.
  *
  * @remark
  *   This position is in absolute screen coordinates and represents
  *   the upper-left corner of the viewport.
  *
  * @param[in] x
  *   The viewport's x position in absolute screen coordinates.
  * @param[in] y
  *   The viewport's y position in absolute screen coordinates.
  */
  void setViewportPosition(int x, int y);

  /**
  * Set the virtual screen coordinate position of the display.
  *
  * @param[in] displayPosition
  *   The position of the display.
  */
  void setDisplayPosition(const zspace::common::Vector2& displayPosition);

  /**
  * Set the offset from the physical display.
  * 
  * @param[in] displayOffset
  *   The offset from the physical display.
  * @param[in] unitType
  *   The type of desired units. Defaults to TYPE_METERS. Other types include TYPE_INCHES and TYPE_FEET.
  *
  * @warning
  *   Display offsets equal to 0.0f will result in failure.
  *
  * @return
  *   True for success, false for failure.
  */
  bool setDisplayOffset(const zspace::common::Vector3& displayOffset, 
                        zspace::common::Unit::Type unitType = zspace::common::Unit::TYPE_METERS);

  /**
  * Set the angle of the physical display that the frustum's viewport is currently on.
  *
  * @param[in] displayAngle
  *   The angle of the physical display.
  *
  * @warning
  *   Display angles below -90 or above 90 degrees will result in failure.
  *
  * @return
  *   True for success, false for failure.
  */
  bool setDisplayAngle(const zspace::common::Vector2& displayAngle);

	bool setDisplayDimensions(HDC hdc);

	bool setDisplayDimensions(float displaySizeX, float displaySizeY, int displayResX, int displayResY);

  void calculateCoordinateSpaces();

  void calculateProjection(zspace::common::Vector3 eye, zspace::common::Matrix4& projection);

  void buildOffAxisFrustum(zspace::common::Matrix4 & frustum,
                           float l,     // Minimum x-value of the view volume.
                           float r,     // Maximum x-value of the view volume.
                           float b,     // Minimum y-value of the view volume.
                           float t,     // Maximum y-value of the view volume.
                           float zn,    // Minimum z-value of the view volume.
                           float zf);   // Maximum z-value of the view volume.
};

/** @} */   // End of group: Stereo

ZSPACE_STEREO_NS_END

#endif // __ZSPACE_STEREO_FRUSTUM_H__
