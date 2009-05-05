
/* $Id: Pvmftidtoh.c,v 1.2 1998/09/21 20:27:49 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmftidtohost (tid,dtid)
#else
__fortran PVMFTIDTOHOST (tid, dtid)
#endif

int *dtid, *tid;
{
    *dtid = pvm_tidtohost(*tid);
}

