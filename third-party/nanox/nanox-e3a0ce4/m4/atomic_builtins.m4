AC_DEFUN([AC_CHECK_GXX_ATOMIC_BUILTINS], [
   AC_MSG_CHECKING([for atomic builtins in GCC])
   AC_LANG_PUSH([C++])
   AC_LINK_IFELSE(
      [AC_LANG_PROGRAM([],
         [[
          int foo = -10;
          int bar = 10;
          __sync_fetch_and_add(&foo, bar);
         ]]
      )],
      [ac_cv_have_builtins=yes],
      [ac_cv_have_builtins=no])
      if test x"$ac_cv_have_builtins" = xno; then
         CXXFLAGS+=" -march=i686"
         AC_LINK_IFELSE(
            [AC_LANG_PROGRAM([],
               [[
                int foo = -10;
                int bar = 10;
                __sync_fetch_and_add(&foo, bar);
               ]]
            )],
            [ac_cv_have_builtins_insist=yes],
            [ac_cv_have_builtins_insist=no])
         if test x"$ac_cv_have_builtins_insist" = xno; then
            AC_MSG_RESULT([no])
            AC_MSG_ERROR([Gcc atomic builtins are necessary to compile this library])
         else
            AC_MSG_RESULT([yes])
         fi
      else
         AC_MSG_RESULT([yes])
      fi
   AC_LANG_POP([C++])
])

