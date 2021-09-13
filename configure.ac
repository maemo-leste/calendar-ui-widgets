AC_INIT([calendar-ui-widgets], [0.5])
AC_CANONICAL_SYSTEM

AC_CONFIG_MACRO_DIRS([m4])
AM_CONFIG_HEADER(config.h)

AM_MAINTAINER_MODE

AM_INIT_AUTOMAKE

AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_LIBTOOL

AC_HEADER_STDC

AC_PATH_X
AC_PATH_XTRA
AC_SUBST(X_CFLAGS)

AM_GCONF_SOURCE_2

PKG_PROG_PKG_CONFIG
PKG_CHECK_VAR([GLIB_MKENUMS], [glib-2.0], [glib_mkenums])

#+++++++++++++++
# Misc programs
#+++++++++++++++

AC_SUBST(CFLAGS)

PKG_CHECK_MODULES(HILDON, hildon-1)
AC_SUBST(HILDON_CFLAGS)
AC_SUBST(HILDON_LIBS)

#+++++++++++++++++++
# Directories setup
#+++++++++++++++++++

AC_CONFIG_FILES([
Makefile
src/Makefile
calendar-ui-widgets.pc
])

AC_OUTPUT
