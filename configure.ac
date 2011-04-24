# If the library source code has changed at all since the last update, then
# increment revision.
# If any interfaces have been added, removed, or changed since the last update,
# increment current, and set revision to 0.
# If any interfaces have been added since the last public release, then
# increment age.  
# If any interfaces have been removed since the last public release, then set
# age to 0.
m4_define([lib_current],2)
m4_define([lib_revision],0)
m4_define([lib_age],0)

# Setup Automake
AC_INIT(libtobicore,[lib_release.lib_current.lib_age.lib_revision],michele.tavella@epfl.ch)
AC_CONFIG_SRCDIR([src/libtobicore/TCBlock.hpp])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config/config.h])

# Setup automake
AM_INIT_AUTOMAKE([no-dist-gzip dist-bzip2 color-tests subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CANONICAL_HOST
AC_CANONICAL_BUILD

# Checks for programs.
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_CXX_C_O
LT_INIT([win32-dll disable-static])
AC_SUBST(LIBTOOL_DEPS)

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Check for necessay libraries
AC_LANG_PUSH([C++])
AC_CHECK_HEADERS([rapidxml.hpp rapidxml_print.hpp], [], 
	 AC_MSG_ERROR([The RapidXML headers must be installed.]), [])
AC_LANG_POP([C++])

AC_SUBST([CURRENT],[lib_current])
AC_SUBST([REVISION],[lib_revision])
AC_SUBST([AGE],[lib_age])
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([Makefile doc/Makefile doc/examples/Makefile doc/doxygen.cfg\
		 src/libtobicore/Makefile src/libtobicore/tobicore.pc \
		 src/libtobiic/Makefile src/libtobiic/tobiic.pc	\
		 src/libtobiid/Makefile src/libtobiid/tobiid.pc])
AC_OUTPUT

