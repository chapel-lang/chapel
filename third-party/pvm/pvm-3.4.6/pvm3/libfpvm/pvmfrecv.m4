
/* $Id: pvmfrecv.m4,v 1.2 1996/10/04 15:27:31 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfrecv) ARGS(`tid, msgtag, info')
int *tid, *msgtag, *info;
{
	*info = pvm_recv(*tid, *msgtag);
}

