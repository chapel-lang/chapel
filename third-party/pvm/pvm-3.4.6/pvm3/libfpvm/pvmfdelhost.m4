
/* $Id: pvmfdelhost.m4,v 1.2 1996/10/04 15:27:00 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfdelhost) ARGS(`STRING_ARG(host), info')
STRING_ARG_DECL(host);
int *info;
{
	char thost[MAX_HOST_NAME + 1];
	char *ptr = thost;
	int cc;

	/*
	 * Copy the host name to make sure there's
	 * a NUL at the end.
	 */
	if (ftocstr(thost, sizeof(thost), STRING_PTR(host), STRING_LEN(host))) {
		*info = PvmBadParam;
		return;
	}

	*info = pvm_delhosts(&ptr, 1, &cc);
	if (*info >= 0)
		*info = cc;
}

