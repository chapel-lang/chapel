
/* $Id: pvmfnotify.m4,v 1.2 1996/10/04 15:27:21 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfnotify) ARGS(`about, msgtag, ntid, tids, info')
int *about, *ntid, *tids, *msgtag, *info;
{
	*info = pvm_notify(*about, *msgtag, *ntid, tids);
}

