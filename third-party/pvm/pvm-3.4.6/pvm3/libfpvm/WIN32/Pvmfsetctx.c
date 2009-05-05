
/* $Id: Pvmfsetctx.c,v 1.2 1998/09/21 20:27:36 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif

#include "pvm3.h"
#include "pvm_consts.h"

void
#ifdef IMA_WIN32_WATCOM 
pvmfsetcontext (new_ctx, old_ctx)
#else
__fortran PVMFSETCONTEXT (new_ctx, old_ctx)
#endif
int *new_ctx, *old_ctx;
{
	*old_ctx = pvm_setcontext(*new_ctx);
}

