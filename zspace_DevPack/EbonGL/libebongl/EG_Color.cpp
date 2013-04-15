#include "EG_Color.hpp"
#include <iostream>

using namespace EbonGL;
using namespace std;

EG_Color::EG_Color(void)
{
    theColor.resize(4, 1);
}//constructor

EG_Color::EG_Color(GLfloat red,
                   GLfloat green,
                   GLfloat blue,
                   GLfloat alpha)
{
    theColor.push_back(red);
    theColor.push_back(green);
    theColor.push_back(blue);
    theColor.push_back(alpha);
}//constructor

EG_Color::EG_Color(const EG_Color &source)
{
    for (unsigned int i = 0; i < source.theColor.size(); i++)
        theColor.push_back(source.theColor.at(i));
}//copy constructor

EG_Color::~EG_Color(void)
{
}//destructor

EG_Color& EG_Color::operator= (const EG_Color &source)
{
    if (this != &source)
    {
        for (unsigned int i = 0; i < source.theColor.size(); i++)
            theColor.at(i) = source.theColor.at(i);
    }//if this

    return *this;
}//=

void EG_Color::setColor(GLfloat red,
                        GLfloat green,
                        GLfloat blue,
                        GLfloat alpha)
{
    theColor.at(0) = red;
    theColor.at(1) = green;
    theColor.at(2) = blue;
    theColor.at(3) = alpha;
}//setColor

void EG_Color::setColor(const EG_Color &newColor)
{
    for (unsigned int i = 0; i < newColor.theColor.size(); i++)
        theColor.at(i) = newColor.theColor.at(i);
}//setColor

GLfloat* EG_Color::toArray(void)
{
    return &theColor.at(0);
}//toArray

GLfloat EG_Color::red(void) const
{
    return theColor.at(0);
}//getRed

GLfloat EG_Color::green(void) const
{
    return theColor.at(1);
}//getGreen

GLfloat EG_Color::blue(void) const
{
    return theColor.at(2);
}//getBlue

GLfloat EG_Color::alpha(void) const
{
    return theColor.at(3);
}//getAlpha
