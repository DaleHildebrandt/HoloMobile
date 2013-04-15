#//////////////////////////////////////////////////////////////////////////////
#//
#//  Copyright (C) 2007-2012 Infinite Z, Inc.  All Rights Reserved.
#//
#//  File:       CMakeLists.txt
#//  Content:    CMake file to build OpenGl HoloTerrain sample project.
#//
#//////////////////////////////////////////////////////////////////////////////

set (SAMPLE_NAME            HoloTerrain)
set (WIN32_EXECUTABLE       true)
set (COMPILE_DEFINITIONS    "")
set (PROPERTY_FOLDER        Samples/Render/OpenGl)
set (SOURCE_DESTINATION     Samples/Render/OpenGl/HoloTerrain)
set (EXECUTABLE_DESTINATION Bin)

# Source Files
set (
  SOURCE_FILES
    CustomStereoNoTracking.cpp
    CustomStereoNoTracking.h
    OpenGlScene.h
    OpenGlScene.cpp
    OpenGlWindow.h
    OpenGlWindow.cpp
	ObjectTracker.h
	ObjectTracker.cpp
	SocketCom.h
	SocketCom.cpp
	ModelLoader.h
	ModelLoader.cpp
	Cube.h
	Cube.cpp
	Terrain.cpp
	Terrain.h

	MathHelper.cpp
	MathHelper.hpp
	Input/Client.h
	Input/ViconDataStreamClient.h
	Input/ViconDataStreamClient.cpp
	libmathhelp/EulerAngle.hpp
	libmathhelp/EulerAngle.cpp
	libmathhelp/Matrix3x3.hpp
	libmathhelp/Matrix3x3.cpp
	libmathhelp/MHSettings.hpp
	libmathhelp/MHVector.hpp	
	libmathhelp/MHVector.cpp
	libmathhelp/Plane.hpp
	libmathhelp/Plane.cpp
	libmathhelp/Point2D.hpp
	libmathhelp/Point2D.cpp
	libmathhelp/Point3D.hpp
	libmathhelp/Point3D.cpp
	libmathhelp/Quaternion.hpp
	libmathhelp/Quaternion.cpp
	Model/Calibrator.h
	Model/Calibrator.cpp
	Model/Tablet.cpp
	Model/Tablet.h
	Model/Finger.cpp
	Model/Finger.h
	
)

# Media Files
set (
  MEDIA_FILES
    ../../../Resource/Infinite_Z_Logo.bmp
    ../../../Resource/Infinite_Z.ico
)

# Library Files
set (
  LIBRARY_FILES
    optimized ZSCommon
    debug     ZSCommon_D
    
    optimized ZSStereo
    debug     ZSStereo_D

    optimized ZSTracker
    debug     ZSTracker_D

    glew32
    openGL32
    glu32
    SOIL
    ViconDataStreamSDK_CPP

)

# Add The Sample
ADD_SAMPLE (
  "${SAMPLE_NAME}"
  "${SOURCE_FILES}"
  "${SHARED_SOURCE_FILES}"
  "${MEDIA_FILES}"
  "${LIBRARY_FILES}"
  "${WIN32_EXECUTABLE}"
  "${COMPILE_DEFINITIONS}"
  "${PROPERTY_FOLDER}"
  "${SOURCE_DESTINATION}"
  "${EXECUTABLE_DESTINATION}"
)  
