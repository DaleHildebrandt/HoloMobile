/*
 * Copyright (C) 2009-2012 David C. McCallum
 * For conditions of distribution and use, see the copyright notice in MathHelper.hpp
*/

#ifndef MHSETTINGS_HPP_INCLUDED
#define MHSETTINGS_HPP_INCLUDED

#include <vector>

//### Uncomment this to allow a DLL to be built ###
//#define USE_MH_DLL

//### Detect if we're compiling under Windows ###
#ifdef _WIN32
	#define MH_WINDOWS
#endif

//### We want to use dllexport when building the lib, and dllimport when using it ###
#ifdef USE_MH_DLL
    #ifdef MH_WINDOWS
        #ifdef BUILDING_MATHHELPER_DLL
            #define MH_INTERFACE __declspec(dllexport)
        #else
            #define MH_INTERFACE __declspec(dllimport)
        #endif
    #else
        #define MH_INTERFACE
    #endif
#else
    #define MH_INTERFACE
#endif

//### Set the precision of MathHelper's floating point operations, should
//    be either a float or a double.                                      ###
namespace MHTypes
{
    MH_INTERFACE typedef double MHFloat;
}//namespace MHTypes

//### Allow std::vector<MHFloat> to be exported from the DLL ###
#ifdef USE_MH_DLL
    #ifdef MH_WINDOWS
        template class MH_INTERFACE std::allocator<float>;
        template class MH_INTERFACE std::vector<float, std::allocator<float> >;

        template class MH_INTERFACE std::allocator<double>;
        template class MH_INTERFACE std::vector<double, std::allocator<double> >;
    #endif
#endif

#endif
