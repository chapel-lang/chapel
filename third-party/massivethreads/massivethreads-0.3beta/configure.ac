AC_INIT([massivethreads], [0.21], [massivethreads@eidos.ic.i.u-tokyo.ac.jp], [massivethreads], [http://code.google.com/p/massivethreads/])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
# required when you build shared libraries, relying on libtool
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC
# use -O3 optimization instead of -O2
if test "$ac_test_CFLAGS" = set; then
  CFLAGS=$ac_save_CFLAGS
elif test $ac_cv_prog_cc_g = yes; then
  if test "$GCC" = yes; then
    CFLAGS="-g -O3 -Wall -ftls-model=initial-exec"
  else
    CFLAGS="-g"
  fi
else
  if test "$GCC" = yes; then
    CFLAGS="-O3 -Wall -ftls-model=initial-exec"
  else
    CFLAGS=
  fi
fi
AC_PROG_AWK
AM_PROG_AS
AC_CHECK_LIB(dl, dlopen, [], [AC_MSG_ERROR(Test for dlopen failed.)])
AC_CHECK_LIB(pthread, pthread_create, [], [AC_MSG_ERROR(Test for libpthread failed.)])

# Check size
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([void*])

# Check functions
# AC_CHECK_FUNCS(__builtin_unreachable __builtin_clz)

# initialize libtool
LT_INIT
AC_CONFIG_FILES([Makefile tests/Makefile tests/fib/Makefile])
AC_CONFIG_HEADERS([config.h])
AC_OUTPUT
