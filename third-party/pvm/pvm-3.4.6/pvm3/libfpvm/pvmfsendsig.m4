
/* $Id: pvmfsendsig.m4,v 1.2 1996/10/04 15:27:35 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfsendsig) ARGS(`tid, signum, info')
int *tid, *signum, *info;
{
	*info = pvm_sendsig(*tid, *signum);
}

