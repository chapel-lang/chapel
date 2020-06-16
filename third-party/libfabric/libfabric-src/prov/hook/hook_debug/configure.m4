dnl Configury specific to the libfabrics debug hooking provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl

AC_DEFUN([FI_HOOK_DEBUG_CONFIGURE],[
    # Determine if we can support the debug hooking provider
    hook_debug_happy=0
    AS_IF([test x"$enable_hook_debug" != x"no"], [hook_debug_happy=1])
    AS_IF([test x"$hook_debug_dl" == x"1"], [
	hook_debug_happy=0
	AC_MSG_ERROR([debug hooking provider cannot be compiled as DL])
    ])
    AS_IF([test $hook_debug_happy -eq 1], [$1], [$2])

])
