
/* $Id: Pvmfstartpvmd.c,v 1.2 1998/09/21 20:27:45 pvmsrc Exp $ */

#ifdef WIN32
#include "..\..\src\pvmwin.h"
#endif

#include <stdio.h>
#include "pvm3.h"
#include "pvm_consts.h"
#include "pvmalloc.h"

void 
#ifdef IMA_WIN32_WATCOM
__fortran PVMFSTARTPVMD 
#else
pvmfstartpvmd
#endif

(args_ptr, block, info, args_len)
char * args_ptr; int args_len;
int *block;
int *info;
{
    char *nargs;			/* args (null terminated) */
    int ac = 0;				/* # of agruments initialize to 0 */
    char **av;				/* argument vector */
    register char *ch;			/* temp char pointer */
    register char *beg;			/* pointer to begining of word */
    register char *end;			/* pointer to end of word */
    register int len;			/* length of word */
    register int i;			/* loop index */

#if ( DEBUG )
    /* printout incoming nargs */
    printf( "%d \"%s\"\n", args_len, args_ptr );
#endif
    /* Some Fortran compilers allow for strings of zero length */
    if ( args_len == 0 ) {	/* null args */
	av = (char **)0;		/* make av a null pointer */
	goto pvmd;
    } else {				/* something in args */
	if ( ( nargs = TALLOC( args_len + 1, char, "nargs" ) )
	    == NULL ) {
	    pvmlogerror("pvmfstartpvmd() can't get memory\n");
	    goto bail;
	}
	/* copy args (ftocptr() could have been used for that) */
	strncpy( nargs, args_ptr, args_len );
	nargs[args_len] = '\0'; /* terminate with null */
    }
    ch = nargs;				/* pointer at the begining of nargs */
    /* assume a max of 32 args (8 should be OK from pvmd3(3PVM)) */
    if ( ( av = TALLOC( 32, char *, "av" ) ) == NULL ) {
	pvmlogerror("pvmfstartpvmd() can't get memory\n");
	PVM_FREE( nargs );		/* avoid memory leeks */
	goto bail;
    }
    /* at this point nargs contain at least 1 char */
    for ( ; ; ) {
	/* move forward until no space nor tab */
	for( ; *ch == ' ' || *ch == '\t'; ch++ );
	beg = ch;			/* that's the begining of a word */
	/* move forward until space, tab or null */
	for( ; *ch != ' ' && *ch != '\t' && *ch != '\0'; ch++ );
	end = ch;			/* that's the end of a word */
	len = end - beg;		/* length of the word */
	/* if nothing else then space, tab or null has been found */
	/* then end of nargs is reached (exit point of loop) */
	if ( len == 0 ) break;
	/* allocate memory for word in agument vector */
	if ( ( av[ac] = TALLOC( len + 1, char, "av[ac]" ) ) == NULL ) {
	    pvmlogerror("pvmfstartpvmd() can't get memory\n");
	    PVM_FREE( nargs );		/* avoid memory leeks */
	    for ( i = 0; i < ac; i++ )
		PVM_FREE( av[i] );
	    goto bail;
	}
	strncpy( av[ac], beg, len );	/* copy word to arg vector */
	*( av[ac] + len ) = '\0';	/* null terminate word */
	ac++;				/* increment arg counter */
    }
#if ( DEBUG )
    printf( "ac is %d\n", ac );
#endif
    if ( ac == 0 ) {
	/* no args found */
	PVM_FREE( av );			/* free allocated memory */
	av = (char **)0;		/* make av a NULL pointer */
#if ( DEBUG )
    } else {
	/* printout result */
	for ( i = 0; i < ac; i++ )
	    printf( "av[%2d] is \"%s\"\n", i, av[i] );
#endif
    }
    PVM_FREE( nargs );			/* free allocated memory */
 pvmd:
    /* hopefully pvm_start_pvmd is freeing allocated mem for av */
    *info = pvm_start_pvmd( ac, av, *block );
    return;
 bail:
    *info = -1;
    return;
}

/*
 *----------------------------------------------------------------------
 * RCS identification
 *----------------------------------------------------------------------
 * $Author: pvmsrc $
 * $Date: 1998/09/21 20:27:45 $
 * $Locker:  $
 * $Revision: 1.2 $
 * $Source: /home/nfs/pvmsrc/.CVS/PVM/pvm3.4/libfpvm/WIN32/Pvmfstartpvmd.c,v $
 * $State: Exp $
 *----------------------------------------------------------------------
 * For GNU Emacs:
 *----------------------------------------------------------------------
 * Local Variables:
 * mode: C
 * abbrev-mode: t
 * comment-column: 40
 * version-control: t
 * End:
 *----------------------------------------------------------------------
 */
