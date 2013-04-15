/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#include "Quaternion.hpp"
#include <math.h>
#include "Matrix3x3.hpp"

#ifdef MH_WINDOWS
    #include "../MathHelper.hpp"
#else
    #include "MathHelper.hpp"
#endif

using namespace std;
using namespace MHTypes;

const MHFloat Quaternion::EPSILON = 0.001f;

Quaternion::Quaternion(void)
{
    w = 0;
    x = 0;
    y = 0;
    z = 0;
}//constructor

Quaternion::Quaternion(const Quaternion &source)
{
    w = source.w;
    x = source.x;
    y = source.y;
    z = source.z;
}//copy constructor

void Quaternion::fromEuler(const EulerAngle &eulerAngles)
{
    fromEuler(eulerAngles.pitch, eulerAngles.yaw, eulerAngles.roll);
}//fromEuler

void Quaternion::fromEuler(MHFloat pitch,
                           MHFloat yaw,
                           MHFloat roll)
{
    pitch *= MathHelper::degToRad();
    yaw *= MathHelper::degToRad();
    roll *= MathHelper::degToRad();

    MHFloat sx = sin(pitch * (MHFloat)0.5);
    MHFloat sy = sin(yaw   * (MHFloat)0.5);
    MHFloat sz = sin(roll  * (MHFloat)0.5);

    MHFloat cx = cos(pitch * (MHFloat)0.5);
    MHFloat cy = cos(yaw   * (MHFloat)0.5);
    MHFloat cz = cos(roll  * (MHFloat)0.5);

    MHFloat cycz = cy * cz;
    MHFloat sysz = sy * sz;

    w = (cx * cycz) + (sx * sysz);
    x = (sx * cycz) - (cx * sysz);
    y = (cx * sy * cz) + (sx * cy * sz);
    z = (cx * cy * sz) - (sx * sy * cz);
}//fromEuler

Matrix3x3 Quaternion::getRotationMatrix(void) const
{
    Matrix3x3 result;//the rotation matrix to return
    MHFloat xx = x * x;
    MHFloat xy = x * y;
    MHFloat xz = x * z;
    MHFloat xw = x * w;
    MHFloat yy = y * y;
    MHFloat yz = y * z;
    MHFloat yw = y * w;
    MHFloat zz = z * z;
    MHFloat zw = z * w;

    result.set(0, 1 - 2 * (yy + zz));
    result.set(3,     2 * (xy - zw));
    result.set(6,     2 * (xz + yw));

    result.set(1,     2 * (xy + zw));
    result.set(4, 1 - 2 * (xx + zz));
    result.set(7,     2 * (yz - xw));

    result.set(2,     2 * (xz - yw));
    result.set(5,     2 * (yz + xw));
    result.set(8, 1 - 2 * (xx + yy));

    return result;
}//getRotationMatrix

Quaternion Quaternion::multiply(const Quaternion &second) const
{
    Quaternion result;//the result to return

    result.w = (w * second.w) - (x * second.x) - (y * second.y) - (z * second.z);
    result.x = (w * second.x) + (x * second.w) + (y * second.z) - (z * second.y);
    result.y = (w * second.y) + (y * second.w) + (z * second.x) - (x * second.z);
    result.z = (w * second.z) + (z * second.w) + (x * second.y) - (y * second.x);

    return result;
}//multiply

Quaternion Quaternion::conjugate(void)
{
    Quaternion result;//the conjugate to return

    result.w = w;
    result.x = x * -1;
    result.y = y * -1;
    result.z = z * -1;

    return result;
}//conjugate

MHFloat Quaternion::normal(void) const
{
    return (w * w) + (x * x) + (y * y) + (z * z);
}//normal

MHFloat Quaternion::magnitude(void) const
{
    return sqrt(normal());
}//magnitude

Quaternion Quaternion::normalize(void)
{
    Quaternion result = *this;//the normalized version to return
    MHFloat mag = magnitude();

    result.w /= mag;
    result.x /= mag;
    result.y /= mag;
    result.z /= mag;

    return result;
}//normalize

MHFloat Quaternion::distance(Quaternion quatTwo) const
{
    Quaternion difference;//the change in rotation between the two quaternions
    Quaternion inverse;//the inverse of quatTwo
    MHFloat result = 0;//the rotational difference in degrees

    //### Find the difference between the current and old orientations ###
    inverse = quatTwo.conjugate();
    difference = multiply(inverse);//this->multiply

    //### Convert the difference to degrees ###
    result = static_cast<MHFloat>(2) * acos(difference.w);
    result *= static_cast<MHFloat>(180) / static_cast<MHFloat>(3.14159265);

    result = fabs(result);
    if (result > 180)
        result = 360 - result;

	#ifdef _WIN32
		if (result != result)
			result = 0;
	#else
		if (isnan(result) != 0)
			result = 0;
	#endif

    return result;
}//distance

Quaternion Quaternion::slerp(const Quaternion &quatTwo,
                             MHFloat amount,
                             SlerpMethod method)
{
    Quaternion result;//the rotation to return
    MHFloat angle = 0;
    MHFloat cos_angle = 0;
    MHFloat scale_from = 0;
    MHFloat scale_to = 0;
    MHFloat sin_angle = 0;

    cos_angle = (this->x * quatTwo.x) + (this->y * quatTwo.y) + (this->z * quatTwo.z) + (this->w * quatTwo.w);

    if (((method == Q_SHORT)             && (cos_angle < 0.0)) ||
        ((method == Q_LONG)              && (cos_angle > 0.0)) ||
        ((method == Q_CLOCKWISE)         && (this->w > quatTwo.w)) ||
        ((method == Q_COUNTER_CLOCKWISE) && (this->w < quatTwo.w)))
    {
        cos_angle = -cos_angle;
        result.w    = -quatTwo.w;
        result.x    = -quatTwo.x;
        result.y    = -quatTwo.y;
        result.z    = -quatTwo.z;
    }//if method || method ...
    else
    {
        result.w = quatTwo.w;
        result.x = quatTwo.x;
        result.y = quatTwo.y;
        result.z = quatTwo.z;
    }//else method || method...

    if ((1.0 - fabs(cos_angle)) > EPSILON)
    {
        /* spherical linear interpolation (SLERP) */
        angle = acos(cos_angle);
        sin_angle  = sin(angle);
        scale_from = static_cast<MHFloat>(sin((1.0 - amount) * angle) / sin_angle);
        scale_to   = sin(amount * angle) / sin_angle;
    }
    else
    {
        /* to prevent divide-by-zero, resort to linear interpolation */
        scale_from = 1.0f - amount;
        scale_to   = amount;
    }

    result.w = (MHFloat)((scale_from * this->w) + (scale_to * result.w));
    result.x = (MHFloat)((scale_from * this->x) + (scale_to * result.x));
    result.y = (MHFloat)((scale_from * this->y) + (scale_to * result.y));
    result.z = (MHFloat)((scale_from * this->z) + (scale_to * result.z));

    return result;
}//slerp

Quaternion Quaternion::inverse(void)
{
    Quaternion result;//the inverse to return
    Quaternion con = this->conjugate();
    MHFloat norm = this->normal();

    //### Throw an exception if this is a zero quaternion ###
    if (norm == 0)
        throw 1;

    result.w = con.w / norm;
    result.x = con.x / norm;
    result.y = con.y / norm;
    result.z = con.z / norm;

    return result;
}//inverse

Quaternion Quaternion::difference(const Quaternion &quatTwo)
{
    Quaternion result;//the quaternion to return

    result = this->inverse();
    result = result.multiply(quatTwo);

    return result;
}//difference
