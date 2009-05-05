
/* $Id: pvmfgetctx.m4,v 1.2 1997/05/13 17:58:59 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfgetcontext) ARGS(`ctx')
int *ctx;
{
	*ctx = pvm_getcontext();
}

