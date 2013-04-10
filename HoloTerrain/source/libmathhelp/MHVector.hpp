/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#ifndef MH_VECTOR
#define MH_VECTOR

#include "MHSettings.hpp"

namespace MHTypes
{
    class Point3D;
    class Quaternion;

    class MH_INTERFACE MHVector
    {
        public:
            //constructors
            MHVector(void);
            MHVector(MHFloat newX,
                     MHFloat newY,
                     MHFloat newZ);
            MHVector(const MHVector &source);

            //Copies the values from source into this vector
            void copyFrom(const MHVector &source);

            //This vector should have one or more of x, y or z non-zero. We create a Point3D
            //at x, y, z, and rotate it by the quaternion. The resulting position is stored
            //as the new vector. If only one of x, y or z is non-zero, we're essentially taking
            //a forward direction and causing it to point towards the direction given by the
            //quaternion.
            void fromQuaternion(const Quaternion &source);

            //Returns the cross product between this vector and vectorTwo
            MHVector crossProduct(const MHVector &vectorTwo);

            //Return a vector with the same direction as this one, but with a length of 1
            MHVector normalize(void);

            //Returns the length of this vector
            MHFloat length(void);

            //the vector's magnitude and direction. Can be thought of as the end-point of a line segment
            //that starts at the origin
            MHFloat x;
            MHFloat y;
            MHFloat z;

    };//MHVector
}//namespace MathHelperTypes

#endif
