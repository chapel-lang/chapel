dnl Configury specific to the libfabrics trace hooking provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl

AC_DEFUN([FI_TRACE_CONFIGURE],[
    # Determine if we can support the trace hooking provider
    trace_happy=0
    AS_IF([test x"$enable_trace" != x"no"], [trace_happy=1])
    AS_IF([test $trace_happy -eq 1], [$1], [$2])
])
