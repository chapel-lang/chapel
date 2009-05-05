
/* $Id: pvmfrecvinfo.m4,v 1.2 1997/04/01 21:01:57 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfrecvinfo) ARGS(`STRING_ARG(name), mbx_index, flags, bufid')
STRING_ARG_DECL(name);
int *mbx_index, *flags, *bufid;
{

	char tname[MAX_MBOX_NAME + 1];

	/*
	*  Copy the mailbox name to make sure there is
	* a NUL at the end.
	*/
	if (ftocstr(tname, sizeof(tname), STRING_PTR(name), STRING_LEN(name))) {
		*bufid = PvmBadParam;
		return;
	}

	*bufid = pvm_recvinfo(tname, *mbx_index, *flags);
}

