dnl Configury specific to the libfabrics ucx provider

dnl Called to configure this provider
dnl
dnl Arguments:
dnl
dnl $1: action if configured successfully
dnl $2: action if not configured successfully
dnl
AC_DEFUN([FI_UCX_CONFIGURE],[
    # Determine if we can support the ucx provider
    ucx_happy=0
    AS_IF([test x"$enable_ucx" = x"yes"],
              [FI_CHECK_PACKAGE([ucx],
                    [ucp/api/ucp.h],
                    [ucp],
                    [ucp_get_version_string],
                    [-luct -lucm -lucs],
                    [$ucx_PREFIX],
                    [$ucx_LIBDIR],
                    [ucx_happy=1],
                    [ucx_happy=0])
	       AC_CHECK_DECLS([UCP_WORKER_FLAG_IGNORE_REQUEST_LEAK],
		    [],
		    [],
		    [#include <ucp/api/ucp.h>])
         ])
    AS_IF([test $ucx_happy -eq 1], [$1], [$2])
])

