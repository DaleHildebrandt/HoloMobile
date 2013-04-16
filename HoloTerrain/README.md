# HoloTerrain README

## Description
HoloTerrain is a 32-bit VC++ project created in Visual Studio 2010. It draws an interactive 3D terrain onto the screen that moves with respect to a Vicon-tracked mobile device, in an effort to emulate a mobile device that can project a hologram from the top.

## Requirements
This project requires the use of a zSpace 3D stereoscopic system, Vicon motion tracking system, and a mobile Android device (with the accompanying Android application) in order to run. It also requires the installation of the zSpace (32-bit) SDK. This project uses SDK version 2.8. According to zSpace, this project should work on newer versions of the SDK, but I have yet to verify that.

## Setup
Turn on Vicon system; ensure it is loaded with a working configuration (calibrated, etc.). Verify that objects for the mobile device, index finger, and thumb are all correctly recognized by the Vicon system and match the names used in the zSpace code. For the default calibration file that is provided, you must set the origin of the Vicon coordinate system by placing the calibration wand in front of the zSpace screen (where you would typically have a keyboard). Refer to the following figure for orienting the calibration wand correctly (notice that the intersection point of the wand is positioned more to the right of the screen than the left):

				|		|
				| zSpace Screen	|
			 	 ---------------
					|
		Calib. Wand		|
			====------------|---	
					|
			
Make sure the ViconDataStreamClient code has the correct IP Address of the system running the Vicon software. Launch the HoloTerrain application. The window will likely appear as completely silver. Launch the accompanying android Application on the mobile device that is being tracked. Now, if you hold the mobile device in front of you, with the top of the device pointed to the screen, you should see a green terrain on a black background appear in front of you. The green terrain should follow the movements of the mobile device.

## What is Referenced in Lib/Inc/Bin Folders
### zSpace SDK 2.8
Allows for the display and head-tracking of the zSpace system, and any other general zSpace-related functions. I also 

### GLEW ver.? (OpenGL Extention Wrangler)
OpenGL Extension Wrangler allows the use of more recently introduced openGL rendering features. If you need to use certain newer OpenGL features, you probably have to go through this. Apparently, you can't just get a newer version of OpenGL; it has to be done through OpenGL Extentions.

### OpenGLUT 0.6.3
Standard GLUT is no longer actively being developed. Building off of where GLUT left off, OpenGLUT continues to add additional features making graphics programming even easier. OpenGLUT was used to render text in 3D for POIs and ROIs using glutStrokeString().

### SOIL ver.? (Simple OpenGL Image Library)
Provides an easy way to load textures.

### Vicon DataStream
For getting real-time data from a Vicon Motion Tracking System.

### SDL (Simple MediaDirect Layer)
For taking screenshots. Typical screenshots seem to only capture one of the two front framebuffers, but it may be possible to composite both of the front framebuffers to get a 3D image through this library. This is still a TODO.



