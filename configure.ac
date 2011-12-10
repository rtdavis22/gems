#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([gems], [0.1], [rtdavis22@gmail.com])
AM_INIT_AUTOMAKE([foreign silent-rules])
AC_CONFIG_SRCDIR([src/atom.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
LT_INIT([dlopen])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

AC_CONFIG_FILES([Makefile])

# Checks for library functions.

AC_CONFIG_SUBDIRS([deps/gmml])

AC_OUTPUT
