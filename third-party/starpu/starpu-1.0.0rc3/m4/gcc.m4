dnl -*- Autoconf -*-
dnl
dnl Copyright (C) 2011, 2012 Institut National de Recherche en Informatique et Automatique
dnl
dnl StarPU is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU Lesser General Public License as published by
dnl the Free Software Foundation; either version 2.1 of the License, or (at
dnl your option) any later version.
dnl
dnl StarPU is distributed in the hope that it will be useful, but
dnl WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
dnl
dnl See the GNU Lesser General Public License in COPYING.LGPL for more details.

dnl Run its argument with CPPFLAGS pointing to GCC's plug-in API.
AC_DEFUN([_STARPU_WITH_GCC_PLUGIN_API], [
  GCC_PLUGIN_INCLUDE_DIR="`"$CC" -print-file-name=plugin`/include"

  save_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="-I$GCC_PLUGIN_INCLUDE_DIR"

  $1

  CPPFLAGS="$save_CPPFLAGS"
])

dnl Check whether GCC plug-in support is available (GCC 4.5+).
AC_DEFUN([STARPU_GCC_PLUGIN_SUPPORT], [
  AC_REQUIRE([AC_PROG_CC])
  AC_CACHE_CHECK([whether GCC supports plug-ins], [ac_cv_have_gcc_plugins], [
    if test "x$GCC" = xyes; then
      # ICC 12.1.0 and Clang 3.1 (among others) support `--version',
      # define `__GNUC__', and provide a `-print-file-name=plugin'
      # that returns GCC's valid header directory.  This makes them
      # hardly distinguishable from GCC.  Actually, ICC 12.1.0 is able
      # to compile our plug-in, but silently ignores `-fplugin', leading
      # to obvious build failures; thus, it is explicitly excluded below.
      _STARPU_WITH_GCC_PLUGIN_API([
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[#include <gcc-plugin.h>
	      #include <tree.h>
	      #include <gimple.h>

	      #if defined __INTEL_COMPILER || defined __ICC
	      Beware, this compiler is a fake.  Don't use it.
	      #endif

	      tree fndecl; gimple call;]],
	    [[/* Clang 3.1 doesn't support nested functions, so try to
	         discriminate it this way.  */
	      tree foo (void)
	      {
	        return lookup_name (get_identifier ("puts"));
              }
	      fndecl = foo ();
	      call = gimple_build_call (fndecl, 0);]])],
	  [ac_cv_have_gcc_plugins="yes"],
	  [ac_cv_have_gcc_plugins="no"])
      ])
    else
      ac_cv_have_gcc_plugins="no"
    fi
  ])

  if test "x$ac_cv_have_gcc_plugins" = "xyes"; then
    dnl Check for specific features.
    dnl
    dnl Reason:
    dnl   build_call_expr_loc_array -- not in GCC 4.5.x; appears in 4.6
    dnl   build_call_expr_loc_vec   -- likewise
    dnl   build_array_ref           -- present but undeclared in 4.6.1
    dnl   build_zero_cst            -- not in GCC 4.5.x; appears in 4.6
    _STARPU_WITH_GCC_PLUGIN_API([
      AC_CHECK_DECLS([build_call_expr_loc_array, build_call_expr_loc_vec,
                      build_array_ref, build_zero_cst],
        [], [], [#include <gcc-plugin.h>
	         #include <tree.h>])

      dnl Work around header naming issues introduced upstream and in Debian
      dnl (see <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=631082>).
      AC_CHECK_HEADERS([c-common.h c-pragma.h c-family/c-common.h c-family/c-pragma.h],
        [], [], [#include <gcc-plugin.h>
	         #include <tree.h>])

      AC_DEFINE_UNQUOTED([STARPU_INCLUDE_DIR],
        ["`eval "echo $includedir"`/starpu/$STARPU_EFFECTIVE_VERSION"],
        [Define to the directory where StarPU's headers are installed.])
    ])
  fi

  AC_SUBST([GCC_PLUGIN_INCLUDE_DIR])
])

dnl Substitute `STARPU_GCC_VERSION_MAJOR' and `STARPU_GCC_VERSION_MINOR'.
AC_DEFUN([STARPU_GCC_VERSION], [
  AC_COMPUTE_INT([STARPU_GCC_VERSION_MAJOR], [__GNUC__])
  AC_COMPUTE_INT([STARPU_GCC_VERSION_MINOR], [__GNUC_MINOR__])
  AC_SUBST([STARPU_GCC_VERSION_MAJOR])
  AC_SUBST([STARPU_GCC_VERSION_MINOR])
])
