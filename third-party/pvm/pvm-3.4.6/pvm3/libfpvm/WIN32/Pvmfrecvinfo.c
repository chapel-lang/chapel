
/* $Id: Pvmfrecvinfo.c,v 1.2 1998/09/21 20:27:19 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfrecvinfo (name_ptr, mbx_index, flags, bufid, name_len)
#else
__fortran PVMFRECVINFO (name_ptr, mbx_index, flags, bufid, name_len)
#endif

char * name_ptr; int name_len;
int *mbx_index, *flags, *bufid;
{

	char tname[MAX_MBOX_NAME + 1];

	/*
	*  Copy the mailbox name to make sure there is
	* a NUL at the end.
	*/
	if (ftocstr(tname, sizeof(tname), name_ptr, name_len)) {
		*bufid = PvmBadParam;
		return;
	}

	*bufid = pvm_recvinfo(tname, *mbx_index, *flags);
}

