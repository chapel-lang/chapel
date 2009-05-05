
/* $Id: pvmfmkbuf.m4,v 1.2 1996/10/04 15:27:18 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfmkbuf) ARGS(`encoding, mbuf')
int *encoding, *mbuf;
{
	*mbuf = pvm_mkbuf(*encoding);
}

