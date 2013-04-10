#ifndef VICONDATASTREAMCLIENT_H
#define VICONDATASTREAMCLIENT_H

#include "Client.h"
#include <iostream>
#include <time.h>
#include <process.h> 
#include <Vector>
#include <winsock.h>
//#include <EbonGL.hpp>
#include "../MathHelper.hpp" 
#include "../Model/Tablet.h"
#include "../Model/Finger.h"


class ViconDataStreamClient{
public:
	void createViconClient();
	void getViconStream();
	void disconnectVicon();
	static  unsigned int __stdcall ListenViconWrapper(void*c);

    MHTypes::Point3D getObjectPosition(std::string SubjectName);
    void getTabletPosition(std::string SubjectName, Tablet& tablet);
	void getTabletRotation(std::string SubjectName, Tablet& tablet);

	void getFingerPosition(std::string SubjectName, Finger& finger);
    //MHTypes::Point3D getObjectHoloPosition(std::string SubjectName);
    MHTypes::EulerAngle getObjectRotation(std::string SubjectName);
    MHTypes::Quaternion getObjectRotationQuaternion(std::string SubjectName);

private:
	// Make a new client
	ViconDataStreamSDK::CPP::Client *MyClient;

    MHTypes::Point3D getObjectMarker(ViconDataStreamSDK::CPP::Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation);

    MHTypes::Point3D position;

	MHTypes::Point3D finger_pos;

    // Markers of the mobile device
    MHTypes::Point3D top_left;
    MHTypes::Point3D top_right;
    MHTypes::Point3D bottom_left;
    MHTypes::Point3D bottom_right;

    MHTypes::Point3D virtual_top_right;
    MHTypes::Point3D holo_position;

    MHTypes::EulerAngle tablet_rotation_angle;
    MHTypes::Quaternion tablet_rotation_quaternion;

    //static std::vector<double> tempCoords;
	static std::vector<double> wandCoords;
	//void parse3DData();

};

#endif