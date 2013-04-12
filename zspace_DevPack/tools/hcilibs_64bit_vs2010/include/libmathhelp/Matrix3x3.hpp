/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

/* DESCRIPTION
    A 3x3 rotation matrix. Uses flight dynamics terminology to represent
    angles: pitch = x, yaw = y, roll = z. Rotations are performed according to:

    "Mathematical convention requires that a positive rotation angle
     generates a clockwise rotation when looking from the origin towards
     the positive end of the rotation axis."

     We use the following convention:
        x increases to the right
        y increases upwards
        z increases forwards

     The functions take parameters in degrees, ranging from -180 to +180
*/

#ifndef MH_MATRIX3X3
#define MH_MATRIX3X3

#include <vector>
#include "MHSettings.hpp"

namespace MHTypes
{
    class Point3D;
    class EulerAngle;

    class MH_INTERFACE Matrix3x3
    {
        public:
            //Constructors
            Matrix3x3(void);
            Matrix3x3(const Matrix3x3 &source);

            //Destructor
            ~Matrix3x3(void);

            //Creates a 3x3 rotation matrix from the specified Euler angles
            void fromEuler(const EulerAngle &euler);
            void fromEuler(MHTypes::MHFloat pitch,
                           MHTypes::MHFloat yaw,
                           MHTypes::MHFloat roll);

            //Converts the matrix to its Euler angle representation. Not guaranteed to
            //return the shortest rotation. Sets angles to nan if they're 90 or -90.
            EulerAngle toEuler(void);

            //Rotates original about the origin, and returns the new coordinates
            Point3D rotatePoint(const Point3D &original);

            //Returns the matrix's determinant
            MHTypes::MHFloat determinant(void);

            //Sets inverse to the inverse of this matrix.
            //Returns false if the inverse couldn't be found
            bool inverse(Matrix3x3 &inverted);

            //Returns the matrix value at the specified index.
            //Indices are laid out like so:
            //0 1 2
            //3 4 5
            //6 7 8
            MHTypes::MHFloat get(unsigned int index);

            //Changes the value at the specified index
            void set(unsigned int index,
                     MHTypes::MHFloat newValue);

        private:
            //The values making up the matrix.
            std::vector<MHTypes::MHFloat> values;

    };//Matrix3x3
};//MathHelperTypes

#endif
