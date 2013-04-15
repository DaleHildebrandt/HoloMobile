/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

/* DESCRIPTION
    Quaternions are the preferred method of performing rotational interpolation,
    and for performing calculations between angles. (say, combining them)
    Quaternions don't suffer from gimbal lock (unlike Euler angles), and provide
    smooth interpolations, unlike rotation matrices.

    Interpolation is provided by Spherical Linear intERPolation (slerp).

    The proper order is:

    1) Convert Euler angles to quaternions

    2) Perform whatever calculations are needed (such as slerp)

    3) Convert the resulting quaternions to rotation matrixes

    4) Rotate points using the matrixes.

    Many of these functions were adapted from the Allegro Gaming Library, and
    from the Math and Quaternion FAQ.
*/

#ifndef MH_QUATERNION
#define MH_QUATERNION

#include <vector>
#include "MHSettings.hpp"

namespace MHTypes
{
    class Matrix3x3;
    class EulerAngle;

    //The method to use for spherical linear interpolation
    enum SlerpMethod
    {
        Q_SHORT,              //use the shortest path
        Q_LONG,               //rotation will be greater than 180 degrees
        Q_CLOCKWISE,          //rotation will be clockwise when viewed from above
        Q_COUNTER_CLOCKWISE,  //rotation will be counter-clockwise when viewed from above
        Q_USER                //quaternions are interpolated exactly as given
    };//SlerpMethod

    class MH_INTERFACE Quaternion
    {
        public:
            //constructors
            Quaternion(void);
            Quaternion(const Quaternion &source);

            //Creates a rotation quaternion from the given Euler angles, in degrees.
            void fromEuler(const EulerAngle &eulerAngles);
            void fromEuler(MHTypes::MHFloat pitch,
                           MHTypes::MHFloat yaw,
                           MHTypes::MHFloat roll);

            //Returns the 3x3 rotation matrix corresponding to this quaternion
            //(You could then use the matrix to get Euler angles)
            Matrix3x3 getRotationMatrix(void) const;

            //Multiplies this quaternion by second and returns the result.
            //Rotates this quaternion by second
            //This function is non-commutative, so the order of multiplications matter
            Quaternion multiply(const Quaternion &second) const;

            //Returns the quaternion's conjugate
            Quaternion conjugate(void);

            //Returns the sum of the squares of each component
            MHTypes::MHFloat normal(void) const;

            //Returns the quaternion's magnitude
            MHTypes::MHFloat magnitude(void) const;

            //Returns the normalized version of this quaternion. Quaternions may eventually need to be normalized due
            //to rounding errors on floating point numbers. You can test if a quaternion needs normalizing if
            //sqrt(w^2 + x^2 + y^2 + z^2) is beyond some threshold away from 1.
            Quaternion normalize(void);

            //Returns the rotational distance to quatTwo in degrees. quatTwo must be normalized.
            MHTypes::MHFloat distance(Quaternion quatTwo) const;

            //Performs Spherical Linear intERPolation (slerp) between this quaternion and quatTwo.
            //Amount should be a value between zero (returns this quaternion) and 1 (returns quatTwo).
            Quaternion slerp(const Quaternion &quatTwo,
                             MHTypes::MHFloat amount,
                             SlerpMethod method);

            //The return value is the quaternion that will rotate this quaternion to quatTwo.
            Quaternion difference(const Quaternion &quatTwo);

            //Returns this quaternion's inverse. Throws an exception if the normal is zero.
            Quaternion inverse(void);

            //The quaternion's components
            MHTypes::MHFloat w;
            MHTypes::MHFloat x;
            MHTypes::MHFloat y;
            MHTypes::MHFloat z;

        private:
            static const MHTypes::MHFloat EPSILON;

    };//Quaternion
};//namespace MathHelperTypes

#endif
