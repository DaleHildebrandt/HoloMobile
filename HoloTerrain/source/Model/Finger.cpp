#include "Finger.h"
#include <sstream>


Finger::Finger(Calibrator& calib)
{
	calibrator = calib;

	holoCube = new Cube();
    holoCube->setPosition(MHTypes::Point3D(0.0, 0.0, 0.0));
	holoCube->setColor(1.0f, 1.0f, 1.0f);
	holoCube->setSize(0.04f);

	dirScale = 0.15f;
	holoPos = MHTypes::Point3D(0.0, 0.0, 0.0);
}

void Finger::updatePosition(MHTypes::Point3D pos){

        // Assign new values 
        /*calibrator.transformViconData( MHTypes::Point3D(new_bottom_left.x, new_bottom_left.y, new_bottom_left.z), bottom_left);
        calibrator.transformViconData( MHTypes::Point3D(new_bottom_right.x, new_bottom_right.y, new_bottom_right.z), bottom_right);
        calibrator.transformViconData( MHTypes::Point3D(new_top_left.x, new_top_left.y, new_top_left.z), top_left );
        calibrator.transformViconData( MHTypes::Point3D(new_top_right.x, new_top_right.y, new_top_right.z), top_right);*/

		calibrator.transformViconData(MHTypes::Point3D(pos.x, pos.y, pos.z), finger_center);

        // Calculate virtual points (center and holo)
        MHTypes::Point3D right_segment = MHTypes::Point3D(top_right.x - bottom_right.x, top_right.y - bottom_right.y, top_right.z - bottom_right.z);
        MHTypes::Point3D virtual_top_right = MHTypes::Point3D(top_right.x + right_segment.x, top_right.y + right_segment.y, top_right.z + right_segment.z);
        
        // Get holo position
        //getMiddlePoint(top_left, virtual_top_right, tablet_holo);

        // Get center of tablet
        //getMiddlePoint(top_left, bottom_right, finger_center);

        // Update reference points and vectors 
        //updateReferenceVectors();

		zspace::common::Vector3 dir = zspace::common::Vector3(top_right.x - bottom_right.x, top_right.y - bottom_right.y, top_right.z - bottom_right.z);
		dir.normalise();

		holoPos = MHTypes::Point3D(finger_center.x + dir.x*dirScale, finger_center.y + dir.y*dirScale, finger_center.z + dir.z*dirScale);
		holoCube->setPosition(finger_center);
		//holoCube->setPosition(holoPos);
		//terr->setPosition(holoPos);


        // Calculate rotation
        //calculateRotation();
}

void Finger::getMiddlePoint(MHTypes::Point3D p1, MHTypes::Point3D p2, MHTypes::Point3D& middle){
    middle.x = (p1.x + p2.x)/2;
    middle.y = (p1.y + p2.y)/2;
    middle.z = (p1.z + p2.z)/2;
}

void Finger::renderHolo()
{
	ostringstream ss;
	ss << "HoloCube finger Position: " << holoCube->getPosition().x << " " << holoCube->getPosition().y << " " << holoCube->getPosition().z << std::endl;
	//OutputDebugString(ss.str().c_str());

	glPushMatrix();
	glTranslatef(0.0f, 0.11f, 0.0f);
	//glTranslatef(0.0f, 0.11f, 0.05f);
	//holoCube->render();

	glPopMatrix();

}

Cube* Finger::getPointCube()
{
	return holoCube;
}

MHTypes::Point3D Finger::getPosition()
{
	return holoCube->getPosition();
}

Finger::~Finger(void)
{
}
