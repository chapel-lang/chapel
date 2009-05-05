
/* $Id: pvmfsetopt.m4,v 1.2 1996/10/04 15:27:37 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfsetopt) ARGS(`what, val, info')
int *what, *val, *info;
{
	*info = pvm_setopt(*what, *val);
}

