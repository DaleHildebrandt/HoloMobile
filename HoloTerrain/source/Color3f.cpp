//Color3f.cpp
//## Definition ##
//An RGB Color

#include "Color3f.h"

//## Constructor ##
Color3f::Color3f(float red, float green, float blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

//## Destructor ##
Color3f::~Color3f(void)
{

}

//## Getters ##
float Color3f::getRed()
{
	return red;
}
float Color3f::getGreen()
{
	return green;
}
float Color3f::getBlue()
{
	return blue;
}

//## Setters ##
void Color3f::setRed(float red)
{
	this->red = red;
}

void Color3f::setGreen(float green)
{
	this->green = green;
}

void Color3f::setBlue(float blue)
{
	this->blue = blue;
}

void Color3f::setRGB(float color)
{
	this->red = color;
	this->green = color;
	this->blue = color;
}

void Color3f::setRGB(float red, float green, float blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

