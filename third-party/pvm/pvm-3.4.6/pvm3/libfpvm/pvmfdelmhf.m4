
/* $Id: pvmfdelmhf.m4,v 1.2 1997/05/13 17:58:56 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfdelmhf) ARGS(`mhid, info')
int *mhid, *info;
{
	*info = pvm_delmhf(*mhid);
}

