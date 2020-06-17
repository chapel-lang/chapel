dnl
dnl Copyright (c) 2015-2019 Cray Inc. All rights reserved.
dnl Copyright (c) 2015-2018 Los Alamos National Security, LLC.
dnl                         All rights reserved.
dnl
dnl This software is available to you under a choice of one of two
dnl licenses.  You may choose to be licensed under the terms of the GNU
dnl General Public License (GPL) Version 2, available from the file
dnl COPYING in the main directory of this source tree, or the
dnl BSD license below:
dnl
dnl     Redistribution and use in source and binary forms, with or
dnl     without modification, are permitted provided that the following
dnl     conditions are met:
dnl
dnl      - Redistributions of source code must retain the above
dnl        copyright notice, this list of conditions and the following
dnl        disclaimer.
dnl
dnl      - Redistributions in binary form must reproduce the above
dnl        copyright notice, this list of conditions and the following
dnl        disclaimer in the documentation and/or other materials
dnl        provided with the distribution.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
dnl "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
dnl LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
dnl FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
dnl COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
dnl INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
dnl BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
dnl LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
dnl CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
dnl LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
dnl ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
dnl POSSIBILITY OF SUCH DAMAGE.
dnl

dnl Configury specific to the libfabrics GNI provider

dnl Called to configure this provider

m4_include([config/fi_pkg.m4])

AC_DEFUN([FI_GNI_CONFIGURE],[
        # Determine if we can support the gni provider
        # have to pull in pkg.m4 manually
        ugni_lib_happy=0
        udreg_lib_happy=0
        gni_header_happy=0
        alps_lli_happy=0
        alps_util_happy=0
        have_criterion=false
        criterion_tests_present=true
        gni_CPPFLAGS=
        gni_LDFLAGS=
	gnitest_CPPFLAGS=
	gnitest_LDFLAGS=
        gnitest_LIBS=
        xpmem_happy=0
        kdreg_happy=0


        AC_ARG_ENABLE([xpmem],
                      [AS_HELP_STRING([--enable-xpmem],
                                      [Enable xpmem (gni provider) @<:@default=yes@:>@])],
                      )

        AC_ARG_ENABLE([ugni-static],
                      [AS_HELP_STRING([--enable-ugni-static],
                                      [Enable static linking with uGNI.  Recommended for KNL.])],
                     )

        AS_IF([test x"$enable_gni" != x"no"],
               [FI_PKG_CHECK_MODULES([CRAY_GNI_HEADERS], [cray-gni-headers],
                                 [gni_header_happy=1
                                  gni_CPPFLAGS="$CRAY_GNI_HEADERS_CFLAGS $gni_CPPFLAGS"
                                 ],
                                 [gni_header_happy=0])
              ])

        AS_IF([test "$gni_header_happy" -eq 1],
              [FI_PKG_CHECK_MODULES_STATIC([CRAY_UGNI], [cray-ugni],
                                 [ugni_lib_happy=1
                                  gni_CPPFLAGS=$CRAY_UGNI_CFLAGS
                                  gni_LDFLAGS=$CRAY_UGNI_LIBS
                                 ],
                                 [ugni_lib_happy=0])

               AS_IF([test x"$enable_ugni_static" == x"yes" && test $ugni_lib_happy -eq 1],
                     [gni_LDFLAGS=$(echo $gni_LDFLAGS | sed -e 's/lugni/l:libugni.a/')],[])

               FI_PKG_CHECK_MODULES_STATIC([CRAY_ALPS_LLI], [cray-alpslli],
                                 [alps_lli_happy=1
                                  gni_CPPFLAGS="$CRAY_ALPS_LLI_CFLAGS $gni_CPPFLAGS"
                                  gni_LDFLAGS="$CRAY_ALPS_LLI_LIBS $gni_LDFLAGS"
                                 ],
                                 [alps_lli_happy=0])
               FI_PKG_CHECK_MODULES([CRAY_ALPS_UTIL], [cray-alpsutil],
                                 [alps_util_happy=1
                                  gni_CPPFLAGS="$CRAY_ALPS_UTIL_CFLAGS $gni_CPPFLAGS"
                                  gni_LDFLAGS="$CRAY_ALPS_UTIL_LIBS $gni_LDFLAGS"
                                 ],
                                 [alps_util_happy=0])
               FI_PKG_CHECK_MODULES([CRAY_UDREG], [cray-udreg],
                                 [udreg_lib_happy=1
                                  gni_CPPFLAGS="-DHAVE_UDREG $CRAY_UDREG_CFLAGS $gni_CPPFLAGS"
                                  gni_LDFLAGS="$CRAY_UDREG_LIBS $gni_LDFLAGS"
                                 ],
                                 [udreg_lib_happy=0])
               AS_IF([test x"$enable_xpmem" != x"no"],
                     [FI_PKG_CHECK_MODULES([CRAY_XPMEM], [cray-xpmem],
                                 [AC_DEFINE_UNQUOTED([HAVE_XPMEM], [1], [Define to 1 if xpmem available])
                                  gni_CPPFLAGS="$CRAY_XPMEM_CFLAGS $gni_CPPFLAGS"
                                  gni_LDFLAGS="$CRAY_XPMEM_LIBS $gni_LDFLAGS"
                                 ],
                                 [xpmem_happy=0])
                      ],
                      [AC_DEFINE_UNQUOTED([HAVE_XPMEM], [0], [Define to 1 if xpmem available])
                      ])

               gni_path_to_gni_pub=${CRAY_GNI_HEADERS_INCLUDE_OPTS:2}/gni_pub.h
               dnl Trim the leading -I in order to provide a path

               AC_CHECK_TYPES([gni_ct_cqw_post_descriptor_t], [],
                              [AC_MSG_WARN([GNI provider requires CLE 5.2.UP04 or higher. Disabling gni provider.])
                               gni_header_happy=0
                              ],
                              [[#include "$gni_path_to_gni_pub"]])

               AS_IF([test -d $srcdir/prov/gni/test],
                     [AC_ARG_WITH([criterion], [AS_HELP_STRING([--with-criterion],
                     [Location for criterion unit testing framework])])],
                     [criterion_tests_present=false])

               if test "$with_criterion" != "" && test "$with_criterion" != "no"; then
	             if test "$enable_direct" != "" && test "$enable_direct" != "no"; then
		     	gnitest_CPPFLAGS="-I$srcdir/prov/gni/include"
		     fi

                     AS_IF([test "$criterion_tests_present" = "true"],
                           [AC_MSG_CHECKING([criterion path])
                            if test -d "$with_criterion"; then
                                AC_MSG_RESULT([yes])
                                gnitest_CPPFLAGS="-I$with_criterion/include -DHAVE_UDREG $CRAY_UDREG_CFLAGS $gnitest_CPPFLAGS"
                                gnitest_LIBS="-lcriterion -ludreg  $gnitest_LIBS"

                                if test -d "$with_criterion/lib"; then
                                        gnitest_LDFLAGS="$CRAY_ALPS_LLI_STATIC_LIBS -L$with_criterion/lib -Wl,-rpath=$with_criterion/lib $gnitest_LDFLAGS"
                                        have_criterion=true
                                elif test -d "$with_criterion/lib64"; then
                                        gnitest_LDFLAGS="$CRAY_ALPS_LLI_STATIC_LIBS -L$with_criterion/lib64 -Wl,-rpath=$with_criterion/lib64 $gnitest_LDFLAGS"
                                        have_criterion=true
                                else
                                        have_criterion=false
                                fi

                                gnitest_LDFLAGS="$CRAY_UDREG_LIBS $gnitest_LDFLAGS"
                                FI_PKG_CHECK_MODULES([CRAY_PMI], [cray-pmi],
                                                     [],
                                                     [have_criterion=false])
                            else
                                AC_MSG_RESULT([no])
                                AC_MSG_ERROR([criterion requested but invalid path given])
                            fi],
                           [AC_MSG_ERROR([criterion requested tests not available])])
               fi

               AC_CHECK_DECL([HAVE_ATOMICS],
                             [],
                             [cc_version=`$CC --version | head -n1`
                              AC_MSG_WARN(["$cc_version" doesn't support native atomics.  Disabling GNI provider.])
                              ugni_lib_happy=0])



dnl kdreg configury handling:
dnl First check to see config line has --with-kdreg arg.  If yes and something other than
dnl no, use the old way, otherwise if with_kdreg is not equal to no, try pkg-config method.
dnl Note kdreg only supplies an include file, no library

                AC_ARG_WITH([kdreg], [AS_HELP_STRING([--with-kdreg],
                             [Install directory for kdreg headers])])

                AS_IF([test "$with_kdreg" != "" && test "$with_kdreg" != "no"],
                      [gni_CPPFLAGS="-I$with_kdreg/include $gni_CPPFLAGS"
                       gnitest_CPPFLAGS="-I$with_kdreg/include $gnitest_CPPFLAGS"
                       kdreg_happy=1],
                      [AS_IF([test "$with_kdreg" != "no"],
                       [FI_PKG_CHECK_MODULES([CRAY_KDREG], [cray-kdreg],
                                             [kdreg_happy=1
                                              gni_CPPFLAGS="$CRAY_KDREG_CFLAGS $gni_CPPFLAGS"
                                              gnitest_CPPFLAGS="$CRAY_KDREG_CFLAGS $gnitest_CPPFLAGS"],
                                             [kdreg_happy=0])])])

dnl
dnl             double check that kdreg_pub.h is available
dnl
                AS_IF([test "$kdreg_happy" = "1"],
                      [CPPFLAGS="$CPPFLAGS $gni_CPPFLAGS"
                       AC_CHECK_HEADER([kdreg_pub.h],
                                       [],
                                       [kdreg_happy=0])])
                AC_DEFINE_UNQUOTED([HAVE_KDREG],[$kdreg_happy], [Define to 1 if kdreg available])

        ])


        AM_CONDITIONAL([HAVE_CRITERION], [test "x$have_criterion" = "xtrue"])
        AS_IF([test "x$have_criterion" = "xtrue"],
              [AC_DEFINE_UNQUOTED([HAVE_CRITERION], [1], [Define to 1 if criterion requested and available])],
              [AC_DEFINE_UNQUOTED([HAVE_CRITERION], [0], [Define to 1 if criterion requested and available])])

        AC_SUBST(gni_CPPFLAGS)
        AC_SUBST(gni_LDFLAGS)
	AC_SUBST(gnitest_CPPFLAGS)
        AC_SUBST(gnitest_LDFLAGS)
        AC_SUBST(gnitest_LIBS)

        AS_IF([test $gni_header_happy -eq 1 -a $ugni_lib_happy -eq 1 \
               -a $alps_lli_happy -eq 1 -a $alps_util_happy -eq 1 \
               -a $udreg_lib_happy -eq 1], [$1], [$2])
])
