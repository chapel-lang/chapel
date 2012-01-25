# -*- Autoconf -*-
#
# Copyright (c)      2008  Sandia Corporation
#
# This macro based heavily on the ompi_check_asm macro found in 
# Open MPI.  License for the original macro is below

# Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
#                         University Research and Technology
#                         Corporation.  All rights reserved.
# Copyright (c) 2004-2007 The University of Tennessee and The University
#                         of Tennessee Research Foundation.  All rights
#                         reserved.
# Copyright (c) 2004-2006 High Performance Computing Center Stuttgart, 
#                         University of Stuttgart.  All rights reserved.
# Copyright (c) 2004-2006 The Regents of the University of California.
#                         All rights reserved.
# Copyright (c) 2006-2007 Los Alamos National Security, LLC.  All rights
#                         reserved. 
# Copyright (c) 2006-2007 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2006-2007 Voltaire, Inc. All rights reserved.
# Copyright (c) 2006      Sandia National Laboratories. All rights reserved.
# Copyright (c) 2006-2007 Sun Microsystems, Inc.  All rights reserved.
#                         Use is subject to license terms.
# Copyright (c) 2006-2007 The University of Houston. All rights reserved.
# Copyright (c) 2006      Myricom, Inc.  All rights reserved.
# Copyright (c) 2007-2008 UT-Battelle, LLC. All rights reserved.
#
# $COPYRIGHT$
#
# Additional copyrights may follow
#
# $HEADER$
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# - Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
#
# - Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer listed
#   in this license in the documentation and/or other materials


dnl QTHREAD_TRY_ASSEMBLE(asm-code, [action-if-success], [action-if-fail])
dnl
dnl Attempt to assemble asm-code.  If success, run action-if-success.
dnl Otherwise, run action-if-fail.  Neither action-if-success nor
dnl action-if-fail are required.
dnl
dnl No preprocessing is guaranteed to be done on asm-code.  Some
dnl compilers do not run the preprocessor on assembly files.  
dnl
dnl On failure, asm-test.s will be included in config.out
AC_DEFUN([QTHREAD_TRY_ASSEMBLE],
[cat >conftest.s <<EOF
[$1]
EOF
if test "$CC" = "$CCAS" ; then
    qthread_assemble="$CCAS $CCASFLAGS -c conftest.s >conftest.out 2>&1"
else
    qthread_assemble="$CCAS $CCASFLAGS -o conftest.o conftest.s >conftest.out 2>&1"
fi
if AC_TRY_EVAL(qthread_assemble); then
  # save the warnings
  cat conftest.out >&AC_FD_CC
  ifelse([$2],,:,[$2])
else
  # save compiler output and failed program
  cat conftest.out >&AC_FD_CC
  echo "configure: failed program was:" >&AC_FD_CC
  cat conftest.s >&AC_FD_CC
  ifelse([$3],,:,[$3])
fi
rm -rf conftest*
unset qthread_assemble
])dnl


dnl #################################################################
dnl
dnl QTHREAD_CHECK_SPARCV8PLUS
dnl
dnl #################################################################
AC_DEFUN([QTHREAD_CHECK_SPARCV8PLUS],[
    AC_MSG_CHECKING([if have Sparc v8+/v9 support])
    sparc_result=0
    QTHREAD_TRY_ASSEMBLE([$qthread_cv_asm_text
	casa [%o0] 0x80, %o1, %o2],
                [sparc_result=1],
                [sparc_result=0])
    if test "$sparc_result" = "1" ; then
        AC_MSG_RESULT([yes])
        ifelse([$1],,:,[$1])
    else
        AC_MSG_RESULT([no])
        ifelse([$2],,:,[$2])
    fi

    unset sparc_result
])dnl


dnl #################################################################
dnl
dnl QTHREAD_CHECK_INLINE_GCC
dnl
dnl Check if the compiler is capable of doing GCC-style inline
dnl assembly.  Some compilers emit a warning and ignore the inline
dnl assembly (xlc on OS X) and compile without error.  Therefore,
dnl the test attempts to run the emited code to check that the
dnl assembly is actually run.  To run this test, one argument to
dnl the macro must be an assembly instruction in gcc format to move 
dnl the value 0 into the register containing the variable ret.  
dnl For PowerPC, this would be:
dnl
dnl   "li %0,0" : "=&r"(ret)
dnl
dnl DEFINE QTHREAD_GCC_INLINE_ASSEMBLY to 0 or 1 depending on GCC
dnl                support
dnl
dnl #################################################################
AC_DEFUN([QTHREAD_CHECK_INLINE_C_GCC],[
  assembly="$1"
  asm_result="unknown"

  AC_MSG_CHECKING([if $CC supports GCC inline assembly])
  if test ! "$assembly" = "" ; then
    AC_RUN_IFELSE([AC_LANG_SOURCE([[
int main() {
int ret = 1;
__asm__ __volatile__ ($assembly);
return ret;
}]])],
      [asm_result="yes"], [asm_result="no"], 
      [asm_result="unknown"])
  else
    assembly="test skipped - assuming no"
  fi

  # if we're cross compiling, just try to compile and figure good enough
  AS_IF([test "$asm_result" = "unknown"],
    [AC_LINK_IFELSE([AC_LANG_SOURCE([[
int main()
{
int ret = 1;
__asm__ __volatile__ ($assembly);
return ret;
}]])],
      [asm_result="yes"], [asm_result="no"])
  ])

  AC_MSG_RESULT([$asm_result])

  AS_IF([test "$asm_result" = "yes"],
    [AC_DEFINE([HAVE_GCC_INLINE_ASSEMBLY], [1],
       [Whether C compiler supports GCC style inline assembly])
     $2], [$3])

  unset have_c_gcc_inline_asm assembly asm_result
])dnl


AC_DEFUN([QTHREAD_CHECK_ASSEMBLY],[
  AC_REQUIRE([AM_PROG_AS])
  AC_CHECK_SIZEOF([long])

  # find our architecture for purposes of assembly stuff
  qthread_cv_asm_arch="UNSUPPORTED"
  qthread_gcc_inline_assign=""
  case "${host}" in
    i?86-*|x86_64*)
      if test "$ac_cv_sizeof_long" = "4" ; then
        qthread_cv_asm_arch="IA32"
      else
        qthread_cv_asm_arch="AMD64"
      fi
      qthread_gcc_inline_assign='"movl [$]0, %0" : "=&r"(ret)'
    ;;

    ia64-*)
      qthread_cv_asm_arch="IA64"
      qthread_gcc_inline_assign='"mov %0=r0\n;;\n" : "=&r"(ret)'
    ;;

    alpha-*|alphaev[[4-8]]-*|alphaev56-*|alphaev6[[78]]-*)
      qthread_cv_asm_arch="ALPHA"
      qthread_gcc_inline_assign='"bis [$]31,[$]31,%0" : "=&r"(ret)'
    ;;

	tile-*)
	  qthread_cv_asm_arch="TILE"
	  qthread_gcc_inline_assign='"movei %0, 5" : "=&r"(ret)'
	;;

    mips-*|mips64-*)
      # Should really find some way to make sure that we are on
      # a MIPS III machine (r4000 and later)
      qthread_cv_asm_arch="MIPS"
      qthread_gcc_inline_assign='"or %0,[$]0,[$]0" : "=&r"(ret)'
    ;;

    powerpc-*|powerpc64-*)
      if test "$ac_cv_sizeof_long" = "4" ; then
        qthread_cv_asm_arch="POWERPC32"
      elif test "$ac_cv_sizeof_long" = "8" ; then
        qthread_cv_asm_arch="POWERPC64"
      fi
      qthread_gcc_inline_assign='"1: li %0,0" : "=&r"(ret)'
    ;;

    sparc*-*)
      # SPARC v9 (and above) are the only ones with 64bit support
      # if compiling 32 bit, see if we are v9 (aka v8plus) or
      # earlier (casa is v8+/v9). 
      if test "$ac_cv_sizeof_long" = "4" ; then
        QTHREAD_CHECK_SPARCV8PLUS([qthread_cv_asm_arch="SPARCV9_32"])
      elif test "$ac_cv_sizeof_long" = "8" ; then
        qthread_cv_asm_arch="SPARCV9_64"
      fi
      qthread_gcc_inline_assign='"mov 0,%0" : "=&r"(ret)'
    ;;
  esac

  # now that we know our architecture, try to inline assemble
  QTHREAD_CHECK_INLINE_C_GCC([$qthread_gcc_inline_assign], [$1], [$2])

  AC_MSG_CHECKING([for asssembly architecture])
  AC_MSG_RESULT([$qthread_cv_asm_arch])
  result="QTHREAD_$qthread_cv_asm_arch"
  AC_DEFINE_UNQUOTED([QTHREAD_ASSEMBLY_ARCH], [$result],
      [Architecture type of assembly to use])

  unset result asm_format
])dnl
