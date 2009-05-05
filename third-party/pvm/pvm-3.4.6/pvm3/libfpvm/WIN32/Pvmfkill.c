
/* $Id: Pvmfkill.c,v 1.2 1998/09/21 20:26:28 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfkill (tid, info)
#else
__fortran PVMFKILL (tid, info)
#endif
int *tid, *info;
{
	*info = pvm_kill(*tid);
}

