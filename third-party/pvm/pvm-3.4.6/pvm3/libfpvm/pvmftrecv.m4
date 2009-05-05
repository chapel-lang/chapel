
/* $Id: pvmftrecv.m4,v 1.2 1996/10/04 15:27:44 pvmsrc Exp $ */

#include <sys/types.h>
#include <sys/time.h>
#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmftrecv) ARGS(`tid, msgtag, sec, usec, info')
int *tid, *msgtag, *sec, *usec, *info;
{
	struct timeval t;

	t.tv_sec = *sec;
	t.tv_usec = *usec;
	*info = pvm_trecv(*tid, *msgtag, (*sec == -1 ? (struct timeval *)0 : &t));
}

