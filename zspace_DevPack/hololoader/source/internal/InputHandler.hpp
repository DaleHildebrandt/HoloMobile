/* DESCRIPTION
    Obtains device input from VRJuggler. The two main functions are update() and
    getData(). update should be called from VRJuggler's vrj::GlApp::preFrame() function.
    It will obtain the current device states. getData() will retrieve that information
    in a thread-safe manner. We also move the camera viewpoint in this class, so that we
    can apply the camera matrix to wand data, and provide the proper wand position relative
    to the camera in the virtual world.
*/

#ifndef INPUTHANDLER_HPP_INCLUDED
#define INPUTHANDLER_HPP_INCLUDED

//Each element is the button's index inside of buttonPressed, buttonReleased,
//buttonDown and theButtons. Be sure to update NUM_BUTTONS when adding or
//removing buttons from ButtonType.
enum ButtonType
{
    BT_TRIGGER = 0,
    BT_FAR_LEFT = 1,
    BT_BOTTOM_RIGHT = 2,
    BT_FAR_RIGHT = 3,
    BT_HAT = 4
};//ButtonType

//The number of buttons we're tracking
static const int NUM_DEVICE_BUTTONS = 5;

#ifndef EG_GLUT_MODE

#include <gadget/Type/PositionInterface.h>
#include <GL/gl.h>
#include <gadget/Type/DigitalInterface.h>
#include "MathHelper.hpp"
#include "EbonGL.hpp"

class InputHandler
{
    public:
        //constructor
        InputHandler(void);

        //destructor
        ~InputHandler(void);

        //Start receiving data from the input devices
        void initialize(void);

        //Call to update device data.
        void update(void);

        //Move the camera viewpoint by the distance specified in movementVector.
        //Returns an array of 16 values corresponding to the resulting 4x4 navigation matrix
        std::vector<GLfloat> moveCamera(MHTypes::MHVector movementVector);

        //Rotate the camera viewpoint by the specified amount, in degrees
        //Returns an array of 16 values corresponding to the resulting 4x4 navigation matrix
        //cameraOrient will be set to the camera's new orientation
        std::vector<GLfloat> rotateCamera(MHTypes::EulerAngle amount);

        //Set the camera to the new orientation
        std::vector<GLfloat> setRotation(MHTypes::EulerAngle newRotation);

        //Retrieve the current device values. newTriggerPressed will be true if the trigger
        //was pressed since the last call to getData, so it may be true even if newTriggerDown
        //is false.
        void getData(MHTypes::EulerAngle &newWandOrient,
                        MHTypes::Point3D &newWandPos,
                        MHTypes::EulerAngle &newCameraOrient,
                        MHTypes::EulerAngle &newAbsWandOrient,
                        std::vector<bool> &newButtonDown,
                        std::vector<bool> &newButtonPressed,
                        std::vector<bool> &newButtonReleased);

    private:
        //Tracks the wand position and orientation
		gadget::PositionInterface mWand;

		//Tracks the head (glasses) position and orientation
		gadget::PositionInterface mHead;

        //Lets us provide the proper wand position, relative to the character's position
        //in the virtual world.
        gmtl::Matrix44f navigationMatrix;

        //############### Mutex Group InputMutex ###############

        //Controls access to buttonPressed, buttonDown, buttonReleased, wandPos and wandOrient
        EbonGL::EG_Mutex inputMutex;

        //the wand's orientation in degrees, relative to the camera's position in the scene
        MHTypes::EulerAngle wandOrient;

        //The wand's orientation in degrees, relative to the display
        MHTypes::EulerAngle absWandOrient;

        //the wand's position in scene coordinates
        MHTypes::Point3D wandPos;

        //the camera's orientation in the scene
        MHTypes::EulerAngle cameraOrient;

        //The buttons we're tracking
        std::vector<gadget::DigitalInterface*> theButtons;

        //True if the corresponding button was pressed or released since the last logic update
        std::vector<bool> buttonPressed;
        std::vector<bool> buttonReleased;

		//True if the button is currently down, false otherwise. Used to check for changes
		//in the button state.
        std::vector<bool> buttonDown;

};//class InputHandler

#endif
#endif
