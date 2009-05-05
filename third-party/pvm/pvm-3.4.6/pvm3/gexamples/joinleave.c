
static char rcsid[] =
	"$Id: joinleave.c,v 1.3 1999/03/12 17:57:38 pvmsrc Exp $";

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
	join and leave a lot 

*/

#include <stdio.h>
#include "pvm3.h"
#define MAXJOINS 10000
int
main(argc, argv)
int argc;
char *argv[];
{
	int mytid, mygid, njoins;
	int cnt = 0;

	mytid = pvm_mytid();
	fprintf(stderr, "%s 0x%x enrolled\n", argv[0], mytid);
	if (mytid < 0) {
		pvm_perror(argv[0]);
		return -1;
		}
        if (argc > 1)
           njoins = atoi(argv[1]);
        else
           njoins = MAXJOINS;

	/* join a group */
	while (cnt++ <= njoins) {
		if((mygid = pvm_joingroup("JoinLeave")) < 0) {
			pvm_perror("JoinLeave");
			break;
			}
		if((mygid = pvm_lvgroup("JoinLeave")) < 0) {
			pvm_perror("JoinLeave");
			break;
			}
		if ((cnt%100)==0) printf("Joined %d times\n", cnt);
		}
return 0;
}
