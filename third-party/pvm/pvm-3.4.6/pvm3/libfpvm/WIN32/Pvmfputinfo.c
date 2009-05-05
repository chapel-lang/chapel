
/* $Id: Pvmfputinfo.c,v 1.2 1998/09/21 20:27:16 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfputinfo (name_ptr, bufid, flags, mbx_index, name_len)
#else
__fortran PVMFPUTINFO(name_ptr, bufid, flags, mbx_index, name_len)
#endif
char * name_ptr; int name_len;
int *bufid, *flags, *mbx_index;
{

	char tname[MAX_MBOX_NAME + 1];

	/*
	*  Copy the mailbox name to make sure there is
	* a NUL at the end.
	*/
	if (ftocstr(tname, sizeof(tname), name_ptr, name_len)) {
		*mbx_index = PvmBadParam;
		return;
	}

	*mbx_index = pvm_putinfo(tname, *bufid, *flags);
}

