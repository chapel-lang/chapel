
/* $Id: Pvmfprobe.c,v 1.2 1998/09/21 20:27:11 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfprobe (tid, msgtag, info)
#else
__fortran PVMFPROBE (tid, msgtag, info)
#endif

int *tid, *msgtag, *info;
{
   *info = pvm_probe(*tid, *msgtag);
}

