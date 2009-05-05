
/* $Id: pvmfdelinfo.m4,v 1.1 1997/04/01 20:14:53 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfdelinfo) ARGS(`STRING_ARG(name), mbx_index, flags, info')
STRING_ARG_DECL(name);
int *mbx_index, *flags, *info;
{

	char tname[MAX_MBOX_NAME + 1];

	/*
	*  Copy the mailbox name to make sure there is
	* a NUL at the end.
	*/
	if (ftocstr(tname, sizeof(tname), STRING_PTR(name), STRING_LEN(name))) {
		*info = PvmBadParam;
		return;
	}

	*info = pvm_delinfo(tname, *mbx_index, *flags);
}

