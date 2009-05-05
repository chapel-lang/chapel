
/* $Id: pvmfinitsend.m4,v 1.2 1996/10/04 15:27:13 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfinitsend) ARGS(`tid, info')
int *tid, *info;
{
	*info = pvm_initsend(*tid);
}

