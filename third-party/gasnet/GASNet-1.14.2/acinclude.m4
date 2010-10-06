dnl   $Source: /var/local/cvs/gasnet/acinclude.m4,v $
dnl     $Date: 2010/04/17 00:25:35 $
dnl $Revision: 1.137.2.7 $
dnl Description: m4 macros
dnl Copyright 2004,  Dan Bonachea <bonachea@cs.berkeley.edu>
dnl Terms of use are as specified in license.txt

dnl insert comments to improve readability of generated configure script
pushdef([gasnet_fun_level],0)
define([gasnet_fun_comment],[# $1])
AC_DEFUN([GASNET_FUN_BEGIN],[
pushdef([gasnet_fun_level],incr(defn([gasnet_fun_level])))
gasnet_fun_comment(vvvvvvvvvvvvvvvvvvvvvv [$1] vvvvvvvvvvvvvvvvvvvvvv (L:gasnet_fun_level))
])
AC_DEFUN([GASNET_FUN_END],[
gasnet_fun_comment(^^^^^^^^^^^^^^^^^^^^^^ [$1] ^^^^^^^^^^^^^^^^^^^^^^ (L:gasnet_fun_level))
popdef([gasnet_fun_level])
])

dnl determine the autoconf version used to build configure script 
AC_DEFUN([GASNET_GET_AUTOCONF_VERSION],[
GASNET_FUN_BEGIN([$0])
AC_REQUIRE([AC_PROG_AWK])
AC_MSG_CHECKING(autoconf version)
dnl AUTOCONF_VERSION=`cat ${srcdir}/configure | perl -e '{ while (<STDIN>) { if (m/enerated.*utoconf.*([[0-9]]+)\.([[0-9]]+).*/) { print "[$]1.[$]2\n"; exit 0 } } }'`
AUTOCONF_VERSION_STR=`cat ${srcdir}/configure | $AWK '/.*enerated.*utoconf.*([[0-9]]+).([[0-9]]+).*/ { [match]([$]0,"[[0-9]]+.[[0-9]]+"); print [substr]([$]0,RSTART,RLENGTH); exit 0 } '`
AUTOCONF_VERSION=`echo $AUTOCONF_VERSION_STR | $AWK -F. '{ printf("%i%i",[$]1,[$]2); }'`
AC_MSG_RESULT($AUTOCONF_VERSION_STR)
GASNET_FUN_END([$0])
])

AC_DEFUN([GASNET_FORBID_PROGRAM_TRANSFORM],[
GASNET_FUN_BEGIN([$0])
  # echo program_prefix=$program_prefix  program_suffix=$program_suffix program_transform_name=$program_transform_name
  # undo prefix autoconf automatically adds during cross-compilation
  if test "$cross_compiling" = yes && test "$program_prefix" = "${target_alias}-" ; then
    program_prefix=NONE
  fi
  # normalize empty prefix/suffix
  if test -z "$program_prefix" ; then
    program_prefix=NONE
  fi
  if test -z "$program_suffix" ; then
    program_suffix=NONE
  fi
  # canonicalize transforms caused by empty prefix/suffix
  if expr "$program_transform_name" : 's.^..$' >/dev/null || \
     expr "$program_transform_name" : 's.$$..$' >/dev/null || \
     expr "$program_transform_name" : 's.$$..;s.^..$' >/dev/null ; then
    program_transform_name="s,x,x,"
  fi
  if test "$program_prefix$program_suffix$program_transform_name" != "NONENONEs,x,x," ; then
    GASNET_MSG_ERROR([This configure script does not support --program-prefix, --program-suffix or --program-transform-name. Users are recommended to instead use --prefix with a unique directory and make symbolic links as desired for renaming.])
  fi
GASNET_FUN_END([$0])
])

dnl GASNET_GCC_VERSION_CHECK(type)  type=CC or CXX
AC_DEFUN([GASNET_GCC_VERSION_CHECK],[
GASNET_FUN_BEGIN([$0($1)])
AC_MSG_CHECKING(for known buggy compilers)
badgccmsg=""
AC_TRY_COMPILE([
#if __GNUC__ == 2 && __GNUC_MINOR__ == 96 && __GNUC_PATCHLEVEL__ == 0
# error
#endif
],[ ], [:], [
AC_MSG_RESULT([$1] is gcc 2.96)
badgccmsg="Use of gcc/g++ 2.96 for compiling this software is strongly discouraged. \
It is not an official GNU release and has many serious known bugs, especially \
in the optimizer, which may lead to bad code and incorrect runtime behavior. \
Consider using \$[$1] to select a different compiler."
GASNET_IF_ENABLED(allow-gcc296, Allow the use of the broken gcc/g++ 2.96 compiler, [
  GASNET_MSG_WARN([$badgccmsg])
  ],[
  AC_MSG_ERROR([$badgccmsg \
  You may enable use of this broken compiler at your own risk by passing the --enable-allow-gcc296 flag.])
])
])
AC_TRY_COMPILE([
#if __GNUC__ == 3 && __GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ <= 2
# error
#endif
],[ ], [:], [
AC_MSG_RESULT([$1] is gcc 3.2.0-2)
badgccmsg="Use of gcc/g++ 3.2.0-2 for compiling this software is strongly discouraged. \
This version has a serious known bug in the optimizer regarding structure copying, \
which may lead to bad code and incorrect runtime behavior when optimization is enabled. \
Consider using \$[$1] to select a different compiler."
GASNET_IF_ENABLED(allow-gcc32, Allow the use of the known broken gcc/g++ 3.2.0-2 compiler, [
  GASNET_MSG_WARN([$badgccmsg])
  ],[
  AC_MSG_ERROR([$badgccmsg \
  You may enable use of this broken compiler at your own risk by passing the --enable-allow-gcc32 flag.])
])
])
AC_TRY_COMPILE([
#if __GNUC__ == 4 && __GNUC_MINOR__ < 3
# error
#endif
],[ ], [:], [
AC_MSG_RESULT([$1 is gcc 4.x, for x < 3])
badgccmsg="Use of gcc/g++ 4.0, 4.1 or 4.2 for compiling this software is strongly discouraged. \
These versions have a known bug in the optimizer regarding aliasing analysis which may lead \
to bad code and incorrect runtime behavior when optimization is enabled. \
Consider using \$[$1] to select a different compiler."
GASNET_IF_ENABLED(allow-gcc4, Allow the use of a broken gcc/g++ 4.0-4.2 compiler, [
  GASNET_MSG_WARN([$badgccmsg])
  ],[
  AC_MSG_ERROR([$badgccmsg \
  You may enable use of this broken compiler at your own risk by passing the --enable-allow-gcc4 flag.\
  If you do so, please see the documentation on --enable-conservative-local-copy for a possible \
work around for the gcc-4.x bug.])
])
])
if test -z "$badgccmsg"; then
  AC_MSG_RESULT(ok)
fi
GASNET_FUN_END([$0($1)])
])

AC_DEFUN([GASNET_FIX_SHELL],[
GASNET_FUN_BEGIN([$0])
AC_MSG_CHECKING(for good shell)
if test "$BASH" = '' && test `uname` = HP-UX; then
  AC_MSG_RESULT([no, switching to bash])
  case [$]# in
    0) exec bash - "[$]0"        ;;
    *) exec bash - "[$]0" "[$]@" ;;
  esac
else
  AC_MSG_RESULT(yes)
fi
GASNET_FUN_END([$0])
])

dnl find full pathname for a given header file, if it exists and AC_SUBST it
AC_DEFUN([GASNET_FIND_HEADER],[
GASNET_FUN_BEGIN([$0($1)])
AC_REQUIRE([AC_PROG_AWK])
AC_CHECK_HEADERS($1)
pushdef([lowername],patsubst(patsubst(patsubst([$1], [/], [_]), [\.], [_]), [-], [_]))
pushdef([uppername],translit(lowername,'a-z','A-Z'))
if test "$ac_cv_header_[]lowername" = "yes"; then
  AC_MSG_CHECKING(for location of $1)
  echo "#include <$1>" > conftest.c
  header_pathname=
  if test "$GASNET_FIND_HEADER_CPP"; then
    echo "$GASNET_FIND_HEADER_CPP conftest.c" >&5
    header_pathname=`$GASNET_FIND_HEADER_CPP conftest.c 2>&5 | grep $1 | head -1`
    header_pathname=`echo $header_pathname | $AWK '{ printf("%s",[$]3); }'`
  fi
  if test -z "$header_pathname"; then
    echo "$CPP conftest.c" >&5
    header_pathname=`$CPP conftest.c 2>&5 | grep $1 | head -1`
    header_pathname=`echo $header_pathname | $AWK '{ printf("%s",[$]3); }'`
  fi
  if test -z "$header_pathname"; then
    # IBM xlc doesn't always emit include file name in output: try /usr/include
    if test -r /usr/include/$1; then
        header_pathname="\"/usr/include/$1\""
    fi
  fi
  if test -z "$header_pathname"; then
    AC_MSG_RESULT(unknown)
    GASNET_MSG_WARN(Unable to detect pathname of $1 - pretending it doesn't exist)
    have=0
  else
    AC_MSG_RESULT($header_pathname)
    have=1
  fi
else
  header_pathname=
  have=0
fi
PATH_[]uppername=$header_pathname
HAVE_[]uppername=$have
AC_SUBST(PATH_[]uppername)
AC_SUBST(HAVE_[]uppername)
popdef([uppername])
popdef([lowername])
GASNET_FUN_END([$0($1)])
])

dnl do AC_CHECK_SIZEOF and also AC_SUBST the result, second arg is optional prefix
AC_DEFUN([GASNET_CHECK_SIZEOF],[
  GASNET_FUN_BEGIN([$0($1,$2)])
  pushdef([typename],patsubst(patsubst([$1], [\ ], [_]), [\*], [p]))
  pushdef([barename],sizeof_[]typename)
  pushdef([lowername],translit($2[]barename,'A-Z','a-z'))
  pushdef([uppername],translit($2[]barename,'a-z','A-Z'))

  if test "$cross_compiling" = "yes" ; then
    uppername=
    GASNET_TRY_CACHE_EXTRACT_EXPR([sizeof($1) (binary probe)],uppername,[],[sizeof($1)],uppername)
    if test -z "$uppername" ; then # last resort is to use CROSS var
      GASNET_CROSS_VAR(uppername,uppername)
    fi
    ac_cv_[]lowername=$uppername
  fi
  dnl use bare AC_CHECK_SIZEOF here to get correct .h behavior & avoid duplicate defs
  GASNET_PUSHVAR(ac_cv_[]barename,"$ac_cv_[]lowername")
  if test "$ac_cv_[]barename" = "" ; then
    unset ac_cv_[]barename
    unset ac_cv_type_[]typename
  fi
  if test "$2" != "" ; then
    AC_MSG_CHECKING([$2 size:])
  fi
  AC_CHECK_SIZEOF($1, $uppername) 
  gasnet_checksizeoftmp_[]lowername="$ac_cv_[]barename"
  GASNET_POPVAR(ac_cv_[]barename)
  ac_cv_[]lowername=$gasnet_checksizeoftmp_[]lowername
  uppername=$gasnet_checksizeoftmp_[]lowername
  if test "$uppername" = "0" -o "$uppername" = "" -o "$ac_cv_[]lowername" != "$uppername"; then
    GASNET_MSG_ERROR(failed to find sizeof($1))
  fi
  if test "$2" != ""; then
    dnl work around an irritating autoheader bug - 
    dnl different autoheader versions handle the auto-AC_DEFINE done by
    dnl AC_CHECK_SIZEOF differently. This mantra should ensure we get exactly one
    dnl copy of each def in the config.h.in for any autoheader version
    ac_cv_[]uppername[]_indirect=uppername
    dnl following must appear exactly once to prevent errors
    AC_DEFINE_UNQUOTED($ac_cv_[]uppername[]_indirect,$uppername) 
  fi
  AC_SUBST(uppername)

  popdef([barename])
  popdef([typename])
  popdef([lowername])
  popdef([uppername])
  GASNET_FUN_END([$0($1,$2)])
])

dnl GASNET_CHECK_INTTYPES(headername) 
dnl AC_DEFINE and set HAVE_HEADERNAME_H if the header exists
dnl AC_DEFINE and AC_SUBST COMPLETE_HEADERNAME_H if it contains all the inttypes 
dnl that we care about (all of which are mandated by C99 and POSIX!)
AC_DEFUN([GASNET_CHECK_INTTYPES_HELPERPROG],[
    #include <$1>
    int check() {
    	int8_t    i8;
    	uint8_t   u8;
    	int16_t  i16;
    	uint16_t u16;
    	int32_t  i32;
    	uint32_t u32;
    	int64_t  i64;
    	uint64_t u64;
    	intptr_t  ip;
    	uintptr_t up;
	if (sizeof(i8) != 1) return 1;
	if (sizeof(u8) != 1) return 1;
	if (sizeof(i16) != 2) return 1;
	if (sizeof(u16) != 2) return 1;
	if (sizeof(i32) != 4) return 1;
	if (sizeof(u32) != 4) return 1;
	if (sizeof(i64) != 8) return 1;
	if (sizeof(u64) != 8) return 1;
	if (sizeof(ip) != sizeof(void*)) return 1;
	if (sizeof(up) != sizeof(void*)) return 1;
        return 0;
    }
])
dnl detect whether a given inttypes header is available and complete, with optional prefix
AC_DEFUN([GASNET_CHECK_INTTYPES],[
  GASNET_FUN_BEGIN([$0($1,$2)])
  pushdef([lowername],patsubst(patsubst(patsubst([$1], [/], [_]), [\.], [_]), [-], [_]))
  pushdef([uppername],translit(lowername,'a-z','A-Z'))
 GASNET_PUSHVAR(HAVE_[]uppername,"")
 dnl force a recheck
 unset ac_cv_header_[]lowername 
 AC_CHECK_HEADERS([$1])
 GASNET_POPVAR(HAVE_[]uppername)
 if test "$ac_cv_header_[]lowername" = "yes"; then
  [$2]HAVE_[]uppername=1
  AC_SUBST([$2]HAVE_[]uppername)
  AC_DEFINE([$2]HAVE_[]uppername)
  if test "$cross_compiling" = "yes" ; then
    dnl if cross-compiling, just ensure the header can build the inttypes program and hope for the best
    GASNET_TRY_CACHE_CHECK([for a complete $1],[$2]COMPLETE_[]uppername,[
      GASNET_CHECK_INTTYPES_HELPERPROG($1)
    ],[ return check(); ], [
      [$2]COMPLETE_[]uppername=1
      AC_SUBST([$2]COMPLETE_[]uppername)
      AC_DEFINE([$2]COMPLETE_[]uppername)
    ])
  else 
    dnl otherwise, build and run the inttypes program to ensure the header values are actually correct
    GASNET_TRY_CACHE_RUN([for a complete $1],[$2]COMPLETE_[]uppername,[
      GASNET_CHECK_INTTYPES_HELPERPROG($1)
      int main(void) { return check(); }
    ],[ 
      [$2]COMPLETE_[]uppername=1
      AC_SUBST([$2]COMPLETE_[]uppername)
      AC_DEFINE([$2]COMPLETE_[]uppername)
    ])
  fi
 fi
  popdef([lowername])
  popdef([uppername])
  GASNET_FUN_END([$0($1,$2)])
])

dnl PR828: AM_CONDITIONAL must appear on all control paths
dnl this macro runs them for a prefix which is not encountered
AC_DEFUN([GASNET_SETUP_INTTYPES_DUMMY], [ 
  pushdef([cvsizeof],translit(ac_cv_[$1]sizeof_,'A-Z','a-z'))
  if test x"$[]cvsizeof[]int$[]cvsizeof[]long$[]cvsizeof[]void_p" = x444; then
    $1[]PLATFORM_ILP32=yes
  fi
  if test x"$[]cvsizeof[]int$[]cvsizeof[]long$[]cvsizeof[]void_p" = x488; then
    $1[]PLATFORM_LP64=yes
  fi
  if test x"$[]cvsizeof[]int$[]cvsizeof[]long$[]cvsizeof[]void_p" = x888; then
    $1[]PLATFORM_ILP64=yes
  fi
  dnl following worksaround buggy automake which mishandles m4 expansions in AM_CONDITIONAL
  dnl these versions just shut up its whining (but still provide correct values)
  AM_CONDITIONAL(PLATFORM_ILP32, test "$PLATFORM_ILP32" = "yes") 
  AM_CONDITIONAL(PLATFORM_LP64,  test "$PLATFORM_LP64" = "yes")
  AM_CONDITIONAL(PLATFORM_ILP64, test "$PLATFORM_ILP64" = "yes")
  dnl and now the real versions..
  AM_CONDITIONAL($1[]PLATFORM_ILP32, test "$[$1]PLATFORM_ILP32" = "yes") 
  AM_CONDITIONAL($1[]PLATFORM_LP64,  test "$[$1]PLATFORM_LP64" = "yes")
  AM_CONDITIONAL($1[]PLATFORM_ILP64, test "$[$1]PLATFORM_ILP64" = "yes")
  popdef([cvsizeof])
])

dnl all the inttypes goop required for portable_inttypes.h
dnl second arg is optional prefix for defs
AC_DEFUN([GASNET_SETUP_INTTYPES], [ 
  GASNET_FUN_BEGIN([$0($1,$2)])
  GASNET_CHECK_SIZEOF(char, $1)
  GASNET_CHECK_SIZEOF(short, $1)
  GASNET_CHECK_SIZEOF(int, $1)
  GASNET_CHECK_SIZEOF(long, $1)
  GASNET_CHECK_SIZEOF(long long, $1)
  GASNET_CHECK_SIZEOF(void *, $1)

  GASNET_SETUP_INTTYPES_DUMMY($1) 
 
  GASNET_CHECK_INTTYPES(stdint.h,$1)
  GASNET_CHECK_INTTYPES(inttypes.h,$1)
  GASNET_CHECK_INTTYPES(sys/types.h,$1)
 
  [$1]INTTYPES_DEFINES="-D[$1]SIZEOF_CHAR=$[$1]SIZEOF_CHAR -D[$1]SIZEOF_SHORT=$[$1]SIZEOF_SHORT -D[$1]SIZEOF_INT=$[$1]SIZEOF_INT -D[$1]SIZEOF_LONG=$[$1]SIZEOF_LONG -D[$1]SIZEOF_LONG_LONG=$[$1]SIZEOF_LONG_LONG -D[$1]SIZEOF_VOID_P=$[$1]SIZEOF_VOID_P"
  GASNET_APPEND_DEFINE([$1]INTTYPES_DEFINES, [$1]HAVE_STDINT_H)
  GASNET_APPEND_DEFINE([$1]INTTYPES_DEFINES, [$1]COMPLETE_STDINT_H)
  GASNET_APPEND_DEFINE([$1]INTTYPES_DEFINES, [$1]HAVE_INTTYPES_H)
  GASNET_APPEND_DEFINE([$1]INTTYPES_DEFINES, [$1]COMPLETE_INTTYPES_H)
  GASNET_APPEND_DEFINE([$1]INTTYPES_DEFINES, [$1]HAVE_SYS_TYPES_H)
  GASNET_APPEND_DEFINE([$1]INTTYPES_DEFINES, [$1]COMPLETE_SYS_TYPES_H)
 
  AC_SUBST([$1]INTTYPES_DEFINES)
  GASNET_FUN_END([$0($1,$2)])
])


dnl Appends -Dvar_to_define onto target_var, iff var_to_define is set
dnl GASNET_APPEND_DEFINE(target_var, var_to_define)
AC_DEFUN([GASNET_APPEND_DEFINE],[
GASNET_FUN_BEGIN([$0])
  if test "$[$2]" != ""; then
    [$1]="$[$1] -D[$2]"
  fi
GASNET_FUN_END([$0])
]) 

dnl GASNET_SUBST_TEXT(varname, text to subst)
dnl perform subst for multi-line text fields
AC_DEFUN([GASNET_SUBST_TEXT],[
  GASNET_FUN_BEGIN([$0($1,...)])
  mkdir -p "$TOP_BUILDDIR/.subst_text"
  $1="$TOP_BUILDDIR/.subst_text/$1"
  cat > $$1 <<EOF
$2
EOF
  AC_SUBST_FILE($1)
  GASNET_FUN_END([$0($1,...)])
])

dnl push a new value into variable varname, saving the old value
dnl GASNET_PUSHVAR(varname, new value)
AC_DEFUN([GASNET_PUSHVAR],[
  GASNET_FUN_BEGIN([$0($1,$2)])
  dnl echo "old value of $1: $[$1]"
  if test "$_pushcnt_$1" = "" ; then
    _pushcnt_$1=0
  fi
  if test "$_total_pushcnt" = "" ; then
    _total_pushcnt=0
  fi
  if test "${$1+set}" = set; then
   _gasnet_pushvar_isset=1
  else
   _gasnet_pushvar_isset=0
  fi
  eval _pushedvar_$1_$_pushcnt_$1=\$[$1]
  eval _pushedvarset_$1_$_pushcnt_$1=$_gasnet_pushvar_isset
  _pushcnt_$1=`expr $_pushcnt_$1 + 1`
  _total_pushcnt=`expr $_total_pushcnt + 1`
  $1=$2
  echo "pushed new $1 value: $[$1]" >&5
  GASNET_FUN_END([$0($1,$2)])
]) 
dnl push a variable, then unset it
AC_DEFUN([GASNET_PUSHVAR_UNSET],[
  GASNET_FUN_BEGIN([$0($1)])
    GASNET_PUSHVAR($1,"<unset>") 
    unset $1
  GASNET_FUN_END([$0($1)])
])

dnl restore the old value of varname, from a previous push
dnl GASNET_POPVAR(varname)
AC_DEFUN([GASNET_POPVAR],[
  GASNET_FUN_BEGIN([$0($1)])
  if test "$_pushcnt_$1" -ge "1"; then
    _pushcnt_$1=`expr $_pushcnt_$1 - 1`
    _total_pushcnt=`expr $_total_pushcnt - 1`
    eval _gasnet_pushvar_isset=\$_pushedvarset_$1_$_pushcnt_$1
    if test "$_gasnet_pushvar_isset" = "1" ; then
      eval $1=\$_pushedvar_$1_$_pushcnt_$1
      echo "popping $1 back to: $[$1]" >&5
    else
      unset $1
      echo "popping $1 back to: <unset>" >&5
    fi
  else
    GASNET_MSG_ERROR([INTERNAL ERROR: GASNET_PUSH/POPVAR underflow on $1])
  fi
  GASNET_FUN_END([$0($1)])
]) 

AC_DEFUN([GASNET_PUSHPOP_CHECK],[
GASNET_FUN_BEGIN([$0])
  if test "$_total_pushcnt" -ge "1" ; then
    GASNET_MSG_ERROR([INTERNAL ERROR: GASNET_PUSH/POPVAR mismatch: $_total_pushcnt more pushes than pops])
  fi
GASNET_FUN_END([$0])
])

dnl add file to list of executable outputs that should be marked +x
dnl would be nice to use AC_CONFIG_COMMANDS() for each file, but autoconf 2.53
dnl  stupidly fails to execute commands having the same tag as a config output file
dnl  on subsequent calls to config.status
AC_DEFUN([GASNET_FIX_EXEC],[
GASNET_FUN_BEGIN([$0($1)])
  cv_prefix[]exec_list="$cv_prefix[]exec_list $1"
GASNET_FUN_END([$0($1)])
])

dnl ensure the "default" command is run on every invocation of config.status
AC_DEFUN([GASNET_FIX_EXEC_SETUP],[[
GASNET_FUN_BEGIN([$0])
  dnl round-about method ensure autoconf 2.53 picks up depfiles command
  if test "\${config_commands+set}" != set ; then
    config_commands="default"
  fi
  CONFIG_COMMANDS="\$config_commands"
  cv_prefix[]exec_list="$cv_prefix[]exec_list"
GASNET_FUN_END([$0])
]])

AC_DEFUN([GASNET_FIX_EXEC_OUTPUT],[[
GASNET_FUN_BEGIN([$0])
  for file in $cv_prefix[]exec_list; do
   case "$CONFIG_FILES" in
     *${file}*) chmod +x ${file} ;;
   esac
  done
GASNET_FUN_END([$0])
]])

dnl bug1657: remove the autoconf-generated PACKAGE_* macros from config.h file
AC_DEFUN([GASNET_SANITIZE_PACKAGE],[[
GASNET_FUN_BEGIN([$0($1)])
  if test -f '$1' -a -n "`grep PACKAGE_NAME '$1'`" ; then
    mv '$1' '$1.dirty'
    cat '$1.dirty' | grep -v '#define PACKAGE_' > '$1'
    rm -f '$1.dirty'
  fi
GASNET_FUN_END([$0($1)])
]])

AC_DEFUN([GASNET_LIBGCC],[
GASNET_FUN_BEGIN([$0])
AC_REQUIRE([AC_PROG_CC])
AC_CACHE_CHECK(for libgcc link flags, cv_prefix[]lib_gcc,
[if test "$GCC" = yes; then
  #LIBGCC="`$CC -v 2>&1 | sed -n 's:^Reading specs from \(.*\)/specs$:-L\1 -lgcc:p'`"
  LIBGCC="-L`$CC -print-libgcc-file-name | xargs dirname` -lgcc"
  if test -z "$LIBGCC"; then
    GASNET_MSG_ERROR(cannot find libgcc)
  fi
fi
cv_prefix[]lib_gcc="$LIBGCC"])
LIBGCC="$cv_prefix[]lib_gcc"
AC_SUBST(LIBGCC)
GASNET_FUN_END([$0])
])

AC_DEFUN([GASNET_LIBM],[
GASNET_FUN_BEGIN([$0])
AC_REQUIRE([AC_PROG_CC])
GASNET_PUSHVAR(LIBS,"$LIBS")
case "$target_os" in
  darwin*)
    # libm is just an alias for the system default lib
    # Naming it explicitly causes linker failures when linking w/ mpich
  ;;
  *)
    # sin should be in everyone's libm if they've got one.
    AC_CHECK_LIB(m, sin, LIBM="-lm", LIBM="")
  ;;
esac
AC_SUBST(LIBM)
GASNET_POPVAR(LIBS)
GASNET_FUN_END([$0])
])

dnl GASNET_ENV_DEFAULT(envvar-name, default-value)
dnl  load an environment variable, using default value if it's missing from env.
dnl  caches the results to guarantee reconfig gets the originally loaded value
dnl  also adds a --with-foo-bar= option for the env variable FOO_BAR
AC_DEFUN([GASNET_ENV_DEFAULT],[
  GASNET_FUN_BEGIN([$0($1,$2)])
  pushdef([lowerdashname],patsubst(translit([$1],'A-Z','a-z'), _, -))
  pushdef([lowerscorename],patsubst(translit([$1],'A-Z','a-z'), -, _))
  
  AC_MSG_CHECKING(for $1 in environment)

  dnl create the help prompt just once, and only if not suppressed
  ifdef(with_expanded_[$1], [], [
   ifdef([GASNET_ENV_DEFAULT_SUPPRESSHELP], [], [
    dnl don't advertise the autoconf vars, which we might not reliably catch
    ifelse(index([ CC CFLAGS LDFLAGS CPPFLAGS CPP CXX CXXFLAGS CXXCPP ],[ $1 ]),[-1],[
    AC_ARG_WITH(lowerdashname, 
       GASNET_OPTION_HELP(with-[]lowerdashname[]=, value for [$1]), 
      [], [])
    ], [])
   ])
  ])
  define(with_expanded_[$1], [set])

  envval_src_[$1]="cached"
  AC_CACHE_VAL(cv_prefix[]envvar_$1, [
      case "${[$1]-__NOT_SET__}" in
	__NOT_SET__) 
            if test "$with_[]lowerscorename" != ""; then
	      cv_prefix[]envvar_$1="$with_[]lowerscorename"
	      envval_src_[$1]=given
	    else
	      cv_prefix[]envvar_$1="[$2]"
	      envval_src_[$1]=default
	    fi 
	    ;;
	*)  cv_prefix[]envvar_$1="$[$1]"
	    envval_src_[$1]=given
      esac
  ])

  [$1]="$cv_prefix[]envvar_$1"
  case "$envval_src_[$1]" in
      'cached')
	  AC_MSG_RESULT([using cached value \"$[$1]\"]) ;;
      'default')
	  AC_MSG_RESULT([no, defaulting to \"$[$1]\"]) ;;
      'given')
	  AC_MSG_RESULT([yes, using \"$[$1]\"]) ;;
      *) GASNET_MSG_ERROR(_GASNET_ENV_DEFAULT broken)
  esac

  popdef([lowerdashname])
  popdef([lowerscorename])
  GASNET_FUN_END([$0($1,$2)])
])

dnl $1 = optional env variables to restore
AC_DEFUN([GASNET_START_CONFIGURE],[
  GASNET_FUN_BEGIN([$0($1)])
  GASNET_PATH_PROGS(PWD_PROG, pwd, pwd)

  define([GASNET_CONFIGURE_WARNING_LOCAL],[.[]cv_prefix[]configure_warnings.tmp])
  if test -z "$GASNET_CONFIGURE_WARNING_GLOBAL" ; then
    GASNET_CONFIGURE_WARNING_GLOBAL="`$PWD_PROG`/GASNET_CONFIGURE_WARNING_LOCAL"
    export GASNET_CONFIGURE_WARNING_GLOBAL
  fi
  rm -f "GASNET_CONFIGURE_WARNING_LOCAL"

  dnl Save and display useful info about the configure environment
  GASNET_GET_AUTOCONF_VERSION()
  AC_MSG_CHECKING(for configure settings) 
  AC_MSG_RESULT([])
  CONFIGURE_ARGS="$ac_configure_args"
  AC_SUBST(CONFIGURE_ARGS)
  AC_MSG_RESULT( configure args: $CONFIGURE_ARGS)
  dnl ensure the cache is used in all reconfigures
  if test "$cache_file" = "/dev/null" ; then
    GASNET_MSG_WARN([configure cache_file setting got lost - you may need to run a fresh ./Bootstrap])
    cache_file=config.cache
  fi
  ac_configure_args="$ac_configure_args --cache-file=$cache_file"
  dnl don't trust shell's builtin pwd, because it may include symlinks
  TOP_SRCDIR=`cd ${srcdir} && ${PWD_PROG}` 
  AC_MSG_RESULT( TOP_SRCDIR:     $TOP_SRCDIR)
  AC_SUBST(TOP_SRCDIR)
  TOP_BUILDDIR=`${PWD_PROG}`
  AC_MSG_RESULT( TOP_BUILDDIR:   $TOP_BUILDDIR)
  AC_SUBST(TOP_BUILDDIR)
  dnl check against bug 1083 (spaces in directory name break things)
  if `echo $TOP_SRCDIR | grep ' ' >/dev/null 2>/dev/null`; then
    AC_MSG_ERROR(TOP_SRCDIR contains space characters - please unpack the source in a different directory.)
  fi
  if `echo $TOP_BUILDDIR | grep ' ' >/dev/null 2>/dev/null`; then
    AC_MSG_ERROR(TOP_BUILDDIR contains space characters - please build in a different directory.)
  fi
  dnl set AM_CONDITIONAL BUILD_IS_SRC for ease of use in generated Makefiles
  AM_CONDITIONAL(BUILD_IS_SRC, test "$TOP_BUILDDIR" = "$TOP_SRCDIR")
  dnl set AC_SUBST variable BUILD_IS_SRC for ease of use in generated scripts
  if test "$TOP_BUILDDIR" = "$TOP_SRCDIR"; then
    BUILD_IS_SRC=yes
  else
    BUILD_IS_SRC=no
  fi
  AC_SUBST(BUILD_IS_SRC)

  # Handle prefix defaulting with subconfigures
  # When users pass --prefix on the command-line, this is automatically passed to subconfigures
  #  and everything lives happily in the same tree
  # Without --prefix, different packages may default to different directories
  # (due to AC_PREFIX_DEFAULT), so ensure that all subconfigures 
  # default to the same location (as determined by the outermost configure)
  if test "$prefix" = "NONE" -a -n "$ac_default_prefix" ; then
    GASNET_SUBCONFIGURE_ARG(--prefix="$ac_default_prefix")
  fi

  SYSTEM_NAME="`hostname`"
  AC_SUBST(SYSTEM_NAME)
  case "$target" in 
   # Apple gcc has -arch options for cross-compilation, but target binaries may still work due to Rosetta (making our cross-compilation support unnecessary)
   # ensure we report the correct target tuple
   *-apple-darwin*)
     _GASNET_GCCVER=`${CC:-gcc} -v 2>&1`
     _GASNET_GCCISAPPLE=`echo "$_GASNET_GCCVER" | grep 'gcc version' | grep 'Apple '`
     _GASNET_GCCTARGET=`echo "$_GASNET_GCCVER" | /usr/bin/perl -ne 'print \[$]1 if (m/--target=(\S+)/);'`
     _GASNET_GCCCPU=`echo "$_GASNET_GCCVER" | /usr/bin/perl -ne 'print \[$]1 if (m/--target=([[^-]]+)/);'`
     if test "$_GASNET_GCCISAPPLE" -a "$_GASNET_GCCTARGET" -a "$_GASNET_GCCCPU" ; then
        case "$target" in
         $_GASNET_GCCCPU-*) ;; 
         *) 
         GASNET_MSG_WARN([Apple gcc is cross-compiling for $_GASNET_GCCTARGET, readjusting configure target])
         target="$_GASNET_GCCTARGET" 
         target_alias="$target" 
         ac_cv_target="$target" 
         ac_cv_target_alias="$target" 
         target_cpu="$_GASNET_GCCCPU" 
        esac
     fi
     ;;
  esac
  SYSTEM_TUPLE="$target"
  AC_SUBST(SYSTEM_TUPLE)
  AC_MSG_RESULT( system info:      $SYSTEM_NAME $SYSTEM_TUPLE)
  BUILD_USER=`whoami 2> /dev/null || id -un 2> /dev/null || echo $USER`
  BUILD_ID="`date` $BUILD_USER"
  AC_MSG_RESULT( build id:       $BUILD_ID)
  AC_SUBST(BUILD_ID)

  # ensure exec_list doesn't grow continuously each time we reconfigure
  unset cv_prefix[]exec_list

  GASNET_RESTORE_AUTOCONF_ENV([CC CXX CFLAGS CXXFLAGS CPPFLAGS LIBS MAKE GMAKE AR AS RANLIB PERL SUM LEX YACC $1])
  GASNET_FUN_END([$0($1)])
])

AC_DEFUN([GASNET_END_CONFIGURE],[
  GASNET_ERR_CLEANUP()
  GASNET_SAVE_AUTOCONF_ENV()
  GASNET_PUSHPOP_CHECK()
])

dnl AC_DEFINE the configure information variables detected by GASNET_START_CONFIGURE, with prefix
AC_DEFUN([GASNET_DEFINE_CONFIGURE_VARS],[
  GASNET_FUN_BEGIN([$0])
  AC_REQUIRE([GASNET_START_CONFIGURE])
  AC_DEFINE_UNQUOTED($1_[]CONFIGURE_ARGS, "$CONFIGURE_ARGS")
  AC_DEFINE_UNQUOTED($1_[]SYSTEM_NAME,    "$SYSTEM_NAME")
  AC_DEFINE_UNQUOTED($1_[]SYSTEM_TUPLE,   "$SYSTEM_TUPLE")
  AC_DEFINE_UNQUOTED($1_[]BUILD_ID,       "$BUILD_ID")
  GASNET_FUN_END([$0])
])

dnl GASNET_RESTORE_AUTOCONF_ENV(env1 env2 env3) 
dnl  call at top of configure.in to restore cached environment variables 
dnl  inspected by autoconf macros. Pass in names of variables
AC_DEFUN([GASNET_RESTORE_AUTOCONF_ENV],[
  GASNET_FUN_BEGIN([$0($1)])
  dnl  pushdef = get a variable prefix variable which won't be cached.
  pushdef([nc_prefix],patsubst(cv_prefix,_cv_,_))
  if test "$nc_prefix[]acenv_list" != ""; then
    GASNET_MSG_ERROR(_GASNET_RESTORE_AUTOCONF_ENV called more than once with prefix = "cv_prefix")
  fi
  nc_prefix[]acenv_list="$1"
  AC_MSG_CHECKING(for cached autoconf environment settings)
  AC_MSG_RESULT("") 
  for varname in $1; do
    val=`eval echo '$'"cv_prefix[]acenv_$varname"`
    if test "$val" != ""; then
      eval $varname=\"$val\"
      AC_MSG_RESULT([$varname=\"$val\"]) 
    fi
  done
  popdef([nc_prefix])
  GASNET_FUN_END([$0($1)])
])

dnl GASNET_SAVE_AUTOCONF_ENV() 
dnl  cache the environment variables inspected by autoconf macros
AC_DEFUN([GASNET_SAVE_AUTOCONF_ENV],[
  GASNET_FUN_BEGIN([$0])
  for varname in $cv_prefix[]acenv_list; do
    val=`eval echo '$'"$varname"`
    if test "$val" != ""; then
      cachevarname=cv_prefix[]acenv_$varname
      eval $cachevarname=\"$val\"
    fi
  done
  GASNET_FUN_END([$0])
])

dnl m4 substr fiasco:
dnl autoconf 2.13 has a working version of the m4 function 'substr', 
dnl  but no m4_substr (and no format or m4_format)
dnl autoconf 2.58 has working versions of m4_substr and m4_format, 
dnl  but no substr or format
dnl This incantation ensures m4_substr works regardless
ifdef([substr],[define([m4_substr], defn([substr]))])

AC_DEFUN([GASNET_OPTION_HELP],[  --$1 ]m4_substr[([                         ],len([$1]))[$2]])

dnl provide a --with-foo=bar configure option
dnl action-withval runs for a named value in $withval (or withval=yes if named arg missing)
dnl action-without runs for --without-foo or --with-foo=no
dnl action-none runs for no foo arg given
dnl GASNET_WITH(foo, description, action-withval, [action-without], [action-none])
AC_DEFUN([GASNET_WITH],[
GASNET_FUN_BEGIN([$0($1,...)])
AC_ARG_WITH($1,GASNET_OPTION_HELP(with-$1=value,[$2]), [
  case "$withval" in
    no) :
        $4 ;;
    *)  $3 ;;
  esac
  ],[
   :
   $5
  ])
GASNET_FUN_END([$0($1,...)])
])

AC_DEFUN([GASNET_IF_ENABLED_NOHELP],[
case "$enable_[]patsubst([$1], -, _)" in
  '' | no) :
      $3 ;;
  *)  $2 ;;
esac
])

AC_DEFUN([GASNET_IF_ENABLED],[
GASNET_FUN_BEGIN([$0($1,...)])
AC_ARG_ENABLE($1,GASNET_OPTION_HELP(enable-$1,[$2]))
GASNET_IF_ENABLED_NOHELP([$1],[$3],[$4])
GASNET_FUN_END([$0($1,...)])
])

AC_DEFUN([GASNET_IF_DISABLED],[
GASNET_FUN_BEGIN([$0($1,...)])
AC_ARG_ENABLE($1,GASNET_OPTION_HELP(disable-$1,[$2]))
case "$enable_[]patsubst([$1], -, _)" in
  '' | yes) :
       $4 ;;
  *)   $3 ;;
esac
GASNET_FUN_END([$0($1,...)])
])

AC_DEFUN([GASNET_IF_ENABLED_WITH_AUTO],[
GASNET_FUN_BEGIN([$0($1,...)])
AC_ARG_ENABLE($1,GASNET_OPTION_HELP(enable-$1,[$2]))
AC_ARG_ENABLE($1,GASNET_OPTION_HELP(disable-$1,[$2]))
case "$enable_[]patsubst([$1], -, _)" in
  no)  $4 ;;
  yes) $3 ;;
  *)   $5 ;;
esac
GASNET_FUN_END([$0($1,...)])
])

AC_DEFUN([GASNET_SUBST],[
$1="$2"
AC_SUBST($1)])

AC_DEFUN([GASNET_SUBST_FILE],[
$1="$2"
AC_SUBST_FILE($1)])

AC_DEFUN([GASNET_CHECK_PROGS],[
GASNET_FUN_BEGIN([$0($1,$2,$3)])
case "$$1" in
  '') AC_CHECK_PROGS($1,$2)
      ;;
  *) AC_MSG_CHECKING(for $3)
     AC_MSG_RESULT($$1)
      ;;
esac
case "$$1" in
  '') AC_MSG_ERROR(cannot find $3)
      ;;
esac
GASNET_FUN_END([$0($1,$2,$3)])
])

AC_DEFUN([GASNET_PATH_PROGS],[
GASNET_FUN_BEGIN([$0($1,$2,$3)])
case "$$1" in
  '') AC_PATH_PROGS($1,$2)
      ;;
  *) AC_MSG_CHECKING(for $3)
     AC_MSG_RESULT($$1)
      ;;
esac
case "$$1" in
  '') AC_MSG_ERROR(cannot find $3)
      ;;
esac
GASNET_FUN_END([$0($1,$2,$3)])
])

dnl GASNET_GETFULLPATH(var)
dnl var contains a program name, optionally followed by arguments
dnl expand the program name to a fully qualified pathname if not already done
AC_DEFUN([GASNET_GETFULLPATH_CHECK],[
GASNET_IF_DISABLED(full-path-expansion, [Disable expansion of program names to full pathnames], 
                   [cv_prefix[]_gfp_disable=1])
])
AC_DEFUN([GASNET_GETFULLPATH],[
GASNET_FUN_BEGIN([$0($1)])
AC_REQUIRE([AC_PROG_AWK])
AC_REQUIRE([GASNET_GETFULLPATH_CHECK])
if test "$cv_prefix[]_gfp_disable" = ""; then
  gasnet_gfp_progname=`echo "$$1" | $AWK -F' ' '{ print [$]1 }'`
  gasnet_gfp_progargs=`echo "$$1" | $AWK -F' ' 'BEGIN { ORS=" "; } { for (i=2;i<=NF;i++) print $i; }'`
  gasnet_gfp_progname0=`echo "$gasnet_gfp_progname" | $AWK '{ print sub[]str([$]0,1,1) }'`
  if test "$gasnet_gfp_progname0" != "/" ; then
    if test "`echo $gasnet_gfp_progname | grep '/'`" != "" ; then
      AC_MSG_ERROR([$1 was set to a relative path: $gasnet_gfp_progname -- please use an absolute pathname])
    fi
    # clear cached values, in case this is a pushed var
    unset cv_prefix[]_gfp_fullprogname_$1
    unset ac_cv_path_[]cv_prefix[]_gfp_fullprogname_$1
    # [AC_PATH_PROG](cv_prefix[]_gfp_fullprogname_$1, $gasnet_gfp_progname,[])
    AC_PATH_PROG(cv_prefix[]_gfp_fullprogname_$1, $gasnet_gfp_progname,[])
    AC_MSG_CHECKING(for full path expansion of $1)
    if test "$cv_prefix[]_gfp_fullprogname_$1" != "" ; then
      $1="$cv_prefix[]_gfp_fullprogname_$1 $gasnet_gfp_progargs"
    fi
    AC_MSG_RESULT($$1)
  fi
fi
GASNET_FUN_END([$0($1)])
])

dnl GASNET_FOLLOWLINKS(var)
dnl var contains a filename
dnl If it is a symlink, follow it until a non-symlink is reached
dnl Designed not to use readlink, which might not exist.
AC_DEFUN([GASNET_FOLLOWLINKS],[
GASNET_FUN_BEGIN([$0($1)])
AC_REQUIRE([AC_PROG_AWK])
  gasnet_fl_file="$$1"
  gasnet_fl_link=`/bin/ls -al "$gasnet_fl_file" | $AWK 'BEGIN{FS=">"}{split([$]2,A," ") ; print A[[1]]}'`
  while test "$gasnet_fl_link"; do
    gasnet_fl_file="$gasnet_fl_link"
    gasnet_fl_link=`/bin/ls -al "$gasnet_fl_file" | $AWK 'BEGIN{FS=">"}{split([$]2,A," ") ; print A[[1]]}'`
  done
  $1="$gasnet_fl_file"
GASNET_FUN_END([$0($1)])
])

dnl GASNET_CHECK_LIB(library, function, action-if-found, action-if-not-found, other-flags, other-libraries)
AC_DEFUN([GASNET_CHECK_LIB],[
GASNET_FUN_BEGIN([$0($1,$2,...)])
GASNET_PUSHVAR(LDFLAGS,"$LDFLAGS $5")
AC_CHECK_LIB($1, $2, $3, $4, $6)
GASNET_POPVAR(LDFLAGS)
GASNET_FUN_END([$0($1,$2,...)])
])

dnl GASNET_TRY_RUNCMD(command, action-success-nooutput, action-success-output, action-error)
dnl run a command, and take action based on the result code and output (in $gasnet_cmd_stdout/$gasnet_cmd_stderr)
AC_DEFUN([GASNET_TRY_RUNCMD],[
  GASNET_FUN_BEGIN([$0($1,...)])
  echo \"$1\" >&5
  ( $1 ) > conftest-runcmdout 2> conftest-runcmderr
  gasnet_cmd_result="$?"
  gasnet_cmd_stdout="`cat conftest-runcmdout`"
  gasnet_cmd_stderr="`cat conftest-runcmderr`"
  cat conftest-runcmdout >&5
  cat conftest-runcmderr >&5
  echo gasnet_cmd_result=$gasnet_cmd_result >&5
  rm -rf conftest*
  if test "$gasnet_cmd_result" = "0" ; then 
    if test -z "$gasnet_cmd_stdout$gasnet_cmd_stderr" ; then
      :
      $2 
    else
      :
      $3 
    fi
  else
    :
    $4 
  fi
  GASNET_FUN_END([$0($1,...)])
])

dnl GASNET_TRY_CCOMPILE_WITHWARN(includes, function-body, action-success, action-warning, action-error)
dnl Compile a C program and take different actions based on complete success, error or warning
dnl Automatically handles compilers that issue unrelated warnings on every compile

AC_DEFUN([GASNET_TRY_CCOMPILE_WITHWARN],[
  GASNET_FUN_BEGIN([$0(...)])
  GASNET_TRY_CCOMPILE_WITHWARN_NORETRY([$1],[$2],[$3],[
    dnl got a warning - does same warning also happen with an empty program?
    _GASNET_TRY_COMPILE_WITHWARN_OUTTMP="$gasnet_cmd_stdout"
    _GASNET_TRY_COMPILE_WITHWARN_ERRTMP="$gasnet_cmd_stderr"
    GASNET_TRY_CCOMPILE_WITHWARN_NORETRY([],[],[
        dnl no warning on empty program => warning caused by input
	gasnet_cmd_stdout="$_GASNET_TRY_COMPILE_WITHWARN_OUTTMP"
	gasnet_cmd_stderr="$_GASNET_TRY_COMPILE_WITHWARN_ERRTMP"
    	$4
    ],[ dnl still got a warning - is the same?
      if test "$gasnet_cmd_stdout$gasnet_cmd_stderr" = "$_GASNET_TRY_COMPILE_WITHWARN_OUTTMP$_GASNET_TRY_COMPILE_WITHWARN_ERRTMP" ; then
        dnl identical warnings => no new warnings caused by program
	$3
      else
        dnl different warnings => program is likely causal factor
	gasnet_cmd_stdout="$_GASNET_TRY_COMPILE_WITHWARN_OUTTMP"
	gasnet_cmd_stderr="$_GASNET_TRY_COMPILE_WITHWARN_ERRTMP"
	$4
      fi
    ],[ dnl got an error on an empty program!
      GASNET_MSG_ERROR([unknown failure case in TRY_CCOMPILE_WITHWARN])
    ])
  ],[$5])
  GASNET_FUN_END([$0(...)])
])

dnl for internal use only
AC_DEFUN([GASNET_TRY_CCOMPILE_WITHWARN_NORETRY],[
  GASNET_FUN_BEGIN([$0(...)])
  gasnet_testname=gasnet-conftest
  gasnet_testfile=${gasnet_testname}.c
  gasnet_compile_cmd="${CC-cc} -c $CFLAGS $CPPFLAGS $gasnet_testfile"
  cat > $gasnet_testfile <<EOF
#include "confdefs.h"
$1
int main(void) {
$2
; return 0; }
EOF
  GASNET_TRY_RUNCMD([$gasnet_compile_cmd], [$3], [
    echo "configure: warned program was:" >&5
    cat $gasnet_testfile >&5
    $4
    ],[
    echo "configure: failed program was:" >&5
    cat $gasnet_testfile >&5
    $5
    ])
  rm -f ${gasnet_testname}.*
  GASNET_FUN_END([$0(...)])
])

dnl GASNET_TRY_CXXCOMPILE_WITHWARN(includes, function-body, action-success, action-warning, action-error)
dnl Compile a C++ program and take different actions based on complete success, error or warning
dnl Automatically handles compilers that issue unrelated warnings on every compile

AC_DEFUN([GASNET_TRY_CXXCOMPILE_WITHWARN],[
  GASNET_FUN_BEGIN([$0(...)])
  GASNET_TRY_CXXCOMPILE_WITHWARN_NORETRY([$1],[$2],[$3],[
    dnl got a warning - does same warning also happen with an empty program?
    _GASNET_TRY_COMPILE_WITHWARN_OUTTMP="$gasnet_cmd_stdout"
    _GASNET_TRY_COMPILE_WITHWARN_ERRTMP="$gasnet_cmd_stderr"
    GASNET_TRY_CXXCOMPILE_WITHWARN_NORETRY([],[],[
        dnl no warning on empty program => warning caused by input
	gasnet_cmd_stdout="$_GASNET_TRY_COMPILE_WITHWARN_OUTTMP"
	gasnet_cmd_stderr="$_GASNET_TRY_COMPILE_WITHWARN_ERRTMP"
    	$4
    ],[ dnl still got a warning - is the same?
      if test "$gasnet_cmd_stdout$gasnet_cmd_stderr" = "$_GASNET_TRY_COMPILE_WITHWARN_OUTTMP$_GASNET_TRY_COMPILE_WITHWARN_ERRTMP" ; then
        dnl identical warnings => no new warnings caused by program
	$3
      else
        dnl different warnings => program is likely causal factor
	gasnet_cmd_stdout="$_GASNET_TRY_COMPILE_WITHWARN_OUTTMP"
	gasnet_cmd_stderr="$_GASNET_TRY_COMPILE_WITHWARN_ERRTMP"
	$4
      fi
    ],[ dnl got an error on an empty program!
      GASNET_MSG_ERROR([unknown failure case in TRY_CXXCOMPILE_WITHWARN])
    ])
  ],[$5])
  GASNET_FUN_END([$0(...)])
])

dnl for internal use only
AC_DEFUN([GASNET_TRY_CXXCOMPILE_WITHWARN_NORETRY],[
  GASNET_FUN_BEGIN([$0(...)])
  gasnet_testname=gasnet-conftest
  gasnet_testfile=${gasnet_testname}.cc
  gasnet_compile_cmd="${CXX-c++} -c $CXXFLAGS $CPPFLAGS $gasnet_testfile"
  cat > $gasnet_testfile <<EOF
#include "confdefs.h"
$1
int main(void) {
$2
; return 0; }
EOF
  GASNET_TRY_RUNCMD([$gasnet_compile_cmd], [$3], [
    echo "configure: warned program was:" >&5
    cat $gasnet_testfile >&5
    $4
    ],[
    echo "configure: failed program was:" >&5
    cat $gasnet_testfile >&5
    $5
    ])
  rm -f ${gasnet_testname}.*
  GASNET_FUN_END([$0(...)])
])

dnl GASNET_TRY_COMPILE_WITHWARN(type, ...) where type is C or CXX
dnl Indirection for GASNET_TRY_CCOMPILE_WITHWARN or GASNET_TRY_CXXCOMPILE_WITHWARN
AC_DEFUN([GASNET_TRY_COMPILE_WITHWARN],
  GASNET_TRY_[$1]COMPILE_WITHWARN[([$2],[$3],[$4],[$5],[$6])])

dnl GASNET_TRY_CFLAG(flags, action-if-supported, action-if-not-supported)
AC_DEFUN([GASNET_TRY_CFLAG],[
GASNET_FUN_BEGIN([$0($1)])
GASNET_PUSHVAR(CFLAGS,"$CFLAGS $1")
AC_MSG_CHECKING(for C compiler flag $1)
GASNET_TRY_CCOMPILE_WITHWARN_NORETRY([], [], [
 AC_MSG_RESULT(yes)
 GASNET_POPVAR(CFLAGS)
 $2
], [
 dnl some compilers issue a warning on *every* compile, 
 dnl so save the warning and try again without the flag being tested, 
 dnl to verify the warning we saw is actually a new warning
 _GASNET_TRY_CFLAG_TMP="$gasnet_cmd_stdout$gasnet_cmd_stderr"
 GASNET_POPVAR(CFLAGS)
 GASNET_TRY_CCOMPILE_WITHWARN_NORETRY([], [], [
   dnl warning disappeared when flag removed => flag is the cause
   AC_MSG_RESULT(no/warning: $_GASNET_TRY_CFLAG_TMP)
   $3
 ],[ 
   if test "$gasnet_cmd_stdout$gasnet_cmd_stderr" = "$_GASNET_TRY_CFLAG_TMP" ; then
     dnl got same warning => flag does not create new warnings
     AC_MSG_RESULT(yes/persistent-warning: $_GASNET_TRY_CFLAG_TMP)
     $2
   else
     dnl warnings differ with and without flag => flag is probably a causal factor
     AC_MSG_RESULT(no/new-warning: $_GASNET_TRY_CFLAG_TMP)
     $3
   fi
 ],[ dnl got an error - should never happen?
   GASNET_MSG_ERROR([unknown failure case in TRY_CFLAG])
 ])
], [
 AC_MSG_RESULT(no/error: $gasnet_cmd_stdout$gasnet_cmd_stderr)
 GASNET_POPVAR(CFLAGS)
 $3
])
GASNET_FUN_END([$0($1)])
])

dnl GASNET_TRY_CXXFLAG(flags, action-if-supported, action-if-not-supported)
AC_DEFUN([GASNET_TRY_CXXFLAG],[
GASNET_FUN_BEGIN([$0($1)])
GASNET_PUSHVAR(CXXFLAGS,"$CXXFLAGS $1")
AC_MSG_CHECKING(for C++ compiler flag $1)
GASNET_TRY_CXXCOMPILE_WITHWARN_NORETRY([], [], [
 AC_MSG_RESULT(yes)
 GASNET_POPVAR(CXXFLAGS)
 $2
], [
 dnl some compilers issue a warning on *every* compile, 
 dnl so save the warning and try again without the flag being tested, 
 dnl to verify the warning we saw is actually a new warning
 _GASNET_TRY_CXXFLAG_TMP="$gasnet_cmd_stdout$gasnet_cmd_stderr"
 GASNET_POPVAR(CXXFLAGS)
 GASNET_TRY_CCOMPILE_WITHWARN_NORETRY([], [], [
   dnl warning disappeared when flag removed => flag is the cause
   AC_MSG_RESULT(no/warning: $_GASNET_TRY_CXXFLAG_TMP)
   $3
 ],[ 
   if test "$gasnet_cmd_stdout$gasnet_cmd_stderr" = "$_GASNET_TRY_CXXFLAG_TMP" ; then
     dnl got same warning => flag does not create new warnings
     AC_MSG_RESULT(yes/persistent-warning: $_GASNET_TRY_CXXFLAG_TMP)
     $2
   else
     dnl warnings differ with and without flag => flag is probably a causal factor
     AC_MSG_RESULT(no/new-warning: $_GASNET_TRY_CXXFLAG_TMP)
     $3
   fi
 ],[ dnl got an error - should never happen?
   GASNET_MSG_ERROR([unknown failure case in TRY_CXXFLAG])
 ])
], [
 AC_MSG_RESULT(no/error: $gasnet_cmd_stdout$gasnet_cmd_stderr)
 GASNET_POPVAR(CXXFLAGS)
 $3
])
GASNET_FUN_END([$0($1)])
])

dnl GASNET_SET_CHECKED_CFLAGS CCVAR CFLAGSVAR DEFAULT_CFLAGS SAFE_CFLAGS
dnl Set CFLAGSVAR to a values that works with CCVAR 
dnl if CFLAGSVAR is already set, then keep it
dnl otherwise, if DEFAULT_CFLAGS works, then use it
dnl otherwise, use SAFE_CFLAGS
AC_DEFUN([GASNET_SET_CHECKED_CFLAGS],[
GASNET_FUN_BEGIN([$0(...)])
if test "$[$1]" = "no" ; then
  : # Skip
elif test "$[$2]" != "" ; then
  GASNET_ENV_DEFAULT([$2], []) # user-provided flags
else
  GASNET_ENV_DEFAULT([$2], [$3]) # try DEFAULT_CFLAGS
  GASNET_PUSHVAR(CC,"$[$1]")
  GASNET_PUSHVAR(CFLAGS,"")
    GASNET_TRY_CFLAG([$[$2]], [], [
	GASNET_MSG_WARN([Unable to use default $2="$[$2]" so using "$4" instead. Consider manually seting $2])
        $2="$4"
    ])
  GASNET_POPVAR(CC)
  GASNET_POPVAR(CFLAGS)
fi
GASNET_FUN_END([$0(...)])
])

dnl GASNET_CHECK_OPTIMIZEDDEBUG CCVAR CFLAGSVAR EXTRAARGS INCLUDES 
dnl Ensure the compiler CC doesn't create a conflict between
dnl optimization and debugging.
AC_DEFUN([GASNET_CHECK_OPTIMIZEDDEBUG],[
GASNET_FUN_BEGIN([$0(...)])
 if test "$enable_debug" = "yes" ; then
  AC_MSG_CHECKING([$1 for debug vs. optimize compilation conflict])
  AC_LANG_SAVE
  AC_LANG_C
  GASNET_PUSHVAR(CC,"$[$1]")
  GASNET_PUSHVAR(CFLAGS,"$[$2] $3")
  AC_TRY_COMPILE( [
    $4
    #if defined(__OPTIMIZE__) || defined(NDEBUG)
	choke me
    #endif
  ], [ ], [ AC_MSG_RESULT(no) ], [
    AC_MSG_RESULT([yes])
    GASNET_MSG_ERROR([User requested --enable-debug but $1 or $2 has enabled optimization (-O) or disabled assertions (-DNDEBUG). Try setting $1='$[$1] -O0 -UNDEBUG' or changing $2])
  ])
  GASNET_POPVAR(CC)
  GASNET_POPVAR(CFLAGS)
  AC_LANG_RESTORE
 fi
GASNET_FUN_END([$0(...)])
])

dnl GASNET_CHECK_RESTRICT(PREFIX, opt compiler-desc)
dnl Checks if 'restrict' C99 keyword (or variants) supported
dnl #defines [PREFIX]_RESTRICT to correct variant, or to nothing
dnl #defines [PREFIX]_RESTRICT_MAY_QUALIFY_TYPEDEFS if appropriate
AC_DEFUN([GASNET_CHECK_RESTRICT],[
GASNET_FUN_BEGIN([$0])
  dnl Check for restrict keyword
  pushdef([cvprefix],translit([$1],'A-Z','a-z'))
  restrict_keyword=""
  # Due to xlc/mpcc_r oddity on AIX, we check "__restrict__" before "restrict".
  # Both ID the same, but xlc accepts either while mpcc_r only takes "__restrict__".
  if test "$restrict_keyword" = ""; then
    GASNET_TRY_CACHE_CHECK($2 for __restrict__ keyword, cvprefix[]__restrict__,
      [int dummy(void * __restrict__ p) { return 1; }], [],
      restrict_keyword="__restrict__")
  fi
  if test "$restrict_keyword" = ""; then
    GASNET_TRY_CACHE_CHECK($2 for restrict keyword, cvprefix[]restrict,
      [int dummy(void * restrict p) { return 1; }], [],
      restrict_keyword="restrict")
  fi
  if test "$restrict_keyword" = ""; then
    GASNET_TRY_CACHE_CHECK($2 for __restrict keyword, cvprefix[]__restrict,
      [int dummy(void * __restrict p) { return 1; }], [],
      restrict_keyword="__restrict")
  fi
  AC_DEFINE_UNQUOTED([$1]_RESTRICT, $restrict_keyword)
  GASNET_TRY_CACHE_CHECK($2 for restrict qualifying typedefs, cvprefix[]_restrict_typedefs,
    [typedef void *foo_t;
     int dummy(foo_t [$1]_RESTRICT p) { return 1; }], [],
    AC_DEFINE([$1]_RESTRICT_MAY_QUALIFY_TYPEDEFS))
  popdef([cvprefix])
GASNET_FUN_END([$0])
])

dnl INTERNL USE ONLY
AC_DEFUN([GASNETI_C_OR_CXX],[ifelse(index([$1],[CXX]),[-1],[C],[CXX])])

dnl check whether a given gcc/g++ attribute is available
dnl GASNET_CHECK_GNU_ATTRIBUTE(PREFIX, compiler-name, attribute-name, declaration, code)
dnl If PREFIX contains "CXX" then test is run as LANG_CPLUSPLUS
dnl Caller is responsible for setting of CC and friends in the MPI_CC case
AC_DEFUN([GASNET_CHECK_GNU_ATTRIBUTE],[
  GASNET_FUN_BEGIN([$0($1,$2,$3)])
  pushdef([uppername],translit(patsubst([$3], [_], []),'a-z','A-Z'))
  pushdef([cachevar],cv_prefix[]translit([$1]_attr_[]uppername,'A-Z','a-z'))
  AC_CACHE_CHECK($2 for __attribute__(($3)), cachevar,
    GASNET_TRY_COMPILE_WITHWARN(GASNETI_C_OR_CXX([$1]), [$4], [$5], [
          cachevar='yes'
      ],[ dnl cachevar="no/warning: $gasnet_cmd_stdout$gasnet_cmd_stderr"
          cachevar='no/warning'
      ],[ dnl cachevar="no/error: $gasnet_cmd_stdout$gasnet_cmd_stderr"
          cachevar='no/error'
    ])
  )
  if test "$cachevar" = yes; then
      AC_DEFINE($1_ATTRIBUTE_[]uppername)
      AC_DEFINE($1_ATTRIBUTE)
  else
      AC_DEFINE($1_ATTRIBUTE_[]uppername, 0)
  fi
  GASNET_FUN_END([$0($1,$2,$3)])
  popdef([cachevar])
  popdef([uppername])
]) 

dnl GASNET_GET_GNU_ATTRIBUTES(PREFIX, opt compiler-name)
dnl Check all gcc attributes of interest/importance to GASNet
dnl If PREFIX contains "CXX"  then test is run as LANG_CPLUSPLUS
dnl Caller must setup CC, CFLAGS, etc for MPI_CC case.
AC_DEFUN([GASNET_GET_GNU_ATTRIBUTES],[
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__always_inline__],
            [__attribute__((__always_inline__)) int dummy(void) { return 1; }])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__noinline__],
            [__attribute__((__noinline__)) int dummy(void) { return 1; }])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__malloc__],
            [#include <stdlib.h>
	     __attribute__((__malloc__)) void * dummy(void) { return malloc(14); }])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__warn_unused_result__],
            [__attribute__((__warn_unused_result__)) void * dummy(void) { return 0; }])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__used__],
            [#include <stdlib.h>
	     __attribute__((__used__)) void dummy(void) { abort(); }])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__unused__],
            [void dummy(void) { __attribute__((__unused__)) int pointless; return; }])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__may_alias__],
            [typedef int __attribute__((__may_alias__)) dummy;])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__noreturn__],
            [#include <stdlib.h>
	     __attribute__((__noreturn__)) void dummy(void) { abort(); }])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__const__],
            [__attribute__((__const__)) int dummy(int x) { return x+1; }])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__pure__],
            [__attribute__((__pure__)) int dummy(int x) { return x+1; }])
  GASNET_CHECK_GNU_ATTRIBUTE([$1], [$2], [__format__],
            [__attribute__((__format__ (__printf__, 1, 2))) void dummy(const char *fmt,...) { }])

  pushdef([cachevar],cv_prefix[]translit([$1],'A-Z','a-z')[]_attr_format_funcptr)
  AC_CACHE_CHECK($2 for __attribute__((__format__)) on function pointers, cachevar,
    GASNET_TRY_COMPILE_WITHWARN(GASNETI_C_OR_CXX([$1]), [
          __attribute__((__format__ (__printf__, 1, 2))) extern void (*dummy)(const char *fmt,...);
      ], [], [ cachevar='yes' ],[ cachevar='no/warning' ],[ cachevar='no/error' ])
  )
  if test "$cachevar" = yes; then
      AC_DEFINE([$1]_ATTRIBUTE_FORMAT_FUNCPTR)
  else
      AC_DEFINE([$1]_ATTRIBUTE_FORMAT_FUNCPTR, 0)
  fi
  popdef([cachevar])
])

dnl  Check to see if __thread attribute exists and works
dnl  Caller must setup CFLAGS/LIBS to support pthreaded compilation
dnl  GASNET_CHECK_TLS_SUPPORT(action-if-yes, action-if-no)
AC_DEFUN([GASNET_CHECK_TLS_SUPPORT],[
  GASNET_FUN_BEGIN([$0])
  GASNET_TRY_CACHE_RUN_WITHCC([whether the GCC __thread extension is supported.], tls_supported, [
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>

#define NTHREADS 5
pthread_t p[[NTHREADS]];
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
extern __thread long tlocal;
void foo() { tlocal = 4; } /* extern + use + definition crashes gcc-3.3/ia64 */
__thread long tlocal = 0;

/* trigger a bug on gcc-3.4/solaris/SPARC - misalignment during compilation with -g leads to 
   a link error: relocation error: R_SPARC_TLS_DTPOFF32 .. symbol misc2: offset 0xff351dd1 is non-aligned
*/
__thread int  misc  = 0;
__thread long misc2 = 0;

void * thread_start (void *arg) {
  int id = *((int *)arg);
  int *return_val = malloc(sizeof(int));
  if (pthread_mutex_lock (&lock)) { exit (2); }
  /* if the value is truly thread-local, this assignment
     will yield the value 1, for each thread. If tlocal
     is instead a process global static value then tlocal
     will be incremented by each thread, and its final
     value will be the number of threads. */
  tlocal += 1;
  if (pthread_mutex_unlock (&lock)) { exit (2); }
  *return_val = tlocal;
  /* detect an Intel 9.0/9.1 bug on x86/Linux (n2001) */
  if (misc != 0 || misc2 != 0) *return_val = -1;
  return return_val;
}
],[
  int i;
  for (i = 0; i < NTHREADS; ++i) {
      int *id = (int *)malloc(sizeof(int));
      *id = i;
      if (pthread_create(&p[[i]], NULL, thread_start, id)) { exit (2); }
  }
  for (i = 0; i < NTHREADS; ++i) {
      int *rc;
      if (pthread_join (p[[i]], (void **)&rc)) { exit (2); }
      if (*rc != 1) { exit (1); }
  }
  return 0;
],[$1],[$2])
  GASNET_FUN_END([$0])
])

dnl Output compilation error information, if available and do a AC_MSG_ERROR
dnl should be used within the failed branch of the compile macro, otherwise
dnl use GASNET_ERR_SAVE() in the failed branch to save the error info
AC_DEFUN([GASNET_MSG_ERROR],[
pushdef([usermsg],[patsubst([$1],["],[\\"])])
echo
echo "configure error: usermsg"
if test "" ; then
if test -f "conftest.$ac_ext" ; then
  errfile=conftest.$ac_ext
else
  errfile=gasnet_errsave_file
fi
if test -f "$errfile" ; then
  echo
  echo " --- Failed program --- "
  cat $errfile
  echo " -----------------------"
fi
fi
if test -f "conftest.err" ; then
  errfile=conftest.err
else
  errfile=gasnet_errsave_err
fi
if test -f "$errfile" ; then
  echo
  echo "Compilation error: "
  echo
  cat $errfile
fi
echo
CONFIG_FILE=`pwd`/config.log
AC_MSG_ERROR(See $CONFIG_FILE for details.)
popdef([usermsg])
])

AC_DEFUN([GASNET_ERR_SAVE],[
if test -f "conftest.$ac_ext" ; then
  cp conftest.$ac_ext gasnet_errsave_file
fi
if test -f "conftest.err" ; then
  cp conftest.err gasnet_errsave_err
fi
])

AC_DEFUN([GASNET_ERR_CLEANUP],[
  rm -f gasnet_errsave_file gasnet_errsave_err
])

dnl Output a warning, and also save it to the warning list for summary display
AC_DEFUN([GASNET_MSG_WARN],[
  pushdef([usermsg],[patsubst([$1],["],[\\"])])
  GASNET_FUN_BEGIN([$0()])
  AC_MSG_WARN([$1])
  echo "usermsg" >> "GASNET_CONFIGURE_WARNING_LOCAL"
  echo " " >> "GASNET_CONFIGURE_WARNING_LOCAL"
  GASNET_FUN_END([$0()])
  popdef([usermsg])
])

dnl Display the warning summary
AC_DEFUN([GASNET_MSG_WARN_FINISH],[
  if test -f "GASNET_CONFIGURE_WARNING_LOCAL" ; then
    echo "--------------------------------------------------------------------" >&2
    echo "--------------------------------------------------------------------" >&5
    echo "configure warning summary:" >&2
    echo "configure warning summary:" >&5
    echo " " >&2
    echo " " >&5
    cat "GASNET_CONFIGURE_WARNING_LOCAL" >&2
    cat "GASNET_CONFIGURE_WARNING_LOCAL" >&5
    mv "GASNET_CONFIGURE_WARNING_LOCAL" "GASNET_CONFIGURE_WARNING_LOCAL"-
    if test -n "$GASNET_CONFIGURE_WARNING_GLOBAL" ; then
      cat "GASNET_CONFIGURE_WARNING_LOCAL"- >> "$GASNET_CONFIGURE_WARNING_GLOBAL"
    fi
    rm -f "GASNET_CONFIGURE_WARNING_LOCAL" "GASNET_CONFIGURE_WARNING_LOCAL"-
  fi
])

dnl compile a program for a success/failure
dnl GASNET_TRY_CACHE_CHECK(description,cache_name,includes,program,action-on-success,action-on-failure)
AC_DEFUN([GASNET_TRY_CACHE_CHECK],[
GASNET_FUN_BEGIN([$0($1,$2,...)])
AC_CACHE_CHECK($1, cv_prefix[]$2,
AC_TRY_COMPILE([$3], [$4], cv_prefix[]$2=yes, cv_prefix[]$2=no))
if test "$cv_prefix[]$2" = yes; then
  :
  $5
else
  :
  $6
fi
GASNET_FUN_END([$0($1,$2,...)])
])


dnl link a program for a success/failure
dnl GASNET_TRY_CACHE_LINK(description,cache_name,includes,program,action-on-success,action-on-failure)
AC_DEFUN([GASNET_TRY_CACHE_LINK],[
GASNET_FUN_BEGIN([$0($1,$2,...)])
AC_CACHE_CHECK($1, cv_prefix[]$2,
AC_TRY_LINK([$3], [$4], cv_prefix[]$2=yes, cv_prefix[]$2=no))
if test "$cv_prefix[]$2" = yes; then
  :
  $5
else
  :
  $6
fi
GASNET_FUN_END([$0($1,$2,...)])
])

dnl run a program for a success/failure
dnl GASNET_TRY_CACHE_RUN(description,cache_name,program,action-on-success,action-on-failure)
AC_DEFUN([GASNET_TRY_CACHE_RUN],[
GASNET_FUN_BEGIN([$0($1,$2,...)])
AC_CACHE_CHECK($1, cv_prefix[]$2,
AC_TRY_RUN([$3], cv_prefix[]$2=yes, cv_prefix[]$2=no, AC_MSG_ERROR(no default value for cross compiling)))
if test "$cv_prefix[]$2" = yes; then
  :
  $4
else
  :
  $5
fi
GASNET_FUN_END([$0($1,$2,...)])
])

dnl compile and run a program and execute success/failure paths 
dnl cross-compilation will skip the run and assume success
dnl GASNET_TRY_CACHE_RUN_WITHCC(description,cache_name,includes,program,action-on-success,action-on-failure)
AC_DEFUN([GASNET_TRY_CACHE_RUN_WITHCC],[
  GASNET_FUN_BEGIN([$0($1,$2,...)])
  if test "$cross_compiling" = "yes" ; then
    GASNET_TRY_CACHE_LINK([$1],[$2],[$3],[$4],[$5],[$6])
  else
    GASNET_TRY_CACHE_RUN([$1],[$2],[
      $3
      int main(void) {
        $4
	return 0;
      }
    ],[$5],[$6])
  fi
  GASNET_FUN_END([$0($1,$2,...)])
])

dnl compile and run a program, error out if one fails (cross-compilation will skip the run)
dnl GASNET_TRY_CACHE_VERIFY_RUN(description,cache_name,includes,program,errormsg-on-failure)
AC_DEFUN([GASNET_TRY_CACHE_VERIFY_RUN],[
  GASNET_TRY_CACHE_RUN_WITHCC([$1],[$2],[$3],[$4],[ ],[ GASNET_MSG_ERROR([$5]) ])
])

dnl run a program to extract the value of a runtime expression 
dnl the provided code should set the integer val to the relevant value
dnl GASNET_TRY_CACHE_RUN_EXPR(description,cache_name,headers,code_to_set_val,result_variable)
AC_DEFUN([GASNET_TRY_CACHE_RUN_EXPR],[
GASNET_FUN_BEGIN([$0($1,$2,...)])
AC_CACHE_CHECK($1, cv_prefix[]$2,
AC_TRY_RUN([
  #include "confdefs.h"
  #include <stdio.h>
  #include <stdlib.h>
  $3
  int main(void) {
    FILE *f=fopen("conftestval", "w");
    int val = 0;
    if (!f) exit(1);
    { $4; }
    fprintf(f, "%d\n", (int)(val));
    return 0;
  }], cv_prefix[]$2=`cat conftestval`, cv_prefix[]$2=no, AC_MSG_ERROR(no default value for cross compiling)))
if test "$cv_prefix[]$2" != no; then
  :
  $5=$cv_prefix[]$2
fi
GASNET_FUN_END([$0($1,$2,...)])
])

AC_DEFUN([GASNET_PROG_CPP], [
  GASNET_FUN_BEGIN([$0])
  AC_PROVIDE([$0])
  AC_PROG_CC
  AC_PROG_CPP
  GASNET_GETFULLPATH(CPP)
  AC_SUBST(CPP)
  AC_SUBST(CPPFLAGS)
  AC_MSG_CHECKING(for working C preprocessor)
  AC_LANG_SAVE
  AC_LANG_C
  gasnet_progcpp_extrainfo=
  dnl deal with preprocessors who foolishly return success exit code even when they saw #error
  if test -n "`$CPP -version 2>&1 < /dev/null | grep MIPSpro`" ; then
    dnl The MIPSPro compiler has a broken preprocessor exit code by default, fix it
    dnl Using this flag is preferable to ensure that #errors encountered during compilation are fatal
    gasnet_progcpp_extrainfo=" (added -diag_error 1035 to deal with broken MIPSPro preprocessor)"
    CFLAGS="$CFLAGS -diag_error 1035"
    CPPFLAGS="$CPPFLAGS -diag_error 1035"    
  fi
  dnl final check
  AC_TRY_CPP([
    # error
  ], [GASNET_MSG_ERROR(Your C preprocessor is broken - reported success when it should have failed)], [])
  AC_TRY_CPP([], [], [GASNET_MSG_ERROR(Your C preprocessor is broken - reported failure when it should have succeeded)])
  AC_TRY_CPP([
    #ifdef __cplusplus
      #error __cplusplus should not be defined in a C preprocessor!
    #endif
  ], [], [GASNET_MSG_ERROR([Your C preprocessor is broken, it erroneously defines __cplusplus. This software requires a true, working ANSI C compiler - a C++ compiler is not an acceptable replacement.])])
  AC_MSG_RESULT(yes$gasnet_progcpp_extrainfo)
  if test "$CPP" = "/lib/cpp" ; then
    badlibcppmsg="Autoconf detected your preprocessor to be '/lib/cpp' instead of '$CC -E'. This is almost always a mistake, resulting from either a broken C compiler or an outdated version of autoconf. Proceeding is very likely to result in incorrect configure decisions."
    GASNET_IF_ENABLED(allow-libcpp, Allow the use of /lib/cpp for preprocessing, [
      GASNET_MSG_WARN([$badlibcppmsg])
    ],[
      AC_MSG_ERROR([$badlibcppmsg \
        You may enable use of this preprocessor at your own risk by passing the --enable-allow-libcpp flag.])
    ])
  fi
  AC_LANG_RESTORE
  GASNET_FUN_END([$0])
])

AC_DEFUN([GASNET_PROG_CXXCPP], [
  GASNET_FUN_BEGIN([$0])
  AC_PROVIDE([$0])
  AC_PROG_CXX
  AC_PROG_CXXCPP
  GASNET_GETFULLPATH(CXXCPP)
  AC_SUBST(CXXCPP)
  AC_SUBST(CXXCPPFLAGS)
  AC_MSG_CHECKING(for working C++ preprocessor)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  gasnet_progcxxcpp_extrainfo=
  dnl deal with preprocessors who foolishly return success exit code even when they saw #error
  if test -n "`$CXXCPP -version 2>&1 < /dev/null | grep MIPSpro`" ; then
    dnl The MIPSPro compiler has a broken preprocessor exit code by default, fix it
    dnl Using this flag is preferable to ensure that #errors encountered during compilation are fatal
    gasnet_progcxxcpp_extrainfo=" (added -diag_error 1035 to deal with broken MIPSPro preprocessor)"
    CXXFLAGS="$CXXFLAGS -diag_error 1035"
    CXXCPPFLAGS="$CXXCPPFLAGS -diag_error 1035"    
  fi
  dnl final check
  AC_TRY_CPP([
    # error
  ], [GASNET_MSG_ERROR(Your C++ preprocessor is broken - reported success when it should have failed)], [])
  AC_TRY_CPP([], [], [GASNET_MSG_ERROR(Your C++ preprocessor is broken - reported failure when it should have succeeded)])
  AC_TRY_CPP([
    #ifndef __cplusplus
      #error __cplusplus must be defined in a C++ preprocessor!
    #endif
  ], [], [GASNET_MSG_ERROR([Your C++ preprocessor is broken, it fails to define __cplusplus.])])
  AC_MSG_RESULT(yes$gasnet_progcxxcpp_extrainfo)
  AC_LANG_RESTORE
  GASNET_FUN_END([$0])
])

AC_DEFUN([GASNET_PROG_CC], [
  GASNET_FUN_BEGIN([$0])
  GASNET_PROG_CPP
  GASNET_GETFULLPATH(CC)
  AC_SUBST(CC)
  AC_SUBST(CFLAGS)
  AC_MSG_CHECKING(for working C compiler)
  AC_LANG_SAVE
  AC_LANG_C
  AC_TRY_COMPILE([], [
    fail for me
  ], [GASNET_MSG_ERROR(Your C compiler is broken - reported success when it should have failed)], [])
  AC_TRY_COMPILE([ #include <stdio.h>
                   #include <stdlib.h>
		 ], [ printf("hi\n"); exit(0); ], 
     [], [GASNET_MSG_ERROR(Your C compiler is broken - reported failure when it should have succeeded)])
  AC_TRY_COMPILE([
    double *p;
    void *foo(double *d) { 
      return d; 
    }
  ], [
    double d;
    /* (void *) is compatible with any pointer type in a C program */
    p = foo((void *)&d);
  ], [], [GASNET_MSG_ERROR([Your C compiler is broken, it fails to compile a simple C program using implicit void* conversion. This software requires a true, working ANSI C compiler - note that a C++ compiler is not an acceptable replacement.])])
  AC_TRY_LINK([ extern int some_bogus_nonexistent_symbol(); ], [ int x = some_bogus_nonexistent_symbol(); ],
              [GASNET_MSG_ERROR(Your C linker is broken - reported success when it should have failed)], [])
  AC_TRY_LINK([ #include <stdio.h>
                #include <stdlib.h>
              ], [ printf("hi\n"); exit(0); ], 
     [], [GASNET_MSG_ERROR(Your C link is broken - reported failure when it should have succeeded)])
  AC_MSG_RESULT(yes)
  AC_MSG_CHECKING(if user enabled cross-compile)
  GASNET_IF_ENABLED(cross-compile, [ Enable cross-compilation (experimental) ], [
    AC_MSG_RESULT(yes)
    cross_compiling=yes 
    CROSS_COMPILING=1
    ac_cv_prog_cc_cross=yes 
  ], [
    dnl reset autoconf cross compilation setting, which is wrong if executables are broken
    AC_MSG_RESULT(no)
    cross_compiling=no
    CROSS_COMPILING=0
    ac_cv_prog_cc_cross=no
    AC_MSG_CHECKING([working C compiler executables])
    AC_TRY_RUN([int main(void) { return 0; }], [AC_MSG_RESULT(yes)],
  	     [AC_MSG_RESULT(no) GASNET_MSG_ERROR([Cannot run executables created with C compiler. If you're attempting to cross-compile, use --enable-cross-compile])], 
  	     [GASNET_MSG_ERROR(Internal configure error - please report)])
  ])
  AM_CONDITIONAL(CROSS_COMPILING, test "$cross_compiling" = "yes")
  AC_SUBST(CROSS_COMPILING)
  AC_LANG_RESTORE
  GASNET_FUN_END([$0])
])

AC_DEFUN([GASNET_PROG_CXX], [
  GASNET_FUN_BEGIN([$0])
  GASNET_PROG_CXXCPP
  GASNET_GETFULLPATH(CXX)
  AC_SUBST(CXX)
  AC_SUBST(CXXFLAGS)
  AC_MSG_CHECKING(for working C++ compiler)
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  AC_TRY_COMPILE([], [
    fail for me
  ], [GASNET_MSG_ERROR(Your C++ compiler is broken - reported success when it should have failed)], [])
  AC_TRY_COMPILE([ #include <stdio.h>
                   #include <stdlib.h>
                 ], [ printf("hi\n"); exit(0); ], 
     [], [GASNET_MSG_ERROR(Your C++ compiler is broken - reported failure when it should have succeeded)])
  AC_TRY_LINK([ extern int some_bogus_nonexistent_symbol(); ], [ int x = some_bogus_nonexistent_symbol(); ],
              [GASNET_MSG_ERROR(Your C++ linker is broken - reported success when it should have failed)], [])
  AC_TRY_LINK([ #include <stdio.h>
                   #include <stdlib.h>
              ], [ printf("hi\n"); exit(0); ], 
     [], [GASNET_MSG_ERROR(Your C++ link is broken - reported failure when it should have succeeded)])
  AC_MSG_RESULT(yes)
  dnl reset autoconf cross compilation setting, which is wrong if executables are broken
  AC_MSG_CHECKING(if user enabled cross-compile)
  GASNET_IF_ENABLED(cross-compile, [ Enable cross-compilation (experimental) ], [
    AC_MSG_RESULT(yes)
    cross_compiling=yes 
    ac_cv_prog_cxx_cross=yes 
  ], [
    dnl reset autoconf cross compilation setting, which is wrong if executables are broken
    AC_MSG_RESULT(no)
    cross_compiling=no
    ac_cv_prog_cxx_cross=no
    AC_MSG_CHECKING([working C++ compiler executables])
    AC_TRY_RUN([int main(void) { return 0; }], [AC_MSG_RESULT(yes)],
  	     [AC_MSG_RESULT(no) GASNET_MSG_ERROR([Cannot run executables created with C++ compiler. If you're attempting to cross-compile, use --enable-cross-compile])], 
  	     [GASNET_MSG_ERROR(Internal configure error - please report)])
  ])
  AC_LANG_RESTORE
  GASNET_FUN_END([$0])
])

AC_DEFUN([GASNET_HOSTCC_BEGIN], [
  GASNET_FUN_BEGIN([$0])
  if test "$CROSS_COMPILING" != "1" ; then
    AC_MSG_ERROR([Internal error - please report])
  fi

  GASNET_PUSHVAR(CC,"$HOST_CC")
  GASNET_PUSHVAR(CFLAGS,"$HOST_CFLAGS")
  GASNET_PUSHVAR(LDFLAGS,"$HOST_LDFLAGS")
  GASNET_PUSHVAR(LIBS,"$HOST_LIBS")
  dnl push all the other goop that AC_PROG_C(PP) caches away
  GASNET_PUSHVAR_UNSET(CPP)
  GASNET_PUSHVAR_UNSET(CPPFLAGS)
  GASNET_PUSHVAR_UNSET(ac_cv_prog_CC)
  GASNET_PUSHVAR_UNSET(ac_cv_prog_CPP)
  GASNET_PUSHVAR_UNSET(ac_cv_c_compiler_gnu)
  GASNET_PUSHVAR_UNSET(ac_cv_prog_cc_g)
  GASNET_PUSHVAR_UNSET(ac_cv_prog_cc_stdc)
  GASNET_PUSHVAR(cross_compiling,"no")

  GASNET_FUN_END([$0])
])

AC_DEFUN([GASNET_HOSTCC_END], [
  GASNET_FUN_BEGIN([$0])
  if test "$CROSS_COMPILING" != "1" ; then
    AC_MSG_ERROR([Internal error - please report])
  fi

  GASNET_POPVAR(CC)
  GASNET_POPVAR(CFLAGS)
  GASNET_POPVAR(LDFLAGS)
  GASNET_POPVAR(LIBS)
  GASNET_POPVAR(CPP)
  GASNET_POPVAR(CPPFLAGS)
  GASNET_POPVAR(ac_cv_prog_CC)
  GASNET_POPVAR(ac_cv_prog_CPP)
  GASNET_POPVAR(ac_cv_c_compiler_gnu)
  GASNET_POPVAR(ac_cv_prog_cc_g)
  GASNET_POPVAR(ac_cv_prog_cc_stdc)
  GASNET_POPVAR(cross_compiling)

  GASNET_FUN_END([$0])
])

dnl fetch the host C compiler
AC_DEFUN([GASNET_PROG_HOSTCC], [
GASNET_FUN_BEGIN([$0])
if test "$cross_compiling" = "yes" ; then
  HOST_MSG="When cross-compiling, \$HOST_CC or --with-host-cc= must be set to indicate a C compiler for the host machine (ie the machine running this configure script)"
  pushdef([GASNET_ENV_DEFAULT_SUPPRESSHELP],1)
  GASNET_ENV_DEFAULT(HOST_CC, )
  GASNET_ENV_DEFAULT(HOST_CFLAGS, )
  GASNET_ENV_DEFAULT(HOST_LDFLAGS, )
  GASNET_ENV_DEFAULT(HOST_LIBS, )
  popdef([GASNET_ENV_DEFAULT_SUPPRESSHELP])
  AC_SUBST(HOST_CC)
  AC_SUBST(HOST_CFLAGS)
  AC_SUBST(HOST_LDFLAGS)
  AC_SUBST(HOST_LIBS)
  if test ! "$HOST_CC" ; then
    AC_MSG_ERROR([$HOST_MSG])
  fi
  GASNET_HOSTCC_BEGIN
    GASNET_PROG_CC
    AC_LANG_SAVE
    AC_LANG_C
    GASNET_PUSHVAR(cross_compiling,"no")
    AC_MSG_CHECKING([working host C compiler executables])
    AC_TRY_RUN([int main(void) { return 0; }], [AC_MSG_RESULT(yes)],
             [AC_MSG_RESULT(no) GASNET_MSG_ERROR($HOST_MSG)],
             [GASNET_MSG_ERROR(Internal configure error - please report)])
    GASNET_POPVAR(cross_compiling)
    HOST_CC="$CC"
    HOST_CPP="$CPP"
    HOST_CPPFLAGS="$CPPFLAGS"
    HOST_CFLAGS="$CFLAGS"
    HOST_LDFLAGS="$LDFLAGS"
    HOST_LIBS="$LIBS"
    AC_LANG_RESTORE
  GASNET_HOSTCC_END
fi
GASNET_FUN_END([$0])
])

dnl fetch the host C++ compiler
dnl this is a two part macro which must be called one after the other at the top level
dnl in order to avoid some annoying bugs in autoconf AC_REQUIRE 
AC_DEFUN([GASNET_PROG_HOSTCXX], [
GASNET_FUN_BEGIN([$0])
if test "$cross_compiling" = "yes" ; then
  HOST_MSG="When cross-compiling, \$HOST_CXX or --with-host-cxx= must be set to indicate a C++ compiler for the host machine (ie the machine running this configure script)"
  pushdef([GASNET_ENV_DEFAULT_SUPPRESSHELP],1)
  GASNET_ENV_DEFAULT(HOST_CXX, )
  GASNET_ENV_DEFAULT(HOST_CXXFLAGS, )
  GASNET_ENV_DEFAULT(HOST_CXX_LDFLAGS, )
  GASNET_ENV_DEFAULT(HOST_CXX_LIBS, )
  popdef([GASNET_ENV_DEFAULT_SUPPRESSHELP])
  AC_SUBST(HOST_CXX)
  AC_SUBST(HOST_CXXFLAGS)
  AC_SUBST(HOST_CXX_LDFLAGS)
  AC_SUBST(HOST_CXX_LIBS)
  if test ! "$HOST_CXX" ; then
    AC_MSG_ERROR([$HOST_MSG])
  fi
  GASNET_PUSHVAR(CXX,"$HOST_CXX")
  GASNET_PUSHVAR(CXXFLAGS,"$HOST_CXXFLAGS")
  GASNET_PUSHVAR(LDFLAGS,"$HOST_CXX_LDFLAGS")
  GASNET_PUSHVAR(LIBS,"$HOST_CXX_LIBS")
  dnl push all the other goop that AC_PROG_CXX(CPP) caches away
  GASNET_PUSHVAR_UNSET(CXXCPP)
  GASNET_PUSHVAR_UNSET(ac_cv_prog_CXX)
  GASNET_PUSHVAR_UNSET(ac_cv_prog_CXXCPP)
  GASNET_PUSHVAR_UNSET(ac_cv_cxx_compiler_gnu)
  GASNET_PUSHVAR_UNSET(ac_cv_prog_cxx_g)
    GASNET_PROG_CXX
    AC_LANG_SAVE
    AC_LANG_CPLUSPLUS
    GASNET_PUSHVAR(cross_compiling,"no")
    AC_MSG_CHECKING([working host CXX compiler executables])
    AC_TRY_RUN([int main(void) { return 0; }], [AC_MSG_RESULT(yes)],
             [AC_MSG_RESULT(no) GASNET_MSG_ERROR($HOST_MSG)],
             [GASNET_MSG_ERROR(Internal configure error - please report)])
    GASNET_POPVAR(cross_compiling)
    HOST_CXX="$CXX"
    HOST_CXXCPP="$CXXCPP"
    HOST_CXXFLAGS="$CXXFLAGS"
    HOST_CXX_LDFLAGS="$LDFLAGS"
    HOST_CXX_LIBS="$LIBS"
    AC_LANG_RESTORE
  GASNET_POPVAR(CXX)
  GASNET_POPVAR(CXXFLAGS)
  GASNET_POPVAR(CXXCPP)
  GASNET_POPVAR(LDFLAGS)
  GASNET_POPVAR(LIBS)
  GASNET_POPVAR(ac_cv_prog_CXX)
  GASNET_POPVAR(ac_cv_prog_CXXCPP)
  GASNET_POPVAR(ac_cv_cxx_compiler_gnu)
  GASNET_POPVAR(ac_cv_prog_cxx_g)
fi
GASNET_FUN_END([$0])
])

dnl find working version of perl.  Checks to see if 'bytes' module is available,
dnl and sets GASNET_PERL_BYTESFLAG to either '-Mbytes' or empty string, for
dnl scripts that need to ward off Perl/UTF-8 issues 
dnl similiarly determines if LANG=en_US is available, setting GASNET_PERL_LANG
AC_DEFUN([GASNET_PROG_PERL],[
  GASNET_FUN_BEGIN([$0])
  GASNET_PATH_PROGS(PERL, perl5 perl, perl)
  MIN_PERL_VERSION="5.005"
  AC_MSG_CHECKING(for perl version $MIN_PERL_VERSION or later)
  if $PERL -e "require $MIN_PERL_VERSION;" 2>/dev/null; then
    AC_MSG_RESULT(yes)
  else
    AC_MSG_ERROR(cannot find perl $MIN_PERL_VERSION or later)
  fi
  if $PERL -Mbytes -e "exit 0" 2>/dev/null; then
    GASNET_PERL_BYTESFLAG="-Mbytes"
  else
    GASNET_PERL_BYTESFLAG=
  fi
  AC_SUBST(GASNET_PERL_BYTESFLAG)
  if test -n "`env LC_ALL=en_US LANG=en_US $PERL -e 'exit 0' 2>&1 | grep locale`"; then
    GASNET_PERL_LANG=C
  else
    GASNET_PERL_LANG=en_US
  fi
  AC_SUBST(GASNET_PERL_LANG)
  GASNET_FUN_END([$0])
])

dnl GASNET_IFDEF(preprocsymbol[, action-if-defined, action-if-notdefined, forcecompile]) 
dnl test whether preprocsymbol is defined and execute appropriate action
dnl forcecompile forces a full compile, otherwise just use preprocessor
AC_DEFUN([GASNET_IFDEF],[
GASNET_FUN_BEGIN([$0($1,...,$4)])
if test -z "$4" ; then
AC_TRY_CPP([
#ifndef $1
# error
#endif], [$2], [$3])
else
AC_TRY_COMPILE([
#ifndef $1
# error
#endif], [ ], [$2], [$3])
fi
GASNET_FUN_END([$0($1,...,$4)])
])

dnl GASNET_FAMILY_CACHE_CHECK(lang-display-name, (CC|MPI_CC|CXX), family_output_var)
AC_DEFUN([GASNET_FAMILY_CACHE_CHECK],[
GASNET_FUN_BEGIN([$0($1,$2,$3)])
AC_REQUIRE_CPP
AC_CACHE_CHECK(for $1 compiler family, $3, [
  if test "$2" != "CXX" ; then
    _GASNET_FAMILY_CACHE_CHECK_PREPROC="$CPP"
  else
    _GASNET_FAMILY_CACHE_CHECK_PREPROC="$CXXCPP"
  fi
  if test "`echo '$_GASNET_FAMILY_CACHE_CHECK_PREPROC' | grep '[$]$1'`" = "" ; then
    # preprocessor may differ from true compiler, so force full compilation testing
    _force_compile=1
  else
    _force_compile=
  fi
  $3=unknown

  dnl start with compilers having very slow preprocessors
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(__xlC__, $3=XLC, [], $_force_compile)
  fi
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(_CRAYC, $3=Cray, [], $_force_compile)
  fi
  dnl gcc-like compilers, which may define __GNUC__ - order matters here
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(__GNUC__, $3=GNU, [], $_force_compile) 
    dnl Note GNUC one above must precede many of those below
    GASNET_IFDEF(__PATHCC__, $3=Pathscale, [], $_force_compile)
    GASNET_IFDEF(__PGI, $3=PGI, [], $_force_compile)
    GASNET_IFDEF(__INTEL_COMPILER, $3=Intel, [], $_force_compile)
    GASNET_IFDEF(__OPENCC__, $3=Open64, [], $_force_compile)
  fi
  dnl other vendor compilers
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(__DECC, $3=Compaq, [], $_force_compile) # Compaq C
    GASNET_IFDEF(__DECCXX, $3=Compaq, [], $_force_compile) # Compaq C++
  fi
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(__SUNPRO_C, $3=Sun, [], $_force_compile)  # Sun C
    GASNET_IFDEF(__SUNPRO_CC, $3=Sun, [], $_force_compile) # Sun C++
  fi
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(__HP_cc, $3=HP, [], $_force_compile)  # HP C
    GASNET_IFDEF(__HP_aCC, $3=HP, [], $_force_compile) # HP aCC (C++)
  fi
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(_SGI_COMPILER_VERSION, $3=MIPS, [], $_force_compile)
  fi
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(__MTA__, $3=MTA, [], $_force_compile)
  fi
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(__KCC, $3=KAI, [], $_force_compile)
  fi
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(__TINYC__, $3=TINY, [], $_force_compile)
  fi
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(__LCC__, $3=LCC, [], $_force_compile)
  fi

  dnl compilers lacking specific identifying marks - identify by platform
  if test "$$3" = "unknown"; then
    GASNET_IFDEF(_SX, $3=NEC)
    GASNET_IFDEF(__sgi, $3=MIPS) # MIPSPro 7.3.0 and earlier
  fi
])
if test "$$3" != "GNU" ; then
  dnl Some compilers (eg Intel 8.0) define __GNUC__ even though they are definitely not GNU C
  dnl Don't believe their filthy lies
  case $2 in 
    CC) ac_cv_c_compiler_gnu=no
        GCC=""
    ;;
    CXX) ac_cv_cxx_compiler_gnu=no
        GXX=""
    ;;
  esac
fi
$2_FAMILY=$$3
$2_UNWRAPPED=$$2
case $$3 in
  GNU) $2_WRAPPED=$$2 ;;
  *)   $2_WRAPPED="\$(top_builddir)/cc-wrapper \$($2_FAMILY) \$($2_UNWRAPPED)" ;;
esac
AC_SUBST($2_FAMILY)
AC_SUBST($2_UNWRAPPED)
AC_SUBST($2_WRAPPED)
GASNET_SUBST_FILE(cc_wrapper_mk, cc-wrapper.mk)
GASNET_FUN_END([$0($1,$2,$3)])
])


dnl deal with a buggy version of autoconf which assumes alloca returns char *
AC_DEFUN([GASNET_FUNC_ALLOCA_HELPER],[
  patsubst([$*], [p = alloca], [p = (char *)alloca])
])

AC_DEFUN([GASNET_FUNC_ALLOCA],[
  AC_SUBST(ALLOCA)
  GASNET_FUNC_ALLOCA_HELPER(AC_FUNC_ALLOCA)
])

dnl Set command for use in Makefile.am to install various files
dnl This command should remove all the magic used to run from the build
dnl directory, as well as deal with setting of the prefix at install time.
AC_DEFUN([GASNET_SET_INSTALL_CMD],[
GASNET_INSTALL_CMD="sed -e '/###NOINSTALL###/d' -e 's@###INSTALL_PREFIX###@\$(prefix)@g'"
AC_SUBST(GASNET_INSTALL_CMD)
])

dnl pass $1 to all subconfigures invoked recursively from this configure script
AC_DEFUN([GASNET_SUBCONFIGURE_ARG],[
GASNET_FUN_BEGIN([$0($1)])
ac_configure_args="$ac_configure_args $1"
GASNET_FUN_END([$0($1)])
])

dnl GASNET_SUBCONFIGURE_INVOKE(subdirname, [shared_cache:0/1])
dnl recursively invoke a subconfigure in $1
AC_DEFUN([GASNET_SUBCONFIGURE_INVOKE],[
GASNET_FUN_BEGIN([$0($1)])
 if test "$no_recursion" != yes; then
  ac_config_dir="$1"
  if test ! -f $srcdir/$ac_config_dir/configure; then
     AC_MSG_ERROR(failed to invoke sub-configure in $ac_config_dir - file not found)
  fi

  # Remove --cache-file, --srcdir and --prefix arguments so they do not pile up.
  ac_sub_configure_args=
  ac_prev=
  for ac_arg in $ac_configure_args; do
    if test -n "$ac_prev"; then
      ac_prev=
      continue
    fi
    case "$ac_arg" in
    -cache-file | --cache-file | --cache-fil | --cache-fi \
    | --cache-f | --cache- | --cache | --cach | --cac | --ca | --c)
      ac_prev=cache_file ;;
    -cache-file=* | --cache-file=* | --cache-fil=* | --cache-fi=* \
    | --cache-f=* | --cache-=* | --cache=* | --cach=* | --cac=* | --ca=* | --c=*)
      ;;
    -srcdir | --srcdir | --srcdi | --srcd | --src | --sr)
      ac_prev=srcdir ;;
    -srcdir=* | --srcdir=* | --srcdi=* | --srcd=* | --src=* | --sr=*)
      ;;
    -prefix | --prefix | --prefi | --pref | --pre)
      ac_prev=prefix ;;
    -prefix=* | --prefix=* | --prefi=* | --pref=* | --pre=*)
      ;;
    *) ac_sub_configure_args="$ac_sub_configure_args $ac_arg" ;;
    esac
  done

  # Always prepend --prefix to ensure using the same prefix in sub-configs
  if test "$prefix" = "NONE" -a -n "$ac_default_prefix" ; then
    ac_sub_configure_args="--prefix=$ac_default_prefix $ac_sub_configure_args"
  else
    ac_sub_configure_args="--prefix=$prefix $ac_sub_configure_args"
  fi

  echo configuring in $ac_config_dir

  case "$srcdir" in
  .) ;;
  *)
    if test -d ./$ac_config_dir || mkdir -p ./$ac_config_dir; then :;
    else
     AC_MSG_ERROR(failed to create `pwd`/$ac_config_dir)
    fi
    ;;
  esac

  ac_popdir=`pwd`
  cd $ac_config_dir

  # A "../" for each directory in /$ac_config_dir.
  ac_dots=`echo $ac_config_dir|sed -e 's%^\./%%' -e 's%[[^/]]$%&/%' -e 's%[[^/]]*/%../%g'`

  case "$srcdir" in
  .) # No --srcdir option.  We are building in place.
    ac_sub_srcdir=$srcdir ;;
  /*) # Absolute path.
    ac_sub_srcdir=$srcdir/$ac_config_dir ;;
  *) # Relative path.
    ac_sub_srcdir=$ac_dots$srcdir/$ac_config_dir ;;
  esac

  ac_sub_configure="$ac_sub_srcdir/configure"
 
  ac_sub_cache_file=
  if test "$2" = "1"; then
    # Make the cache file name correct relative to the subdirectory.
    case "$cache_file" in
    /*) ac_sub_cache_file=--cache-file=$cache_file ;;
    *) # Relative path.
      ac_sub_cache_file="--cache-file=$ac_dots$cache_file" ;;
    esac
  fi

  echo "running ${CONFIG_SHELL-/bin/sh} $ac_sub_configure $ac_sub_configure_args" \
       " $ac_sub_cache_file --srcdir=$ac_sub_srcdir"
  # The eval makes quoting arguments work.
  if eval ${CONFIG_SHELL-/bin/sh} $ac_sub_configure $ac_sub_configure_args \
          $ac_sub_cache_file --srcdir=$ac_sub_srcdir
  then :
  else
     AC_MSG_ERROR(sub-configure failed for $ac_config_dir)
  fi

  cd "$ac_popdir"
 fi
GASNET_FUN_END([$0($1)])
])

dnl GASNET_SUBCONFIGURE_EXTRACT(subdirname, varname)
dnl extract an AC_SUBST variable result from a recursively invoked subconfigure
dnl and set it in the current scope
AC_DEFUN([GASNET_SUBCONFIGURE_EXTRACT],[
GASNET_FUN_BEGIN([$0($1)])
  AC_REQUIRE([GASNET_PROG_PERL])
  AC_MSG_CHECKING([for $2 in $1])
  _subconfig_extract_file="$1/config.status"
  if test ! -f $_subconfig_extract_file; then
     AC_MSG_ERROR([failed to open $_subconfig_extract_file - file not found])
  fi
  _subconfig_extract_result=`$PERL -n -e 'if (m/^S\[["('$2')"\]]="(.*)"/) { print "[$]1='"'"'[$]2'"'"'"; };' -e 'if (m/^s(.)\@('$2')\@\1([[^\1]]*)\1/) { my ($var,$val) = ([$]2,[$]3); $val =~ s/\|#_!!_#\|//g; print "$var='"'"'$val'"'"'"; };' $_subconfig_extract_file`
  if test -n "$_subconfig_extract_result" ; then
    eval $_subconfig_extract_result
    AC_MSG_RESULT($[$2])
  else
    AC_MSG_RESULT(failed)
    AC_MSG_ERROR([failed to extract $2 variable value from $_subconfig_extract_file])
  fi
GASNET_FUN_END([$0($1)])
])

dnl fetch a cross-compilation variable, if we are cross compiling
dnl GASNET_CROSS_VAR(variable-to-set, basicname)
AC_DEFUN([GASNET_CROSS_VAR],[
  GASNET_FUN_BEGIN([$0($1,$2)])
  pushdef([cross_varname],CROSS_$2)
  if test "$cross_compiling" = "yes" ; then
    pushdef([GASNET_ENV_DEFAULT_SUPPRESSHELP],1)
    GASNET_ENV_DEFAULT(cross_varname,)
    popdef([GASNET_ENV_DEFAULT_SUPPRESSHELP])
    if test "$cross_varname" = "" ; then
      AC_MSG_ERROR([This configure script requires \$cross_varname be set for cross-compilation])
    else 
      $1="$cross_varname"
    fi
  fi
  popdef([cross_varname])
  GASNET_FUN_END([$0($1,$2)])
])

dnl query the numerical value of a system signal and AC_SUBST it
AC_DEFUN([GASNET_GET_SIG], [
  GASNET_FUN_BEGIN([$0])
  if test "$cross_compiling" = "yes" ; then
    GASNET_TRY_CACHE_EXTRACT_EXPR([value of SIG$1 (binary probe)],SIG$1,[#include <signal.h>],[SIG$1],SIG$1)
    if test -z "$SIG$1" ; then # last resort is to use CROSS var
      GASNET_CROSS_VAR(SIG$1,SIG$1)
    fi
  else 
    GASNET_TRY_CACHE_RUN_EXPR([value of SIG$1], SIG$1, [#include <signal.h>], [val = (int)SIG$1;], SIG$1)
  fi
  AC_SUBST(SIG$1)
  GASNET_FUN_END([$0])
])

dnl If PTHREAD_INCLUDE and/or PTHREAD_LIB set, check to see that pthread.h and libpthread exist,
dnl and set -I and -L to use them.  Die if set, but files don't exist
AC_DEFUN([GASNET_CHECK_OVERRIDE_PTHREADS], [
  GASNET_FUN_BEGIN([$0])
  GASNET_ENV_DEFAULT(PTHREADS_INCLUDE, )
  GASNET_ENV_DEFAULT(PTHREADS_LIB, )
  if test -n "$PTHREADS_INCLUDE" || test -n "$PTHREADS_LIB"; then
    if test -z "$PTHREADS_INCLUDE" || test -z "$PTHREADS_LIB"; then
        AC_MSG_ERROR(['Both \$PTHREADS_INCLUDE and \$PTHREADS_LIB must be set, or neither'])
    fi
    # test to see if files exist
    if test ! -f "$PTHREADS_INCLUDE/pthread.h"; then 
        AC_MSG_ERROR(["Could not find $PTHREADS_INCLUDE/pthread.h: bad \$PTHREADS_INCLUDE"])
    fi
    if test ! -f "$PTHREADS_LIB/libpthread.a" || test ! -f "$PTHREADS_LIB/libpthread.so" ; then 
        AC_MSG_ERROR(["Could not find $PTHREADS_LIB/libpthread.{a,so}: bad \$PTHREADS_LIB"])
    fi
    PTHREAD_INCLUDE_FLAGS="-I$PTHREADS_INCLUDE"
    SYS_HEADER_INST="$PTHREAD_INCLUDE_FLAGS $SYS_HEADER_INST"
    SYS_HEADER_BLD="$PTHREAD_INCLUDE_FLAGS $SYS_HEADER_BLD"
    LDFLAGS="-L$PTHREADS_LIB $LDFLAGS"
    dnl Allow us to ship patches for certain broken pthread.h implementations
    GASNET_ENV_DEFAULT(PTHREADS_PATCH, )
    if test -n "$PTHREADS_PATCH"; then 
      PTHREADS_PATCHFILE=
      for file in "$TOP_SRCDIR/$PTHREADS_PATCH" \
                  "$TOP_SRCDIR/gasnet/$PTHREADS_PATCH" ; do 
        if test -f "$file" ; then 
	  PTHREADS_PATCHFILE="$file"
	fi
      done
      if test -z "$PTHREADS_PATCHFILE" ; then
        AC_MSG_ERROR([Could not find PTHREADS_PATCH file $PTHREADS_PATCH])
      fi
      PATCHED_HEADERS_DIR="$TOP_BUILDDIR/patched-headers"
      mkdir -p "$PATCHED_HEADERS_DIR"
      /usr/bin/patch -N -o "$PATCHED_HEADERS_DIR/pthread.h" -i "$PTHREADS_PATCHFILE" "$PTHREADS_INCLUDE/pthread.h" || \
        AC_MSG_ERROR([failed to apply patch $PTHREADS_PATCHFILE to $PTHREADS_INCLUDE/pthread.h - try again without PTHREADS_PATCH option])
      PATCHED_HEADER="pthread.h"
      # PATCHED_HEADERS_DIR must precede PTHREADS_INCLUDE to override it
      SYS_HEADER_INST="-I###INSTALL_PREFIX###/include/patched-headers $SYS_HEADER_INST"
      SYS_HEADER_BLD="-I$PATCHED_HEADERS_DIR $SYS_HEADER_BLD"
    fi
  fi
  AC_SUBST(SYS_HEADER_BLD)
  AC_SUBST(SYS_HEADER_INST)
  AC_SUBST(PATCHED_HEADER)
  GASNET_FUN_END([$0])
])

dnl check for endianness in a cross-compiling friendly way (using an object scan)
dnl argument is optional prefix to WORDS_BIGENDIAN setting
AC_DEFUN([GASNET_BIGENDIAN], [
GASNET_FUN_BEGIN([$0($1)])
AC_REQUIRE([GASNET_PROG_PERL])
if test "$cross_compiling" = "no" ; then
  GASNET_TRY_CACHE_RUN_EXPR([whether byte ordering is bigendian $1], c_bigendian,[ ],[
    { /* Are we little or big endian?  From Harbison&Steele.  */
      union {
        long l;
        char c[[sizeof (long)]];
      } u;
      u.l = 1;
      val = (u.c[[sizeof (long) - 1]] == 1);
    }], $1[]WORDS_BIGENDIAN)
else
  AC_MSG_CHECKING(whether byte ordering is bigendian (binary probe) $1)
  pushdef([endiancode],[[
	short ascii_mm[] = { 0x4249, 0x4765, 0x6E44, 0x6961, 0x6E53, 0x7953, 0 };
	short ascii_ii[] = { 0x694C, 0x5454, 0x656C, 0x6E45, 0x6944, 0x6E61, 0 };
	void _ascii() { char* s = (char*) ascii_mm; s = (char*) ascii_ii; }
	short ebcdic_ii[] = { 0x89D3, 0xE3E3, 0x8593, 0x95C5, 0x89C4, 0x9581, 0 };
	short ebcdic_mm[] = { 0xC2C9, 0xC785, 0x95C4, 0x8981, 0x95E2, 0xA8E2, 0 };
	void _ebcdic() { char* s = (char*) ebcdic_mm; s = (char*) ebcdic_ii; }
	int foo() { _ascii (); _ebcdic (); return 0; }
  ]])
  $1[]WORDS_BIGENDIAN=""
  dnl try both link and compile, because some systems (eg X1) have an unscannable
  dnl string table in one or the other. Start with compile because it's faster
  pushdef([endianscan],[
      # use perl here, because some greps barf on binary files (eg Solaris)
      if test `$PERL -ne 'if (m/BIGenDianSyS/) { print "yes\n"; }' $GASNET_EXAMINE_BIN` ; then
         $1[]WORDS_BIGENDIAN=1
      fi
      if test `$PERL -ne 'if (m/LiTTleEnDian/) { print "yes\n"; }' $GASNET_EXAMINE_BIN` ; then
        if test -z "$[$1]WORDS_BIGENDIAN" ; then
          $1[]WORDS_BIGENDIAN=0
        else
          $1[]WORDS_BIGENDIAN=""
        fi
      fi
  ])
  GASNET_COMPILE_EXAMINE([ endiancode ],[foo();],[ endianscan ],[GASNET_MSG_ERROR(error building endian probe)])
  if test -z "$[$1]WORDS_BIGENDIAN" ; then
    GASNET_LINK_EXAMINE([ endiancode ],[foo();],[ endianscan ],[GASNET_MSG_ERROR(error building endian probe)])
  fi
  popdef([endianscan])
  popdef([endiancode])
  AC_MSG_RESULT($[$1]WORDS_BIGENDIAN)
fi 
if test "$[$1]WORDS_BIGENDIAN" = "1"; then
  AC_DEFINE($1[]WORDS_BIGENDIAN, 1, [whether byteorder is bigendian])
elif test "$[$1]WORDS_BIGENDIAN" = ""; then
  GASNET_MSG_ERROR(Inconsistent results from endian probe)
fi
GASNET_FUN_END([$0($1)])
])

dnl GASNET_COMPILE_EXAMINE(headers,body,action-success,action-failure)
dnl compile the program given by headers and body
dnl if it suceeds, run action-success with $GASNET_EXAMINE_BIN set to filename of generated object
dnl else, run action-failure
AC_DEFUN([GASNET_COMPILE_EXAMINE], [
AC_REQUIRE([AC_OBJEXT])
GASNET_FUN_BEGIN([$0(...)])
  cat >conftest.$ac_ext <<"EOF"
#include "confdefs.h"
$1
  int main(void) { 
$2
  return 0; }
EOF
  if test ! -f conftest.$ac_ext ; then
     AC_MSG_ERROR(failed to write conftest.$ac_ext)
  fi
  if { (eval echo "$as_me:$LINENO: \"$ac_compile\"") >&5
       (eval $ac_compile) 2>&5
       ac_status=$?
       echo "$as_me:$LINENO: \$? = $ac_status" >&5
       (exit $ac_status); } && \
       test -f conftest.${ac_objext} ; then
    GASNET_PUSHVAR(GASNET_EXAMINE_BIN,"gasnet-examine-bin-$LINENO.${ac_objext}")
    mv conftest.${ac_objext} $GASNET_EXAMINE_BIN
    $3
    rm -f $GASNET_EXAMINE_BIN
    GASNET_POPVAR(GASNET_EXAMINE_BIN)
  else
    echo Failed program: >&5
    cat conftest.$ac_ext >&5
    $4
  fi
GASNET_FUN_END([$0(...)])
]) 

dnl GASNET_LINK_EXAMINE(headers,body,action-success,action-failure)
dnl link the program given by headers and body
dnl if it suceeds, run action-success with $GASNET_EXAMINE_BIN set to filename of generated executable
dnl else, run action-failure
AC_DEFUN([GASNET_LINK_EXAMINE], [
GASNET_FUN_BEGIN([$0(...)])
  cat >conftest.$ac_ext <<"EOF"
#include "confdefs.h"
$1
  int main(void) { 
$2
  return 0; }
EOF
  if test ! -f conftest.$ac_ext ; then
     AC_MSG_ERROR(failed to write conftest.$ac_ext)
  fi
  if { (eval echo "$as_me:$LINENO: \"$ac_link\"") >&5
       (eval $ac_link) 2>&5
       ac_status=$?
       echo "$as_me:$LINENO: \$? = $ac_status" >&5
       (exit $ac_status); } && \
       test -f conftest$ac_exeext ; then
    GASNET_PUSHVAR(GASNET_EXAMINE_BIN,"gasnet-examine-bin-$LINENO$ac_exeext")
    mv conftest$ac_exeext $GASNET_EXAMINE_BIN
    $3
    rm -f $GASNET_EXAMINE_BIN
    GASNET_POPVAR(GASNET_EXAMINE_BIN)
  else
    echo Failed program: >&5
    cat conftest.$ac_ext >&5
    $4
  fi
GASNET_FUN_END([$0(...)])
]) 

dnl build a program and extract the value of a compile-time constant string expression
dnl GASNET_TRY_CACHE_EXTRACT_STR(description,cache_name,headers,expression,result_variable)
AC_DEFUN([GASNET_TRY_CACHE_EXTRACT_STR],[
AC_REQUIRE([GASNET_PROG_PERL])
GASNET_FUN_BEGIN([$0($1,$2,...)])
AC_CACHE_CHECK($1, cv_prefix[]$2,[
cv_prefix[]$2=""
pushdef([embedcode],[
 #include <stdio.h>
 extern const char *s; 
 const char *s = "$gasnetextractstr: (-(|" $4 "|)-) $";
])
pushdef([unpackcode],[
   _extract_prog='BEGIN{$/="\0";} if (m/\$gasnetextractstr: \(-\(\|(.+?)\|\)-\) \$/) { print "[$]1";}' 
   cv_prefix[]$2=`$PERL -ne "$_extract_prog" $GASNET_EXAMINE_BIN`
])
 GASNET_COMPILE_EXAMINE([$3
   embedcode ],[ printf("%s",s); ],
   [ unpackcode ],[GASNET_MSG_ERROR(Failed while compile extracting $4)])
if test -z "$cv_prefix[]$2" ; then
 GASNET_LINK_EXAMINE([$3
   embedcode ],[ printf("%s",s); ],
   [ unpackcode ],[GASNET_MSG_ERROR(Failed while link extracting $4)])
fi
popdef([unpackcode])
popdef([embedcode])
])
if test -n "$cv_prefix[]$2" ; then
  $5=$cv_prefix[]$2
fi
GASNET_FUN_END([$0($1,$2,...)])
])

dnl build a program and extract the value of a compile-time constant expression
dnl GASNET_TRY_CACHE_EXTRACT_EXPR(description,cache_name,headers,expression,result_variable)
AC_DEFUN([GASNET_TRY_CACHE_EXTRACT_EXPR],[
AC_REQUIRE([GASNET_PROG_PERL])
GASNET_FUN_BEGIN([$0($1,$2,...)])
AC_CACHE_CHECK($1, cv_prefix[]$2,[
cv_prefix[]$2=""
pushdef([embedcode],[
 #include <stdio.h>
 extern char s[[]]; 
 #define DIGIT(d) (((char)(( (($4)>0?($4):-($4)) >>(d*4))&0xF))+0x40)
 char s[[]] = {'$','g','a','s','n','e','t','e','x','t','r','a','c','t','e','x','p','r',':',' ',
             (($4)>=0?' ':'-'),
             ' ',
	     DIGIT(7),DIGIT(6),DIGIT(5),DIGIT(4),
	     DIGIT(3),DIGIT(2),DIGIT(1),DIGIT(0),
             ' ','$','\0'};
])
pushdef([unpackcode],[
   _extract_prog='BEGIN{$/="\$";} if (m/^gasnetextractexpr: ([[ -]]) (.+?) \$/) { map($val=($val<<4)+($_-0x40),unpack("C8",[$]2)); print "-" if ([$]1 eq "-"); print $val;}' 
   cv_prefix[]$2=`$PERL -ne "$_extract_prog" $GASNET_EXAMINE_BIN`
])
 GASNET_COMPILE_EXAMINE([$3
   embedcode ],[ char *p = s; while (*p) printf("%c",*(p++)); ],
   [ unpackcode ],[GASNET_MSG_ERROR(Failed while compile extracting $4)])
if test -z "$cv_prefix[]$2" ; then
 GASNET_LINK_EXAMINE([$3
   embedcode ],[ char *p = s; while (*p) printf("%c",*(p++)); ],
   [ unpackcode ],[GASNET_MSG_ERROR(Failed while link extracting $4)])
fi
popdef([unpackcode])
popdef([embedcode])
])
if test -n "$cv_prefix[]$2" ; then
  $5=$cv_prefix[]$2
fi
GASNET_FUN_END([$0($1,$2,...)])
])

dnl We want AC_DISABLE_OPTION_CHECKING if it exists
ifdef([AC_DISABLE_OPTION_CHECKING],
      [define([GASNET_NO_CHECK_OPTS], defn([AC_DISABLE_OPTION_CHECKING]))],
      [define([GASNET_NO_CHECK_OPTS], [])])
