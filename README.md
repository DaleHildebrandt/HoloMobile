####HoloMobile README####

## Description
HoloMobile is a research project investigating use of mobile devices that have holograms positioned around the top area of the device. The hologram positions and orients itself in accordance to the movements of the mobile device, as if it was attached. For example, the hologram may display a cube; if the user rotates the mobile 180 degrees, the cube will also rotate 180 degrees, revealing the backside of the cube.

## Terminology
I will refer to "terrain", "3D map", and "hologram" almost interchangeably. "Android device" and "mobile device" are interchangeable as well.

## Requirements
This project requires a number of different technologies in order to operate correctly:

1: zSpace System
This includes a PC capable of running zSpace applications in stereoscopic 3D, the zSpace display & accompanying motion-tracked glasses. The zSpace stylus is only necessary if you require a custom calibration (detailed in the HoloTerrain README). The zSpace SDK version 2.8 (32-bit) was used to test the HoloTerrain project. According to infinite Z, makers of the zSpace, this application should be compatible with all future versions of the SDK, but not previous ones.

2: Vicon Motion Tracking System
This project was only tested using the Vicon Tracker software. It may be possible -- likely even -- that other Vicon software, such as Blade, may also work as well, but it remains to be tested. 

## Organization
This HoloMobile application consists of two projects that communicate to one another via socket communication.

The first project can be found in the HoloTerrain folder. This project utilizes the zSpace system for drawing the holograms that appear to be attached to the mobile device. It also uses tracked data from the Vicon system for interactions and allows the hologram to move with the mobile device. For more details, see the README in the HoloTerrain folder.

The second project can be found in the HoloTerrainAndroid folder. This project is to be run on an Android mobile device. It provides a 2D GUI that allows the user to switch between the various interaction modes. It also features a 2D representation of the hologram displayed by the zSpace. For more details see the README in the HoloTerrainAndroid folder.


## Setup

zSpace SDK 2.8 or above should be installed on the zSpace computer. 

The IP addresses for the various socket communications must be set appropriately:
In the zSpace code, set the IP address to the address of the Vicon PC. In the Android code, set the IP address to be the address of the zSpace PC.

There are three tracked objects that must be registered with the Vicon system under the following names:
"samsung_tab" - Represents the mobile device running the Android application. Allows the hologram to stay attached to the mobile device.
"terrain_finger" - Used to track the index finger of the user. Allows the user to interact with the holographic image.
"terrain_thumb" - Used to track the thumb, on the same hand as the index finger object is. Allows the user to interact with the holographic image.

For the default calibration file (calibrates the real world coordinate system with the zSpace display coordinate system) to work correctly, you must set the origin of the Vicon coordinate system by placing the calibration wand in front of the zSpace screen (where you would typically have a keyboard). Refer to the following figure for orienting the calibration wand correctly (notice that the intersection point of the wand is positioned more to the right of the screen than the left):
![Alt text](images/SettingViconOrigin.jpg?raw=true)



Place the zTools folder in your C: directory, so the project refereces the appropriate DLL, header, and lib files. There may be additional installation required,


## Interaction Modes
The Android device is primarily used to switch between the six different interaction modes, while the user's tracked fingers perform the interactions via a pinch gesture on the display area of the hologram. The six interaction modes are as follows:

1. Place Path: The can define a path by placing a starting and ending point on the hologram.
2. Draw Path: The user can directly paint a path onto the hologram by moving their finger.
3. Place POI: The user can place a point of interest on the hologram. This POI is comprised of a marker and a annotation, which is entered on the Android device upon switching into this mode.
4. Place ROI: In addition to placing a POI, a radius is also defined around the POI. This radius is adjusted by moving tracked fingers away from the POI after it is placed.
5. Filter Height: The user can place an upper and lower clipping plane in the hologram, causing only terrain within a particular height range to be rendered in the hologram.
6. Select Height: The user can select a particular height, with the 2D representation of the terrain on the Android device, only displaying areas of the map that are of similar height to the selected height.

## Communication Conventions
Format: Messages are always delimeted by commas.

Interaction Mode Selection:
Typical communication begins with a message being sent from the Android device to the zSpace PC in order to change interaction modes. This is typically just the name of the mode (ie. "placepath"). However, with POIs and ROIs, it is accompanied by a string for setting the annotation (ie. "placepoi,Broken Bridge"). Once the zSpace receives this message, it sends back a confirmation message, by prefixing the original string with "CFM,"(ie. "CFM,placepoi,Broken Bridge), and then changes the interaction mode allowing the user to perform an interaction.

Interaction Updates:
As interactions are being performed, the zSpace system sends messages back to allow the android device to update its 2D map to reflect what is being done on the 3D map (ie. A POI is located in the same position on both the 2D and 3D versions of the map). This message first identifies what needs to be modified, followed by the updated values, such as position on map, radius, ect (this will vary depending upon what is being updated).
