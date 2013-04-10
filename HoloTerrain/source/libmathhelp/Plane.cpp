/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#include "Plane.hpp"

#ifdef MH_WINDOWS
    #include "../MathHelper.hpp"
#else
    #include "MathHelper.hpp"
#endif

using namespace std;
using namespace MHTypes;

Plane::Plane(void)
{
    equation.resize(4, 0);
}//constructor

Plane::Plane(const Point3D &newPoint1,
             const Point3D &newPoint2,
             const Point3D &newPoint3)
{
    equation.resize(4, 0);
    fromPoints(newPoint1, newPoint2, newPoint3);
}//constructor

Plane::~Plane(void)
{
}//destructor

void Plane::fromPoints(const Point3D &newPoint1,
                       const Point3D &newPoint2,
                       const Point3D &newPoint3)
{
    point1 = newPoint1;
    point2 = newPoint2;
    point3 = newPoint3;

    /*
        A = y1 (z2 - z3) + y2 (z3 - z1) + y3 (z1 - z2)
        B = z1 (x2 - x3) + z2 (x3 - x1) + z3 (x1 - x2)
        C = x1 (y2 - y3) + x2 (y3 - y1) + x3 (y1 - y2)
        - D = x1 (y2 z3 - y3 z2) + x2 (y3 z1 - y1 z3) + x3 (y1 z2 - y2 z1)

        Note that if the points are colinear then the normal (A,B,C) as calculated above will be (0,0,0).
    */

    equation.at(0) = (point1.y * (point2.z - point3.z))  +  (point2.y * (point3.z - point1.z))  +  (point3.y * (point1.z - point2.z));
    equation.at(1) = (point1.z * (point2.x - point3.x))  +  (point2.z * (point3.x - point1.x))  +  (point3.z * (point1.x - point2.x));
    equation.at(2) = (point1.x * (point2.y - point3.y))  +  (point2.x * (point3.y - point1.y))  +  (point3.x * (point1.y - point2.y));

    equation.at(3) = point1.x * ((point2.y * point3.z) - (point3.y * point2.z))
                   + point2.x * ((point3.y * point1.z) - (point1.y * point3.z))
                   + point3.x * ((point1.y * point2.z) - (point2.y * point1.z));
    equation.at(3) *= -1;
}//fromPoints

vector<MHFloat> Plane::getEquation(void)
{
    return equation;
}//equation

/*
The normal to the plane is the vector (A,B,C).
*/

MHFloat Plane::getSide(const Point3D &position)
{
    return (equation.at(0) * position.x) + (equation.at(1) * position.y)
         + (equation.at(2) * position.z) + equation.at(3);
}//getSide

bool Plane::lineIntersect(const Point3D &lineA,
                          const Point3D &lineB,
                          Point3D &intersect)
{
    Matrix3x3 matrixOne;//the first matrix in the plane-intercept equation
    Matrix3x3 inverseOne;//the inverse of matrix one
    vector<MHFloat> matrixTwo;//the second matrix in the equation
    Point3D scaledPoint;//temporary variable used by intersection formula
    MHFloat t = 0;//the result of the matrix formula below
    bool result = true;//false if the line is parallel to the plane

    //|t|   |xa-xb   x1-x0   x2-x0|^-1 |xa-x0|
    //|u| = |ya-yb   y1-y0   y2-y0|    |ya-y0|
    //|v|   |za-zb   z1-z0   z2-z0|    |za-z0|

    //### Initialize Matrix One ###
    matrixOne.set(0, lineA.x - lineB.x);
    matrixOne.set(1, point2.x - point1.x);
    matrixOne.set(2, point3.x - point1.x);

    matrixOne.set(3, lineA.y - lineB.y);
    matrixOne.set(4, point2.y - point1.y);
    matrixOne.set(5, point3.y - point1.y);

    matrixOne.set(6, lineA.z - lineB.z);
    matrixOne.set(7, point2.z - point1.z);
    matrixOne.set(8, point3.z - point1.z);

    //### Initialize Matrix Two ###
    matrixTwo.push_back(lineA.x - point1.x);
    matrixTwo.push_back(lineA.y - point1.y);
    matrixTwo.push_back(lineA.z - point1.z);

    //### Calculate the inverse of Matrix One ###
    if (!matrixOne.inverse(inverseOne))
        result = false;

    //### Obtain the intersection with the plane ###
    else
    {
        t = (inverseOne.get(0) * matrixTwo.at(0)) + (inverseOne.get(1) * matrixTwo.at(1)) + (inverseOne.get(2) * matrixTwo.at(2));
        //u = (inverseOne.at(3) * matrixTwo.at(0)) + (inverseOne.at(4) * matrixTwo.at(1)) + (inverseOne.at(5) * matrixTwo.at(2));
        //v = (inverseOne.at(6) * matrixTwo.at(0)) + (inverseOne.at(7) * matrixTwo.at(1)) + (inverseOne.at(8) * matrixTwo.at(2));

        //intersect = lineA + (lineB - lineA)*t

        scaledPoint.x = (lineB.x - lineA.x) * t;
        scaledPoint.y = (lineB.y - lineA.y) * t;
        scaledPoint.z = (lineB.z - lineA.z) * t;

        intersect.x = lineA.x + scaledPoint.x;
        intersect.y = lineA.y + scaledPoint.y;
        intersect.z = lineA.z + scaledPoint.z;
    }//else inverse3x3

    return result;
}//lineIntersect

MHVector Plane::surfaceNormal(void)
{
    MHVector result;//the surface normal to return
    MHVector surface1;//two surface vectors defining this plane
    MHVector surface2;

    //### Calculate the first surface vector ###
    surface1.x = point1.x - point2.x;
    surface1.y = point1.y - point2.y;
    surface1.z = point1.z - point2.z;

    //### Calculate the second surface vector ###
    surface2.x = point2.x - point3.x;
    surface2.y = point2.y - point3.y;
    surface2.z = point2.z - point3.z;

    //### Find the normalized cross-product ###
    result = surface1.crossProduct(surface2);
    result = result.normalize();

    return result;
}//surfaceNormal
