
/* $Id: pvmfparent.m4,v 1.2 1996/10/04 15:27:25 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfparent) ARGS(`tid')
int *tid;
{
	*tid = pvm_parent();
}

