
/* $Id: Pvmfsiblings.c,v 1.4 2002/06/14 13:52:17 pvmsrc Exp $ */

#include <stdio.h>
#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"

void 
#ifdef IMA_WIN32_WATCOM
pvmfsiblings (nsiblings, sibidx, stid)
#else
__fortran PVMFSIBLINGS (nsiblings, sibidx, stid)
#endif

int *nsiblings, *sibidx, *stid;
{
	static int nsib = -1;
	static int *sibs;
	if (nsib == -1)
		nsib = pvm_siblings(&sibs);

	if ( nsiblings != NULL )
		*nsiblings = nsib;

	if ( sibidx != NULL && stid != NULL )
	{
		if (*sibidx >= 0 && *sibidx< nsib)
			*stid = sibs[*sibidx];
		else
			*stid = PvmNoTask;
	}
}

