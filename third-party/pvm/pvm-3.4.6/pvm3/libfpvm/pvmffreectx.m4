
/* $Id: pvmffreectx.m4,v 1.2 1997/05/13 17:58:58 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmffreecontext) ARGS(`ctx, info')
int *ctx, *info;
{
	*info = pvm_setcontext(*ctx);
}

