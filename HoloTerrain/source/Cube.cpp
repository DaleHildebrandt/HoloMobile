//Cube.cpp
//## Definition ##
//Drawable cube object

#include <Windows.h>
#include "Cube.h"
#include <sstream>
using namespace std;

//## Constructor ##
Cube::Cube(void){
	text = "Hello World!";
	cubeSize = 0.05f;
	color = new Color3f(1.0f, 0.0f, 0.0f);
	loc = new MHTypes::Point3D(0.0f, 0.0f, 0.0f);
	trans = new MHTypes::Point3D(0.0f, 0.0f, 0.0f); 
	isHidden = false;
	cubeAngle = zspace::common::Radian(0.0f);
	cubeAxis = zspace::common::Vector3(0.0f, 0.0f, 0.0f);

	//Sets rotation matrix to identity matrix
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
//## Destructor ##
Cube::~Cube(void)
{
	delete loc;
	delete trans;
	delete color;
}

//## Setters ##
void Cube::setSize(float size){
	cubeSize = size;
}
void Cube::setColor(float r, float g, float b)
{
	color->setRGB(r, g, b);
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

void Cube::setText(std::string text)
{
	this->text = text;
}

void Cube::setRotMat(float* rotMat)
{
	for(int i = 0; i < 16; i++){
		rotMatrix[i] = rotMat[i];
	}

	//Store the 4x4 inverse rotation matrix as well
	for(int i = 0; i <16; i++){
		invRotMatrix[i] = rotMatrix[((i%4)*4) + (i/4)];
	}
}

void Cube::setRotation(MHTypes::Quaternion rot)
{
	quat = rot;
}

//## Getters ##
MHTypes::Point3D Cube::getPosition(void)
{
	return MHTypes::Point3D(loc->x + trans->x, loc->y + trans->y, loc->z + trans->z);
}

//## Display ##
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
	int textLength;
  if(!isHidden){
	  glColor3f(color->getRed(), color->getGreen(), color->getBlue()); //set color
	  glMatrixMode(GL_MODELVIEW);
	  glPushMatrix(); //Save pre-cube view matrix
	  
	  glTranslatef(loc->x, loc->y, loc->z); //Position cube
	  glTranslatef(trans->x, trans->y, trans->z); //Additional translation to cube
	  //glMultMatrixf(rotMatrix); //Rotate cube

	  // Render the cube
	  glBegin(GL_QUADS);

	  const float halfSize = cubeSize/2.0f;

	  //Draw each textured face of the cube
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
	  if(!text.empty()){

		textLength = glutStrokeLength(GLUT_STROKE_ROMAN, (unsigned char*)(text.c_str()));
		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glLineWidth(3.0f);
		glTranslatef(0.0f, 0.01f, 0.0f);
		glMultMatrixf(invRotMatrix);
		//glRotatef(90,0.0f, 1.0f, 0.0f);
		glScalef(1/20000.0f, 1/20000.0f, 200);
		glTranslatef(-textLength/2.0f, 0.0f, 0.0f);

		glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)(text.c_str()));
		glPopMatrix();




	  }
	  
	  glPopMatrix(); //Restore pre-cube view matrix
  }	

}