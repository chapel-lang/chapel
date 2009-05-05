
/* $Id: Pvmfhalt.c,v 1.2 1998/09/21 20:26:21 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfhalt (info)
#else
__fortran PVMFHALT (info)
#endif
	int *info;
{
	*info = pvm_halt();
}

