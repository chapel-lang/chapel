
/* $Id: pvmfstartpvmd.m4,v 1.3 2007/04/19 23:15:22 pvmsrc Exp $ */

/*
 *----------------------------------------------------------------------
 * Time-stamp: "1995/11/21 13:31:32 thibaud@kether.cgd.ucar.edu"
 *----------------------------------------------------------------------
 */
/*
 *----------------------------------------------------------------------
 * pvmfstartpvmd.m4
 *
 * Interface to Fortran for pvm_start_pvmd()
 *----------------------------------------------------------------------
 */

#include <stdio.h>
#include "pvm3.h"
#include "pvm_consts.h"
#include "pvmalloc.h"

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

void
FUNCTION(pvmfstartpvmd) ARGS(`STRING_ARG(args), block, info')
STRING_ARG_DECL(args);
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
    printf( "%d \"%s\"\n", STRING_LEN(args), STRING_PTR(args) );
#endif
    /* Some Fortran compilers allow for strings of zero length */
    if ( STRING_LEN(args) == 0 ) {	/* null args */
	av = (char **)0;		/* make av a null pointer */
	goto pvmd;
    } else {				/* something in args */
	if ( ( nargs = TALLOC( STRING_LEN(args) + 1, char, "nargs" ) )
	    == NULL ) {
	    pvmlogerror("pvmfstartpvmd() can't get memory\n");
	    goto bail;
	}
	/* copy args (ftocptr() could have been used for that) */
	strncpy( nargs, STRING_PTR(args), STRING_LEN(args) );
	nargs[STRING_LEN(args)] = '\0'; /* terminate with null */
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
 * $Date: 2007/04/19 23:15:22 $
 * $Locker:  $
 * $Revision: 1.3 $
 * $Source: /home/nfs/pvmsrc/.CVS/PVM/pvm3.4/libfpvm/pvmfstartpvmd.m4,v $
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
