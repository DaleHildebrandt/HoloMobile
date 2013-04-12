/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#include "Matrix3x3.hpp"
#include "Point3D.hpp"
#include "EulerAngle.hpp"

#ifdef MH_WINDOWS
    #include "../MathHelper.hpp"
#else
    #include "MathHelper.hpp"
#endif

using namespace std;
using namespace MHTypes;

Matrix3x3::Matrix3x3(void)
{
    values.resize(9, 0);
}//constructor

Matrix3x3::Matrix3x3(const Matrix3x3 &source)
{
    for (unsigned int i = 0; i < source.values.size(); i++)
        values.push_back(source.values.at(i));
}//copy constructor

Matrix3x3::~Matrix3x3(void)
{
}//destructor

void Matrix3x3::fromEuler(MHFloat pitch,
                          MHFloat yaw,
                          MHFloat roll)
{
    fromEuler(EulerAngle(pitch, yaw, roll));
}//fromEuler

void Matrix3x3::fromEuler(const EulerAngle &euler)
{
    MHFloat pitch = euler.pitch * (MathHelper::degToRad() * -1);
    MHFloat yaw = euler.yaw * (MathHelper::degToRad() * -1);
    MHFloat roll = euler.roll * (MathHelper::degToRad() * -1);

    MHFloat A = cos(pitch);
    MHFloat B = sin(pitch);
    MHFloat C = cos(yaw);
    MHFloat D = sin(yaw);
    MHFloat E = cos(roll);
    MHFloat F = sin(roll);

    MHFloat AD = A * D;
    MHFloat BD = B * D;

    // | x' |   |  CE      -CF       D  |   | x |
    // | y' | = |  BDE+AF  -BDF+AE  -BC | . | y |
    // | z' |   | -ADE+BF   ADF+BE   AC |   | z |

    values.at(0)  =   C * E;
    values.at(1)  =  -C * F;
    values.at(2)  =   D;

    values.at(3)  =  BD * E + A * F;
    values.at(4)  = -BD * F + A * E;
    values.at(5)  =  -B * C;

    values.at(6)  = -AD * E + B * F;
    values.at(7)  =  AD * F + B * E;
    values.at(8)  =   A * C;
}//fromEuler

EulerAngle Matrix3x3::toEuler(void)
{
    EulerAngle result;//the euler angles to return
    MHFloat cosYaw = 0;
    MHFloat trX = 0;
    MHFloat trY = 0;

    result.yaw = asin(values.at(2));
    cosYaw = cos(result.yaw);
    result.yaw *= MathHelper::radToDeg();

    //### No gimbal lock ###
    if (fabs(cosYaw) > 0.005)
    {
        trX =  values.at(8) / cosYaw;
        trY = -values.at(5) / cosYaw;
        result.pitch = atan2(trY, trX) * MathHelper::radToDeg();

        trX =  values.at(0) / cosYaw;
        trY = -values.at(1) / cosYaw;
        result.roll = atan2(trY, trX) * MathHelper::radToDeg();
    }//if fabs()

    //### Handle gimbal lock ###
    else
    {
        result.pitch = 0;
        trX = values.at(4);
        trY = values.at(3);
        result.roll = atan2(trY, trX) * MathHelper::radToDeg();
    }//else fabs()

    result.pitch *= -1;
    result.yaw *= -1;
    result.roll *= -1;

    return result;
}//toEuler

MHFloat Matrix3x3::get(unsigned int index)
{
    return values.at(index);
}//getValue

void Matrix3x3::set(unsigned int index,
                    MHFloat newValue)
{
    values.at(index) = newValue;
}//setValue

MHFloat Matrix3x3::determinant(void)
{
    return values.at(0) * ((values.at(4) * values.at(8)) - (values.at(7) * values.at(5)))
         - values.at(1) * ((values.at(3) * values.at(8)) - (values.at(6) * values.at(5)))
         + values.at(2) * ((values.at(3) * values.at(7)) - (values.at(6) * values.at(4)));
}//determinant3x3

bool Matrix3x3::inverse(Matrix3x3 &inverted)
{
    MHFloat deter = determinant();
    bool result = true;//false if the inverse can't be computed

    if (fabs(deter) < 0.0005)
    {
       //m3_identity(inverse);

       result = false;
    }//if fabs()
    else
    {
        inverted.set(0,  (values.at(4) * values.at(8) - values.at(5) * values.at(7)) / deter);
        inverted.set(1, -(values.at(1) * values.at(8) - values.at(7) * values.at(2)) / deter);
        inverted.set(2,  (values.at(1) * values.at(5) - values.at(4) * values.at(2)) / deter);
        inverted.set(3, -(values.at(3) * values.at(8) - values.at(5) * values.at(6)) / deter);
        inverted.set(4,  (values.at(0) * values.at(8) - values.at(6) * values.at(2)) / deter);
        inverted.set(5, -(values.at(0) * values.at(5) - values.at(3) * values.at(2)) / deter);
        inverted.set(6,  (values.at(3) * values.at(7) - values.at(6) * values.at(4)) / deter);
        inverted.set(7, -(values.at(0) * values.at(7) - values.at(6) * values.at(1)) / deter);
        inverted.set(8,  (values.at(0) * values.at(4) - values.at(1) * values.at(3)) / deter);
    }//else fabs()

    return result;
}//inverse3x3

Point3D Matrix3x3::rotatePoint(const Point3D &original)
{
    Point3D result;//the rotated point to return

    result.x = (values.at(0) * original.x) + (values.at(1) * original.y) + (values.at(2) * original.z);
    result.y = (values.at(3) * original.x) + (values.at(4) * original.y) + (values.at(5) * original.z);
    result.z = (values.at(6) * original.x) + (values.at(7) * original.y) + (values.at(8) * original.z);

    return result;
}//rotatePoint
