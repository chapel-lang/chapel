
/* $Id: pvmfnrecv.m4,v 1.2 1996/10/04 15:27:23 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfnrecv) ARGS(`tid, msgtag, info')
int *tid, *msgtag, *info;
{
	*info = pvm_nrecv(*tid, *msgtag);
}

