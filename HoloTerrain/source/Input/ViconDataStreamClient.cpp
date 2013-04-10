///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) OMG Plc 2009.
// All rights reserved.  This software is protected by copyright
// law and international treaties.  No part of this software / document
// may be reproduced or distributed in any form or by any means,
// whether transiently or incidentally to some other use of this software,
// without the written permission of the copyright owner.
//
///////////////////////////////////////////////////////////////////////////////
#include "Client.h"
#include "ViconDataStreamClient.h"
#include <sstream>
#include <Windows.h>

using namespace ViconDataStreamSDK::CPP;


CRITICAL_SECTION m_cs2;
//std::vector<double> ViconDataStreamClient::tempCoords;
std::vector<double> ViconDataStreamClient::wandCoords;
bool outputOn = false;

namespace
{
    std::string Adapt( const bool i_Value )
    {
        return i_Value ? "True" : "False";
    }

    std::string Adapt( const Direction::Enum i_Direction )
    {
        switch( i_Direction )
        {
        case Direction::Forward:
            return "Forward";
        case Direction::Backward:
            return "Backward";
        case Direction::Left:
            return "Left";
        case Direction::Right:
            return "Right";
        case Direction::Up:
            return "Up";
        case Direction::Down:
            return "Down";
        default:
            return "Unknown";
        }
    }

    std::string Adapt( const DeviceType::Enum i_DeviceType )
    {
        switch( i_DeviceType )
        {
        case DeviceType::ForcePlate:
            return "ForcePlate";
        case DeviceType::Unknown:
        default:
            return "Unknown";
        }
    }

    std::string Adapt( const Unit::Enum i_Unit )
    {
        switch( i_Unit )
        {
        case Unit::Meter:
            return "Meter";
        case Unit::Volt:
            return "Volt";
        case Unit::NewtonMeter:
            return "NewtonMeter";
        case Unit::Newton:
            return "Newton";
        case Unit::Unknown:
        default:
            return "Unknown";
        }
    }
}

void ViconDataStreamClient::createViconClient(){

    //Initilize the critical section
    InitializeCriticalSection(&m_cs2);

 
	//Sets the IP address of the PC (running Vicon) to connect to
	//std::string HostName = "localhost:801";
    std::string HostName = "130.179.30.171:801";
    MyClient = new Client();

    // Connect to a server
    std::cout << "Connecting to " << HostName << " ..." << std::flush;
    while( !MyClient->IsConnected().Connected )
    {
        // Direct connection
        MyClient->Connect( HostName );

        // Multicast connection
        // MyClient->ConnectToMulticast( HostName, "224.0.0.0" );

        std::cout << ".";
        //Sleep(1);
    }
    std::cout << std::endl;

    // Enable some different data types
    MyClient->EnableSegmentData();
    MyClient->EnableMarkerData();
    MyClient->EnableUnlabeledMarkerData();
    MyClient->EnableDeviceData();

    std::cout << "Segment Data Enabled: "          << Adapt( MyClient->IsSegmentDataEnabled().Enabled )         << std::endl;
    std::cout << "Marker Data Enabled: "           << Adapt( MyClient->IsMarkerDataEnabled().Enabled )          << std::endl;
    std::cout << "Unlabeled Marker Data Enabled: " << Adapt( MyClient->IsUnlabeledMarkerDataEnabled().Enabled ) << std::endl;
    std::cout << "Device Data Enabled: "           << Adapt( MyClient->IsDeviceDataEnabled().Enabled )          << std::endl;

    // Set the streaming mode
    MyClient->SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPull );
    // MyClient->SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ClientPullPreFetch );
    // MyClient->SetStreamMode( ViconDataStreamSDK::CPP::StreamMode::ServerPush );

    //#### Set the global up axis
	/*MyClient->SetAxisMapping( Direction::Right, 
                            Direction::Forward, 
                            Direction::Up );*/ // Y-up
	//Correct
	MyClient->SetAxisMapping( Direction::Right, 
                            Direction::Up, 
                            Direction::Backward);


    Output_GetAxisMapping _Output_GetAxisMapping = MyClient->GetAxisMapping();
    std::cout << "Axis Mapping: X-" << Adapt( _Output_GetAxisMapping.XAxis ) 
        << " Y-" << Adapt( _Output_GetAxisMapping.YAxis ) 
        << " Z-" << Adapt( _Output_GetAxisMapping.ZAxis ) << std::endl;

    // Discover the version number
    Output_GetVersion _Output_GetVersion = MyClient->GetVersion();
    std::cout << "Version: " << _Output_GetVersion.Major << "." 
        << _Output_GetVersion.Minor << "." 
        << _Output_GetVersion.Point << std::endl;

}

void ViconDataStreamClient::getViconStream()
{
    while(true){
        if (outputOn){
            // Get a frame
            std::cout << "Waiting for new frame...";
        }

        while( MyClient->GetFrame().Result != Result::Success )
        {
            //sleep(1);
            //std::cout << ".";
        }
        //std::cout << std::endl;

        // Get the frame number
        Output_GetFrameNumber _Output_GetFrameNumber = MyClient->GetFrameNumber();
        //std::cout << "Frame Number: " << _Output_GetFrameNumber.FrameNumber << std::endl;

        // Get the timecode
        Output_GetTimecode _Output_GetTimecode  = MyClient->GetTimecode();
		//std::cout << "Timecode: " << _Output_GetTimecode.Seconds << " seconds" << std::endl;

        if (outputOn){
            std::cout << "Timecode: "
                << _Output_GetTimecode.Hours               << "h "
                << _Output_GetTimecode.Minutes             << "m " 
                << _Output_GetTimecode.Seconds             << "s "
                << _Output_GetTimecode.Frames              << "f "
                << _Output_GetTimecode.SubFrame            << "sf "
                << Adapt( _Output_GetTimecode.FieldFlag ) << " " 
                << _Output_GetTimecode.Standard            << " " 
                << _Output_GetTimecode.SubFramesPerFrame   << " " 
                << _Output_GetTimecode.UserBits            << std::endl << std::endl;

        }

        // Get the latency
        //std::cout << "Latency: " << MyClient->GetLatencyTotal().Total << "s" << std::endl;


        if (outputOn){
            for( unsigned int LatencySampleIndex = 0 ; LatencySampleIndex < MyClient->GetLatencySampleCount().Count ; ++LatencySampleIndex )
            {
                std::string SampleName  = MyClient->GetLatencySampleName( LatencySampleIndex ).Name;
                double      SampleValue = MyClient->GetLatencySampleValue( SampleName ).Value;

                std::cout << "  " << SampleName << " " << SampleValue << "s" << std::endl;
            }
            std::cout << std::endl;
        }

        // Lock the Critical section when recording subjects
        EnterCriticalSection(&m_cs2);

        // Count the number of subjects
        unsigned int SubjectCount = MyClient->GetSubjectCount().SubjectCount;
        if (outputOn)
            std::cout << "Subjects (" << SubjectCount << "):" << std::endl;
        for( unsigned int SubjectIndex = 0 ; SubjectIndex < SubjectCount ; ++SubjectIndex )
        {
            if (outputOn)
                std::cout << "  Subject #" << SubjectIndex << std::endl;

            // Get the subject name
            std::string SubjectName = MyClient->GetSubjectName( SubjectIndex ).SubjectName;
            if (outputOn)
                std::cout << "            Name: " << SubjectName << std::endl;

            // Get the root segment
            std::string RootSegment = MyClient->GetSubjectRootSegmentName( SubjectName ).SegmentName;
            if (outputOn)
                std::cout << "    Root Segment: " << RootSegment << std::endl;

            // Count the number of segments
            unsigned int SegmentCount = MyClient->GetSegmentCount( SubjectName ).SegmentCount;
            if (outputOn)
                std::cout << "    Segments (" << SegmentCount << "):" << std::endl;

            for( unsigned int SegmentIndex = 0 ; SegmentIndex < SegmentCount ; ++SegmentIndex )
            {	
                if (outputOn)
                    std::cout << "      Segment #" << SegmentIndex << std::endl;

                // Get the segment name
                std::string SegmentName = MyClient->GetSegmentName( SubjectName, SegmentIndex ).SegmentName;
                if (outputOn)
                    std::cout << "          Name: " << SegmentName << std::endl;

                // Get the segment parent
                std::string SegmentParentName = MyClient->GetSegmentParentName( SubjectName, SegmentName ).SegmentName;
                if (outputOn)
                    std::cout << "        Parent: " << SegmentParentName << std::endl;

                // Get the segment's children
                unsigned int ChildCount = MyClient->GetSegmentChildCount( SubjectName, SegmentName ).SegmentCount;
                if (outputOn)
                    std::cout << "     Children (" << ChildCount << "):" << std::endl;
                for( unsigned int ChildIndex = 0 ; ChildIndex < ChildCount ; ++ChildIndex )
                {
                    std::string ChildName = MyClient->GetSegmentChildName( SubjectName, SegmentName, ChildIndex ).SegmentName;
                    if (outputOn)
                        std::cout << "       " << ChildName << std::endl;
                }

            }

            // Count the number of markers
            unsigned int MarkerCount = MyClient->GetMarkerCount( SubjectName ).MarkerCount;
            if (outputOn)
                std::cout << "    Markers (" << MarkerCount << "):" << std::endl;
            for( unsigned int MarkerIndex = 0 ; MarkerIndex < MarkerCount ; ++MarkerIndex )
            {
                // Get the marker name
                std::string MarkerName = MyClient->GetMarkerName( SubjectName, MarkerIndex ).MarkerName;

                // Get the marker parent
                std::string MarkerParentName = MyClient->GetMarkerParentName( SubjectName, MarkerName ).SegmentName;

                // Get the global marker translation
                Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation =
                    MyClient->GetMarkerGlobalTranslation( SubjectName, MarkerName );

                if (outputOn){
                    std::cout << "      Marker #" << MarkerIndex            << ": "
                        << MarkerName             << " ("
                        << _Output_GetMarkerGlobalTranslation.Translation[ 0 ]  << ", "
                        << _Output_GetMarkerGlobalTranslation.Translation[ 1 ]  << ", "
                        << _Output_GetMarkerGlobalTranslation.Translation[ 2 ]  << ") "
                        << Adapt( _Output_GetMarkerGlobalTranslation.Occluded ) << std::endl;
                }

                /*if ((MarkerIndex == 1) && (SubjectIndex == 1)) {


                // We clear the vector of coords
                wandCoords.clear();
                // Third while to get each element
                for(int i=0; i<3; i++)
                {
                wandCoords.push_back(_Output_GetMarkerGlobalTranslation.Translation[ i ]);
                }
                /*  tempCoords[0]=_Output_GetMarkerGlobalTranslation.Translation[ 0 ];
                tempCoords[1]=_Output_GetMarkerGlobalTranslation.Translation[ 1 ];
                tempCoords[2]=_Output_GetMarkerGlobalTranslation.Translation[ 2 ];

                }*/

            }

        }

        //Release the Critical section
        LeaveCriticalSection(&m_cs2);


    }
}


void ViconDataStreamClient::disconnectVicon(){
    MyClient->DisableSegmentData();
    MyClient->DisableMarkerData();
    MyClient->DisableUnlabeledMarkerData();
    MyClient->DisableDeviceData();

    // Disconnect and dispose
    int t = clock();
    std::cout << " Disconnecting..." << std::endl;
    MyClient->Disconnect();
    int dt = clock() - t;
    double secs = (double) (dt)/(double)CLOCKS_PER_SEC;
    std::cout << " Disconnect time = " << secs << " secs" << std::endl;
}


unsigned int __stdcall ViconDataStreamClient::ListenViconWrapper(void* c)
{
    ViconDataStreamClient * cli = (ViconDataStreamClient*)c;   // the tricky cast

    cli->getViconStream(); 
    //    static_cast<Server*>(o)->ListenSocket(o);

    return 1;
}

/**
** Returns the position of a random marker of an object
**/
MHTypes::Point3D ViconDataStreamClient::getObjectPosition(std::string SubjectName)
{
    //std::cout << "Entering critical section" << std::endl;
    // Lock the Critical section
    EnterCriticalSection(&m_cs2);
    //std::cout << "Entered critical section" << std::endl;

    wandCoords.clear();

    // Get the marker name
    std::string MarkerName = MyClient->GetMarkerName( SubjectName, 1 ).MarkerName;
	std::cout << MarkerName << std::endl;
    // Get the marker parent
    std::string MarkerParentName = MyClient->GetMarkerParentName( SubjectName, MarkerName ).SegmentName;

    // Get the global marker translation
    Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation =
        MyClient->GetMarkerGlobalTranslation( SubjectName, MarkerName );

    wandCoords.push_back( _Output_GetMarkerGlobalTranslation.Translation[ 0 ]);
    wandCoords.push_back( _Output_GetMarkerGlobalTranslation.Translation[ 1 ]);
    wandCoords.push_back( _Output_GetMarkerGlobalTranslation.Translation[ 2 ]);

    //std::cout << "Vicon Wand Coords:" << wandCoords[0] << " " << wandCoords[1] << " "<< wandCoords[2] << std::endl;

    //Release the Critical section
    LeaveCriticalSection(&m_cs2);

    return MHTypes::Point3D(wandCoords[0], wandCoords[1], wandCoords[2]);
}

/**
** Gets the four corners of the tablet. 
**/
void ViconDataStreamClient::getTabletPosition(std::string SubjectName, Tablet& tablet)
{
    // Lock the Critical section
    EnterCriticalSection(&m_cs2);

    // Get the global marker translation
    // 1. Get mobile markers position
    top_right = this->getObjectMarker(MyClient->GetMarkerGlobalTranslation( SubjectName, "top_right" ));
    top_left = this->getObjectMarker(MyClient->GetMarkerGlobalTranslation( SubjectName, "top_left" ));
    bottom_right = this->getObjectMarker(MyClient->GetMarkerGlobalTranslation( SubjectName, "bottom_right" ));
    bottom_left = this->getObjectMarker(MyClient->GetMarkerGlobalTranslation( SubjectName, "bottom_left" ));

	/*std::ostringstream ss;
	std::string test;
	ss << "BottomLeft: " << bottom_left.x << std::endl;
	test = ss.str();
	OutputDebugString(test.c_str());*/

    tablet.updatePosition(bottom_left, bottom_right, top_left, top_right);

    //Release the Critical section
    LeaveCriticalSection(&m_cs2);
}

/**
** Gets the four corners of the finger object. 
**/
void ViconDataStreamClient::getFingerPosition(std::string SubjectName, Finger& finger)
{



	// Lock the Critical section
    EnterCriticalSection(&m_cs2);

     // Get the marker name
    std::string MarkerName = MyClient->GetMarkerName( SubjectName, 1 ).MarkerName;

    // Get the marker parent
    std::string MarkerParentName = MyClient->GetMarkerParentName( SubjectName, MarkerName ).SegmentName;

    // Get the global marker translation
    Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation =
        MyClient->GetMarkerGlobalTranslation( SubjectName, MarkerName );

    finger_pos = this->getObjectMarker( _Output_GetMarkerGlobalTranslation);
	finger.updatePosition(finger_pos);

    //display.fingerEvent(finger_pos);

    //Release the Critical section
    LeaveCriticalSection(&m_cs2);


 //   // Lock the Critical section
 //   EnterCriticalSection(&m_cs2);

 //   // Get the global marker translation
 //   // 1. Get mobile markers position
 //   top_right = this->getObjectMarker(MyClient->GetMarkerGlobalTranslation( SubjectName, "top_right" ));
 //   top_left = this->getObjectMarker(MyClient->GetMarkerGlobalTranslation( SubjectName, "top_left" ));
 //   bottom_right = this->getObjectMarker(MyClient->GetMarkerGlobalTranslation( SubjectName, "bottom_right" ));
 //   bottom_left = this->getObjectMarker(MyClient->GetMarkerGlobalTranslation( SubjectName, "bottom_left" ));

	///*std::ostringstream ss;
	//std::string test;
	//ss << "BottomLeft: " << bottom_left.x << std::endl;
	//test = ss.str();
	//OutputDebugString(test.c_str());*/

 //   finger.updatePosition(bottom_left, bottom_right, top_left, top_right);

 //   //Release the Critical section
 //   LeaveCriticalSection(&m_cs2);
}

void ViconDataStreamClient::getTabletRotation(std::string SubjectName, Tablet& tablet)
{
	// Lock the Critical section
    EnterCriticalSection(&m_cs2);


   // wandCoords.clear();
   ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationQuaternion quat_angle = MyClient->GetSegmentGlobalRotationQuaternion(SubjectName, SubjectName); // ->GetSegmentGlobalRotationEulerXYZ(SubjectName, SubjectName);
    tablet_rotation_quaternion = MHTypes::Quaternion();
    tablet_rotation_quaternion.x = quat_angle.Rotation[0]; 
    tablet_rotation_quaternion.y = quat_angle.Rotation[1]; 
    tablet_rotation_quaternion.z = quat_angle.Rotation[2];
    tablet_rotation_quaternion.w = quat_angle.Rotation[3];

    std::cout << "Quat ANGLE: w " << tablet_rotation_quaternion.w << "; " << 
                             "x " << tablet_rotation_quaternion.x << "; " << 
                             "y " << tablet_rotation_quaternion.y << "; " << 
                             "z " << tablet_rotation_quaternion.z  << std::endl; 

	tablet.updateRotation(tablet_rotation_quaternion);
 
    //Release the Critical section
    LeaveCriticalSection(&m_cs2);


	//// Lock the Critical section
 //   EnterCriticalSection(&m_cs2);

	//ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationEulerXYZ euler_angle = MyClient->GetSegmentGlobalRotationEulerXYZ(SubjectName, SubjectName);
 //  
 ////   std::ostringstream ss;
	////std::string test;
	////ss << "Pitch: " << euler_angle.Rotation[0] << " Yaw: " << euler_angle.Rotation[1] << " Roll: " << euler_angle.Rotation[2] << std::endl;
	////test = ss.str();
	////OutputDebugString(test.c_str());

 //   tablet_rotation_angle = MHTypes::EulerAngle(-euler_angle.Rotation[0]*MathHelper::radToDeg(), 
 //                                               -euler_angle.Rotation[1]*MathHelper::radToDeg(), 
 //                                               -euler_angle.Rotation[2]*MathHelper::radToDeg());
	//tablet.updateRotation(tablet_rotation_angle);

	////Release the Critical section
 //   LeaveCriticalSection(&m_cs2);

}



MHTypes::Point3D ViconDataStreamClient::getObjectMarker(ViconDataStreamSDK::CPP::Output_GetMarkerGlobalTranslation _Output_GetMarkerGlobalTranslation){
   MHTypes::Point3D markerPos;

   markerPos.x = _Output_GetMarkerGlobalTranslation.Translation[ 0 ];
   markerPos.y = _Output_GetMarkerGlobalTranslation.Translation[ 1 ];
   markerPos.z = _Output_GetMarkerGlobalTranslation.Translation[ 2 ];

   return markerPos;
}

MHTypes::EulerAngle ViconDataStreamClient::getObjectRotation(std::string SubjectName)
{
    // Lock the Critical section
    EnterCriticalSection(&m_cs2);

   // wandCoords.clear();
    ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationEulerXYZ euler_angle = MyClient->GetSegmentGlobalRotationEulerXYZ(SubjectName, SubjectName);
   
   /* std::cout << "EULER ANGLE BEFORE: pitch " << euler_angle.Rotation[0] << "; " << 
                          "roll " << euler_angle.Rotation[1] << "; " << 
                          "yaw " << euler_angle.Rotation[2] << std::endl; */
    
    tablet_rotation_angle = MHTypes::EulerAngle(euler_angle.Rotation[0]*MathHelper::radToDeg(), 
                                                euler_angle.Rotation[1]*MathHelper::radToDeg(), 
                                                euler_angle.Rotation[2]*MathHelper::radToDeg());

   /* std::cout << "EULER ANGLE AFTER: pitch " << tablet_rotation_angle.pitch << "; " << 
                          "roll " << tablet_rotation_angle.roll << "; " << 
                          "yaw " << tablet_rotation_angle.yaw << std::endl; */

    //Release the Critical section
    LeaveCriticalSection(&m_cs2);

    return tablet_rotation_angle;
}


MHTypes::Quaternion ViconDataStreamClient::getObjectRotationQuaternion(std::string SubjectName)
{
    // Lock the Critical section
    EnterCriticalSection(&m_cs2);


   // wandCoords.clear();
   ViconDataStreamSDK::CPP::Output_GetSegmentGlobalRotationQuaternion quat_angle = MyClient->GetSegmentGlobalRotationQuaternion(SubjectName, SubjectName); // ->GetSegmentGlobalRotationEulerXYZ(SubjectName, SubjectName);
    tablet_rotation_quaternion = MHTypes::Quaternion();
    tablet_rotation_quaternion.x = quat_angle.Rotation[0]; 
    tablet_rotation_quaternion.y = quat_angle.Rotation[1]; 
    tablet_rotation_quaternion.z = quat_angle.Rotation[2];
    tablet_rotation_quaternion.w = quat_angle.Rotation[3];

    std::cout << "Quat ANGLE: w " << tablet_rotation_quaternion.w << "; " << 
                             "x " << tablet_rotation_quaternion.x << "; " << 
                             "y " << tablet_rotation_quaternion.y << "; " << 
                             "z " << tablet_rotation_quaternion.z  << std::endl; 
 
    //Release the Critical section
    LeaveCriticalSection(&m_cs2);

    return tablet_rotation_quaternion;
}



/*void ViconDataStreamClient::parse3DData(){

// Lock the Critical section
EnterCriticalSection(&m_cs2);

coords.clear();

// do critical things here
for (unsigned int i = 0; i < ViconDataStreamClient::tempCoords.size(); ++i) {
coords.push_back( tempCoords[i]);
}

//Release the Critical section
LeaveCriticalSection(&m_cs2);


// Display coords vector
std::cout << "Coords vector size " << ViconDataStreamClient::coords.size() << " : " ;
for (unsigned int i = 0; i < ViconDataStreamClient::coords.size(); ++i) {
std::cout << ViconDataStreamClient::coords[i] << " ";
}
std::cout << std::endl;

}*/

