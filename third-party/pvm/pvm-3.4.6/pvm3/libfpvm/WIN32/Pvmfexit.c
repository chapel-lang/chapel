
/* $Id: Pvmfexit.c,v 1.2 1998/09/21 20:25:52 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfexit(info)
#else
__fortran PVMFEXIT (info)
#endif

int *info;
{
	*info = pvm_exit();
}
