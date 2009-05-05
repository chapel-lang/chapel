
/* $Id: pvmfgetrbuf.m4,v 1.2 1996/10/04 15:27:07 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfgetrbuf) ARGS(`mbuf')
int *mbuf;
{
	*mbuf = pvm_getrbuf();
}

