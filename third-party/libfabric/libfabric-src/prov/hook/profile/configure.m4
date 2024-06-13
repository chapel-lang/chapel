dnl Configury specific to the libfabrics profile hooking provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl

AC_DEFUN([FI_PROFILE_CONFIGURE],[
    # Determine if we can support the profile hooking provider
    profile_happy=0
    AS_IF([test x"$enable_profile" != x"no"], [profile_happy=1])
    AS_IF([test $profile_happy -eq 1], [$1], [$2])
])
