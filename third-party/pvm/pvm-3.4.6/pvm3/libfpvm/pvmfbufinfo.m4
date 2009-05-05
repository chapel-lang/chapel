
/* $Id: pvmfbufinfo.m4,v 1.2 1996/10/04 15:26:57 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfbufinfo) ARGS(`mid, len, type, tid, info')
int *mid, *len, *type, *tid, *info;
{
	*info = pvm_bufinfo(*mid, len, type, tid);
}

