#ifdef EG_ZSPACE_MODE
//CLASS DEF: Handles a lot of the basic head (movement) and stylus (movement + button inputs) tracking.
//USAGE: Instantiate ObjectTracker
//		Call UpdateHead and UpdateStylus in the update loop of your program.

#include "ObjectTracker.h"
#include <sstream>
#include "ZSpace/Common/Math/MathConverterGl.h"

using namespace std;

//## Constructor ##
ObjectTracker::ObjectTracker(zspace::stereo::StereoFrustum* stereoFrustum, zspace::stereo::StereoWindow* stereoWindow)
{
	trackerSystem = new zspace::tracker::TrackerSystem();
	this->stereoFrustum = stereoFrustum;
	this->stereoWindow = stereoWindow;
	
}
//## Destructor ##
ObjectTracker::~ObjectTracker(void)
{
	trackerSystem->removeReference();
	if(head)
		head->removeReference();
	if(stylus)
		stylus->removeReference();
}

//DEF: Gets current pose of head; forwards that information to viewing frustum.
//PARAMETERS:
//	stereoFrustum: The viewing frustum to update based on new head pose.
//RETURNS:
//	true: Head found
//	false: Head not found
bool ObjectTracker::UpdateHead()
{
	bool headFound = false;
	// Update the tracking information.
	trackerSystem->captureTargets();

	// Grab the latest 6DOF head pose (position and orientation) and pass it off
	// to the zSpace StereoFrustum.  This allows the StereoFrustum to take the head 
	// position/rotation into account when calculating the left and right eye
	// view/projection matrices.
	head = trackerSystem->getDefaultTrackerTarget(zspace::tracker::TrackerTarget::TYPE_HEAD);

	// Check to make sure the head target is not NULL.  If the head target is NULL,
	// this signifies that the default head target cannot be found.
	if (head)
	{
		headFound = true;
		zspace::common::Matrix4 headPose = zspace::common::Matrix4::IDENTITY();
		head->getPose(headPose);
		stereoFrustum->setHeadPose(headPose);
	}

	return headFound;
}

bool ObjectTracker::updateStylus(zspace::common::Matrix4 &stylusPose)
{
	bool stylusFound = false;

	 zspace::tracker::TrackerTarget* primaryTarget = trackerSystem->getDefaultTrackerTarget(zspace::tracker::TrackerTarget::TYPE_PRIMARY);

	  if (primaryTarget)
	  {
		zspace::common::Matrix4 primaryPose      = zspace::common::Matrix4::IDENTITY();
		zspace::common::Matrix4 primaryWorldPose = zspace::common::Matrix4::IDENTITY();
    
		primaryTarget->getPose(primaryPose);
		convertPoseToWorldSpace(primaryPose, primaryWorldPose);

		stylusPose = primaryWorldPose;

		//OpenGlScene::setStylusTransform(primaryWorldPose);
	  }

	return stylusFound;
}

MHTypes::Point3D ObjectTracker::getStylusPos()
{
	return MHTypes::Point3D(primaryPose.getTrans().x, primaryPose.getTrans().y, primaryPose.getTrans().z);
}

MHTypes::Point3D ObjectTracker::getStylusDispPos()
{
	return MHTypes::Point3D(displayWorldPose.getTrans().x, displayWorldPose.getTrans().y, displayWorldPose.getTrans().z -0.25f);
}

//Gets the position along the facing ray of the stylus
MHTypes::Point3D ObjectTracker::getStylusRayPos()
{
	float posnAlongRay = 0.2f; //How far down the ray the point is
	return MHTypes::Point3D(stylusWorldPos.x + stylusWorldDir.x*posnAlongRay, stylusWorldPos.y + stylusWorldDir.y*posnAlongRay, stylusWorldPos.z + stylusWorldDir.z*posnAlongRay);
}

//Gets the translation from the tip of stylus down ray
MHTypes::Point3D ObjectTracker::getTransFromStylus()
{
	float posnAlongRay = 0.2f; //How far down the ray the point is
	return MHTypes::Point3D(stylusWorldDir.x*posnAlongRay, stylusWorldDir.y *posnAlongRay, stylusWorldDir.z * posnAlongRay);

}

//Gets position of tip of stylus in display space
MHTypes::Point3D ObjectTracker::getStylusTipPos()
{
	return MHTypes::Point3D(stylusWorldPos.x, stylusWorldPos.y, stylusWorldPos.z);
}

zspace::common::Radian ObjectTracker::getStylusRotAngle()
{
	return stylusWorldRotAngle;
}

zspace::common::Vector3 ObjectTracker::getStylusRotAxis()
{
	return stylusWorldRotAxis;
}

void ObjectTracker::convertPoseToWorldSpace(const zspace::common::Matrix4& pose, zspace::common::Matrix4& worldPose)
{
  GLfloat monoModelViewGl[16];
  zspace::common::Matrix4 cameraLocalToWorld   = zspace::common::Matrix4::IDENTITY();
  zspace::common::Matrix4 trackerToCameraSpace = zspace::common::Matrix4::IDENTITY();
  zspace::common::Vector3 viewportOffset       = zspace::common::Vector3::ZERO();

  // Grab the mono (non-stereoscopic) model-view matrix in OpenGl format.
  glMatrixMode(GL_MODELVIEW);
  glGetFloatv(GL_MODELVIEW_MATRIX, monoModelViewGl);

  // Convert the OpenGl model-view matrix to the zSpace Matrix4 format and
  // invert it to obtain the non-stereo camera's matrix.
  zspace::common::MathConverterGl::convertMatrixGlToMatrix4(monoModelViewGl, cameraLocalToWorld);
  cameraLocalToWorld = cameraLocalToWorld.inverse();

  // Grab the display that the zSpace StereoWindow is currently on in order to 
  // calculate the tracker-to-camera space transform as well as the viewport offset.
  // Both the tracker-to-camera space transform and viewport offset are required
  // for transforming a tracker space pose into the application's world space.
  const zspace::common::DisplayInfo::Display* display = stereoWindow->getCurrentDisplay();

  if (display)
  {
    trackerToCameraSpace = zspace::common::DisplayInfo::getTrackerToCameraSpaceTransform(display);
    viewportOffset       = zspace::common::DisplayInfo::getViewportOffset(display,
                                                                          stereoWindow->getX(),
                                                                          stereoWindow->getY(),
                                                                          stereoWindow->getWidth(),
                                                                          stereoWindow->getHeight());
  }

  worldPose = cameraLocalToWorld * trackerToCameraSpace * zspace::common::Matrix4::getTrans(-viewportOffset) * pose;
}


#endif