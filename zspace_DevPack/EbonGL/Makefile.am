## Process this file with automake to produce Makefile.in

AM_CXXFLAGS = -D_REENTRANT -Wall
AM_LDFLAGS = -pthread
LDADD = libebongl.a -lz -lboost_system-mt

lib_LIBRARIES = libebongl.a

libebongl_a_SOURCES = libebongl/EG_Color.cpp \
	libebongl/EG_Cuboid.cpp \
	libebongl/EG_Engine.cpp \
	libebongl/EG_EngineInt.cpp \
	libebongl/EG_Light.cpp \
	libebongl/EG_LogWriter.cpp \
	libebongl/EG_Mutex.cpp \
	libebongl/EG_Object.cpp \
	libebongl/EG_PointList.cpp \
	libebongl/EG_Polygon.cpp \
	libebongl/EG_SafeScene.cpp \
	libebongl/EG_SceneElement.cpp \
	libebongl/EG_Utils.cpp \
	libebongl/EG_String.cpp \
	libebongl/EG_BoolVector.cpp

nobase_include_HEADERS = EbonGL.hpp \
	libebongl/EG_Color.hpp \
	libebongl/EG_Cuboid.hpp \
	libebongl/EG_Engine.hpp \
	libebongl/EG_EngineInt.hpp \
	libebongl/EG_Light.hpp \
	libebongl/EG_LogWriter.hpp \
	libebongl/EG_Mutex.hpp \
	libebongl/EG_Object.hpp \
	libebongl/EG_PointList.hpp \
	libebongl/EG_Polygon.hpp \
	libebongl/EG_SafeScene.hpp \
	libebongl/EG_SceneElement.hpp \
	libebongl/EG_Utils.hpp \
	libebongl/EG_String.hpp \
	libebongl/EG_BoolVector.hpp
