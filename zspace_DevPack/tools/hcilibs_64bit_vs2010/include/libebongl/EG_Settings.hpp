#ifndef EG_SETTINGS_HPP_INCLUDED
#define EG_SETTINGS_HPP_INCLUDED

#ifdef _WIN32
	#define EBONGL_WINDOWS
    #include <windows.h>
#endif

#include <GL/glu.h>
#include <GL/gl.h>
#include "MathHelper.hpp"

namespace EbonGL
{
    class EG_Polygon;
    class EG_SafeScene;
}//namespace EbonGL

#ifdef EBONGL_WINDOWS
    #ifdef USE_MH_DLL
	    #pragma warning( disable : 4267 )

        template class MH_INTERFACE std::allocator<MHTypes::Point3D*>;
        template class MH_INTERFACE std::vector<MHTypes::Point3D*, std::allocator<MHTypes::Point3D*> >;

        template class MH_INTERFACE std::allocator<GLushort>;
        template class MH_INTERFACE std::vector<GLushort, std::allocator<GLushort> >;

        template class MH_INTERFACE std::allocator<EbonGL::EG_Polygon*>;
        template class MH_INTERFACE std::vector<EbonGL::EG_Polygon*, std::allocator<EbonGL::EG_Polygon*> >;

        template class MH_INTERFACE std::allocator<EbonGL::EG_SafeScene*>;
        template class MH_INTERFACE std::vector<EbonGL::EG_SafeScene*, std::allocator<EbonGL::EG_SafeScene*> >;
    #endif
#else
	#include <stdint.h>
#endif

#endif
