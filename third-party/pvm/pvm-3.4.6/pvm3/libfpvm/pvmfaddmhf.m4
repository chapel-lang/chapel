
/* $Id: pvmfaddmhf.m4,v 1.5 1997/07/23 14:36:47 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfaddmhf) ARGS(`src, tag, ctx, mid, foo, info')
#if defined(IMA_UXPM) || defined(IMA_UXPV)
int (**foo)(); /* correction for Fujitsu FORTRAN77 EX */
#else
int (*foo)();
#endif
int *src, *tag, *ctx, *mid, *info;
{
#if defined(IMA_UXPM) || defined(IMA_UXPV)
	*info = pvm_addmhf(*src, *tag, *ctx, *foo);
#else
	*info = pvm_addmhf(*src, *tag, *ctx, foo);
#endif

}

