
/* $Id: Pvmfhostsync.c,v 1.2 1998/09/21 20:26:23 pvmsrc Exp $ */

#include <sys/types.h>
#ifndef WIN32
#include <sys/time.h>
#include "pvm3.h"
#else
#include "..\..\include\pvm3.h"
#include "..\..\src\pvmwin.h"
#include <time.h>
#endif


#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfhostsync (host, sec, usec, dsec, dusec, info)
#else
__fortran PVMFHOSTSYNC (host, sec, usec, dsec, dusec, info)
#endif

int *host, *sec, *usec, *dsec, *dusec, *info;
{
	struct timeval t, dt;

	*info = pvm_hostsync(*host, &t, &dt);
	*sec = t.tv_sec;
	*usec = t.tv_usec;
	*dsec = dt.tv_sec;
	*dusec = dt.tv_usec;
}

