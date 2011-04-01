dnl#### based on http://gcc.gnu.org/onlinedocs/libstdc++/manual/backwards.html
dnl
dnl SYNOPSIS
dnl
dnl   QTHREAD_HASH_MAPS([if-found],[if-not-found])
dnl
dnl DESCRIPTION
dnl
dnl   This macro checks for working hash containers.

AC_DEFUN([QTHREAD_HASH_MAPS], [
  AC_LANG_SAVE
  AC_LANG_CPLUSPLUS
  ac_found_without_flags=no
  ac_found_a_hash=no
  AC_CHECK_HEADERS([unordered_map tr1/unordered_map],
	[ac_found_without_flags=yes
	 ac_found_a_hash=yes
	 break])
  AS_IF([test "x$ac_found_without_flags" == xno],
    [ac_save_CXXFLAGS="$CXXFLAGS"
	 ac_save_CPPFLAGS="$CPPFLAGS"
	 unset ac_cv_header_unordered_map
	 unset ac_cv_header_tr1_unordered_map
	 CXXFLAGS="$ac_save_CXXFLAGS -std=gnu++0x"
	 CPPFLAGS="$ac_save_CPPFLAGS -std=gnu++0x"
	 AC_CHECK_HEADERS([unordered_map tr1/unordered_map],
	   [ac_found_a_hash=yes
	    ac_save_CXXFLAGS="$ac_save_CXXFLAGS -std=gnu++0x"
		break])
	 CXXFLAGS="$ac_save_CXXFLAGS"
	 CPPFLAGS="$ac_save_CPPFLAGS"
	 ])
  AC_LANG_RESTORE
  AS_IF([test "x$ac_found_a_hash" == xno],
    [AC_CACHE_CHECK([for ext/hash_map],
	   [ac_cv_cxx_ext_hash_map],
	   [AC_LANG_SAVE
	    AC_LANG_CPLUSPLUS
	    ac_save_CXXFLAGS="$CXXFLAGS"
	    CXXFLAGS="$CXXFLAGS -Werror -Wno-deprecated"
	    AC_TRY_COMPILE([#include <ext/hash_map>],
		  [using __gnu_cxx::hash_map;],
		  [ac_cv_cxx_ext_hash_map=yes
		   ac_found_a_hash=yes
		   CXXFLAGS="$ac_save_CXXFLAGS -Wno-deprecated"],
		  [ac_cv_cxx_ext_hash_map=no
		   CXXFLAGS="$ac_save_CXXFLAGS"])
	    AC_LANG_RESTORE
	    ])
	AS_IF([test "x$ac_cv_cxx_ext_hash_map" = xyes],
	  [AC_DEFINE(HAVE_EXT_HASH_MAP,1,[Define if ext/hash_map is present.])])
	])
	AS_IF([test "x$ac_found_a_hash" == xyes], [$1], [$2])
])
