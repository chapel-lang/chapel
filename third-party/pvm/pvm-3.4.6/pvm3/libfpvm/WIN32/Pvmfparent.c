
/* $Id: Pvmfparent.c,v 1.2 1998/09/21 20:26:59 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfparent (tid)
#else
__fortran PVMFPARENT (tid)
#endif

int *tid;
{
	*tid = pvm_parent();
}

