
static char rcsid[] =
	"$Id: ptest.c,v 1.3 2009/01/23 01:12:52 pvmsrc Exp $";

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

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include "pvm3.h"
#define GRP "group"
#define DSIZE 10000

extern int pvmdebmask;

main(argc, argv)
int argc;
char **argv;
{

int i;
int j;
int k;
int l;
int mytid;
int mynod;
int niter = 8;
int ntask = 4;
int pnod;
int mydbmask = 0;
int myiter = 1;
double tstarray[DSIZE];

	mytid = pvm_mytid();

	if (argc > 1)
		ntask = atoi(argv[1]);
	else
	{
		printf("usage: ptest <nprocs> [debugmask] [niter] [sync/iteration]");
		exit (-1);
	}	


	if (argc > 2)
		mydbmask = atoi(argv[2]);

	if (argc > 3)
		niter = atoi(argv[3]);

	if (argc > 4)
		myiter = atoi(argv[4]);

	
	printf("%x understands %d processes\n", mytid, ntask); 

	mynod = pvm_joingroup(GRP);


	if (mynod == 0) 
		pvmdebmask= mydbmask;

	pvm_freezegroup(GRP,ntask);

	pnod = pvm_gettid(GRP,0);



	for (i = 0; i < niter; i ++)
	{
 		for (k = 0; k < DSIZE; k ++)
			tstarray[k] = 2*k + mynod + (i*DSIZE);
	
		if (mynod == 0)
		{
			printf("Receiving data iter %d ...",i);
	
			if ( i > niter/2)
				printf(" (pvm_recv) ");
			else
				printf(" (pvm_precv) ");

			printf("\n");
			/* i_dump(1); */
			for (j = 1; j < ntask; j ++)
			{
				if ( i > niter/2)
				{
					pvm_recv(pvm_gettid(GRP,j),0);
					pvm_upkdouble(tstarray,DSIZE,1);
				}
				else
				{	
					pvm_precv(pvm_gettid(GRP,j), 0, tstarray, DSIZE, 
					PVM_DOUBLE, (int*)0, (int*)0, (int*)0);
				}
				for (k = 0; k < DSIZE; k++)
					if( tstarray[k] != (2*k + j + i*DSIZE) )
					{
						printf("Array match failed on iter %d, task %d index %d\n",	i, j, k);
						printf("Data should have been %f and was %f \n",
							(double)(2*k + j + i*DSIZE), tstarray[k]);
						printf("Next 10 values are :\n");
						l = k;
						for ( k = l; k < l + 10; k++)
						printf("%d should have been %f was  %f \n",k,
							(double)(2*k + j + i*DSIZE), tstarray[k]);
						exit(-1);
					}
			}
			printf("Data validated from all tasks! \n");
			if ( (i % myiter) == 0)
			{
				pvm_initsend(PvmDataDefault);
				pvm_pkint(&i,1,1);
				for (l =1; l < ntask; l++)
					pvm_send(pvm_gettid(GRP,l),10);
			}
		}
		else
		{
			pvm_psend(pnod, 0, tstarray, DSIZE, PVM_DOUBLE);
			if ( (i % myiter ) == 0)
			{
				pvm_recv(pnod,10);
				pvm_upkint(&k,1,1);

				if (k != i)
				{
					printf("%x received iteration count %d instead of %d \n",
					mytid, k, i);
					exit(-1);
				}
			}
		}
	}
	pvm_lvgroup(GRP);
	pvm_exit();
}

