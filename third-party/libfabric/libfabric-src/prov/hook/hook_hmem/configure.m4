dnl Configury specific to the libfabrics hmem hooking provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl

AC_DEFUN([FI_HOOK_HMEM_CONFIGURE],[
    # Determine if we can support the hmem hooking provider
    hook_hmem_happy=0
    AS_IF([test x"$enable_hook_hmem" != x"no"], [hook_hmem_happy=1])
    AS_IF([test $hook_hmem_happy -eq 1], [$1], [$2])
])
