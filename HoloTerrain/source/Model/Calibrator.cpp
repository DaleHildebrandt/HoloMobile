//Calibrator.cpp
//## Definition ##
//Used to convert between Vicon coordinate system and zSpace coordinate system.
//Either uses an existing calibration file (by default), or a new calibration can be made by
//pressing [cntrl + c] while running the application.
//Calibration can be performed by moving the zSpace stylus in order to line up two on-screen
//boxes (one will be attached to stylus, the other will be fixed). The boxes must be lined up
//a total of three times before calibration is complete.

#include <Windows.h>
#include "../Cube.h"
#include "Calibrator.h"
#include <ostream>


using namespace std;

Calibrator::Calibrator(void)
{
    cube_size = 0.1;
	targetCube = new Cube();
    //targetCube = new EbonGL::EG_Cuboid();

    targetCube->setPosition(MHTypes::Point3D(0.0, 0.0, 0.0));
	targetCube->setColor(0.0f, 1.0f, 0.0f);
    //targetCube->setSize(cube_size);
    //targetCube->setAmbientDiffuse(EbonGL::EG_Color(0.0, 1.0, 0.0, 1.0)); // green

	oglCube = new Cube();
    //visboxCube = new EbonGL::EG_Cuboid();
    oglCube->setPosition(MHTypes::Point3D(100.0, 0.0, 0.0));
	oglCube->setColor(1.0f, 0.0f, 0.0f);
    //oglCube->setSize(cube_size);
    //visboxCube->setAmbientDiffuse(EbonGL::EG_Color(1.0, 0.0, 0.0, 1.0)); // red

    //viconCube = new EbonGL::EG_Cuboid();
	viconCube = new Cube();
    viconCube->setPosition(MHTypes::Point3D(100.0, 0.0, 0.0));
	viconCube->setColor(0.0f, 0.0f, 1.0f);
    //viconCube->setSize(cube_size);
    //viconCube->setAmbientDiffuse(EbonGL::EG_Color(0.0, 0.0, 1.0, 1.0)); // blue

	viconHoloCube = new Cube();
    viconHoloCube->setPosition(MHTypes::Point3D(100.0, 0.0, 0.0));
	viconHoloCube->setColor(1.0f, 1.0f, 1.0f);
	viconHoloCube->setSize(0.08f);
}


Calibrator::~Calibrator(void)
{
}

void Calibrator::init(){

    wand_Visbox_pos = MHTypes::Point3D(0.0, 0.0, 0.0);
	wand_Visbox_angle = zspace::common::Radian(0.0f);
	wand_Visbox_axis = zspace::common::Vector3(0.0f, 0.0f, 0.0f);
	wand_Vicon_pos = MHTypes::Point3D(0.0, 0.0, 0.0);
	wand_Vicon_rot = MHTypes::EulerAngle(0.0, 0.0, 0.0);

    // Set Vicon calibration values
    importViconCalibration();
    state = 3;
}


/*void Calibrator::postInitialization(EbonGL::EG_Engine *glEngine){
    //### Create my cube ###
    glEngine->addSolid(targetCube);
    glEngine->addSolid(visboxCube);
    glEngine->addSolid(viconCube);

    targetCube->hide();
}*/

/*
* Updates the calibration state
*/
void Calibrator::updateCalibrationState(){
	string str;
	stringstream ss;


	OutputDebugString(str.c_str());
    if ((state == 0) && getDistanceCubes(targetCube, oglCube) < 0.005) {
        ss << "STATE 0: Wand : " << wand_Visbox_pos.x << ";" << wand_Visbox_pos.y << ";" << wand_Visbox_pos.z << "  +++++++   Vicon :"
            << wand_Vicon_pos.x << ";" << wand_Vicon_pos.y << ";" << wand_Vicon_pos.z << endl;
		str = ss.str();
		OutputDebugString(str.c_str());

        state = 1;
        setInitialTranslation();
        transformViconData(MHTypes::Point3D(wand_Vicon_pos.x, wand_Vicon_pos.y, wand_Vicon_pos.z),   wand_Vicon_pos);

        ss << "STATE 1: Wand : " << wand_Visbox_pos.x << ";" << wand_Visbox_pos.y << ";" << wand_Visbox_pos.z << "  +++++++   Vicon :"
            << wand_Vicon_pos.x << ";" << wand_Vicon_pos.y << ";" << wand_Vicon_pos.z << endl;
		str = ss.str();
		OutputDebugString(str.c_str());

        targetCube->setPosition(MHTypes::Point3D(0.1, 0.0, 0.0));

		//oglCube->setPosition(MHTypes::Point3D(0.0f, 0.0f, 0.0f));
    } else if ((state == 1) && getDistanceCubes(targetCube, oglCube) < 0.005  ) {
        ss << "STATE 1: Wand : " << wand_Visbox_pos.x << ";" << wand_Visbox_pos.y << ";" << wand_Visbox_pos.z << "  +++++++   Vicon :"
            << wand_Vicon_pos.x << ";" << wand_Vicon_pos.y << ";" << wand_Vicon_pos.z << endl;
		str = ss.str();
		OutputDebugString(str.c_str());

        state = 2;
        setScale();
       transformViconData(MHTypes::Point3D(wand_Vicon_pos.x, wand_Vicon_pos.y, wand_Vicon_pos.z),   wand_Vicon_pos);

        ss << "STATE 2: Wand : " << wand_Visbox_pos.x << ";" << wand_Visbox_pos.y << ";" << wand_Visbox_pos.z << "  +++++++   Vicon :"
            << wand_Vicon_pos.x << ";" << wand_Vicon_pos.y << ";" << wand_Vicon_pos.z << endl;
		str = ss.str();
		OutputDebugString(str.c_str());
        targetCube->setPosition(MHTypes::Point3D(0.1, 0.1, -0.1));
    } else if ((state == 2) && getDistanceCubes(targetCube, oglCube) < 0.005  ) {
        state = 3;
        writeCalibrationFile();
        
        ss << "STATE 3: Wand : " << wand_Visbox_pos.x << ";" << wand_Visbox_pos.y << ";" << wand_Visbox_pos.z << "  +++++++   Vicon :"
            << wand_Vicon_pos.x << ";" << wand_Vicon_pos.y << ";" << wand_Vicon_pos.z << endl;
		str = ss.str();
		OutputDebugString(str.c_str());

        targetCube->hide();
        //setScale();
    }
}
//Translation from tip down ray, and position of stylus tip (both in display space)
void Calibrator::setWandVisBoxPosition(MHTypes::Point3D trans_pos, MHTypes::Point3D tip_pos)
{
    wand_Visbox_pos = tip_pos;
	wand_Visbox_trans_pos = trans_pos;
    oglCube->setPosition(wand_Visbox_pos);
	oglCube->setTranslation( wand_Visbox_trans_pos);
}

void Calibrator::setWandVisboxRotation(zspace::common::Radian angle, zspace::common::Vector3 axis)
{
	wand_Visbox_angle = angle;
	wand_Visbox_axis = axis;
	oglCube->setRotation(angle, axis);

	string str;
	stringstream ss;

	ss << "Rotation Angle: " << angle.valueDegrees() << endl;
	str = ss.str();
	OutputDebugString(str.c_str());

}

void Calibrator::setWandViconPosition(MHTypes::Point3D pos)
{
	stringstream ss;
	string str;

	ss << "Vicon (pre-transform) Coords: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	//ss << "Vicon Coords: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	str = ss.str();
	if(pos.x > 0.0f || pos.x < 0.0f || pos.y > 0.0f || pos.y < 0.0f || pos.z > 0.0f || pos.z < 0.0f){
		//OutputDebugString(str.c_str());
	}
	wand_Vicon_pos = pos;
	transformViconData(MHTypes::Point3D(pos.x, pos.y, pos.z), wand_Vicon_pos);
	viconCube->setPosition(wand_Vicon_pos);

	ss.clear();
	ss << "Vicon (post-transform) Coords: " << wand_Vicon_pos.x << ", " << wand_Vicon_pos.y << ", " << wand_Vicon_pos.z << endl;
	//ss << "Vicon Coords: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	str = ss.str();
	if(wand_Vicon_pos.x > 0.0f || wand_Vicon_pos.x < 0.0f || wand_Vicon_pos.y > 0.0f || wand_Vicon_pos.y < 0.0f || wand_Vicon_pos.z > 0.0f || wand_Vicon_pos.z < 0.0f){
		//OutputDebugString(str.c_str());
	}

}

void Calibrator::setWandViconPosition(MHTypes::Point3D pos, MHTypes::Point3D topDir, MHTypes::Point3D bottomDir)
{
	float dirScale = 0.15f;
	stringstream ss;
	string str;
	//MHTypes::Point3D pos = tab.getPosition();
	//MHTypes::Point3D topDir = tab.getTopRight();
	//MHTypes::Point3D bottomDir = tab.getBottomRight();

	ss << "Vicon (pre-transform) Coords: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	//ss << "Vicon Coords: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	str = ss.str();
	if(pos.x > 0.0f || pos.x < 0.0f || pos.y > 0.0f || pos.y < 0.0f || pos.z > 0.0f || pos.z < 0.0f){
		//OutputDebugString(str.c_str());
	}

    //transformViconData(MHTypes::Point3D(pos.x, pos.z, -pos.y), wand_Vicon_pos);

    //transformViconData(MHTypes::Point3D(pos.x, pos.y, pos.x), wand_Vicon_pos);

	/*transformViconData(MHTypes::Point3D(pos.x, pos.y, pos.z), wand_Vicon_pos);
    viconCube->setPosition(wand_Vicon_pos);*/
	zspace::common::Vector3 dir = zspace::common::Vector3(topDir.x - bottomDir.x, topDir.y - bottomDir.y, topDir.z - bottomDir.z);
	dir.normalise();
	wand_Vicon_pos = pos;
	//transformViconData(MHTypes::Point3D(pos.x, pos.y, pos.z), wand_Vicon_pos);
	viconCube->setPosition(wand_Vicon_pos);
	//transformViconData(MHTypes::Point3D(pos.x + dir.x*dirScale, pos.y + dir.y*dirScale, pos.z + dir.z*dirScale), wand_Vicon_holo_pos);
	wand_Vicon_holo_pos = MHTypes::Point3D(pos.x + dir.x*dirScale, pos.y + dir.y*dirScale, pos.z + dir.z*dirScale);
	viconHoloCube->setPosition(wand_Vicon_holo_pos);
	//ss << "Sicon Coords: " << sPoint.x << ", " << sPoint.y << ", " << sPoint.z << endl;
	//ss << "Vicon Coords: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	//str = ss.str();
	//OutputDebugString(str.c_str());


	ss << "Vicon (post-transform) Coords: " << wand_Vicon_pos.x << ", " << wand_Vicon_pos.y << ", " << wand_Vicon_pos.z << endl;
	//ss << "Vicon Coords: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	str = ss.str();
	if(wand_Vicon_pos.x > 0.0f || wand_Vicon_pos.x < 0.0f || wand_Vicon_pos.y > 0.0f || wand_Vicon_pos.y < 0.0f || wand_Vicon_pos.z > 0.0f || wand_Vicon_pos.z < 0.0f){
		//OutputDebugString(str.c_str());
	}
	
}

void Calibrator::setWandViconRotation(MHTypes::EulerAngle eulerAngle)
{

	wand_Vicon_rot = MHTypes::EulerAngle(eulerAngle.pitch , eulerAngle.yaw , eulerAngle.roll );
	//viconHoloCube->setRotation(wand_Vicon_rot);

}
bool Calibrator::isDisplayCalibrated(){
    if (state == 3)
        return true;

    return false;
}

/*
* Initial translation: used to align the Vicon coordinates center with the VisBox coordinates center
*/
void Calibrator::setInitialTranslation()
{
    translation = MHTypes::Point3D(-wand_Vicon_pos.x, -wand_Vicon_pos.y, -wand_Vicon_pos.z);
    cout << "Translation value: " << translation.x << ";" << translation.y << ";" << translation.z << endl;

}//

MHTypes::Point3D Calibrator::getInitialTranslation()
{
    return translation;

}//

/*
* Rotation used to align Vicon coordinates angle with VisBox coordinates angle
*/
void Calibrator::setRotation()
{
    eulerAngle = wand_Vicon_pos.angleDeg(wand_Visbox_pos);
    rotation.fromEuler(eulerAngle);
}//

/*
* Sets the scale to transform Vicon coordinates into VisBox coordinates
*/
void Calibrator::setScale()
{
    scale.x = wand_Visbox_pos.x/wand_Vicon_pos.x;

    cout << "Scale value: " << scale.x << endl;
    //scale.y = wand_Visbox_pos.y/wand_Vicon_pos.y;
    //scale.z = wand_Visbox_pos.z/wand_Vicon_pos.z;
}


double Calibrator::getDistanceCubes(Cube *cube1, Cube *cube2){
    MHTypes::Point3D cube1Coords = cube1->getPosition();
    MHTypes::Point3D cube2Coords = cube2->getPosition();

    double distance =   sqrt(carre(cube1Coords.x - cube2Coords.x) 
        + carre(cube1Coords.y - cube2Coords.y) 
        + carre(cube1Coords.z - cube2Coords.z));  
    // cout << "Distance:" << distance << endl;
    return distance;
}

/*
* Transforms Vicon coordinates into VisBox coordinates 
*/
void Calibrator::transformViconData(MHTypes::Point3D viconCoords, MHTypes::Point3D& visboxCoords)
{
    // Translate
    // transCoords = scaleCoords + LastTranslation
    transCoords = translatePoint(viconCoords, translation);

    // Scale
    //scaleCoords = scale x RotCoords
    scaleCoords = MHTypes::Point3D(	(transCoords.x * scale.x), 
        (transCoords.y * scale.x), 
        (transCoords.z * scale.x));
   
    // Rotate
    // RotCoords = rotationmatrix x finalCoords
    //rotCoords = rotation.rotatePoint(transCoords);

    //finalCoords = scaleCoords;
    visboxCoords.x = scaleCoords.x;
    visboxCoords.y = scaleCoords.y; //+ 0.4; // add offset due to wand calibration
    visboxCoords.z = scaleCoords.z;

	
}

MHTypes::Point3D Calibrator::translatePoint(MHTypes::Point3D point, MHTypes::Point3D trans)
{
    return  MHTypes::Point3D(	(point.x + trans.x), 
        (point.y + trans.y), 
        (point.z + trans.z));	
}


/*
* la fonction carre de type int ac pour argument la variable "nombre" de type int
*/
double Calibrator::carre(double nombre)
{
    return (nombre * nombre);
}


void Calibrator::displayViconPos(){
    cout << " Vicon :"
        << wand_Vicon_pos.x << ";" << wand_Vicon_pos.y << ";" << wand_Vicon_pos.z << endl;
}

/*
*  Starts the calibration process
*/ 
void Calibrator::startViconCalibration(){
    if (state == 3) {
        state = 0;
        targetCube->show();
        cout << "START CALIBRATION";
    }
}

///****************************
// CALIBRATION FILE METHODS
//*****************************

void Calibrator::importViconCalibration(){
	//Right/Up/Backward Configuration offsets
	//float TRANS_OFFSET_X = 40.0f;
	float TRANS_OFFSET_X = 135.0f;
	float TRANS_OFFSET_Y = 470.0f;
	float TRANS_OFFSET_Z = 320.0f;
	//Left/Down/Forward Config offsets
	//float TRANS_OFFSET_X = 0.0f;
	//float TRANS_OFFSET_Y = 0.0f;
	//float TRANS_OFFSET_Z = 0.0f;

	//float TRANS_OFFSET_Y = -500.0f;
	//float TRANS_OFFSET = -400.0f;
	float SCALE_OFFSET = 1.5f;
    // Set initial value for calibration parameters
    translation = MHTypes::Point3D(0.0, 0.0, 0.0);
    scale = MHTypes::Point3D(1.0, 1.0, 1.0);
    //rotation = MHTypes::Point3D(0.0, 0.0, 0.0);

    // Import values from calibration file if existing
    ifstream file;
    file.open("calibration.txt"); //open a file
    // Read translation line
    std::vector<std::string> trans_vector = this->getNextLineAndSplitIntoTokens(file);
	translation.x = ::atof(trans_vector.at(0).c_str()) + TRANS_OFFSET_X; //
    translation.y = ::atof(trans_vector.at(1).c_str()) + TRANS_OFFSET_Y;
    translation.z = ::atof(trans_vector.at(2).c_str()) + TRANS_OFFSET_Z;
    cout << "TRANSLATION Values :" << translation.x << ";" << translation.y << ";" << translation.z << endl;
    // Read scale line
    std::vector<std::string> scale_vector = this->getNextLineAndSplitIntoTokens(file);
    scale.x = ::atof(scale_vector.at(0).c_str())/SCALE_OFFSET;//* SCALE_OFFSET;
    scale.y = ::atof(scale_vector.at(1).c_str())/SCALE_OFFSET;//* SCALE_OFFSET;
    scale.z = ::atof(scale_vector.at(2).c_str())/SCALE_OFFSET;//* SCALE_OFFSET;
    cout << "SCALE Values :" << scale.x << ";" << scale.y << ";" << scale.z << endl;
}

void Calibrator::writeCalibrationFile(){
     ofstream file;
     file.open("calibration.txt"); //open a file
      //file<<"Hello file\n"<<75; //write to it
     file << translation.x << ";" << translation.y << ";" << translation.z <<"\n"; //write to it
     file << scale.x << ";" << scale.y << ";" << scale.z <<"\n"; //write to it

    file.close(); //close it
}




std::vector<std::string> Calibrator::getNextLineAndSplitIntoTokens(std::istream& str)
{
    //cout << "NEXT LINE : " << endl; 
    std::vector<std::string>   result;
    std::string                line;
    std::getline(str,line);

    std::stringstream          lineStream(line);
    std::string                cell;

    while(std::getline(lineStream,cell,';'))
    {
        result.push_back(cell);
       // cout << "Next item line : "  << cell << endl; 
    }
    return result;
}

void Calibrator::render()
{
	//targetCube->render();
	//oglCube->render();
	//viconCube->render();
	viconHoloCube->render();

}