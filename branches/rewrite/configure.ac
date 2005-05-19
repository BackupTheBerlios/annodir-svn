dnl configure.ac for annodir. run this through autoconf to produce
dnl a ./configure script (it's best to use autogen.sh)

dnl -- STANDARD THINGS --

AC_PREREQ(2.59)
AC_INIT([annodir], [0.1_alpha1], [ka0ttic@gentoo.org])
AC_CONFIG_SRCDIR([src/annodir.cc])
AC_CONFIG_AUX_DIR([config])
AM_INIT_AUTOMAKE([1.8 dist-bzip2 no-dist-gzip])

test "x$sysconfdir" = "xNONE" && sysconfdir="$ac_default_sysconfdir"
AC_DEFINE_UNQUOTED(SYSCONFDIR, "$sysconfdir", [System configuration directory])

dnl extra options
AC_ARG_ENABLE(debug, [  --enable-debug      enable debugging flags [off]],
    [case "${enableval}" in
	yes)	debug=true
		CXXFLAGS="-ggdb3"
		AC_DEFINE_UNQUOTED(DEBUG, 1, [Enable debugging code]) 
		;;
	no)	debug=false ;;
	*)	AC_MSG_ERROR(--enable-debug accepts yes or no only) ;;
    esac], [debug=false])

dnl check for required programs
AC_LANG_CPLUSPLUS
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LN_S

dnl libtool stuff
AC_DISABLE_SHARED
dnl hack to disable icky f77 checks (stolen from sandbox configure.in)
m4_undefine([AC_PROG_F77])
m4_defun([AC_PROG_F77], [])
AC_PROG_LIBTOOL

LIBTOOL="${LIBTOOL} --silent"
AC_SUBST(LIBTOOL)

dnl SED is defined by LT_AC_PROG_SED ran by AC_PROG_LIBTOOL
AC_SUBST(SED)

AC_MSG_CHECKING([gcc version])
gccver=`${CC} -dumpversion 2>/dev/null | cut -f1 -d.`
AC_MSG_RESULT([$gccver])
if test x$gccver = "x4" ; then
    AC_DEFINE_UNQUOTED(HAVE_GCC4, 1, [GCC Version 4])
fi

dnl -- REQUIRED --

echo "--- checking for required things ---"
AC_HEADER_STDC
AC_CHECK_HEADERS(regex.h,,
    [AC_MSG_ERROR([regex.h is required])])
AC_HEADER_DIRENT(,
    [AC_MSG_ERROR([dirent.h is required])])
AC_CHECK_HEADERS(unistd.h,,
    [AC_MSG_ERROR([unistd.h is required])])
AC_FUNC_STRFTIME(,
    [AC_MSG_ERROR([strftime is required])])
AC_CHECK_FUNCS(strerror,,
    [AC_MSG_ERROR([strerror is required])])
AC_CHECK_FUNCS(strtol,,
    [AC_MSG_ERROR([strtol is required])])
AC_CHECK_FUNCS(asprintf,,
    [AC_CHECK_FUNCS(snprintf,,
    [AC_MSG_ERROR([either asprintf or snprintf is required])])])
AC_CHECK_FUNCS(vasprintf,,
    [AC_CHECK_FUNCS(vsnprintf,,
    [AC_CHECK_FUNCS(vsprintf,,
    [AC_MSG_ERROR([either vasprintf, vsnprintf, or vsprintf is required])])])])
AC_FUNC_CLOSEDIR_VOID

unset tcap
AC_CHECK_HEADERS(readline/readline.h, ,
    [AC_MSG_ERROR([readline/readline.h is required])])
AC_CHECK_LIB(ncurses, tputs, [tcap="$tcap -lncurses"])
AC_CHECK_LIB(readline, readline, [READLINE_LIBS="-lreadline $tcap"], , $tcap)
if test -z "$READLINE_LIBS" ; then
    AC_MSG_ERROR([need a working readline])
fi
AC_SUBST(READLINE_LIBS)

echo "--- checking for optional things ---"
AC_CHECK_HEADERS(getopt.h)
AC_CHECK_FUNCS(getopt_long)

echo "--- done with checks ---"

dnl output
AM_CONFIG_HEADER(config.h)
AC_OUTPUT(Makefile
	  util/Makefile
	  src/Makefile)
