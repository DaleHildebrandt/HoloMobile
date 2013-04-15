/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#include "Point2D.hpp"
#include <math.h>

#ifdef MH_WINDOWS
    #include "../MathHelper.hpp"
#else
    #include "MathHelper.hpp"
#endif

using namespace MHTypes;
using namespace std;

Point2D::Point2D(void)
{
    x = 0;
    y = 0;
}//constructor

Point2D::Point2D(const Point2D &source)
{
    x = source.x;
    y = source.y;
}//copy constructor

Point2D::Point2D(MHFloat newX,
                 MHFloat newY)
{
    x = newX;
    y = newY;
}//constructor

void Point2D::copyFrom(const Point2D &source)
{
    x = source.x;
    y = source.y;
}//copyFrom

MHFloat Point2D::distance(const Point2D pointTwo)
{
    MHFloat horizDist = 0;//the horizontal and vertical components of the distance
    MHFloat vertDist = 0;

    horizDist = fabs(x - pointTwo.x);
    vertDist = fabs(y - pointTwo.y);

    return sqrt((horizDist * horizDist) + (vertDist * vertDist));
}//distance

MHFloat Point2D::angleDeg(const Point2D pointTwo)
{
    MHFloat horizDist = pointTwo.x - x;
    MHFloat vertDist = pointTwo.y - y;

    return atan2(vertDist, horizDist) * MathHelper::radToDeg();
}//angleDeg

MHFloat Point2D::angleRad(const Point2D pointTwo)
{
    MHFloat horizDist = pointTwo.x - x;
    MHFloat vertDist = pointTwo.y - y;

    return atan2(vertDist, horizDist);
}//angleRad

Point2D Point2D::findPointRad(MHFloat distance,
                              MHFloat angle)
{
    Point2D result;//the remote point to return
    MHFloat opposite = 0;//the vertical distance to the remote point
    MHFloat adjacent = 0;//horizontal distance to the remote point

    opposite = sin(angle) * distance;
    adjacent = cos(angle) * distance;

    result.x = x + adjacent;
    result.y = y + opposite;

    return result;
}//findPointRad
