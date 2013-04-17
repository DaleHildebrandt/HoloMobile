# HoloTerrain README

## Description
HoloTerrain is a 32-bit VC++ project created in Visual Studio 2010. It draws an interactive 3D terrain onto the screen that moves with respect to a Vicon-tracked mobile device, in an effort to emulate a mobile device that can project a hologram from the top.

## Requirements
This project requires the use of a zSpace 3D stereoscopic system, Vicon motion tracking system, and a mobile Android device (with the accompanying Android application) in order to run. It also requires the installation of the zSpace (32-bit) SDK. This project uses SDK version 2.8. According to zSpace, this project should work on newer versions of the SDK, but I have yet to verify that.

## Setup
Upon startup, the window will likely appear as completely silver. Launch the accompanying android Application on the mobile device that is being tracked and it should become a black window. Now, if you hold the mobile device in front of you, with the top of the device pointed to the screen, you should see a green terrain on a black background appear in front of you. The green terrain should follow the movements of the mobile device.

If you don't see the terrain, try moving the device all around the tracked space. Try rotating the device as well.

## Custom Calibration
It is strongly recommended that you use the default calibration file provided. However, if for some reason you are unable to set the Vicon origin as outlined in the Setup section of the ![HoloMobile README](../README.md), then you can initiate a new calibration by pressing [Ctrl] + [C] while the application is running. In order for the calibration to work, you must first register the Vicon Stylus as a Vicon-Tracked Object under the name "zspace_stylus4".

In order to complete the calibration, you must use the zSpace stylus to position a cube on top of another, stationary cube. In order to complete calibration, the cubes will need to be matched three times, with the position changing after every successful position match.

## Terrain Rendering
The code that is used to render the Terrain was taken from:
http://www.codeproject.com/Articles/14154/OpenGL-Terrain-Generation-An-Introduction

It generates a terrain based on a heightmap (a 32x32 24-bit grayscale image). Darker (black/drak grey) areas on the heightmap correspond to points of low elevation, while lighter areas (white/light grey) on the heightmap correspond to points of high elevation on the terrain.
#### Default heightmap used:
![](images/heightmap.bmp?raw=true "Heightmap")
#### 3D Terrain produced from this heightmap:
![](images/3DTerrain.JPG?raw=true "3D terrain generated from the above heightmap")

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



