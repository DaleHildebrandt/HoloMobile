#pragma once


#include "ZSpace/Common/Math/Math.h"
#include "ZSpace/Tracker/TrackerSystem.h"
#include "ZSpace/Tracker/TrackerTarget.h"
#include "ZSpace/Tracker/ITrackerButtonCapability.h"


#include "ZSpace/Stereo/StereoFrustum.h"
#include "ZSpace/Stereo/StereoWindow.h"
#include "MathHelper.hpp"

#define DEBUG true
class ObjectTracker
{
public:
	//ObjectTracker(void);
	ObjectTracker(zspace::stereo::StereoFrustum* stereoFrustum, zspace::stereo::StereoWindow* stereoWindow);
	bool UpdateHead();
	bool UpdateStylus(bool* buttonStates);	
	MHTypes::Point3D getStylusPos();
	~ObjectTracker(void);
	MHTypes::Point3D ObjectTracker::getStylusDispPos();
	MHTypes::Point3D ObjectTracker::getStylusRayPos();
	zspace::common::Radian ObjectTracker::getStylusRotAngle();
	zspace::common::Vector3 ObjectTracker::getStylusRotAxis();
	MHTypes::Point3D ObjectTracker::getTransFromStylus();
	MHTypes::Point3D ObjectTracker::getStylusTipPos();


private:
	zspace::tracker::TrackerSystem* trackerSystem;
	zspace::tracker::TrackerTarget* stylus;
	zspace::tracker::TrackerTarget* head;
	zspace::stereo::StereoWindow* stereoWindow;
	zspace::stereo::StereoFrustum* stereoFrustum;
	zspace::common::Matrix4 primaryPose;
	zspace::common::Matrix4 displayWorldPose;
	zspace::common::Vector3 stylusWorldPos; //Position of stylus tip in display space
	zspace::common::Vector3 stylusWorldDir;	//Direction of ray from stylus tip in display space
	zspace::common::Quaternion stylusWorldQuat; //Quaternion for rotation
	zspace::common::Radian stylusWorldRotAngle; //Angle of Rotation
	zspace::common::Vector3 stylusWorldRotAxis; //Axis of Rotation

	zspace::common::Vector3 stylusHeadPos; //Position of stylus tip in head space


	
	void convertPoseToWorldSpace(const zspace::common::Matrix4& pose, zspace::common::Matrix4& worldPose);
	
	
};

