
static char rcsid[] =
	"$Id: spmd.c,v 1.5 2009/01/23 01:12:52 pvmsrc Exp $";

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

/*
*    SPMD example using PVM 3
*    Uses pvm_siblings() to determine number of tasks that were spawned
*    together
*/

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include <sys/types.h>
#include "pvm3.h"
#define MAXNPROC 32

void dowork();

main()
{
    int mytid;                  /* my task id */
    int *tids;                  /* array of task ids */
    int me;                     /* my process number */
    int i;
	int ntids;

    /* enroll in pvm */
    mytid = pvm_mytid();

	/* determine the size of my sibling list */

	ntids = pvm_siblings(&tids);

	for (i = 0; i < ntids; i ++)
		if ( tids[i] == mytid)
		{
			me = i;
			break;
		}
		
	if (me == 0)
	{
		printf("Pass a token through the %3d tid ring:\n", ntids);
		for (i = 0; i < ntids; i ++)
		{
			printf( "%6d -> ", tids[i]);
			if (i % 6 == 0 && i > 0)
				printf("\n");	
		}
		printf("%6d \n", tids[0]);
	}
/*--------------------------------------------------------------------------*/
     
     dowork( me, ntids, tids );

     /* program finished exit pvm */
     pvm_exit();
     exit(1);
}

/* Simple example passes a token around a ring */

void
dowork( me, nproc, tids )
     int me;
     int nproc;
	 int tids[];
{
     int token;
     int src, dest;
     int count  = 1;
     int stride = 1;
     int msgtag = 4;

     /* Determine neighbors in the ring */
     if ( me == 0 )
		src = tids[nproc -1];
	 else
 		src = tids[me -1];

     if (me == nproc - 1)
        dest = tids[0];
     else
        dest = tids[me + 1];

     if( me == 0 )
     { 
        token = dest;
        pvm_initsend( PvmDataDefault );
        pvm_pkint( &token, count, stride );
        pvm_send( dest, msgtag );
        pvm_recv( src, msgtag );
        printf("token ring done\n");
     }
     else
     {
        pvm_recv( src, msgtag );
        pvm_upkint( &token, count, stride );
        pvm_initsend( PvmDataDefault );
        pvm_pkint( &token, count, stride );
        pvm_send( dest, msgtag );
     }
}
