
static char rcsid[] =
	"$Id: trsg.c,v 1.6 2009/01/23 01:20:23 pvmsrc Exp $";

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
 * Example of group Reduce, Scatter, and Gather functions - J.M. Donato
 * 
 * This example calculates the sum of squares of the first N integers
 * in three different ways where 
 *
 *   N = (number of processors)*(number of elements per row)
 *
 * Note:  This is obviously not an efficient way to compute the 
 *        sum of squares, but it is a cutesy example and test case.
 */

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include "pvm3.h"

#define MAXNDATA  20
#define MAXNPROCS 16
#define DFLTNDATA 5
#define DFLTNPROCS 4
#define TASK_NAME "trsg"

#ifdef WIN32
#undef min
#undef max
#endif

#define min(x,y) ( ((x)<(y))? (x) : (y) )
#define max(x,y) ( ((x)>(y))? (x) : (y) )

extern void PvmMin();
extern void PvmMax();
extern void PvmSum();
extern void PvmProduct();

void MaxWithLoc();

main()
{
  int myginst, i, j, gsize, count, nprocs, msgtag, datatype; 
  int info_product, info_user; 
  int  tids[MAXNPROCS], myrow[MAXNDATA], matrix[MAXNDATA*MAXNPROCS]; 
  float values[2];
  int midpoint, bigN, Sum1=0, Sum2=0, SumSquares, rootginst; 
  int PSum = 0, PartSums[MAXNPROCS], dupls[MAXNDATA];
  char *gname = "group_rsg";

  /* join the group */
  myginst = pvm_joingroup(gname);
  pvm_setopt(PvmAutoErr, 1);

  /* I am the first group member, get input, start up copies of myself */
  if ( myginst == 0 )       
  {
    if (pvm_parent() == PvmNoParent)
    {
      printf("\n *** Example use of PVM Reduce, Scatter, and Gather *** ");
      printf("\n Number of processors to use (1-%d)? : ", MAXNPROCS);
      scanf("%d", &nprocs);
      if (nprocs > MAXNPROCS) nprocs = MAXNPROCS;
      printf(" Number of elements per row to use (1-%d)? : ", MAXNDATA);
      scanf("%d", &count);
      if (count > MAXNDATA) count = MAXNDATA;
      printf(" INPUT values: nprocs = %d, count = %d \n", nprocs, count);
    }
    else
    {
      count = DFLTNDATA;
      nprocs = DFLTNPROCS;
    }

    tids[0] = pvm_mytid();

    if (nprocs > 1)
      pvm_spawn(TASK_NAME, (char**)0, 0, "", nprocs-1, &tids[1]);

    /* wait until they have all started, then send input values */
    while (gsize = pvm_gsize(gname) < nprocs) 
		pvmsleep(1);

	pvm_initsend(PvmDataDefault);
    pvm_pkint(&nprocs, 1, 1);
    pvm_pkint(&count,  1, 1);
    pvm_bcast(gname, msgtag=17);
  }
  else
  {
    /* receive the input values */
    pvm_recv(-1, msgtag=17);
    pvm_upkint(&nprocs, 1, 1);
    pvm_upkint(&count,  1, 1);
  }
  
  rootginst = 0;  /* determine the group root */

  /* init the matrix values on the root processor */
  if (myginst == rootginst) 
    for (j=0; j<nprocs; j++)
      for (i=0; i<count; i++)
        matrix[j*count + i] = j*count + i + 1;

  /* scatter rows of matrix to each processor */
  pvm_scatter(myrow, matrix, count, PVM_INT, msgtag=19, gname, rootginst);

  /* this should end up squaring each value on each processor */
  for (i=0; i<count; i++) dupls[i] = (myginst*count + i + 1);
  datatype = PVM_INT;
  PvmProduct(&datatype, myrow, dupls, &count, &info_product);
  if ((myginst == rootginst) && (info_product < 0))
      printf(" ERROR: %d on PvmProduct call \n", info_product);

  /* do partial sum on each proc */
  for (i=0; i<count; i++) PSum += myrow[i];

  /* gather partial sums to the rootginst */
  pvm_gather(PartSums, &PSum, 1, PVM_INT, msgtag=21, gname, rootginst);

  /* do a global sum over myrow, the result goes to rootginst */
  pvm_reduce(PvmSum, myrow, count, PVM_INT, msgtag=23, gname, rootginst);

  /* init values and include location information on each processor */
  midpoint = nprocs/2;
  values[0] = -(myginst - midpoint)*(myginst-midpoint) + count;
  values[1] = myginst;    /* location information */

  /* use a user-defined function in reduce, send answer to rootginst */
  info_user = pvm_reduce(MaxWithLoc, values, 2, PVM_FLOAT, 
                         msgtag=25, gname, rootginst);

  bigN = nprocs*count;
  if (myginst == rootginst)
  {
    /* Complete the Sum of Squares using different methods */
    for (i=0; i<nprocs; i++) Sum1 += PartSums[i]; 
    for (i=0; i<count;  i++) Sum2 += myrow[i];    
    SumSquares = bigN*(bigN+1)*(2*bigN+1)/6;
    if ( (Sum1 == SumSquares) && (Sum2 == SumSquares))
      printf("\n Test OK: Sum of Squares of first %d integers is %d \n",
             bigN, Sum1);
    else
      printf("\n %s%d%s%d%s%d,\n%s%d \n", 
             "ERROR: The Sum of Squares of the first ", bigN, 
             " integers \n was calculated by Sum1 as ", Sum1,
             " and by Sum2 as ", Sum2,
             " for both it should have been ", SumSquares);

    if (info_user < 0) 
      printf(" ERROR: %d on Reduce with User Function\n", info_user);

    if ((values[0] != count) || (values[1] != midpoint)) 
      printf(" ERROR: Should have (%d, %d), but have (%f, %f): \n",
               count, midpoint, values[0], values[1]);
    else
      printf(" Test Ok: Received (%f, %f): ", values[0], values[1]);
    printf("\n");
  }

  /* sync up again, leave group, exit pvm */
  pvm_barrier(gname, nprocs);   
  pvm_lvgroup(gname);
  pvm_exit();
  exit(1);

} /* end main() */


/*
  This function returns the elementwise maximum of two vectors 
  along with location information.
 
  The first num/2 values of x and y are the data values to compare.
  The second num/2 values of x and y are location information
  which is kept with the maximum value determined.

  In the case of a tie in data values, the smaller location 
  is kept to insure the associativity of the operation.
*/

void MaxWithLoc(datatype, x, y, num, info)
int *datatype, *num, *info;
float *x, *y;
{
  int i, count;
  count = (*num) / 2;

  if (*datatype != PVM_FLOAT) { *info = PvmBadParam; return; }

  for (i=0; i<count; i++)
    if (y[i] > x[i])
    {
      x[i]       = y[i];
      x[i+count] = y[i+count];
    }
    else
      if (y[i] == x[i]) x[i+count] = min(x[i+count], y[i+count]);

  *info = PvmOk;
  return;

}  /* end MaxWithLoc() */
