//CLASS DEF: Simplifies a lot of the basic head (movement) and stylus (movement + button inputs) tracking.
//USAGE: Initialize ObjectTracker
//		Call UpdateHead and UpdateStylus in the update loop of your program.

#include "ObjectTracker.h"
#include "OpenGlScene.h"
#include <sstream>
#include "ZSpace/Common/Math/MathConverterGl.h"

using namespace std;


ObjectTracker::ObjectTracker(zspace::stereo::StereoFrustum* stereoFrustum, zspace::stereo::StereoWindow* stereoWindow)
{
	trackerSystem = new zspace::tracker::TrackerSystem();
	this->stereoFrustum = stereoFrustum;
	this->stereoWindow = stereoWindow;
	
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


//DEF: Gets current tracking information on stylus, including pose, buttons pressed
//PARAMETERS: 
//	buttonStates: Array of boolean values, each indicating whether a particular button is pressed (true) or not (false)
//RETURNS:
//	true: Stylus found
//	false: Stylus not found
bool ObjectTracker::UpdateStylus(bool* buttonStates)
{
	bool stylusFound = false;
	// Update the tracking information.
	trackerSystem->captureTargets();
	stylus = trackerSystem->getDefaultTrackerTarget(zspace::tracker::TrackerTarget::TYPE_PRIMARY);

	if(stylus){
		stylusFound = true;
		/////////////////////////////////////////////////////////////
		//Check for any changes in button inputs via the stylus
		//buttonStates[i]:
		//		i = 0 -> large oval button
		//		i = 1 -> right button
		//		i = 2 -> left button
		zspace::tracker::ITrackerButtonCapability* trackerButtonCapability =
			static_cast<zspace::tracker::ITrackerButtonCapability*>(stylus->getCapability(zspace::tracker::ITrackerCapability::TYPE_BUTTON));

		if (trackerButtonCapability){
		    unsigned int numButtons = trackerButtonCapability->getNumButtons();

			buttonStates = new bool[numButtons];


			for (unsigned int i = 0; i < numButtons; ++i)
				buttonStates[i] = false;

			//Detect any changes in each buttons state.
			for (unsigned int buttonId = 0; buttonId < numButtons; ++buttonId)
			{
				//Check if the specified button is up or down.
				bool isButtonPressed = trackerButtonCapability->isButtonPressed(buttonId);

#ifdef DEBUG
				if (isButtonPressed && !buttonStates[buttonId]){

					char output[50];
					sprintf(output, "Button %d Pressed\n", buttonId);
					OutputDebugString(output);
				}
#endif

				//Store off the current button state into the previous button states array
				//so that it can be referenced in the next frame.
				buttonStates[buttonId] = isButtonPressed;
			}	
		}

		////////////////////////////////////////
		//Update the pose of the stylus
		//

		primaryPose      = zspace::common::Matrix4::IDENTITY();
		zspace::common::Matrix4 primaryWorldPose = zspace::common::Matrix4::IDENTITY();
		
		stylus->getPose(primaryPose);
		primaryPose.getTrans().x;
		string str;
		ostringstream ostr;
		//ostr << "Translation: x: " << primaryPose.getTrans().x << " y: " << primaryPose.getTrans().y << "z: " << primaryPose.getTrans().z << "\n";
		//str = ostr.str();
		//OpenGlScene::updateTranslation(primaryPose.getTrans().x, primaryPose.getTrans().y, primaryPose.getTrans().z);

		//OutputDebugString(str.c_str());
		convertPoseToWorldSpace(primaryPose, primaryWorldPose);

		//Get the position of stylus tip, and direction of stylus is pointing

		stylusHeadPos = primaryPose.getTrans();
		stylusWorldPos = primaryWorldPose.getTrans();
		stylusWorldDir = zspace::common::Vector3(-primaryWorldPose[0][2], -primaryWorldPose[1][2], -primaryWorldPose[2][2]);
		stylusWorldQuat = primaryWorldPose.extractQuaternion();
		stylusWorldQuat.ToAngleAxis(stylusWorldRotAngle, stylusWorldRotAxis);

		//###################################################
		//OpenGlScene::setStylusTransform(primaryWorldPose);
		OpenGlScene::setStylusTransform(primaryPose);
	}

	return stylusFound;
}



///THE FOLLOWING CODE NEEDS TO BE CLEANED UP


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

ObjectTracker::~ObjectTracker(void)
{
	trackerSystem->removeReference();
	/*if(head)
		head->removeReference();
	if(stylus)
		stylus->removeReference();*/
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
  displayWorldPose = worldPose;
  //OpenGlScene::updateTranslation(displayWorldPose.getTrans().x, displayWorldPose.getTrans().y, displayWorldPose.getTrans().z);
}
