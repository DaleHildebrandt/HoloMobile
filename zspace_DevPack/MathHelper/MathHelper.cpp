/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#include "MathHelper.hpp"
#include <math.h>
#include <stdlib.h>

using namespace std;
using namespace MHTypes;

const MHFloat MathHelper::RADIANS_TO_DEGREES = static_cast<MHFloat>(57.2957795);
const MHFloat MathHelper::DEGREES_TO_RADIANS = static_cast<MHFloat>(0.0174532925);

MathHelper::MathHelper(void)
{
}//MathHelper

MathHelper::~MathHelper(void)
{
}//destructor

MHFloat MathHelper::shortAngleDistance(MHFloat angle1,
                                       MHFloat angle2)
{
    MHFloat result = 0;//the distance to return

    if ((angle1 > 180) || (angle1 < -180))
        throw 1;
    if ((angle2 > 180) || (angle2 < -180))
        throw 1;

    result = angle1 - angle2;
    if (result > 180)
        result = (360 - result) * -1;
    else if (result < -180)
        result = 360 + result;

    return result * -1;
}//shortAngleDistance

void MathHelper::setRandSeed(unsigned int seed)
{
    srand(seed);
}//setRandSeed

long unsigned int MathHelper::genRandom(long unsigned int minimum,
                                        long unsigned int maximum)
{
    long unsigned int temp = 0;//swap min and max if they're in the wrong order
    long unsigned int range = 0;//the difference between maximum and minimum
    long unsigned int result = 0;//the random number to return

    //### Verify that minimum is smaller than maximum ###
    if (minimum > maximum)
    {
        temp = maximum;
        maximum = minimum;
        minimum = temp;
    }//if minimum

    range = maximum - minimum + 1;

    result = static_cast<long unsigned int>(range * (rand() / (RAND_MAX + 1.0)));
    result += minimum;

    return result;
}//genRandom

MHFloat MathHelper::radToDeg(void)
{
    return RADIANS_TO_DEGREES;
}//radToDeg

MHFloat MathHelper::degToRad(void)
{
    return DEGREES_TO_RADIANS;
}//degToRad
