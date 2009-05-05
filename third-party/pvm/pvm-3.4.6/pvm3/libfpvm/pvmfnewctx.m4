
/* $Id: pvmfnewctx.m4,v 1.2 1997/05/13 17:59:01 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfnewcontext) ARGS(`ctx')
int *ctx;
{
	*ctx = pvm_newcontext();
}

