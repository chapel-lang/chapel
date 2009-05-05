
/* $Id: Pvmfrecv.c,v 1.2 1998/09/21 20:27:18 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfrecv (tid, msgtag, info)
#else
__fortran PVMFRECV (tid, msgtag, info)
#endif
int *tid, *msgtag, *info;
{
   *info = pvm_recv(*tid, *msgtag);
}

