
/* $Id: pvmfmstat.m4,v 1.2 1996/10/04 15:27:19 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfmstat) ARGS(`STRING_ARG(host), mstat')
STRING_ARG_DECL(host);
int *mstat;
{
	char thost[MAX_HOST_NAME + 1];

	/*
	 * Copy the host name to make sure there's
	 * a NUL at the end.
	 */
	if (ftocstr(thost, sizeof(thost), STRING_PTR(host), STRING_LEN(host))) {
		*mstat = PvmBadParam;
		return;
	}

	*mstat = pvm_mstat(thost);
}

