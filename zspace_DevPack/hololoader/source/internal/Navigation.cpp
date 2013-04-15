#ifndef EG_GLUT_MODE

#include "Navigation.hpp"
#include "InputHandler.hpp"

using namespace std;
using namespace EbonGL;
using namespace MHTypes;

const double Navigation::CHAR_ROT_VELOCITY = 1.5;
const double Navigation::MIN_ROT_THRESHOLD = 5;
const double Navigation::MAX_ROT_THRESHOLD = 45;

Navigation::Navigation(EG_Engine *newEngine,
                       InputHandler *newDeviceInput)
{
    glEngine = newEngine;
    deviceInput = newDeviceInput;

    movingCharacter = false;
    doingViewRestore = false;
    slerpAmount = 0;
    charVelocity = 0.05;
}//constructor

Navigation::~Navigation(void)
{
}//destructor

void Navigation::update(bool forwardButtonDown,
                        bool reverseButtonDown,
                        bool lookButtonDown,
                        MHTypes::EulerAngle absWandOrient,
                        MHTypes::EulerAngle cameraOrient)
{
    //### Move the character ###
    if (forwardButtonDown)
    {
        moveCharacter(absWandOrient, charVelocity);
        movingCharacter = true;
    }//if forwardButtonDown

    else if (reverseButtonDown)
    {
        moveCharacter(absWandOrient, charVelocity * -1);
        movingCharacter = true;
    }//else if reverseButtonDown

    //### Look around ###
    else if (lookButtonDown)
    {
        lookAround(absWandOrient);
        movingCharacter = false;
    }//else if lookButtonDown

    //### Set viewpoint pitch and yaw to 0 when finished moving ###
    else
    {
        if (movingCharacter)
        {
            startRestoreView(cameraOrient);

            movingCharacter = false;
            doingViewRestore = true;
        }//else buttonDown

        if (doingViewRestore)
            restoreViewpoint();
    }//else reverseButtonDown
}//update

void Navigation::lookAround(EulerAngle &absWandOrient)
{
    EulerAngle finalAngle;//the angle to move the camera
    vector<GLfloat> navMatrix;//the navigation matrix representing the camera position

    //### Calculate the amount of pitch and yaw to use ###
    finalAngle.pitch = calcNavAngle(absWandOrient.pitch);
    finalAngle.yaw = calcNavAngle(absWandOrient.yaw);

    //### Rotate the camera viewpoint ###
    navMatrix = deviceInput->rotateCamera(finalAngle);
    glEngine->setNavMatrix(navMatrix);
}//lookAround

void Navigation::startRestoreView(EulerAngle cameraOrient)
{
    Point2D originPoint(0, 0);//used to calculate the desired yaw
    MHVector orientVector;//vector representing cameraOrient
    Point2D yawPoint;//the endpoint of a vector representing the desired camera direction,
                    //does not contain height (y)
    MHFloat desiredYaw = 0;//we'll maintain this yaw when restoring the camera

    //### Store the camera's original position ###
    origCameraQuat.fromEuler(cameraOrient);

    //### Face orientVector into the screen ###
    orientVector.z = -1;

    //### Rotate orientVector by the camera orientation ###
    orientVector.fromQuaternion(origCameraQuat);

    //### Calculate the amount of yaw ###
    yawPoint.x = orientVector.x;
    yawPoint.y = orientVector.z;
    desiredYaw = originPoint.angleDeg(yawPoint);

    //### Convert the yaw to coordinate system used by EbonGL ###
    desiredYaw += 90;
    if (desiredYaw > 180)
    {
        desiredYaw = desiredYaw - 180;
        desiredYaw = -180 + desiredYaw;
    }//if desiredYaw

    //### Store the angle to rotate the camera to ###
    desiredViewQuat.fromEuler(EulerAngle(0, desiredYaw, 0));
}//startRestoreView

void Navigation::restoreViewpoint(void)
{
    Quaternion rotateQuat;//the amount to rotate the camera angle
    EulerAngle rotateAngle;//the Euler angles version of rotateQuat
    vector<GLfloat> navMatrix;//the navigation matrix representing the camera position

    //### Rotate origCameraQuat towards desiredViewQuat ###
    slerpAmount += 0.02;
    rotateQuat = origCameraQuat.slerp(desiredViewQuat, slerpAmount, Q_SHORT);
    rotateAngle = rotateQuat.getRotationMatrix().toEuler();

    //### Rotate the camera to rotateQuat ###
    navMatrix = deviceInput->setRotation(rotateAngle);
    glEngine->setNavMatrix(navMatrix);

    //### Stop rotating when we reach the goal ###
    if (slerpAmount >= 0.999)
    {
        slerpAmount = 0;
        doingViewRestore = false;
    }//if slerpAmount
}//restoreViewpoint

void Navigation::moveCharacter(EulerAngle absWandOrient,
                               double distance)
{
    EulerAngle finalAngle;//the angle to move the camera
    vector<GLfloat> navMatrix;//the navigation matrix representing the camera position

    //### Calculate the amount of pitch and yaw to use ###
    finalAngle.pitch = calcNavAngle(absWandOrient.pitch);
    finalAngle.yaw = calcNavAngle(absWandOrient.yaw);

    //### Rotate and translate the camera viewpoint ###
    navMatrix = deviceInput->rotateCamera(finalAngle);
    navMatrix = deviceInput->moveCamera(MHVector(0, 0, distance));
    glEngine->setNavMatrix(navMatrix);
}//moveCharacter

MHFloat Navigation::calcNavAngle(MHFloat origAngle)
{
    MHFloat result = 0;//the angle to return, in degrees

    if (origAngle >= MIN_ROT_THRESHOLD)
    {
        if (origAngle >= MAX_ROT_THRESHOLD)
            result = CHAR_ROT_VELOCITY;
        else
            result = (origAngle / MAX_ROT_THRESHOLD) * CHAR_ROT_VELOCITY;
    }//if origAngle

    else if (origAngle <= MIN_ROT_THRESHOLD * -1)
    {
        if (origAngle <= MAX_ROT_THRESHOLD * -1)
            result = CHAR_ROT_VELOCITY * -1;
        else
            result = (origAngle / MAX_ROT_THRESHOLD) * CHAR_ROT_VELOCITY;
    }//else origAngle

    result *= -1;

    return result;
}//calcNavAngle

void Navigation::setVelocity(double newValue)
{
    charVelocity = newValue;
}//setVelocity

#endif
