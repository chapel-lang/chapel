
/* $Id: pvmfsetrbuf.m4,v 1.2 1996/10/04 15:27:38 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfsetrbuf) ARGS(`mbuf, oldbuf')
int *mbuf, *oldbuf;
{
	*oldbuf = pvm_setrbuf(*mbuf);
}

