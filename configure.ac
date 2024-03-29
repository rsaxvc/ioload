dnl Richard Allen <rsaxvc@rsaxvc.net>
dnl Process this file with autoconf to produce a configure script.

AC_INIT([ioload], [0.7.4], [rsaxvc@rsaxvc.net])

AC_CANONICAL_HOST

AC_CONFIG_SRCDIR([src/main.cpp])

AC_CONFIG_HEADER([config.h])

AC_CONFIG_FILES([
Makefile
ioload.spec
docs/Makefile
docs/ioload.1
src/Makefile
])

AM_INIT_AUTOMAKE

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_GCC_TRADITIONAL
AC_PROG_INSTALL

dnl System dependant jobs
AM_CONDITIONAL(HAVE_LINUX, false)
AM_CONDITIONAL(HAVE_BSD, false)
AM_CONDITIONAL(HAVE_SOLARIS, false)
AM_CONDITIONAL(HAVE_HPUX, false)
case $host_os in
    *linux*)
        AC_DEFINE(HAVE_LINUX, 1, [Define to 1 if your build target is Linux.])
        AM_CONDITIONAL(HAVE_LINUX, true)

        AC_CHECK_FUNCS([memset])
        ;;
    *bsd*)
        AC_DEFINE(HAVE_BSD, 1, [Define to 1 if your build target is BSD.])
        AM_CONDITIONAL(HAVE_BSD, true)
        
        AC_FUNC_MALLOC
        ;;
    *solaris*)
        AC_DEFINE(HAVE_SOLARIS, 1, [Define to 1 if your build target is Solaris.])
        AM_CONDITIONAL(HAVE_SOLARIS, true)
        
        AC_CHECK_LIB(rt, main)
        AC_CHECK_LIB(kstat, kstat_open)
        AC_CHECK_LIB(socket, main)
        AC_CHECK_LIB(nsl, main)
        
        AM_CPPFLAGS="${AM_CPPFLAGS} -I/usr/local/include/ncurses"
        AC_SUBST(AM_CPPFLAGS)
        ;;
    *hpux*)
        AC_DEFINE(HAVE_HPUX, 1, [Define to 1 if your build target is HP-UX.])
        AM_CONDITIONAL(HAVE_HPUX, true)
        
    	AM_CPPFLAGS="${AM_CPPFLAGS} -I/usr/local/include/ncurses"
    	LIBS="-L/usr/local/lib ${LIBS}"
    	AC_SUBST(AM_CPPFLAGS)
        ;;
    *)
        AC_MSG_ERROR([ioload does not support this operating system!])
        ;;
esac

dnl Checks for libraries.
AC_CHECK_LIB(ncurses, initscr)
AC_CHECK_LIB(form, new_form)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h string.h unistd.h curses.h form.h math.h sys/param.h sys/socket.h sys/time.h sys/ioctl.h arpa/inet.h netinet/in.h])

if test "$ac_cv_lib_ncurses_initscr $ac_cv_header_curses_h" != "yes yes"; then
    AC_MSG_ERROR([ncurses library or development files not found. ncurses is required for $PACKAGE_NAME.])
fi
if test "$ac_cv_lib_form_new_form $ac_cv_header_form_h" != "yes yes"; then
    AC_MSG_ERROR([form library or development files not found. These are part of ncurses which is required for $PACKAGE_NAME.])
fi

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_HEADER_TIME

dnl Use some gcc specific options.
if test "$GCC" = "yes" ; then
     CFLAGS="$CFLAGS -Wall"
     CXXFLAGS="$CXXFLAGS -Wall"
fi

dnl Forward sysconfdir to the preprocessor
CPPFLAGS="$CPPFLAGS -DSYSCONFDIR='\"\$(sysconfdir)\"'"

dnl Adds extra arguments to configure script.
AC_ARG_ENABLE(debug,
              AC_HELP_STRING([--enable-debug], [do not strip debugging symbols (default no)]),
	          test "$enableval" = "no" && LDFLAGS="$LDFLAGS -s",
	          LDFLAGS="$LDFLAGS -s")

dnl Checks for library functions.
AC_TYPE_SIGNAL
AC_CHECK_FUNCS([gettimeofday inet_ntoa socket])

AC_OUTPUT
