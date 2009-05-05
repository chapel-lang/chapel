
/* $Id: pvmfexit.m4,v 1.2 1996/10/04 15:27:01 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfexit) ARGS(`info')
	int *info;
{
	*info = pvm_exit();
}

