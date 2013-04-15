###HoloTerrain README###

## Description
HoloTerrain is a VC++ project created in Visual Studio 2010. It draws an interactive 3D terrain onto the screen that moves with respect to a Vicon-tracked mobile device, in an effort to emulate a mobile device that can project a hologram from the top.

## Requirements
This project requires the use of a zSpace 3D stereoscopic system, Vicon motion tracking system, and a mobile Android device (with the accompanying Android application) in order to run. It also requires the installation of the zSpace (32-bit) SDK. This project uses SDK version 2.8.0.1453. According to zSpace, this project should work on earlier versions of the SDK, but I have yet to verify that.

## Setup
Turn on Vicon system; ensure it is loaded with a working configuration (calibrated, etc.). Verify that objects for the mobile device, index finger, and thumb are all correctly recognized by the Vicon system and match the names used in the zSpace code. For the default calibration file that is provided, you must set the origin of the Vicon coordinate system by placing the calibration wand in front of the zSpace screen (where you would typically have a keyboard). Refer to the following figure for orienting the calibration wand correctly (notice that the intersection point of the wand is positioned more to the right of the screen than the left):

			|			|
			| zSpace Screen	|
			 ---------------
					|
		Calib. Wand	|
			====---------
					|
			
Make sure the ViconDataStreamClient code has the correct IP Address of the system running the Vicon software. Launch the HoloTerrain application. The window will likely appear as completely silver. Launch the accompanying android Application on the mobile device that is being tracked. Now, if you hold the mobile device in front of you, with the top of the device pointed to the screen, you should see a green terrain on a black background appear in front of you. The green terrain should follow the movements of the mobile device.