
/* $Id: pvmfaddhost.m4,v 1.2 1996/10/04 15:26:53 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfaddhost) ARGS(`STRING_ARG(host), info')
STRING_ARG_DECL(host);
int *info;
{
	int  dtid;
	char thost[MAX_HOST_NAME + 1];
	char *ptr = thost;

	/*
	 * Copy the host name to make sure there's
	 * a NUL at the end.
	 */
	if (ftocstr(thost, sizeof(thost), STRING_PTR(host), STRING_LEN(host))) {
		*info = PvmBadParam;
		return;
	}

	*info = pvm_addhosts(&ptr, 1, &dtid);
	if (*info >= 0)
		*info = dtid;
}

