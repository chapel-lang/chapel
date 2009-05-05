
/* $Id: pvmfprobe.m4,v 1.2 1996/10/04 15:27:28 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfprobe) ARGS(`tid, msgtag, info')
int *tid, *msgtag, *info;
{
	*info = pvm_probe(*tid, *msgtag);
}

