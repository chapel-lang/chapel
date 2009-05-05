
/* $Id: pvmfprecv.m4,v 1.2 1996/10/04 15:27:27 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfprecv) ARGS(`tid, msgtag, buf, len, dt, atid, atag, alen, info')
	int *tid, *msgtag, *len, *dt, *atid, *atag, *alen, *info;
	char *buf;
{
	*info = pvm_precv(*tid, *msgtag, buf, *len, *dt, atid, atag, alen);
}

