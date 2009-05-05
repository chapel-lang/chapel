
/* $Id: Pvmfpstat.c,v 1.2 1998/09/21 20:27:14 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfpstat (tid, pstat)
#else
__fortran PVMFPSTAT (tid, pstat)
#endif
int *tid, *pstat;
{
   *pstat = pvm_pstat(*tid);
}

