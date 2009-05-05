
static char rcsid[] =
	"$Id: thb.c,v 1.3 1999/03/12 17:57:37 pvmsrc Exp $";

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
	Join a bunch of groups and do some barriers
*/

#include <stdio.h>
#include "pvm3.h"

#define MAXGNAME 32 /* used to be a constant, length not important */
#define MAXGTIDS 32 /* used to be a constant, Number not important */
#define MAXNGROUPS 32 /* used to be constant, Number not important */

int
main(argc, argv)
int argc;
char *argv[];
{
	int mytid, ctid[MAXGTIDS];
	int nproc;
	char g[MAXGNAME];
	int cc;
	int gid[MAXNGROUPS], gs;
	int i;

	if (argc != 2) goto usage;
	if ((nproc = atoi(argv[1])) < 1) goto usage;
		
	mytid = pvm_mytid();
	if (mytid < 0) {
		pvm_perror(argv[0]);
		return -1;
		}

	/* join a bunch of groups */
	for (i = 0; i < MAXNGROUPS; i++) {
		sprintf(g, "group%d", i);
		if((gid[i] = pvm_joingroup(g)) < 0) {
			pvm_perror("joining");
			}
		fprintf(stderr, "joined %s\n", g);
		}

	/* test that i'm in the groups */
	for (i = MAXNGROUPS-1; i  >= 0; i--) {
		sprintf(g, "group%d", i);
		if(gid[i] != (cc = pvm_getinst(g, mytid))) {
			fprintf(stderr, "I'm not myself %d %s\n", cc, g);
			pvm_perror("getinst");
			}
		if(mytid != (cc = pvm_gettid(g, gid[i]))) {
			fprintf(stderr, "I'm not myself %d %s\n", cc, g);
			pvm_perror("gettid");
			}
		}

	/* start a bunch of children */
    printf("spawning ... ");
    pvm_spawn("tnb", (char**)0, 0, "", nproc, ctid);

    for (i = 0; i < nproc; i++) {
        if (ctid[i] < 0) {
            fprintf(stderr, "trouble on spawn number %d\n", ctid[i]);
            pvm_perror("th:");
            }
		else {
			int foo;
			foo = nproc+1;
			/* send a group name to the child */
			pvm_initsend(PvmDataDefault);
			pvm_pkstr(g);
			pvm_pkint(&foo, 1, 1);
			pvm_send(ctid[i], 5);
			}

		}

        printf ("barrier on %s \n", g);
	pvm_barrier(g, nproc+1);
	pvm_initsend(PvmDataDefault);
	pvm_bcast(g, 77);

        printf(" leaving all the groups \n");
	/* leave the groups */
	for (i = 0; i < MAXNGROUPS; i++) {
		sprintf(g, "group%d", i);
		if(pvm_lvgroup(g) < 0)
			pvm_perror("pvm_lvgroup");
		}

	pvm_exit();
	printf("done.\n");
	return 0;
usage:
	fprintf(stderr, "usage: %s <nproc>\n", argv[0]);
	return -1;
}
