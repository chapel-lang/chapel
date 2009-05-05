
/* $Id: Pvmfgetctx.c,v 1.2 1998/09/21 20:26:00 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfgetcontext (ctx)
#else
__fortran PVMFGETCONTEXT (ctx)
#endif

int *ctx;
{
	*ctx = pvm_getcontext();
}

