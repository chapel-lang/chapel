
/* $Id: pvmffreebuf.m4,v 1.2 1996/10/04 15:27:02 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmffreebuf) ARGS(`mbuf, info')
int *mbuf, *info;
{
	*info = pvm_freebuf(*mbuf);
}

