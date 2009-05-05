
static char rcsid[] =
	"$Id: starter.c,v 1.4 2009/01/23 01:12:52 pvmsrc Exp $";

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

char prog_name[256];

void usage();

/* extern variables */
extern int errno, optind;
extern char *optarg;

main(argc, argv)
    int argc;
    char **argv;
{
    int mytid;                  /* my task id */
    int tids[32];		/* slave task ids */
    int n, nproc, numt, i, who, msgtype, nhost, narch, ch;
    float data[100], result[32];
    struct pvmhostinfo *hostp[32];
    int info, ntasks, done, j;
    struct pvmtaskinfo *taskp;

    /* Set number of slaves to start */
    while ((ch = getopt(argc, argv, "n:")) != EOF) {
       switch(ch) {
          case 'n': nproc = atoi(optarg);
                    break;
          default:  usage();
                    break;
       }
    }
    argc -= optind;
    argv += optind;

    if (argc < 1) {
       usage();
    }

    /* Get program name */
    strcpy(prog_name, argv[0]);
    argv += 1;

    printf("%s program to be spawned %d times.\n", prog_name, nproc);

    /* enroll in pvm */
    mytid = pvm_mytid();
    pvm_setopt(PvmAutoErr, 1);
    pvm_catchout(stdout);

    /* start up slave tasks */
    numt=pvm_spawn(prog_name, argv, 0, "", nproc, tids);
    if( numt < nproc ){
       printf("Trouble spawning slaves. Aborting. Error codes are:\n");
       for( i=numt ; i<nproc ; i++ ) {
          printf("TID %d %d\n",i,tids[i]);
       }
       for( i=0 ; i<numt ; i++ ){
          pvm_kill( tids[i] );
       }
       pvm_exit();
       exit();
    }
    
    done = 1;
    taskp = NULL;
    while(done) {
       pvmsleep(1);
       info = pvm_tasks(0, &ntasks, &taskp);
       done = 0;
       for (j=0; j < numt; j++)
          for (i=0; i < ntasks; i++) {
             if (tids[j] == taskp[i].ti_tid) {
                done = 1;
                break;
             }
          }
    }
    /* Program Finished exit PVM before stopping */
    pvm_exit();
}

void usage()
{
    printf("starter -n <number to spawn> <program name> <args>\n");
    exit(1);
}

