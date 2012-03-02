AC_DEFUN([QTHREAD_CHECK_SYSCALLTYPES],[
AS_IF([test "x$1" = xyes],
	  [AC_CHECK_DECLS([SYS_nanosleep,SYS_sleep,SYS_usleep,SYS_system,SYS_select,SYS_wait4,SYS_pread,SYS_connect,SYS_poll,SYS_read,SYS_write,SYS_pwrite],
    [],[],[[#include <sys/syscall.h>]])
AC_CHECK_SIZEOF([socklen_t],[],[[#include <sys/socket.h>]])
AS_IF([test "$ac_cv_sizeof_socklen_t" -eq 4],
	  [socklentype=uint32_t],
	  [AS_IF([test "$ac_cv_sizeof_socklen_t" -eq 8],
	         [socklentype=uint64_t],
			 [AC_MSG_ERROR([socklen_t is an unfortunate size])])])
AC_DEFINE_UNQUOTED([QT_SOCKLENTYPE_T],[$socklentype],[socklen_t compatible uint])
AC_CACHE_CHECK([whether accept() prototype is sane],
               [qt_cv_accept_prototype_sane],
			   [AC_COMPILE_IFELSE([AC_LANG_SOURCE([[
#include <sys/types.h>
#include <sys/socket.h>
int accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len)
{
return 0;
}
]])],
				  [qt_cv_accept_prototype_sane=yes],
				  [qt_cv_accept_prototype_sane=no])])
AS_IF([test "x$qt_cv_accept_prototype_sane" = xyes],
	  [AC_CHECK_DECLS([SYS_accept],[],[],[[#include <sys/syscall.h>]])],
	  [ac_cv_have_decl_SYS_accept=no])])
AM_CONDITIONAL([HAVE_DECL_SYS_ACCEPT], [test "x$ac_cv_have_decl_SYS_accept" == xyes])
AM_CONDITIONAL([HAVE_DECL_SYS_CONNECT], [test "x$ac_cv_have_decl_SYS_connect" == xyes])
AM_CONDITIONAL([HAVE_DECL_SYS_SYSTEM], [test "x$ac_cv_have_decl_SYS_system" == xyes])
AM_CONDITIONAL([HAVE_DECL_SYS_SELECT], [test "x$ac_cv_have_decl_SYS_select" == xyes])
AM_CONDITIONAL([HAVE_DECL_SYS_WAIT4], [test "x$ac_cv_have_decl_SYS_wait4" == xyes])
AM_CONDITIONAL([HAVE_DECL_SYS_PREAD], [test "x$ac_cv_have_decl_SYS_pread" == xyes])
AM_CONDITIONAL([HAVE_DECL_SYS_READ], [test "x$ac_cv_have_decl_SYS_read" == xyes])
AM_CONDITIONAL([HAVE_DECL_SYS_WRITE], [test "x$ac_cv_have_decl_SYS_write" == xyes])
AM_CONDITIONAL([HAVE_DECL_SYS_PWRITE], [test "x$ac_cv_have_decl_SYS_pwrite" == xyes])
AM_CONDITIONAL([HAVE_DECL_SYS_POLL], [test "x$ac_cv_have_decl_SYS_poll" == xyes])
])
