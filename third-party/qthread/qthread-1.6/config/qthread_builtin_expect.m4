AC_DEFUN([QTHREAD_BUILTIN_EXPECT],
[AC_CACHE_CHECK([for __builtin_expect],
  [qthread_cv_builtin_expect],
  [SAVE_CFLAGS="$CFLAGS"
   CFLAGS="-Werror $CFLAGS"
   AC_LINK_IFELSE([AC_LANG_SOURCE([[
int main()
{
	int i;
	if (__builtin_expect(i==0, 0)) { return 0; }
	return 1;
}]])],
	[qthread_cv_builtin_expect="yes"],
	[qthread_cv_builtin_expect="no"])
   CFLAGS="$SAVE_CFLAGS"])
 AS_IF([test "x$qthread_cv_builtin_expect" = "xyes"],
 	   [AC_DEFINE([QTHREAD_EXPECT_OKAY],[1],[If __builtin_expect can be used])])
])
