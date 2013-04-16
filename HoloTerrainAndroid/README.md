# HoloTerrainViewer README


## Description
HoloTerrainViewer is an Android project developed in the Eclipse Android SDK. It displays a user interface for switching between the various interactions with the 3D terrain hologram (produced by the accompanying zSpace code), and also displays a replicated (markers placed on the 3D hologram are also shown on the 2D map in the corresponding location) top-down view of the hologram.

## Instructions for Running
This code is not standalone. It must be run alongside the accompanying zSpace holoTerrain code (or the C++ socket server code, for socket testing purposes) in order to function properly. This Android project should be run only after the zSpace (or C++ socket server) code  has already been launched. 

Since this project establishes a socket connection between the Android device and the computer running the zSpace code, the IP address used to establish the client socket in this Android code must match the IP address of the computer running the zSpace code. Also, the port number must match the port number in the zSpace code (This is upheld by default, so the port number shouldn't require changing).
