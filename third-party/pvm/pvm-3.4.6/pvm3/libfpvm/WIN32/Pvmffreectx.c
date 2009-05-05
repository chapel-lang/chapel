
/* $Id: Pvmffreectx.c,v 1.2 1998/09/21 20:25:56 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmffreecontext (ctx,info)
#else
__fortran PVMFFREECONTEXT (ctx, info)
#endif
int *ctx, *info;
{
	*info = pvm_setcontext(*ctx);
}

