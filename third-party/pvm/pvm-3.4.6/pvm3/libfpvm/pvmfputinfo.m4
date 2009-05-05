
/* $Id: pvmfputinfo.m4,v 1.2 1997/04/01 20:58:58 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfputinfo) ARGS(`STRING_ARG(name), bufid, flags, mbx_index')
STRING_ARG_DECL(name);
int *bufid, *flags, *mbx_index;
{

	char tname[MAX_MBOX_NAME + 1];

	/*
	*  Copy the mailbox name to make sure there is
	* a NUL at the end.
	*/
	if (ftocstr(tname, sizeof(tname), STRING_PTR(name), STRING_LEN(name))) {
		*mbx_index = PvmBadParam;
		return;
	}

	*mbx_index = pvm_putinfo(tname, *bufid, *flags);
}

