
/* $Id: pvmfsetctx.m4,v 1.2 1997/05/13 17:59:02 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfsetcontext) ARGS(`new_ctx, old_ctx')
int *new_ctx, *old_ctx;
{
	*old_ctx = pvm_setcontext(*new_ctx);
}

