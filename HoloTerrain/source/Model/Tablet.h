#pragma once

//#include "EbonGL.hpp"
#include "../TabletServer.h"
#include "Calibrator.h"
#include "../Cube.h"
#include "../Terrain.h"


class Tablet
{
public:
    Tablet(Calibrator& calib);
    ~Tablet(void);

	//void setTerrainStart();
	//void setTerrainEnd();

    void updatePosition( MHTypes::Point3D new_bottom_left,
                        MHTypes::Point3D new_bottom_right,
                        MHTypes::Point3D new_top_left,
                        MHTypes::Point3D new_top_right);
	//void updateRotation(MHTypes::EulerAngle tablet_rotation);
	void updateRotation(MHTypes::Quaternion tablet_rotation);

	void send(const char* str);

    MHTypes::Point3D getPosition();
    MHTypes::Point3D getCenter();
	float* getRotMat();

    MHTypes::Point3D getHoloPosition();
    MHTypes::EulerAngle getRotation();
    MHTypes::Point3D getXRefPoint();
    MHTypes::Point3D getYRefPoint();
    MHTypes::Point3D getZRefPoint();

	//Get corner markers of tablet
    MHTypes::Point3D Tablet::getTopRight();
    MHTypes::Point3D Tablet::getBottomRight();
    MHTypes::Point3D Tablet::getTopLeft();
    MHTypes::Point3D Tablet::getBottomLeft();

	Terrain* Tablet::getTerrain();

	void renderHolo();
   

private:
	float dirScale;
    Calibrator calibrator;

	//Attached Holo Objects
	Cube *holoCube;
	Terrain *terr;

	TabletServer *server;

	//Path-plotting for terrain model
	bool terrainStart;
	bool terrainEnd;

    // Rotation values
    MHTypes::Matrix3x3 rotation;
	MHTypes::Matrix3x3 rotMat;
	float glRotMat[16];
    MHTypes::EulerAngle eulerAngle;

    // VICON MARKERS Points
    MHTypes::Point3D bottom_left;
    MHTypes::Point3D bottom_right;
    MHTypes::Point3D top_left;
    MHTypes::Point3D top_right;

    // Calculated Points
    MHTypes::Point3D tablet_center;
    MHTypes::Point3D tablet_holo;
	MHTypes::Point3D holoPos; //origin of holographic display area.

    // Tablet Reference frame points (x=1, y=1, z=1)
    MHTypes::Point3D    x_ref_point;
    MHTypes::Point3D    y_ref_point;
    MHTypes::Point3D    z_ref_point;

    // Tablet Reference vectors
    MHTypes::MHVector x_ref;
    MHTypes::MHVector y_ref;
    MHTypes::MHVector z_ref;

    // Visbox Reference frame
    MHTypes::MHVector x_ref_abs;
    MHTypes::MHVector y_ref_abs;
    MHTypes::MHVector z_ref_abs;

    //The tablet orientation at the beginning of a rotation operation.
    MHTypes::Quaternion origTabletQuat;

	MHTypes::Quaternion quat;

    //The amount of SLERP to apply to the tablet orientation
    double slerpAmount;

	void initSocket();

    // OLD ANGLE METHODS (TO DELETE IF NOT IN USE)
    void setTabletQuatOrientation(MHTypes::Quaternion desiredTabletQuat);

    // Update Reference vectors
    void updateReferenceVectors();

    // Calculate rotation
    void calculateRotation();

    void getMiddlePoint(MHTypes::Point3D p1, MHTypes::Point3D p2, MHTypes::Point3D& middle);

    MHTypes::Point3D addPointVector(MHTypes::Point3D point, MHTypes::MHVector vector);

    void rotationFromAngles(MHTypes::MHFloat pitch,
                          MHTypes::MHFloat yaw,
                          MHTypes::MHFloat roll);

};

