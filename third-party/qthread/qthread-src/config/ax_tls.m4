# ===========================================================================
#          http://www.gnu.org/software/autoconf-archive/ax_tls.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_TLS([action-if-found], [action-if-not-found])
#
# DESCRIPTION
#
#   Provides a test for the compiler support of thread local storage (TLS)
#   extensions. Defines TLS if it is found. Currently knows about GCC/ICC
#   and MSVC. I think SunPro uses the same as GCC, and Borland apparently
#   supports either.
#
# LICENSE
#
#   Copyright (c) 2008 Alan Woodland <ajw05@aber.ac.uk>
#   Copyright (c) 2010 Diego Elio Petteno` <flameeyes@gmail.com>
#
#   This program is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation, either version 3 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <http://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 10

AC_DEFUN([AX_TLS], [
  AC_ARG_ENABLE([native-tls],
                [AS_HELP_STRING([--disable-native-tls],
								[Prevent use of compiler-based thread-local
								storage. This is sometimes necessary when
								compiler-based thread-local storage does not
								provide zero-based initialization (something
								that pthread_key_t *does* provide).])])
  AC_MSG_CHECKING(for thread local storage (TLS) class)
  AC_CACHE_VAL([ac_cv_tls], [
	AS_IF([test "x$enable_native_tls" != xno], [
    ax_tls_keywords="__thread __declspec(thread) none"
    for ax_tls_keyword in $ax_tls_keywords; do
	   AS_IF([test "$ax_tls_keyword" = "none"],
	         [ac_cv_tls=none
			  break],
			 [AC_LINK_IFELSE(
              [AC_LANG_PROGRAM([[#include <stdlib.h>]],
               [[ static ] $ax_tls_keyword [ int bar;
               exit(1);]])],
               [ac_cv_tls=$ax_tls_keyword ; break],
               [ac_cv_tls=none])])
    done],
	[ac_cv_tls=none])
  ])
  AC_MSG_RESULT($ac_cv_tls)

  AS_IF([test "$ac_cv_tls" != "none"],
    [AC_DEFINE_UNQUOTED([TLS], $ac_cv_tls, [If the compiler supports a TLS storage class define it to that here])
     $1],
    [$2])
])
