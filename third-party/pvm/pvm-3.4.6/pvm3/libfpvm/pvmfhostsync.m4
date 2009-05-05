
/* $Id: pvmfhostsync.m4,v 1.2 1996/10/04 15:27:12 pvmsrc Exp $ */

#include <sys/types.h>
#include <sys/time.h>
#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfhostsync) ARGS(`host, sec, usec, dsec, dusec, info')
int *host, *sec, *usec, *dsec, *dusec, *info;
{
	struct timeval t, dt;

	*info = pvm_hostsync(*host, &t, &dt);
	*sec = t.tv_sec;
	*usec = t.tv_usec;
	*dsec = dt.tv_sec;
	*dusec = dt.tv_usec;
}

