
/* $Id: Pvmfgetinfo.c,v 1.2 1998/09/21 20:26:02 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif
#include "pvm3.h"
#include "pvm_consts.h"


void 
#ifdef IMA_WIN32_WATCOM
pvmfgetmboxinfo (pattern_ptr,
		name_ptr,
		nclassesp,
		nentriesp,
		indexp,
		ownerp,
		flagp,
		infop, pattern_len, name_len)
#else
__fortran PVMFGETMBOXINFO
	(pattern_ptr,
		name_ptr,
		nclassesp,
		nentriesp,
		indexp,
		ownerp,
		flagp,
		infop, pattern_len, name_len)
#endif
	char * pattern_ptr; int pattern_len;
	char * name_ptr; int name_len;
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
		if ( ftocstr( tpattern, sizeof(tpattern), pattern_ptr,
				pattern_len ) ) {
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
		if ( name_ptr )
			ctofstr( name_ptr, name_len,
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

