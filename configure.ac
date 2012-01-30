# 2011-07-08  Michele Tavella <michele.tavella@epfl.ch>
# - If the library source code has changed at all since the last update,
#   then increment revision.
# - If any interfaces have been added, removed, or changed since the last
#   update, increment current, and set revision to 0.
# - If any interfaces have been added since the last public release, then
#   increment age.
# - If any interfaces have been removed since the last public release, then
#   set age to 0.

m4_define([lib_current],7)
m4_define([lib_revision],0)
m4_define([lib_age],0)

AC_INIT(tobicore,[lib_current.lib_revision.lib_age],
		michele.tavella@epfl.ch)
AC_CONFIG_SRCDIR([src/tobicore/TCBlock.hpp])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config/config.h])

AM_INIT_AUTOMAKE([no-dist-gzip dist-bzip2 color-tests subdir-objects])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_CXX_C_O
LT_INIT([win32-dll disable-static])
AC_SUBST(LIBTOOL_DEPS)

AC_HEADER_STDBOOL

save_LIBS="$LIBS"
AC_SEARCH_LIBS([pthread_create], [pthread],
        AC_SUBST([PTHREAD_LIBS], "$LIBS"), 
		AC_MSG_ERROR([The pthread library must be installed.]))
LIBS="$save_LIBS"

case $host in
	*-linux*)
		platform=linux
		;;
	*win32* | *mingw32* | *cygwin* | *windows*)
		platform=win32
		;;
esac

AM_CONDITIONAL([LINUX], [test "$platform" = "linux"])
AM_CONDITIONAL([WIN32], [test "$platform" = "win32"])

AC_CHECK_PROG([DOXYGEN], [doxygen], [doxygen] [no])
AC_CHECK_PROG([MWRAP], [mwrap], [mwrap], [no])
AC_CHECK_PROG([MEX], [mex], [mex], [no])

if test "$platform" = win32; then 
	AC_SUBST([WIN32_LIBS], "-lws2_32")
fi

if test "$platform" = linux ; then 
	if test "$MWRAP" = no ; then 
		AC_MSG_ERROR(Cannot find mwrap.) 
	fi
	if test "$MEX" = no ; then 
		AC_MSG_ERROR(Cannot find mex.) 
	fi
	if test "$DOXYGEN" = no ; then 
		AC_MSG_ERROR(Cannot find doxygen.) 
	fi
fi

AC_SUBST([RELEASE],[lib_release])
AC_SUBST([CURRENT],[lib_current])
AC_SUBST([REVISION],[lib_revision])
AC_SUBST([AGE],[lib_age])
AC_SUBST(AM_LDFLAGS)

AC_CONFIG_FILES([
 Makefile
 doc/Makefile
 doc/doxygen.cfg
 src/tobicore/Makefile
 src/tobicore/tobicore.pc
 doc/examples/tobicore/Makefile
 src/tobiic/Makefile
 src/tobiic/tobiic.pc
 doc/examples/tobiic/Makefile
 src/tobiid/Makefile
 src/tobiid/tobiid.pc
 doc/examples/tobiid/Makefile
 src/tobiplatform/Makefile
 src/tobiplatform/tobiplatform.pc
 doc/examples/tobiplatform/Makefile
 mex/Makefile
 mex/tobicoremat/Makefile
 mex/mextobicore/Makefile
 mex/mextobiic/Makefile
 mex/mextobiid/Makefile])
AC_OUTPUT
