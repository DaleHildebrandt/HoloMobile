#include <Windows.h>
#include "Cube.h"
#include <sstream>
using namespace std;
Cube::Cube(void){

	red = 1.0f;
	green = 0.0f;
	blue = 0.0f;
	cubeSize = 0.05f;
	loc = new MHTypes::Point3D(0.0f, 0.0f, 0.0f);
	trans = new MHTypes::Point3D(0.0f, 0.0f, 0.0f); 
	isHidden = false;
	cubeAngle = zspace::common::Radian(0.0f);
	cubeAxis = zspace::common::Vector3(0.0f, 0.0f, 0.0f);
	rotMatrix[0] = 1.0f;
	rotMatrix[1] = 0.0f;
	rotMatrix[2] = 0.0f;
	rotMatrix[3] = 0.0f;
	rotMatrix[4] = 0.0f;
	rotMatrix[5] = 1.0f;
	rotMatrix[6] = 0.0f;
	rotMatrix[7] = 0.0f;
	rotMatrix[8] = 0.0f;
	rotMatrix[9] = 0.0f;
	rotMatrix[10] = 1.0f;
	rotMatrix[11] = 0.0f;
	rotMatrix[12] = 0.0f;
	rotMatrix[13] = 0.0f;
	rotMatrix[14] = 0.0f;
	rotMatrix[15] = 1.0f;

}

void Cube::setSize(float size){
	cubeSize = size;
}
void Cube::setColor(float r, float g, float b)
{
	red = r;
	green = g;
	blue = b;
}
void Cube::setTranslation(MHTypes::Point3D trans_point)
{
	trans = new MHTypes::Point3D(trans_point.x, trans_point.y, trans_point.z);


}
void Cube::setPosition(MHTypes::Point3D point)
{
	loc = new MHTypes::Point3D(point.x, point.y, point.z);

}

void Cube::setRotation(zspace::common::Radian angle, zspace::common::Vector3 axis)
{
	cubeAngle = angle;
	cubeAxis = axis;

}

void Cube::setRotMat(float* rotMat)
{
	for(int i = 0; i < 16; i++){
		rotMatrix[i] = rotMat[i];
	}

}

void Cube::setRotation(MHTypes::Quaternion rot)
{
	quat = rot;
}


MHTypes::Point3D Cube::getPosition(void)
{
	return MHTypes::Point3D(loc->x + trans->x, loc->y + trans->y, loc->z + trans->z);
}

void Cube::hide()
{
	isHidden = true;
}
void Cube::show()
{
	isHidden = false;
}
void Cube::render()
{
	/*string str;
	ostringstream ss;

	ss << "roll: " << eulerAngle.roll << " pitch: " << eulerAngle.pitch << " yaw: " << eulerAngle.yaw << endl;
	str = ss.str();
	OutputDebugString(str.c_str());*/

  if(!isHidden){
	  glColor3f(red,green,blue);
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix();
	  
	  glTranslatef(loc->x, loc->y, loc->z);
	  glTranslatef(trans->x, trans->y, trans->z);
	  //glRotatef(cubeAngle.valueDegrees(), cubeAxis.x, cubeAxis.y, cubeAxis.z); 
		//glRotatef(-eulerAngle.roll, 0.0, 0.0, 1.0); //roll
		//glRotatef(-eulerAngle.yaw, 0.0, 1.0, 0.0); //yaw
	    //glRotatef(-eulerAngle.pitch, 1.0, 0.0, 0.0); //pitch
	  glMultMatrixf(rotMatrix);

	  


	  // Render the cube
	  glBegin(GL_QUADS);

	  
	  const float halfSize = cubeSize/2.0f;

	  // Side 1
	  glNormal3f(0.0f, 0.0f, -1.0f);
	  glTexCoord2d(1.0,0.0); glVertex3f(-halfSize, -halfSize, -halfSize);
	  glTexCoord2d(1.0,1.0); glVertex3f(-halfSize,  halfSize, -halfSize);
	  glTexCoord2d(0.0,1.0); glVertex3f( halfSize,  halfSize, -halfSize);        
	  glTexCoord2d(0.0,0.0); glVertex3f( halfSize, -halfSize, -halfSize);		

	  // Side 2
	  glNormal3f(0.0f, 0.0f, 1.0f);
	  glTexCoord2d(0.0,1.0); glVertex3f(-halfSize,  halfSize, halfSize);
	  glTexCoord2d(0.0,0.0); glVertex3f(-halfSize, -halfSize, halfSize);        
	  glTexCoord2d(1.0,0.0); glVertex3f( halfSize, -halfSize, halfSize);
	  glTexCoord2d(1.0,1.0); glVertex3f( halfSize,  halfSize, halfSize);

	  // Side 3
	  glNormal3f(0.0f, 1.0f, 0.0f);
	  glTexCoord2d(0.0,1.0); glVertex3f(-halfSize, halfSize, -halfSize);
	  glTexCoord2d(0.0,0.0); glVertex3f(-halfSize, halfSize,  halfSize);
	  glTexCoord2d(1.0,0.0); glVertex3f( halfSize, halfSize,  halfSize);        
	  glTexCoord2d(1.0,1.0); glVertex3f( halfSize, halfSize, -halfSize);

	  // Side 4
	  glNormal3f(0.0f, -1.0f, 0.0f);
	  glTexCoord2d(0.0,1.0); glVertex3f(-halfSize, -halfSize,  halfSize);
	  glTexCoord2d(0.0,0.0); glVertex3f(-halfSize, -halfSize, -halfSize);
	  glTexCoord2d(1.0,0.0); glVertex3f( halfSize, -halfSize, -halfSize);
	  glTexCoord2d(1.0,1.0); glVertex3f( halfSize, -halfSize,  halfSize);

	  // Side 5
	  glNormal3f(1.0f, 0.0f, 0.0f);
	  glTexCoord2d(1.0,1.0); glVertex3f( halfSize,  halfSize, -halfSize);
	  glTexCoord2d(0.0,1.0); glVertex3f( halfSize,  halfSize,  halfSize);        
	  glTexCoord2d(0.0,0.0); glVertex3f( halfSize, -halfSize,  halfSize);
	  glTexCoord2d(1.0,0.0); glVertex3f( halfSize, -halfSize, -halfSize);

	  // Side 6
	  glNormal3f(-1.0f, 0.0f, 0.0f);
	  glTexCoord2d(0.0,1.0); glVertex3f(-halfSize,  halfSize, -halfSize);
	  glTexCoord2d(0.0,0.0); glVertex3f(-halfSize, -halfSize, -halfSize);        
	  glTexCoord2d(1.0,0.0); glVertex3f(-halfSize, -halfSize,  halfSize);
	  glTexCoord2d(1.0,1.0); glVertex3f(-halfSize,  halfSize,  halfSize);


	  glEnd();

	  glPopMatrix();

  }	

}