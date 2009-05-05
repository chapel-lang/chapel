
/* $Id: pvmfgetsbuf.m4,v 1.2 1996/10/04 15:27:08 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfgetsbuf) ARGS(`mbuf')
int *mbuf;
{
	*mbuf = pvm_getsbuf();
}

