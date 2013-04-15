#pragma once
#include "MathHelper.hpp"
#include <GL\glew.h>
#include "ZSpace\Common\Math\Vector3.h"
#include "ZSpace\Common\Math\Math.h"
#include "Color3f.h"

class Cube{

public:
	Cube::Cube(void);
	Cube::~Cube(void);
	void setPosition(MHTypes::Point3D point);
	void setTranslation(MHTypes::Point3D trans_point);
	void setRotation(MHTypes::Quaternion rot);
	MHTypes::Point3D getPosition();
	void setSize(float cubeSize);
	void setColor(float r, float g, float b);
	void render(void);
	void hide(void);
	void show(void);
	void setRotation(zspace::common::Radian angle, zspace::common::Vector3 axis);
	void setRotMat(float* rotMat);

private:
	MHTypes::EulerAngle eulerAngle;
	MHTypes::Quaternion quat;
	float rotMatrix[16];
	zspace::common::Radian cubeAngle;
	zspace::common::Vector3 cubeAxis;
	float cubeSize;
	Color3f* color;
	bool isHidden;
	MHTypes::Point3D *loc;
	MHTypes::Point3D *trans;
	MHTypes::Point3D myLoc;
};