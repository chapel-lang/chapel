
/* $Id: Pvmfmcast.c,v 1.2 1998/09/21 20:26:46 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfmcast (ntid, tids, msgtag, info)
#else
__fortran PVMFMCAST (ntid, tids, msgtag, info)
#endif

int *ntid, *tids, *msgtag, *info;
{
	*info = pvm_mcast(tids, *ntid, *msgtag);
}

