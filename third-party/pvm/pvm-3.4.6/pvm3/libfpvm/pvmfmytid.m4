
/* $Id: pvmfmytid.m4,v 1.2 1996/10/04 15:27:20 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfmytid) ARGS(`tid')
int *tid;
{
	*tid = pvm_mytid();
}

