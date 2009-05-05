
/* $Id: pvmfbcast.m4,v 1.2 1996/10/04 15:26:56 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfbcast) ARGS(`STRING_ARG(group), msgtag, info')
STRING_ARG_DECL(group);
int *msgtag, *info;
{
	char tgroup[MAX_GRP_NAME + 1];

	/*
	 * Copy the group name to make sure there's
	 * a NUL at the end.
	 */
	if (ftocstr(tgroup, sizeof(tgroup), STRING_PTR(group), STRING_LEN(group))){
		*info = PvmBadParam;
		return;
	}

	*info = pvm_bcast(tgroup, *msgtag);
}

