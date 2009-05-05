
/* $Id: pvmfmcast.m4,v 1.2 1996/10/04 15:27:17 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfmcast) ARGS(`ntid, tids, msgtag, info')
int *ntid, *tids, *msgtag, *info;
{
	*info = pvm_mcast(tids, *ntid, *msgtag);
}

