#ifndef FINGER_H
#define FINGER_H

#include "Calibrator.h"
#include "../Cube.h"
#include "../Terrain.h"

class Finger
{
public:
	Finger(Calibrator& calib);
	~Finger(void);
	 void updatePosition( MHTypes::Point3D pos);
	 MHTypes::Point3D getPosition();
	 void renderHolo();
	 Cube* Finger::getPointCube();

private:
	float dirScale;
	Calibrator calibrator;

	Cube *holoCube;

	MHTypes::Point3D finger_center;

	// VICON MARKERS Points
    MHTypes::Point3D bottom_left;
    MHTypes::Point3D bottom_right;
    MHTypes::Point3D top_left;
    MHTypes::Point3D top_right;

	MHTypes::Point3D holoPos; //origin of holographic display area.

	void getMiddlePoint(MHTypes::Point3D p1, MHTypes::Point3D p2, MHTypes::Point3D& middle);
};

#endif