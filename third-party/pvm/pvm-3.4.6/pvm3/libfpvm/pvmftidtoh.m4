
/* $Id: pvmftidtoh.m4,v 1.2 1996/10/04 15:27:43 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmftidtohost) ARGS(`tid, dtid')
int *dtid, *tid;
{
    *dtid = pvm_tidtohost(*tid);
}

