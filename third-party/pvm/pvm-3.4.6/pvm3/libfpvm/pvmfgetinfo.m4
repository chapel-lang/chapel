
/* $Id: pvmfgetinfo.m4,v 1.4 1997/05/08 13:32:56 pvmsrc Exp $ */

#include "pvm3.h"
#include "pvm_consts.h"

void
FUNCTION(pvmfgetmboxinfo)
	ARGS(`STRING_ARG(pattern),
		STRING_ARG(name),
		nclassesp,
		nentriesp,
		indexp,
		ownerp,
		flagp,
		infop')

	STRING_ARG_DECL(pattern);
	STRING_ARG_DECL(name);
	int *nclassesp, *nentriesp, *indexp, *ownerp, *flagp, *infop;
{
	static struct pvmmboxinfo *classes = (struct pvmmboxinfo *) 0;
	static int nclasses = 0;
	static int next = 0;

	char tpattern[MAX_MBOX_NAME + 1];
	int nc;
	int cc;
	int i;

/* if user sets nclassesp to -1 then pvmfgetmboxinfo() will be reset */

    if ( nclasses && nclassesp && (*nclassesp == -1) )
		nclasses = 0;

	if ( !nclasses )
	{
		/*
		 *  Copy the pattern to make sure there is
		 *  a NUL at the end.
		 */
		if ( ftocstr( tpattern, sizeof(tpattern), STRING_PTR(pattern),
				STRING_LEN(pattern) ) ) {
			*infop = PvmBadParam;
			return;
		}

		if ( (cc = pvm_getmboxinfo( tpattern, &nc, &classes )) < 0 ) {
			*infop = cc;
			return;
		}

		nclasses = nc;
		next = 0;

		if ( !nclasses )
			*infop = PvmNotFound;
	}

	if ( next < nclasses )
	{
		if ( STRING_PTR(name) )
			ctofstr( STRING_PTR(name), STRING_LEN(name),
					classes[ next ].mi_name );
		if ( nentriesp ) *nentriesp = classes[ next ].mi_nentries;
		if ( indexp ) {
			for ( i=0 ; i < classes[ next ].mi_nentries ; i++ )
				indexp[i] = classes[ next ].mi_indices[i];
		}
		if ( ownerp ) {
			for ( i=0 ; i < classes[ next ].mi_nentries ; i++ )
				ownerp[i] = classes[ next ].mi_owners[i];
		}
		if ( flagp ) {
			for ( i=0 ; i < classes[ next ].mi_nentries ; i++ )
				flagp[i] = classes[ next ].mi_flags[i];
		}
	}

	if ( next == nclasses )
		nclasses = 0;
}

