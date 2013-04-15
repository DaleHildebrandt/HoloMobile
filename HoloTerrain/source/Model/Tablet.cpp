#include "Tablet.h"

using namespace std;

Tablet::Tablet(Calibrator& calib)
{
    calibrator = calib;


	//define attached hologram
	holoCube = new Cube();
    holoCube->setPosition(MHTypes::Point3D(0.0, 0.0, 0.0));
	holoCube->setColor(1.0f, 1.0f, 1.0f);
	holoCube->setSize(0.03f);




	dirScale = 0.15f;
	holoPos = MHTypes::Point3D(0.0, 0.0, 0.0);

    x_ref_abs = MHTypes::MHVector(1.0, 0.0, 0.0);
    y_ref_abs = MHTypes::MHVector(0.0, 1.0, 0.0);
    z_ref_abs = MHTypes::MHVector(0.0, 0.0, 1.0);

    // init ref vectors
    x_ref = MHTypes::MHVector(0.0, 0.0, 0.0);
    y_ref = MHTypes::MHVector(0.0, 0.0, 0.0);
    z_ref = MHTypes::MHVector(0.0, 0.0, 0.0);

    // init center
    tablet_center = MHTypes::Point3D(0.0, 0.0, 0.0);
    x_ref_point = MHTypes::Point3D(0.0, 0.0, 0.0);
    y_ref_point = MHTypes::Point3D(0.0, 0.0, 0.0);
    z_ref_point = MHTypes::Point3D(0.0, 0.0, 0.0);

    top_right = MHTypes::Point3D(0.0, 0.0, 0.0);
    top_left = MHTypes::Point3D(0.0, 0.0, 0.0);
    bottom_right = MHTypes::Point3D(0.0, 0.0, 0.0);
    bottom_left = MHTypes::Point3D(0.0, 0.0, 0.0);

    eulerAngle = MHTypes::EulerAngle();
	quat = MHTypes::Quaternion();

	initSocket();

	terr = new Terrain(server);
	terr->setPosition(MHTypes::Point3D(0.0, 0.0, 0.0));
	terrainStart = false;
	terrainEnd = false;

}

Tablet::~Tablet(void)
{
}


void Tablet::updatePosition(MHTypes::Point3D new_bottom_left,
                            MHTypes::Point3D new_bottom_right,
                            MHTypes::Point3D new_top_left,
                            MHTypes::Point3D new_top_right){

        // Assign new values 
        calibrator.transformViconData( MHTypes::Point3D(new_bottom_left.x, new_bottom_left.y, new_bottom_left.z), bottom_left);
        calibrator.transformViconData( MHTypes::Point3D(new_bottom_right.x, new_bottom_right.y, new_bottom_right.z), bottom_right);
        calibrator.transformViconData( MHTypes::Point3D(new_top_left.x, new_top_left.y, new_top_left.z), top_left );
        calibrator.transformViconData( MHTypes::Point3D(new_top_right.x, new_top_right.y, new_top_right.z), top_right);

        // Calculate virtual points (center and holo)
        MHTypes::Point3D right_segment = MHTypes::Point3D(top_right.x - bottom_right.x, top_right.y - bottom_right.y, top_right.z - bottom_right.z);
        MHTypes::Point3D virtual_top_right = MHTypes::Point3D(top_right.x + right_segment.x, top_right.y + right_segment.y, top_right.z + right_segment.z);
        
        // Get holo position
        getMiddlePoint(top_left, virtual_top_right, tablet_holo);

        // Get center of tablet
        getMiddlePoint(top_left, bottom_right, tablet_center);

        // Update reference points and vectors 
        updateReferenceVectors();

		zspace::common::Vector3 dir = zspace::common::Vector3(top_right.x - bottom_right.x, top_right.y - bottom_right.y, top_right.z - bottom_right.z);
		dir.normalise();

		holoPos = MHTypes::Point3D(tablet_center.x + dir.x*dirScale, tablet_center.y + dir.y*dirScale, tablet_center.z + dir.z*dirScale);
		holoCube->setPosition(holoPos);
		//holoCube->setPosition(tablet_center);
		terr->setPosition(holoPos);


        // Calculate rotation
        //calculateRotation();
}

void Tablet::updateRotation(MHTypes::Quaternion tablet_rotation)
{
	quat = tablet_rotation;
	holoCube->setRotation(quat);
	terr->setRotation(quat);

	//quat.fromEuler(eulerAngle);
	rotMat = quat.getRotationMatrix();
	
	//Fill rotMatrix as transpose/inverse
	//col 1
	//glRotMat[0] = rotMat.get(0);
	//glRotMat[1] = rotMat.get(3);
	//glRotMat[2] = rotMat.get(6);
	//glRotMat[3] = 0;
	////col 2
	//glRotMat[4] = rotMat.get(1);
	//glRotMat[5] = rotMat.get(4);
	//glRotMat[6] = rotMat.get(7);
	//glRotMat[7] = 0;
	////col 3
	//glRotMat[8] = rotMat.get(2);
	//glRotMat[9] = rotMat.get(5);
	//glRotMat[10] = rotMat.get(8);
	//glRotMat[11] = 0;
	////col 4
	//glRotMat[12] = 0;
	//glRotMat[13] = 0;
	//glRotMat[14] = 0;
	//glRotMat[15] = 1;

	//Convert to 4x4 rotation matrix
	//col 1
	glRotMat[0] = rotMat.get(0);
	glRotMat[1] = rotMat.get(1);
	glRotMat[2] = rotMat.get(2);
	glRotMat[3] = 0;
	//col 2
	glRotMat[4] = rotMat.get(3);
	glRotMat[5] = rotMat.get(4);
	glRotMat[6] = rotMat.get(5);
	glRotMat[7] = 0;
	//col 3
	glRotMat[8] = rotMat.get(6);
	glRotMat[9] = rotMat.get(7);
	glRotMat[10] = rotMat.get(8);
	glRotMat[11] = 0;
	//col 4
	glRotMat[12] = 0;
	glRotMat[13] = 0;
	glRotMat[14] = 0;
	glRotMat[15] = 1;

	holoCube->setRotMat(glRotMat);
	terr->setRotMat(glRotMat);
}

void Tablet::updateReferenceVectors(){
    // Get X_REF (Vector from center to middle of right side)
    getMiddlePoint(bottom_right, top_right, x_ref_point);
    x_ref = MHTypes::MHVector(x_ref_point.x - tablet_center.x, x_ref_point.y - tablet_center.y, x_ref_point.z - tablet_center.z);
    

    // Get Z_REF (Vector from center to middle of bottom side)
    getMiddlePoint(bottom_left, bottom_right, z_ref_point);
    z_ref = MHTypes::MHVector(z_ref_point.x - tablet_center.x, z_ref_point.y - tablet_center.y, z_ref_point.z - tablet_center.z);
   
    // Calculate Y_REF (perpendicular to the tablet plane)(vectorial product)
    y_ref.x = - x_ref.y*z_ref.z + x_ref.z*z_ref.y;
    y_ref.y = - x_ref.z*z_ref.x + x_ref.x*z_ref.z;
    y_ref.z = - x_ref.x*z_ref.y + x_ref.y*z_ref.x;
   
    // Recalculate Z_Ref to be sure it is perpendicular to tohers
    z_ref.x =  x_ref.y*y_ref.z - x_ref.z*y_ref.y;
    z_ref.y =  x_ref.z*y_ref.x - x_ref.x*y_ref.z;
    z_ref.z =  x_ref.x*y_ref.y - x_ref.y*y_ref.x;

    // normalize vectors and update points
    MHTypes::MHVector x_ref_norm = x_ref.normalize();
    x_ref =  MHTypes::MHVector(x_ref_norm.x, x_ref_norm.y, x_ref_norm.z);
    x_ref_point = addPointVector(tablet_center, x_ref_norm);

    MHTypes::MHVector z_ref_norm = z_ref.normalize();
    z_ref =  MHTypes::MHVector(z_ref_norm.x, z_ref_norm.y, z_ref_norm.z);
    z_ref_point = addPointVector(tablet_center, z_ref_norm);

    MHTypes::MHVector y_ref_norm = y_ref.normalize();
    y_ref =  MHTypes::MHVector(y_ref_norm.x, y_ref_norm.y, y_ref_norm.z);
    y_ref_point = addPointVector(tablet_center, y_ref_norm);

    //y_ref_point = addPointVector(tablet_center, y_ref);
    //y_ref = x_ref.crossProduct(z_ref);
    
}

void Tablet::calculateRotation(){
    // Each one of the three rotation angles around the three rotation axis
    
    MHTypes::MHFloat angle_x ; // in radians
    MHTypes::MHFloat angle_y ; // in radians
    MHTypes::MHFloat angle_z ; // in radians

     // Rotation in X axis :
    // Do dot product of y_ref and y_ref_abs then get arc cosine of value = angle
    float dot_product_x = (y_ref.z*y_ref_abs.z + y_ref.y*y_ref_abs.y)/(sqrt(y_ref.z*y_ref.z + y_ref.y*y_ref.y));
    angle_x = MHTypes::MHFloat(acos(dot_product_x));
     // Rotation in Y axis :
    float  dot_product_y = (z_ref.x*z_ref_abs.x + z_ref.z*z_ref_abs.z)/(sqrt(z_ref.z*z_ref.z + z_ref.x*z_ref.x));
    angle_y = MHTypes::MHFloat(acos(dot_product_y));
    // Rotation in Z axis :
    float  dot_product_z = (x_ref.x*x_ref_abs.x + x_ref.y*x_ref_abs.y)/(sqrt(x_ref.y*x_ref.y + x_ref.x*x_ref.x));
    angle_z = MHTypes::MHFloat(acos(dot_product_z));


     cout << //"** Vector_X (" << x_ref.x << ";" <<   x_ref.y << ";" <<  x_ref.z <<
             "** Vector_Y (" << y_ref.x << ";" <<   y_ref.y << ";" <<  y_ref.z 
             //"** Vector_Z (" << z_ref.x << ";" <<   z_ref.y << ";" <<  z_ref.z 
             << " << ANGLE X" << angle_x << endl;
   // cout << "** ANGLE Z" << angle_z << endl;
   // cout << "** DOT X  " << dot_product_x << "** DOT Y  " << dot_product_y <<  "** DOT Z  " << dot_product_z << endl;
   //cout << "** RAD X  " << angle_x << "** RAD Y  " << angle_y <<  "** RAD Z  " << angle_z << endl;

    angle_x = angle_x*MathHelper::radToDeg();
    angle_y = angle_y*MathHelper::radToDeg();
    angle_z = angle_z*MathHelper::radToDeg();

    // X_AXIS
    if (this->y_ref.z < 0) // angle negatif
        angle_x = 360 - angle_x;

    // Y_AXIS
    if (this->z_ref.x < 0) // angle negatif
        angle_y = 360  -angle_y;
    // Z_AXIS
    if (this->x_ref.y < 0) // angle negatif
        angle_z = 360  -angle_z;

  
   //cout << "** ANGLE X  " << angle_x << "** ANGLE Y  " << angle_y <<  "** ANGLE Z  " << angle_z << endl;

   //rotationFromAngles(angle_x, angle_y, angle_z);
 
    this->eulerAngle.pitch = angle_x;
    this->eulerAngle.yaw = angle_y;
    this->eulerAngle.roll = angle_z;

    this->eulerAngle.yaw = asin(z_ref.x);
    this->eulerAngle.roll = acos(x_ref.x/(cos(asin(z_ref.x))));
    this->eulerAngle.pitch = z_ref.z * cos(this->eulerAngle.roll) / x_ref.x;

   // this->eulerAngle = MHTypes::Point3D(x_ref_abs.x, x_ref_abs.y, x_ref_abs.z).angleDeg(MHTypes::Point3D(x_ref.x, x_ref.y, x_ref.z));
    // this->eulerAngle = tablet_center.angleDeg(x_ref_point);
}



void Tablet::rotationFromAngles(MHTypes::MHFloat pitch,
                          MHTypes::MHFloat yaw,
                          MHTypes::MHFloat roll)
{
   
    MHTypes::MHFloat A = cos(pitch);
    MHTypes::MHFloat B = sin(pitch);
    MHTypes::MHFloat C = cos(yaw);
    MHTypes::MHFloat D = sin(yaw);
    MHTypes::MHFloat E = cos(roll);
    MHTypes::MHFloat F = sin(roll);

    MHTypes::MHFloat AD = A * D;
    MHTypes::MHFloat BD = B * D;

    // | x' |   |  CE      -CF       D  |   | x |
    // | y' | = |  BDE+AF  -BDF+AE  -BC | . | y |
    // | z' |   | -ADE+BF   ADF+BE   AC |   | z |

    rotation.set(0, C * E);
    rotation.set(1, -C * F);
    rotation.set(2, D);

    rotation.set(3, BD * E + A * F);
    rotation.set(4, -BD * F + A * E);
    rotation.set(5,  -B * C);

    rotation.set(6, -AD * E + B * F);
    rotation.set(7,  AD * F + B * E);
    rotation.set(8,   A * C);
    
    this->eulerAngle = rotation.toEuler();
}//fromEuler

/*
void Tablet::calculateRotation(){

    // Rotation in Z axis :
    // Do dot product of x_ref and x_ref_abs then get arc cosine of value = angle
    float dot_product = x_ref.x*x_ref_abs.x + x_ref.y*x_ref_abs.y;
    MHTypes::MHFloat angle_radians = MHTypes::MHFloat(acos(dot_product));
    MHTypes::MHFloat angle_degrees = angle_radians*MathHelper::radToDeg();
    
    if (this->x_ref.y < 0) // angle negatif
        angle_degrees = -angle_degrees;

    //cout << "*DOT PRODUCT: "<< dot_product << " X_REF ("<< x_ref.x <<","<< x_ref.y << "," << x_ref.z << ")* Z ROT ANGLE : " << angle_degrees << endl;
    //cout << "** ANGLE Z" << angle_degrees;
    eulerAngle.roll = angle_degrees;

     // Rotation in X axis :
    // Do dot product of y_ref and y_ref_abs
    // then get arc cosine of value = angle
    dot_product = y_ref.z*y_ref_abs.z + y_ref.y*y_ref_abs.y;
    angle_radians = MHTypes::MHFloat(acos(dot_product));
    angle_degrees = angle_radians*MathHelper::radToDeg();
    
    if (this->y_ref.z < 0) // angle negatif
        angle_degrees = -angle_degrees;

    //cout << "*DOT PRODUCT: "<< dot_product << " X_REF ("<< x_ref.x <<","<< x_ref.y << "," << x_ref.z << ")* Z ROT ANGLE : " << angle_degrees << endl;
    cout << "** ANGLE X " << angle_degrees;
    eulerAngle.pitch = angle_degrees;

}*/



void Tablet::getMiddlePoint(MHTypes::Point3D p1, MHTypes::Point3D p2, MHTypes::Point3D& middle){
    middle.x = (p1.x + p2.x)/2;
    middle.y = (p1.y + p2.y)/2;
    middle.z = (p1.z + p2.z)/2;
}

// NOT USED AT THE MOMENT (ERASE)
void Tablet::setTabletQuatOrientation(MHTypes::Quaternion desiredTabletQuat){
    MHTypes::Quaternion rotateQuat;//the amount to rotate the tablet angle

    // transform quaternion x y z
    MHTypes::Point3D newPoint;
    calibrator.transformViconData(MHTypes::Point3D(desiredTabletQuat.x,
                                                    desiredTabletQuat.z,
                                                    - desiredTabletQuat.y), newPoint);
    desiredTabletQuat.x = newPoint.x;
    desiredTabletQuat.y = newPoint.y;
    desiredTabletQuat.z = newPoint.z;

    //### Rotate origTabletQuat towards desiredTabletQuat ###
    slerpAmount = 0.5;
    rotateQuat = origTabletQuat.slerp(desiredTabletQuat, slerpAmount, MHTypes::Q_SHORT);
    eulerAngle = rotateQuat.getRotationMatrix().toEuler();

   /* std::cout << "EULER FROM QUAT: pitch " << tablet_angle.pitch << "; " << 
        "roll " << tablet_angle.roll << "; " << 
                          "yaw " << tablet_angle.yaw << std::endl; */
}

void Tablet::initSocket(){
	HANDLE ht1;
	unsigned uiThread1ID;

	server = new TabletServer();
	// Start the socket
	server->StartSocket();

	// Start thread for connecting to the Tablet
	ht1 = (HANDLE)_beginthreadex( NULL, // security
			0, // stack size
			TabletServer::ListenSocketWrapper,// entry-point-function
			server, // arg list holding the "this" pointer
			//CREATE_SUSPENDED, // so we can later call ResumeThread()
			0,
			&uiThread1ID );

	//boost::thread* t = new boost::thread(boost::bind(&TabletServer::ListenSocketWrapper, &server));
	//boost::thread* thr = new boost::thread(boost::bind(&Foo::some_function, &f));
}

void Tablet::send(const char* str){
// cout << "Sending this event: " << str << endl;
	//if (connectTablet)
	server->sendData(str);
}


void Tablet::renderHolo()
{
	//send("Hello World!\n");
	glPushMatrix();
	//holoCube->render();
	terr->render();
	glPopMatrix();
}

//void Tablet::setTerrainStart(){
//	terrainStart = true;
//	terr->setStart();
//}
//
//void Tablet::setTerrainEnd(){
//	terrainEnd = true;
//	terr->setEnd();
//}


MHTypes::Point3D Tablet::getPosition(){
    return tablet_center;
}

MHTypes::Point3D Tablet::getHoloPosition(){
    return tablet_holo;
}

MHTypes::EulerAngle Tablet::getRotation(){
	quat.fromEuler(eulerAngle);
	eulerAngle = quat.getRotationMatrix().toEuler();
    return eulerAngle;
}

float* Tablet::getRotMat()
{

	return glRotMat;
}
Terrain* Tablet::getTerrain()
{
	return terr;
}

MHTypes::Point3D Tablet::getCenter(){
    return this->tablet_center;
}

MHTypes::Point3D Tablet::getXRefPoint(){
    return x_ref_point;
}

MHTypes::Point3D Tablet::getYRefPoint(){
     return y_ref_point;
}

MHTypes::Point3D Tablet::getZRefPoint(){
     return z_ref_point;
}

MHTypes::Point3D Tablet::addPointVector(MHTypes::Point3D point, MHTypes::MHVector vector){
    return MHTypes::Point3D(point.x + vector.x, point.y + vector.y, point.z + vector.z);
}

MHTypes::Point3D Tablet::getTopRight(){
    return this->top_right;
}

MHTypes::Point3D Tablet::getBottomRight(){
    return this->bottom_right;
}

MHTypes::Point3D Tablet::getTopLeft(){
      return this->top_left;
}

MHTypes::Point3D Tablet::getBottomLeft(){
     return this->bottom_left;
}