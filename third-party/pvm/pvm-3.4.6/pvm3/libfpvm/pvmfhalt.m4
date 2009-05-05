
/* $Id: pvmfhalt.m4,v 1.2 1996/10/04 15:27:11 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfhalt) ARGS(`info')
	int *info;
{
	*info = pvm_halt();
}

