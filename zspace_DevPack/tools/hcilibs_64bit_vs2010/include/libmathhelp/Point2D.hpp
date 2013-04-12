/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#ifndef MH_POINT2D
#define MH_POINT2D

#include "MHSettings.hpp"

namespace MHTypes
{
    class MH_INTERFACE Point2D
    {
        public:
            //constructors
            Point2D(void);
            Point2D(const Point2D &source);
            Point2D(MHTypes::MHFloat newX,
                    MHTypes::MHFloat newY);

            //Copies coordinates from source
            void copyFrom(const Point2D &source);

            //Returns the distance between this point and pointTwo
            MHTypes::MHFloat distance(const Point2D pointTwo);

            //Returns the angle in degrees between this point and pointTwo
            //The angle is measured counterclockwise from the positive x axis
            MHTypes::MHFloat angleDeg(const Point2D pointTwo);

            //Returns the angle in radians between this point and pointTwo,
            //measured counterclockwise from the positive x axis.
            MHTypes::MHFloat angleRad(const Point2D pointTwo);

            //Returns a point at the specified angle and distance from this point.
            //Angle should be in radians
            Point2D findPointRad(MHTypes::MHFloat distance,
                                 MHTypes::MHFloat angle);

            //the point's coordinates
            MHTypes::MHFloat x;
            MHTypes::MHFloat y;
    };//Point2D
}//namespace MathHelperTypes

#endif
