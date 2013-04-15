/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#ifndef MH_PLANE
#define MH_PLANE

#include <vector>
#include "Point3D.hpp"
#include "MHSettings.hpp"

namespace MHTypes
{
    class MHVector;

    class MH_INTERFACE Plane
    {
        public:
            //constructors
            Plane(void);
            Plane(const Point3D &newPoint1,
                  const Point3D &newPoint2,
                  const Point3D &newPoint3);

            //destructor
            ~Plane(void);

            //constructs a plane from the specified points
            void fromPoints(const Point3D &newPoint1,
                            const Point3D &newPoint2,
                            const Point3D &newPoint3);

            //Given two points that make up a line, finds the point of intersection
            //between the line and this plane. Returns false if the line is parallel
            //to the plane.
            bool lineIntersect(const Point3D &lineA,
                               const Point3D &lineB,
                               Point3D &intersect);

            //Returns > 0 if position is on the same side as the surface normal,
            // == 0 if position is right on the plane
            // < 0 if position is on the opposite side of the surface normal
            MHFloat getSide(const Point3D &position);


            //Given three points that make up a plane, return a normal vector with length 1 that is
            //perpendicular to the front surface. The front is the side where pointOne, pointTwo and
            //pointThree are arranged in a clockwise direction. (clockwise winding)
            MHVector surfaceNormal(void);

            //Returns a vector with four elements A, B, C and D from the general equation
            //Ax + By + Cz + D = 0
            std::vector<MHTypes::MHFloat> getEquation(void);

        private:
            //Four elements with the constants from the general equation of
            //a plane Ax + By + Cz + D = 0
            std::vector<MHTypes::MHFloat> equation;

            //Three points that lie on this plane
            Point3D point1;
            Point3D point2;
            Point3D point3;

    };//class Plane
}//namespace MathHelperTypes

#endif
