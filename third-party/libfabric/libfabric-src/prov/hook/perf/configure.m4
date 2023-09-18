dnl Configury specific to the libfabrics perf hooking provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl

AC_DEFUN([FI_PERF_CONFIGURE],[
    # Determine if we can support the perf hooking provider
    perf_happy=0
    AS_IF([test x"$enable_perf" != x"no"], [perf_happy=1])
    AS_IF([test $perf_happy -eq 1], [$1], [$2])
])
