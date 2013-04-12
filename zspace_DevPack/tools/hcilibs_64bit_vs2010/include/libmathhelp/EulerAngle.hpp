/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#ifndef MH_EULERANGLE
#define MH_EULERANGLE

#include "MHSettings.hpp"
#include <iostream>

namespace MHTypes
{
    class MH_INTERFACE EulerAngle
    {
        public:
            //constructors
            EulerAngle(void);
            EulerAngle(const EulerAngle &source);
            EulerAngle(MHTypes::MHFloat newPitch,
                       MHTypes::MHFloat newYaw,
                       MHTypes::MHFloat newRoll);

            //destructor
            ~EulerAngle(void);

            //Copies source's member variables
            void copyFrom(const EulerAngle &source);

            //The Euler angles being stored
            MHTypes::MHFloat pitch;
            MHTypes::MHFloat yaw;
            MHTypes::MHFloat roll;

    };//EulerAngle
}//namespace MHTypes

#endif
