
/* $Id: Pvmfmytid.c,v 1.2 1998/09/21 20:26:51 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"
#ifdef IMA_WIN32_WATCOM
void pvmfmytid (tid)
#else
__fortran PVMFMYTID (tid)
#endif

int *tid;
{
	*tid = pvm_mytid();
}
