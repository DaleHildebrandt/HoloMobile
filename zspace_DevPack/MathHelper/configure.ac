#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_INIT([MathHelper], [1.0], [dmccallum9@gmail.com])
AC_CONFIG_SRCDIR([MathHelper.hpp])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PREREQ(2.65)
AC_CONFIG_HEADERS([libmathhelp/MathHelpConfig.hpp])

#Linker parameters to use when checking for features
LDFLAGS=-lm

# Checks for programs.
AC_PROG_CXX
AC_PROG_RANLIB

# Checks for header files.
AC_CHECK_HEADERS_ONCE([math.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

#Create the Makefile and config.h
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
