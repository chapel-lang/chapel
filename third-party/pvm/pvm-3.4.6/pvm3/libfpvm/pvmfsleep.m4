
/* $Id: pvmfsleep.m4,v 1.1 1999/03/15 19:03:38 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfsleep) ARGS(`secs')
int *secs;
{
	pvmsleep(*secs);
}

