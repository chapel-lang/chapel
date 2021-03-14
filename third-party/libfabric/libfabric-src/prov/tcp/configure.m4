dnl Configury specific to the libfabric tcp provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_TCP_CONFIGURE],[
       # Determine if we can support the tcp provider
       tcp_h_happy=0
       AS_IF([test x"$enable_tcp" != x"no"], [tcp_h_happy=1])
       AS_IF([test $tcp_h_happy -eq 1], [$1], [$2])
])
