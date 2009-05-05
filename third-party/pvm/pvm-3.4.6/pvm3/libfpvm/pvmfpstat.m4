
/* $Id: pvmfpstat.m4,v 1.2 1996/10/04 15:27:30 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfpstat) ARGS(`tid, pstat')
int *tid, *pstat;
{
	*pstat = pvm_pstat(*tid);
}

