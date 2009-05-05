
static char rcsid[] =
	"$Id: gexample.c,v 1.4 1998/11/20 19:46:47 pvmsrc Exp $";

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

/* Example of some group function and reduction functions in PVM 
 *
 * 11 March 1994 - Creation by P. Papadopoulos (phil@msr.epm.ornl.gov)
 * 27 June 1997 - updated to allow spmd-style startups for PVM 3.4
 *
 *
*/
#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif

#include "pvm3.h"

#ifdef WIN32
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

#define min(u,v) ( (u) < (v) ? (u) : (v) )
#define max(u,v) ( (u) > (v) ? (u) : (v) )

#define MATRIXGROUP "matrix"
#define DEFAULT_DIMENSION 100
#define DEFAULT_NPROC     10
#define INITTAG 1000
#define SUMTAG INITTAG + 1
#define PRODTAG SUMTAG + 1
extern void calcprod();
main()
{
int info, mytid, myinst, gsize, nproc = 0;
int maxmax;
int dimension = 0;
int ninst, error;
int tids[32]; int *subblock, *colsum;
double *colprod;
int blksize,nextra,mysrow,i,j,sumsqr,itemp;

int *stids;
int spmd = 0;	/* flag to indicate we were started in a spmd-style */
int nsibs; 


	mytid = pvm_mytid(); /* enroll */

	if ((nsibs = pvm_siblings( &stids )) > 1) /* nsibs spawned together */
		spmd = 1;

	if( (myinst = pvm_joingroup(MATRIXGROUP)) < 0 )
	{
		pvm_perror( "Could not join group \n" );
		pvm_exit();
		exit( -1 );
	}
	
	if ( myinst == 0 )              
	{
		printf(" This program demonstrates some group and reduction  \n");
		printf(" operations in PVM.  The output displays the \n");
		printf(" the product of the first column of a Toeplitz matrix\n");
		printf(" and the matrix 1-norm. The matrix data is distributed \n");
		printf(" among several processors.  The Toeplitz matrix is \n"); 
		printf(" symmetric with the first row being the row \n");
		printf(" vector [1 2 ... n].\n");
		if ( !spmd && pvm_parent() == PvmNoParent )
		{
			while ( nproc <= 0 || nproc > 32 || dimension <= 0)
			{
				printf( " Input dimension ( >0 ) of matrix: " );
				scanf( "%d", &dimension );
				printf( " Input number of tasks (1-32): " );
				scanf( "%d", &nproc );
			}
		}
		else
		{
			nproc = DEFAULT_NPROC;
			dimension = DEFAULT_DIMENSION;
		}

		if (!spmd && nproc > 1) 
		{
			ninst = pvm_spawn( "gexample", (char **) 0, 0, "",  
					nproc-1, tids);  
			nproc = min (nproc, ninst + 1); 
		}

		if (spmd) 
			nproc = nsibs;

		pvm_initsend( PvmDataDefault );
		pvm_pkint( &nproc, 1, 1 );
		pvm_pkint( &dimension, 1, 1 );
		if (spmd)
			pvm_mcast(stids, nsibs, INITTAG);
		else
			pvm_mcast( tids, nproc - 1, INITTAG ); 

		printf("\n       --> Using %d processors <-- \n\n", nproc);
	}

	else /* other nodes receive the number of processors from  0 */
	{
		pvm_recv( -1, INITTAG );
		pvm_upkint( &nproc, 1, 1 );
		pvm_upkint( &dimension, 1, 1);
	}

	/* Make the group static. freezegroup will wait until nproc tids have
		joined the group.                  */

	info = pvm_freezegroup(MATRIXGROUP,nproc);

	/* Map matrix rows to processors --      */ 

	blksize =  dimension/nproc ;
	nextra =   dimension % nproc; 

	if( myinst < nextra ) 
	{
		mysrow = (blksize + 1) * myinst;  
		blksize++; 
	}
	else
		mysrow = (blksize + 1)*(nextra) + blksize*(myinst - nextra);

	subblock = (int *) calloc(blksize*dimension,sizeof(int)); 
	colsum   = (int *) calloc(dimension,sizeof(int));
	colprod  = (double *) calloc(dimension,sizeof(double));

	if (mysrow >= dimension)    /* too many processors ! */
		blksize = 0;

	/* Assign data to this subblock.  The entries below make the entire matrix
		a symmetric Toeplitz matrix (i.e. diagonals are of constant value)  */

	for (i = 0; i < blksize; i++)
		for (j = 0; j < dimension; j++)
			*(subblock + i*dimension + j) = 1 + abs(mysrow + i - j); 

	/* Locally compute the sum of each column and put into colsum */
	for (j = 0; j < dimension; j++)
	{
		colsum[j] = 0;
		colprod[j] = 1.0;
	}

	for (i = 0; i < blksize; i ++)
		for(j = 0; j < dimension; j++)
		{
			itemp = *(subblock + j + i*dimension);
			colsum[j] += abs( itemp );
			colprod[j] *= abs( itemp );
		}

	/* synchronize the computation and then reduce using pvm_sum. This gives
		a row vector that has the all the columns sums  */

	if ( pvm_reduce(PvmSum,colsum,dimension,PVM_INT,SUMTAG,
			MATRIXGROUP,0) < 0  )
		pvm_perror( "pvm_reduce had an error \n") ; 

	pvm_reduce(calcprod,colprod,dimension,PVM_DOUBLE,PRODTAG,
		MATRIXGROUP,0); 

	if( myinst == 0)
	{
		maxmax = 0;
		for (j = 0; j < dimension; j++)
			maxmax = max(colsum[j],maxmax);

		printf(" The 1-Norm is %d  \n" , maxmax );
		printf(" ( Should be the sum of the integers from 1 to %d )\n", 
				dimension );
		printf(" The product of column 1 is %g  \n" , colprod[0] );
		printf(" ( Should be %d factorial)\n", dimension);
	}

	info = pvm_barrier(MATRIXGROUP,-1);  /* make sure processes are finished*/
	if (info < 0) 
		printf("Barrier failed with result code %d\n", info);

	pvm_lvgroup(MATRIXGROUP);

	free(subblock); free(colsum); free(colprod); 
	pvm_exit();
}


/*** A User-defined Reduction Function ***/
void
calcprod(datatype,x,y,num,info)
int *datatype;
double  *x,*y;
int *num, *info;
{
int i;
	for (i = 0 ; i < *num; i++)
		x[i] *= y[i];
}        
