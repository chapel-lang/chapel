
/* $Id: Pvmfdelinfo.c,v 1.2 1998/09/21 20:25:42 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif

#include "pvm3.h"
#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfdelinfo (name_ptr, mbx_index, flags, info, name_len)
#else
__fortran PVMFDELINFO (name_ptr, mbx_index, flags, info, name_len)
#endif

char * name_ptr; int name_len;
int *mbx_index, *flags, *info;
{

	char tname[MAX_MBOX_NAME + 1];

	/*
	*  Copy the mailbox name to make sure there is
	* a NUL at the end.
	*/
	if (ftocstr(tname, sizeof(tname), name_ptr, name_len)) {
		*info = PvmBadParam;
		return;
	}

	*info = pvm_delinfo(tname, *mbx_index, *flags);
}

