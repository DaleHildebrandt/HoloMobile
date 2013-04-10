#pragma once

//#include "EbonGL.hpp"
//#include "../MathHelper.hpp"
#include "../Cube.h"
//#include <GL\glew.h>
#include <fstream>
#include <sstream>
#include <ZSpace\Common\Math\Math.h>
#include <ZSpace\Common\Math\Vector3.h>

class Calibrator
{
public:
    Calibrator(void);
    ~Calibrator(void);

    //Event called before contextInit (loading model meshes)
    virtual void init(void);

    //Initialization called during the first logic update.
    //virtual void postInitialization(EbonGL::EG_Engine *glEngine);

    // Draw method
    void render();

    // Sets calibration cubes position
   // void setViconCube(int x, int y, int z);
    void setWandVisBoxPosition(MHTypes::Point3D trans_pos, MHTypes::Point3D tip_pos);
    //void setWandViconPosition(MHTypes::Point3D pos, MHTypes::Point3D topDir, MHTypes::Point3D bottomDir);
	void setWandViconPosition(MHTypes::Point3D pos, MHTypes::Point3D topDir, MHTypes::Point3D bottomDir);
	void setWandViconPosition(MHTypes::Point3D pos);
	void setWandViconRotation(MHTypes::EulerAngle eulerAngle);

    MHTypes::Point3D getInitialTranslation();

    void displayViconPos();

    bool isDisplayCalibrated();

    void startViconCalibration();
    void transformViconData(MHTypes::Point3D viconCoords, MHTypes::Point3D& visboxCoords);

	void setWandVisboxRotation(zspace::common::Radian angle, zspace::common::Vector3 axis);

    // Updates the Vicon calibration state according to user input
    void updateCalibrationState();

protected:
    //Rendering engine
    //EbonGL::EG_Engine *glEngine;

    //Reference cube
    //EbonGL::EG_Cuboid *targetCube;	
	Cube *targetCube;

    //DTrack coordinates cube
    //EbonGL::EG_Cuboid *visboxCube;
	Cube *oglCube;

    //Vicon coordinates cube
    //EbonGL::EG_Cuboid *viconCube;
	Cube *viconCube;

	Cube *viconHoloCube;
    // Tablet coordinates cube 
    //EbonGL::EG_Cuboid *tabletCube;

private:

    // Imports calibration setting from calibration file
    void importViconCalibration();

    // METHODS TO SET TRANSFORMATION VALUES
    void setInitialTranslation();
    void setRotation();
    void setScale();
    MHTypes::Point3D translatePoint(MHTypes::Point3D point, MHTypes::Point3D trans);

     void writeCalibrationFile();
     std::vector<std::string> getNextLineAndSplitIntoTokens(std::istream& str);


     // Position of the different tracked devices: 
    MHTypes::Point3D wand_Visbox_pos; // position of the wand tracked with Visbox
	zspace::common::Radian wand_Visbox_angle; //rotation angle of stylus tracked by Visbox
	zspace::common::Vector3 wand_Visbox_axis; //rotation axis of stylus tracked by Visbox
	MHTypes::Point3D wand_Visbox_trans_pos; //Translation of object from tip of stylus (down ray)

    MHTypes::Point3D wand_Vicon_pos; // position of the wand tracked with Vicon
	MHTypes::Point3D wand_Vicon_holo_pos; //position of holo displayed in front of Vicon Wand
	MHTypes::EulerAngle wand_Vicon_rot; // rotation of wand tracked with Vicon

    MHTypes::EulerAngle tablet_angle; // rotation of the tablet tracked with Vicon

    // TRANSFORMATION VALUES
    MHTypes::Point3D translation;
    MHTypes::Matrix3x3 rotation;
    MHTypes::EulerAngle eulerAngle;
    MHTypes::Point3D scale;
    //MHTypes::Point3D finalTranslation;

	//Cube *targetCube;

    // Vicon calibration state
    int state;

    // Size of the cube used to calibrate Vicon
    GLdouble cube_size;

    // Gets the distance between two cubes (for Vicon Calibration)
    double getDistanceCubes(Cube *cube1, Cube *cube2);
    double carre(double nombre);

    // VICON Coordinates
    MHTypes::Point3D finalCoords, transCoords, rotCoords, scaleCoords;

   // void WritePointToFile(MHTypes::Point3D pos, std::ofstream file);

};

