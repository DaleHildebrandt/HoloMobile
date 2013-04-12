#ifndef EG_GLUT_MODE

#include "InputHandler.hpp"
#include <vrj/vrjConfig.h>
#include <gmtl/Matrix.h>
#include <gmtl/Generate.h>
#include <gmtl/Vec.h>

using namespace std;
using namespace MHTypes;

InputHandler::InputHandler(void)
{
    buttonDown.resize(NUM_DEVICE_BUTTONS, false);
    buttonPressed.resize(NUM_DEVICE_BUTTONS, false);
    buttonReleased.resize(NUM_DEVICE_BUTTONS, false);

    for (unsigned int i = 0; i < NUM_DEVICE_BUTTONS; i++)
        theButtons.push_back(new gadget::DigitalInterface);
    theButtons.resize(NUM_DEVICE_BUTTONS, NULL);

    gmtl::identity(navigationMatrix);
}//constructor

InputHandler::~InputHandler(void)
{
    for (unsigned int i = 0; i < theButtons.size(); i++)
        delete theButtons.at(i);
    theButtons.clear();
}//destructor

void InputHandler::initialize(void)
{
    mWand.init("VJWand");
    mHead.init("VJHead");

    theButtons.at(BT_TRIGGER)->init("VJButton5");
    theButtons.at(BT_BOTTOM_RIGHT)->init("VJButton0");
    theButtons.at(BT_HAT)->init("VJButton3");
    theButtons.at(BT_FAR_LEFT)->init("VJButton2");
    theButtons.at(BT_FAR_RIGHT)->init("VJButton4");
}//initialize

void InputHandler::update(void)
{
    gmtl::EulerAngleXYZf wandAngle;//the wand orientation
    gmtl::Matrix44f invNavigation;//inverted version of navigationMatrix
    gmtl::Matrix44f wandMatrix;//4x4 translational matrix representing wand position
	gmtl::Point3f wandPoint;//gtml representation of the wand's position

    inputMutex.lock();

    //### Get wand matrix in the virtual world coordinate system ###
    gmtl::invert(invNavigation, navigationMatrix);// Nav matrix is: w_M_vw.  So invert it
    wandMatrix = invNavigation * mWand->getData();

    //### Convert the orientation to a Euler Angle ###
    wandAngle = gmtl::makeRot<gmtl::EulerAngleXYZf>(wandMatrix);
	wandOrient.pitch = wandAngle[0] * MathHelper::radToDeg();
	wandOrient.yaw = wandAngle[1] * MathHelper::radToDeg();
	wandOrient.roll = wandAngle[2] * MathHelper::radToDeg();

    //### Get the wand's relative position ###
    wandPoint = gmtl::makeTrans<gmtl::Point3f>(wandMatrix);
	wandPos = Point3D(wandPoint[0], wandPoint[1], wandPoint[2]);

    //### Get the wand matrix in absolute coordinates ###
    wandMatrix = mWand->getData();

    //### Convert the orientation to a Euler Angle ###
    wandAngle = gmtl::makeRot<gmtl::EulerAngleXYZf>(wandMatrix);
	absWandOrient.pitch = wandAngle[0] * MathHelper::radToDeg();
	absWandOrient.yaw = wandAngle[1] * MathHelper::radToDeg();
	absWandOrient.roll = wandAngle[2] * MathHelper::radToDeg();

    //### Extract the button data ###
    for (unsigned int i = 0; i < NUM_DEVICE_BUTTONS; i++)
    {
        //### Check the wand button ###
	    if ((*theButtons.at(i))->getData())
        {
            if (!buttonDown.at(i))
            {
                buttonDown.at(i) = true;
                buttonPressed.at(i) = true;
            }//if !triggerDown
        }//if getData()

        else
        {
            if (buttonDown.at(i))
            {
                buttonReleased.at(i) = true;
                buttonDown.at(i) = false;
            }//if triggerDown
        }//else getData()
    }//for i

    inputMutex.unlock();
}//update

void InputHandler::getData(EulerAngle &newWandOrient,
                           Point3D &newWandPos,
                           EulerAngle &newCameraOrient,
                           EulerAngle &newAbsWandOrient,
                           vector<bool> &newButtonDown,
                           vector<bool> &newButtonPressed,
                           vector<bool> &newButtonReleased)
{
    inputMutex.lock();

    newWandOrient = wandOrient;
    newWandPos = wandPos;
    newCameraOrient = cameraOrient;
    newAbsWandOrient = absWandOrient;

    for (unsigned int i = 0; i < NUM_DEVICE_BUTTONS; i++)
    {
        newButtonDown.at(i) = buttonDown.at(i);
        newButtonPressed.at(i) = buttonPressed.at(i);
        newButtonReleased.at(i) = buttonReleased.at(i);

        buttonPressed.at(i) = false;
        buttonReleased.at(i) = false;
    }//for i

    inputMutex.unlock();
}//getData

vector<GLfloat> InputHandler::moveCamera(MHTypes::MHVector movementVector)
{
    gmtl::Vec3f dir;//the direction and magnitude of the movement
    vector<GLfloat> result;//the matrix to return

    inputMutex.lock();

    //### Update the navigation matrix ###
    dir = gmtl::Vec3f(movementVector.x, movementVector.y, movementVector.z);
    gmtl::preMult(navigationMatrix, gmtl::makeTrans<gmtl::Matrix44f>(dir));

    //### Extract the resulting matrix values ###
    result.resize(16, 0);
    for (unsigned int i = 0; i < result.size(); i++)
        result.at(i) = navigationMatrix.mData[i];

    inputMutex.unlock();

    return result;
}//moveCamera

std::vector<GLfloat> InputHandler::setRotation(MHTypes::EulerAngle newRotation)
{
    vector<GLfloat> result;//the matrix to return
    gmtl::EulerAngleXYZf gmtlAngle;//gmtl version of resetAngle
    gmtl::EulerAngleXYZf gmtlCameraAngle;//the camera orientation
    EulerAngle cameraAngle;//MathHelper version of gmtlCameraAngle

    inputMutex.lock();

    //### Rotate the camera to 0, 0, 0 ###
    for (unsigned int i = 0; i < 3; i++)
    {
        //Extract the current camera angle
        gmtlCameraAngle = gmtl::makeRot<gmtl::EulerAngleXYZf>(navigationMatrix);

        //Individually undo the pitch, yaw and roll
        for (unsigned int j = 0; j < 3; j++)
            gmtlAngle[j] = 0;
        gmtlAngle[i] = gmtlCameraAngle[i] * -1;
        gmtl::preMult(navigationMatrix, gmtl::makeRot<gmtl::Matrix44f>(gmtlAngle));
    }//for i

    //### Rotate camera to the desired angle ###
    gmtlAngle[0] = newRotation.pitch * MathHelper::degToRad();
    gmtlAngle[1] = newRotation.yaw * MathHelper::degToRad();
    gmtlAngle[2] = newRotation.roll * MathHelper::degToRad();
    gmtl::preMult(navigationMatrix, gmtl::makeRot<gmtl::Matrix44f>(gmtlAngle));

    //### Extract the resulting matrix values ###
    result.resize(16, 0);
    for (unsigned int i = 0; i < result.size(); i++)
        result.at(i) = navigationMatrix.mData[i];

    //### Save the new camera orientation ###
    gmtlCameraAngle = gmtl::makeRot<gmtl::EulerAngleXYZf>(navigationMatrix);
    cameraOrient.pitch = gmtlCameraAngle[0] * MathHelper::radToDeg();
    cameraOrient.yaw = gmtlCameraAngle[1] * MathHelper::radToDeg();
    cameraOrient.roll = gmtlCameraAngle[2] * MathHelper::radToDeg();

    inputMutex.unlock();

    return result;
}//setRotation

vector<GLfloat> InputHandler::rotateCamera(EulerAngle amount)
{
    vector<GLfloat> result;//the matrix to return
    gmtl::EulerAngleXYZf cameraAngle;//the camera orientation
    gmtl::EulerAngleXYZf gmtlAngle(amount.pitch * MathHelper::degToRad(),
                                   amount.yaw * MathHelper::degToRad(),
                                   amount.roll * MathHelper::degToRad());

    inputMutex.lock();

    //### Rotate the camera ###
    gmtl::preMult(navigationMatrix, gmtl::makeRot<gmtl::Matrix44f>(gmtlAngle));

    //### Extract the resulting matrix values ###
    result.resize(16, 0);
    for (unsigned int i = 0; i < result.size(); i++)
        result.at(i) = navigationMatrix.mData[i];

    //### Convert the orientation to a Euler Angle ###
    cameraAngle = gmtl::makeRot<gmtl::EulerAngleXYZf>(navigationMatrix);
	cameraOrient.pitch = cameraAngle[0] * MathHelper::radToDeg();
	cameraOrient.yaw = cameraAngle[1] * MathHelper::radToDeg();
	cameraOrient.roll = cameraAngle[2] * MathHelper::radToDeg();

    inputMutex.unlock();

    return result;
}//rotateCamera

#endif
