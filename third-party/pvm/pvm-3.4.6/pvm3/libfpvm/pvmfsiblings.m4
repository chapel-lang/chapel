
/* $Id: pvmfsiblings.m4,v 1.3 2002/06/14 13:52:03 pvmsrc Exp $ */

#include <stdio.h>
#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfsiblings) ARGS(`nsiblings, sibidx, stid')
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

