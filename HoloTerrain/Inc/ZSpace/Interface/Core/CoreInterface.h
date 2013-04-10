//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
//
//  File:       CoreInterface.h
//  Content:    The generic interface to the zSpace core APIs.
//  SVN Info:   $Id$
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ZSPACE_CORE_INTERFACE_H__
#define __ZSPACE_CORE_INTERFACE_H__

// Dll import/exports or static API.
#if (defined(_LIB) || defined(ZSPACE_STATIC) || defined(ZSPACE_CORE_INTERFACE_STATIC))
#define ZSPACE_CORE_INTERFACE_API
#elif defined(ZSPACE_CORE_INTERFACE_EXPORTS)
#define ZSPACE_CORE_INTERFACE_API __declspec(dllexport)
#else
#define ZSPACE_CORE_INTERFACE_API __declspec(dllimport)
#endif


typedef enum
{
  ZSCI_STEREO_EYE_UNKNOWN = -1,
  ZSCI_STEREO_EYE_LEFT    = 0,
  ZSCI_STEREO_EYE_RIGHT,
  ZSCI_STEREO_EYE_CENTER,
} zsci_stereo_eye;

typedef enum
{
  ZSCI_TRACKER_TARGET_TYPE_UNKNOWN    = -1,
  ZSCI_TRACKER_TARGET_TYPE_HEAD       = 0,
  ZSCI_TRACKER_TARGET_TYPE_PRIMARY,
  ZSCI_TRACKER_TARGET_TYPE_SECONDARY,
} zsci_tracker_target_type;

typedef enum
{
  ZSCI_RENDERER_TYPE_UNKNOWN    = -1,
  ZSCI_RENDERER_TYPE_GL         = 0,
  ZSCI_RENDERER_TYPE_DIRECT3D9,
  ZSCI_RENDERER_TYPE_DIRECT3D10,
  ZSCI_RENDERER_TYPE_DIRECT3D11,
} zsci_renderer_type;


/**
* @addtogroup Interface
* @{
*
*   @addtogroup Core
*   @{
*/

// Only need to export C interface if used by C++ source code.
#ifdef __cplusplus
extern "C" {
#endif

/**
* Check whether or not the zSpace SDK is installed.
*
* @return
*   1 if the zSpace SDK is installed, 0 if not.
*/
ZSPACE_CORE_INTERFACE_API int   zsci_isSdkInstalled();

/**
* Initialize the zSpace core interface.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_initialize();

/**
* Update all of the stereo and tracker information.
*
* @remark
*   This should be called once per frame on the main application's rendering thread.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_update();

/**
* Shutdown the zSpace core interface.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_shutdown();

/**
* Initialize left/right frame detection.
*
* @param[in] rendererType
*   The type of the underlying renderer used in the main application.
* @param[in] reserved
*   A reserved argument depending on the zsci_renderer_type:\n\n
*     ZSCI_RENDERER_TYPE_GL - NULL\n
*     ZSCI_RENDERER_TYPE_DIRECT3D9 - The user should pass in a IDirect3DDevice9*\n
*     ZSCI_RENDERER_TYPE_DIRECT3D10 - The user should pass in a IDXGISwapChain*\n
*     ZSCI_RENDERER_TYPE_DIRECT3D11 - The user should pass in a IDXGISwapChain*
*/
ZSPACE_CORE_INTERFACE_API void  zsci_initializeLeftRightDetection(zsci_renderer_type rendererType, void* reserved = 0);

/**
* Get the display offset of the current display.
*
* @param[out] displayOffset
*   The display offset (in meters): [x, y, z]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getDisplayOffset(float* displayOffset/*[3]*/);

/**
* Get the position of the current display.
*
* @param[out] displayPosition
*   The display position (in pixels): [x, y]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getDisplayPosition(int* displayPosition/*[2]*/);

/**
* Get the angle of the current display.
*
* @param[out] displayAngle
*   The display angle (in degrees): [X angle, Y angle]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getDisplayAngle(float* displayAngle/*[2]*/);

/**
* Get the resolution of the current display.
*
* @param[out] displayResolution
*   The display resolution (in pixels): [x, y]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getDisplayResolution(int* displayResolution/*[2]*/);

/**
* Get the size of the current display.
*
* @param[out] displaySize
*   The display size (in meters): [width, height]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getDisplaySize(float* displaySize/*[2]*/);

/**
* Specify the screen position of the main application window.
*
* @param[in] x
*   The application window's left-most absolute screen coordinate.
* @param[in] y
*   The application window's top-most absolute screen coordinate.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setWindowPosition(int x, int y);

/**
* Get the window's left-most absolute screen coordinate.
*/
ZSPACE_CORE_INTERFACE_API int   zsci_getWindowX();

/**
* Get the window's top-most absolute screen coordinate.
*/
ZSPACE_CORE_INTERFACE_API int   zsci_getWindowY();

/**
* Specify the size of the main application window.
*
* @param[in] width
*   The width of the window (in pixels).
* @param[in] height
*   The height of the window (in pixels).
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setWindowSize(unsigned int width, unsigned int height);

/**
* Get the window's width.
*/
ZSPACE_CORE_INTERFACE_API unsigned int zsci_getWindowWidth();

/**
* Get the window's height.
*/
ZSPACE_CORE_INTERFACE_API unsigned int zsci_getWindowHeight();

/**
* Set the inter-pupillary distance.
*
* @param[in] interPupillaryDistance
*   The inter-pupillary distance (physical separation between the eyes) in meters.
*
* @warning
*   Inter-pupillary distances below 0.0f will result in failure and have 
*   no effect on the frustum.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setInterPupillaryDistance(float interPupillaryDistance);

/**
* Get the inter-pupillary distance.  This is the physical separation between the eyes.
*
* @return
*   The inter-pupillary distance (in meters).
*/
ZSPACE_CORE_INTERFACE_API float zsci_getInterPupillaryDistance();

/**
* Set the stereo level.
*
* @param[in] stereoLevel
*   The stereo level. A stereo level of 1.0f is represents
*   full stereo, 0.0f represents no stereo.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setStereoLevel(float stereoLevel);

/**
* Get the stereo level.
*/
ZSPACE_CORE_INTERFACE_API float zsci_getStereoLevel();

/**
* Set the world scale.
*
* @param[in] worldScale
*   The world scale.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setWorldScale(float worldScale);

/**
* Get the world scale.
*/
ZSPACE_CORE_INTERFACE_API float zsci_getWorldScale();

/**
* Set the field of view scale.
*
* @param[in] fieldOfViewScale
*   The field of view scale.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setFieldOfViewScale(float fieldOfViewScale);

/**
* Get the field of view scale.
*/
ZSPACE_CORE_INTERFACE_API float zsci_getFieldOfViewScale();

/**
* Set the zero parallax offset.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setZeroParallaxOffset(float zeroParallaxOffset);

/**
* Get the zero parallax offset.
*/
ZSPACE_CORE_INTERFACE_API float zsci_getZeroParallaxOffset();

/**
* Set the near clip distance.
*
* @param[in] nearClip
*   The near clip distance (in meters).
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setNearClip(float nearClip);

/**
* Get the near clip distance in meters.
*/
ZSPACE_CORE_INTERFACE_API float zsci_getNearClip();

/**
* Set the far clip distance.
*
* @param[in] farClip
*   The far clip distance (in meters).
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setFarClip(float farClip);

/**
* Get the far clip distance in meters.
*/
ZSPACE_CORE_INTERFACE_API float zsci_getFarClip();

/**
* Set whether or not head tracking is enabled.
*
* @param[in] isHeadTrackingEnabled
*   1 to enable head tracking, 0 to disable it.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setHeadTrackingEnabled(int isHeadTrackingEnabled);

/**
* Check whether or not head tracking is enabled.
*
* @return
*   1 if head tracking is enabled, 0 if not.
*/
ZSPACE_CORE_INTERFACE_API int   zsci_isHeadTrackingEnabled();

/**
* Get the view matrix for a specified eye.
*
* @param[in] stereoEye
*   The eye: ZSCI_STEREO_EYE_LEFT, ZSCI_STEREO_EYE_RIGHT, or ZSCI_STEREO_EYE_CENTER.
* @param[out] viewMatrix
*   The view matrix corresponding to the specified eye.  This is laid out in 
*   column major (OpenGl) format:\n\n
*   [m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getViewMatrix(zsci_stereo_eye stereoEye, float* viewMatrix/*[16]*/);

/**
* Get the projection matrix for a specified eye.
*
* @param[in] stereoEye
*   The eye: ZSCI_STEREO_EYE_LEFT, ZSCI_STEREO_EYE_RIGHT, or ZSCI_STEREO_EYE_CENTER.
* @param[out] projectionMatrix
*   The projection matrix corresponding to the specified eye.  This is laid out in 
*   column major (OpenGl) format:\n\n
*   [m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getProjectionMatrix(zsci_stereo_eye stereoEye, float* projectionMatrix/*[16]*/);

/**
* Get the position of a specified eye.
*
* @param[in] stereoEye
*   The eye: ZSCI_STEREO_EYE_LEFT, ZSCI_STEREO_EYE_RIGHT, or ZSCI_STEREO_EYE_CENTER.
* @param[out] eyePosition
*   The position of the eye: [x, y, z]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getEyePosition(zsci_stereo_eye stereoEye, float* eyePosition/*[3]*/);

/**
* Get the frustum bounds for a specified eye.
*
* @param[in] stereoEye
*   The eye: ZSCI_STEREO_EYE_LEFT, ZSCI_STEREO_EYE_RIGHT, or ZSCI_STEREO_EYE_CENTER.
* @param[out] frustumBounds
*   The frustum bounds corresponding to a specified eye laid out as follows:\n\n
*   [left, right, bottom, top, nearClip, farClip]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getFrustumBounds(zsci_stereo_eye stereoEye, float* frustumBounds/*[6]*/);

/**
* Check whether or not a TrackerTarget of a specified type is enabled.
*
* @param[in] trackerTargetType
*   The type of the TrackerTarget.
*
* @return
*   1 if the TrackerTarget is enabled, 0 if not.
*/
ZSPACE_CORE_INTERFACE_API int   zsci_isTrackerTargetEnabled(zsci_tracker_target_type trackerTargetType);

/**
* Get the pose of a default TrackerTarget based on a specified target type.
*
* @param[in] trackerTargetType
*   The type of TrackerTarget.
* @param[out] pose
*   The TrackerTarget's pose (position/orientation).  This is laid out in 
*   column major (OpenGl) format:\n\n
*   [m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getTrackerTargetPose(zsci_tracker_target_type trackerTargetType, float* pose/*[16]*/);

/**
* Get the number of buttons available to a TrackerTarget.
*
* @param[in] trackerTargetType
*   The type of the TrackerTarget.
*
* @return
*   The number of buttons the TrackerTarget has.
*/
ZSPACE_CORE_INTERFACE_API unsigned int zsci_getNumTrackerTargetButtons(zsci_tracker_target_type trackerTargetType);

/**
* Check whether or not a TrackerTarget's button is pressed.
*
* @param[in] trackerTargetType
*   The type of the TrackerTarget.
* @param[in] buttonId
*   The id of the button.
*
* @return
*   1 if the button is pressed.  0 otherwise.
*/
ZSPACE_CORE_INTERFACE_API int   zsci_isTrackerTargetButtonPressed(zsci_tracker_target_type trackerTargetType, unsigned int buttonId);

/**
* Set whether or not the mouse simulation is enabled.
*
* @remark
*   Mouse simulation will be driven by the primary TrackerTarget,
*   which defaults to the stylus.
*
* @param[in] isMouseSimulationEnabled
*   Whether or not mouse simulation is enabled. 1 to enable simulation,
*   0 to disable it.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setMouseSimulationEnabled(int isMouseSimulationEnabled);

/**
* Set the maximum distance the primary TrackerTarget can be from the display 
* in order to simulate the mouse.
*
* @param[in] mouseSimulationDistance
*   The maximum distance (in meters) the primary TrackerTarget can be from the 
*   display.  Negative values denote an infinite distance.
*/
ZSPACE_CORE_INTERFACE_API void  zsci_setMouseSimulationDistance(float mouseSimulationDistance);

/**
* Get the relative offset to apply to a TrackerTarget's pose when the window/viewport is
* not the full size of the display.
*
* @param[out] viewportOffsetTransform
*   A relative translational offset to apply when transforming the TrackerTarget's pose
*   to world space.  This is laid out in column major (OpenGl) format:\n\n
*   [m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getViewportOffsetTransform(float* viewportOffsetTransform/*[16]*/);

/**
* Get the matrix that represents the transform from tracker space 
* to camera space.
*
* @param[out] trackerToCameraSpaceTransform
*   The matrix the transform from tracker space to camera space.  This is laid out in 
*   column major (OpenGl) format:\n\n
*   [m00, m10, m20, m30, m01, m11, m21, m31, m02, m12, m22, m32, m03, m13, m23, m33]
*/
ZSPACE_CORE_INTERFACE_API void  zsci_getTrackerToCameraSpaceTransform(float* trackerToCameraSpaceTransform/*[16]*/);

#ifdef __cplusplus
}
#endif

/** @} */   // End of group: Core
/** @} */   // End of group: Interface

#endif // __ZSPACE_CORE_INTERFACE_H__