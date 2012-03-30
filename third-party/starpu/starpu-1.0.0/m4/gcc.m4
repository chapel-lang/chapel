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

dnl Set $ac_cv_starpu_gcc_for_plugin to the compiler to use to compile
dnl GCC plug-ins.  It's `gcc' for GCC 4.5/4.6, probably `g++' for 4.7,
dnl and definitely `g++' for 4.8, because the last two build
dnl themselves with `g++', leading to mangled names.
dnl See <http://thread.gmane.org/gmane.comp.gcc.devel/125210> for details.
AC_DEFUN([_STARPU_GCC_PLUGIN_LANGUAGE], [
  AC_CACHE_CHECK([which compiler to use to build GCC plug-ins],
    [ac_cv_starpu_gcc_for_plugin], [
     for GCC_FOR_PLUGIN in "$CC" "$CXX" ""
     do
       if test "x$GCC_FOR_PLUGIN" = "x"; then
	 break;
       fi

       cat > conftest.c <<END_OF_CONFTEST
	 #include <gcc-plugin.h>
         #include <plugin-version.h>
	 #include <cpplib.h>

	 int plugin_is_GPL_compatible;

         extern struct cpp_reader *parse_in; /* C-family front-ends */

	 static void
	 define_something (void *gcc_data, void *user_data)
	 {
	   cpp_define (parse_in, "CONFTEST_GCC_PLUGIN=1");
	 }

	 int
	 plugin_init (struct plugin_name_args *plugin_info,
		      struct plugin_gcc_version *version)
	 {
	   if (!plugin_default_version_check (version, &gcc_version))
	     return 1;
	   register_callback ("conftest", PLUGIN_START_UNIT,
			      define_something, NULL);
	   return 0;
	 }
END_OF_CONFTEST

       # Build the plug-in.
       rm -f conftest.so
       _STARPU_WITH_GCC_PLUGIN_API([
	 _AC_DO(["$GCC_FOR_PLUGIN" "$CPPFLAGS" -fPIC -shared conftest.c -o conftest.so]) || {
	   AC_MSG_ERROR([failed to build a GCC plug-in with `$GCC_FOR_PLUGIN'])
	 }
       ])

       # Attempt to use it.
       save_CFLAGS="$CFLAGS"
       CFLAGS="-fplugin=$PWD/conftest.so"
       AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
			   #ifndef CONFTEST_GCC_PLUGIN
			   # error plug-in not loaded
			   #endif]], [])],
	 [ac_cv_starpu_gcc_for_plugin="$GCC_FOR_PLUGIN"], [:])
       CFLAGS="$save_CFLAGS"

       rm -f conftest.so conftest.c

       if test "x$ac_cv_starpu_gcc_for_plugin" != "x"; then
	 # We're done.
	 break
       fi
     done

     if test "x$ac_cv_starpu_gcc_for_plugin" = "x"; then
       AC_MSG_RESULT([none])
       AC_MSG_ERROR([could not find a suitable compiler for GCC plug-ins])
     fi
  ])

  $1="$ac_cv_starpu_gcc_for_plugin"
])


dnl Check whether GCC plug-in support is available (GCC 4.5+).
AC_DEFUN([STARPU_GCC_PLUGIN_SUPPORT], [
  AC_REQUIRE([AC_PROG_CC])
  AC_REQUIRE([AC_PROG_CXX]) dnl for GCC 4.7+
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
    dnl   builtin_decl_explicit     -- new in 4.7, replaces `built_in_decls'
    dnl   .affects_type_identity    -- new field in 4.7
    _STARPU_WITH_GCC_PLUGIN_API([
      AC_CHECK_DECLS([build_call_expr_loc_array, build_call_expr_loc_vec,
                      build_array_ref, build_zero_cst,
		      builtin_decl_explicit],
        [], [], [#include <gcc-plugin.h>
	         #include <tree.h>])

      dnl Work around header naming issues introduced upstream and in Debian
      dnl (see <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=631082>).
      AC_CHECK_HEADERS([c-common.h c-pragma.h c-family/c-common.h c-family/c-pragma.h],
        [], [], [#include <gcc-plugin.h>
	         #include <tree.h>])

      AC_CHECK_MEMBER([struct attribute_spec.affects_type_identity],
        [AC_DEFINE([HAVE_ATTRIBUTE_SPEC_AFFECTS_TYPE_IDENTITY], [1],
	  [Define to 1 when `struct attribute_spec' has the `affects_type_identity' field.])],
	[],
	[#include <gcc-plugin.h>
	 #include <tree.h>])

    ])


    AC_DEFINE_UNQUOTED([STARPU_INCLUDE_DIR],
      ["`eval "echo $includedir"`/starpu/$STARPU_EFFECTIVE_VERSION"],
      [Define to the directory where StarPU's headers are installed.])

    dnl Now, `gcc' or `g++'?
    _STARPU_GCC_PLUGIN_LANGUAGE([GCC_FOR_PLUGIN])
    AC_SUBST([GCC_FOR_PLUGIN])

    dnl Determine the corresponding Libtool tag.
    if test "$GCC_FOR_PLUGIN" = "$CXX"; then
      GCC_FOR_PLUGIN_LIBTOOL_TAG="CXX"

      # Require C++11, for lambdas and `auto'.
      GCC_FOR_PLUGIN="$GCC_FOR_PLUGIN -std=c++11"
    else
      GCC_FOR_PLUGIN_LIBTOOL_TAG="CC"
    fi
    AC_SUBST([GCC_FOR_PLUGIN_LIBTOOL_TAG])
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
