#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([EbonGL], [1.00], [dmccallum9@gmail.com])
AC_CONFIG_SRCDIR([EbonGL.hpp])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PREREQ(2.65)
AC_CONFIG_HEADERS([libebongl/EbonGLConfig.hpp])

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

# Checks for header files.
AC_CHECK_HEADERS_ONCE([inttypes.h GL/glu.h GL/gl.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

#Create the Makefile and config.h
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
