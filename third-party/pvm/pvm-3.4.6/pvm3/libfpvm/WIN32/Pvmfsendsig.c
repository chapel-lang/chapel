
/* $Id: Pvmfsendsig.c,v 1.2 1998/09/21 20:27:34 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfsendsig (tid, signum, info)
#else
__fortran PVMFSENDSIG (tid, signum, info)
#endif

int *tid, *signum, *info;
{
   *info = pvm_sendsig(*tid, *signum);
}

