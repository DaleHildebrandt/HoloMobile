/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#include "Point3D.hpp"
#include "EulerAngle.hpp"
#include "MHVector.hpp"
#include "../MathHelper.hpp"

using namespace std;
using namespace MHTypes;

Point3D::Point3D(void)
{
    x = 0;
    y = 0;
    z = 0;
}//constructor

Point3D::Point3D(MHFloat newX,
                 MHFloat newY,
                 MHFloat newZ)
{
    x = newX;
    y = newY;
    z = newZ;
}//constructor

Point3D::Point3D(const Point3D &source)
{
    x = source.x;
    y = source.y;
    z = source.z;
}//copy constructor

void Point3D::copyFrom(const Point3D &source)
{
    x = source.x;
    y = source.y;
    z = source.z;
}//copyFrom

MHFloat Point3D::distance(const Point3D &pointTwo)
{
    MHFloat horizDistance = this->x - pointTwo.x;
    MHFloat verticalDistance = this->y - pointTwo.y;
    MHFloat depthDistance = this->z - pointTwo.z;
    MHFloat result = 0;//the distance to return

    result = (horizDistance * horizDistance) + (verticalDistance * verticalDistance) + (depthDistance * depthDistance);
    result = sqrt(result);

    return result;
}//distance

EulerAngle Point3D::angleDeg(const Point3D &pointTwo)
{
    MHTypes::MHFloat length = distance(pointTwo);
    MHTypes::MHVector dirVec;//the direction vector from this point to pointTwo
    EulerAngle result;//the angle to return

    //Find the vector from this point to pointTwo
    dirVec.x = pointTwo.x - x;
    dirVec.y = pointTwo.y - y;
    dirVec.z = pointTwo.z - z;

    //Set pitch to zero
    result.pitch = 0;

    //calculate yaw as arctan(-v.z / v.x) * radToDeg
    result.yaw = atan2(-dirVec.z, dirVec.x);
    result.yaw *= MathHelper::radToDeg();

    //calculate roll as arcsin(v.y / distance) * radToDeg
    result.roll = asin(dirVec.y / length);
    result.roll *= MathHelper::radToDeg();

    return result;   
}//angleDeg
