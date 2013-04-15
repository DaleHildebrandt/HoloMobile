## Process this file with automake to produce Makefile.in

AM_CXXFLAGS = -Wall
AM_LDFLAGS = -lm

LDADD = libmathhelp.a

lib_LIBRARIES = libmathhelp.a

libmathhelp_a_SOURCES = MathHelper.cpp \
	libmathhelp/EulerAngle.cpp \
	libmathhelp/Matrix3x3.cpp \
	libmathhelp/MHVector.cpp \
	libmathhelp/Plane.cpp \
	libmathhelp/Point2D.cpp \
	libmathhelp/Point3D.cpp \
	libmathhelp/Quaternion.cpp

nobase_include_HEADERS = MathHelper.hpp \
	libmathhelp/EulerAngle.hpp \
	libmathhelp/Matrix3x3.hpp \
	libmathhelp/MHVector.hpp \
	libmathhelp/Plane.hpp \
	libmathhelp/Point2D.hpp \
	libmathhelp/Point3D.hpp \
	libmathhelp/Quaternion.hpp \
	libmathhelp/MHSettings.hpp
