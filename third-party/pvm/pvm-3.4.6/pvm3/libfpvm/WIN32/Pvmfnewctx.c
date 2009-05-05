
/* $Id: Pvmfnewctx.c,v 1.2 1998/09/21 20:26:53 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif

#include "pvm3.h"
#include "pvm_consts.h"

void
#ifdef IMA_WIN32_WATCOM
pvmfnewcontext(ctx)
#else
__fortran PVMFNEWCONTEXT(ctx)
#endif
int *ctx;
{
	*ctx = pvm_newcontext();
}

