
/* $Id: Pvmfaddmhf.c,v 1.2 1998/09/21 20:25:29 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

void
#ifdef IMA_WIN32_WATCOM
pvmfaddmhf (src, tag, ctx, mid, foo, info)
#else
__fortran PVMFADDMHF (src, tag, ctx, mid, foo, info)
#endif
int (*foo)();
int *src, *tag, *ctx, *mid, *info;
{
	*info = pvm_addmhf(*src, *tag, *ctx, foo);

}

