
/* $Id: pvmfcatchout.m4,v 1.2 1996/10/04 15:26:58 pvmsrc Exp $ */

#include <stdio.h>
#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfcatchout) ARGS(`yesno, info')
	int *yesno, *info;
{
	*info = pvm_catchout(*yesno ? stdout : 0);
}

