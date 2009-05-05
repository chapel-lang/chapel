
/* $Id: pvmfsetsbuf.m4,v 1.2 1996/10/04 15:27:39 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfsetsbuf) ARGS(`mbuf, oldbuf')
int *mbuf, *oldbuf;
{
	*oldbuf = pvm_setsbuf(*mbuf);
}

