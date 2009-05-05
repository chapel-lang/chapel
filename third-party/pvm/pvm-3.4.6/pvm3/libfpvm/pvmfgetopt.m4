
/* $Id: pvmfgetopt.m4,v 1.2 1996/10/04 15:27:06 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfgetopt) ARGS(`what, val')
int *what, *val;
{
    *val = pvm_getopt(*what);
}

