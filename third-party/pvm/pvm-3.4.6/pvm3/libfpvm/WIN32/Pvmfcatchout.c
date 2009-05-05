
/* $Id: Pvmfcatchout.c,v 1.2 1998/09/21 20:25:37 pvmsrc Exp $ */

#include <stdio.h>
#ifdef WIN32
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#else 
#include "pvm3.h"
#endif

#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfcatchout (yesno,info)
#else
__fortran PVMFCATCHOUT (yesno, info)
#endif
	int *yesno, *info;
{
	*info = pvm_catchout(*yesno ? stdout : 0);
}

