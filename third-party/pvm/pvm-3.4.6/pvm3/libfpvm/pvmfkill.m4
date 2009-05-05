
/* $Id: pvmfkill.m4,v 1.2 1996/10/04 15:27:15 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfkill) ARGS(`tid, info')
int *tid, *info;
{
	*info = pvm_kill(*tid);
}

