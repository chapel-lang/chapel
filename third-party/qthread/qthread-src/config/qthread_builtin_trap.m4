AC_DEFUN([QTHREAD_BUILTIN_TRAP],
[AC_CACHE_CHECK([for __builtin_trap],
  [qthread_cv_builtin_trap],
  [AC_LINK_IFELSE([AC_LANG_SOURCE([[
int main(void)
{
	int i=1;
	__builtin_trap();
	return i;
}]])],
	[qthread_cv_builtin_trap="yes"],
	[qthread_cv_builtin_trap="no"])])
 AS_IF([test "x$qthread_cv_builtin_trap" = "xyes"],
 	   [AC_DEFINE([QTHREAD_TRAP_OKAY],[1],[If __builtin_trap can be used])])
])
