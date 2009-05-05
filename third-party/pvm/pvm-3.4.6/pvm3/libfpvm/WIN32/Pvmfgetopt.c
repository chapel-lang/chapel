
/* $Id: Pvmfgetopt.c,v 1.2 1998/09/21 20:26:05 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfgetopt (what, val)
#else
__fortran PVMFGETOPT (what, val)
#endif
int *what, *val;
{
    *val = pvm_getopt(*what);
}

