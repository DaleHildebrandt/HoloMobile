#ifndef NAVIGATION_HPP_INCLUDED
#define NAVIGATION_HPP_INCLUDED

#ifndef EG_GLUT_MODE

#include "EbonGL.hpp"
#include "MathHelper.hpp"

class InputHandler;

class Navigation
{
    public:
        //constructor
        Navigation(EbonGL::EG_Engine *newEngine,
                    InputHandler *newDeviceInput);

        //destructor
        ~Navigation(void);

        //Move the player position when the appropriate buttons are pressed
        void update(bool forwardButtonDown,
                    bool reverseButtonDown,
                    bool lookButtonDown,
                    MHTypes::EulerAngle absWandOrient,
                    MHTypes::EulerAngle cameraOrient);

        //Set the movement velocity per logic update, default is 0.05
        void setVelocity(double newValue);

        //The amount of rotation to apply when the character turns, in degrees per ENGINE_INTERVAL
        //milliseconds
        static const double CHAR_ROT_VELOCITY;

        //The minimum threshold to register a rotation movement, in degrees
        static const double MIN_ROT_THRESHOLD;

        //If the wand is held at this angle or higher, we'll rotate at CHAR_ROT_VELOCITY.
        //Otherwise, the camera rotation speed will become slower, until it reaches zero at
        //MIN_ROT_THRESHOLD.
        static const double MAX_ROT_THRESHOLD;

    private:
        //Pointer to the graphics engine, we'll update it with the new camera position
        EbonGL::EG_Engine *glEngine;

        //Pointer to the input handler, used to rotate the camera viewpoint
        InputHandler *deviceInput;

        //The camera orientation we'd like to acheive when a viewpoint restore completes.
        MHTypes::Quaternion desiredViewQuat;

        //The camera's orientation at the beginning of a restore viewpoint operation.
        MHTypes::Quaternion origCameraQuat;

        //The amount of SLERP to apply to origCameraQuat, to create a smooth transition
        //to desiredViewQuat
        double slerpAmount;

        //The amount to move the player character after each logic update,
        //if the player is using one of the move buttons.
        double charVelocity;

        //True if the user is currently moving the viewpoint. We use movingCharacter
        //to detect when the user stops moving, so we can initiate a viewpoint restore.
        bool movingCharacter;

        //True if we're currently rotating the camera angle so that roll and pitch are zero.
        bool doingViewRestore;

        //############### FUNCTIONS ###############

        //Move the character viewpoint by the specified distance in the direction
        //given by absWandOrient
        void moveCharacter(MHTypes::EulerAngle absWandOrient,
                            double distance);

        //We return an angle that moves the camera viewpoint towards origAngle,
        //keeping the rotational velocity at or below CHAR_ROT_VELOCITY. The larger
        //origAngle is, the faster the rotational velocity will be.
        MHTypes::MHFloat calcNavAngle(MHTypes::MHFloat origAngle);

        //When the player stops moving, we'll rotate the camera until the pitch
        //and yaw are zero again
        void restoreViewpoint(void);

        //initiate a restore viewpoint operation
        void startRestoreView(MHTypes::EulerAngle cameraOrient);

        //let the user look around without moving, based on the wand angle
        void lookAround(MHTypes::EulerAngle &absWandOrient);

};//Navigation

#endif
#endif
