
/* $Id: pvmfpsend.m4,v 1.2 1996/10/04 15:27:29 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfpsend) ARGS(`tid, msgtag, buf, len, dt, info')
	int *tid, *msgtag, *len, *dt, *info;
	char *buf;
{
	*info = pvm_psend(*tid, *msgtag, buf, *len, *dt);
}

