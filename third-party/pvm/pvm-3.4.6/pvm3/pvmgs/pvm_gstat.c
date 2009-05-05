
static char rcsid[] =
	"$Id: pvm_gstat.c,v 1.8 2001/11/30 20:33:11 pvmsrc Exp $";

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
   pvm_gstat - print the status of all groups held 
*/

#include <stdio.h>
#include "pvm3.h"
#include "pvmalloc.h"
#include "pvmgsd.h"
#include "pvmproto.h"

#define NEWMEM(p,n,t) \
	if (p != (t *) NULL) \
		PVM_FREE(p); \
	p = (t *) PVM_ALLOC(n  * sizeof(t) ,"pvm_gstat")

extern struct timeval *pvmgs_tmout;  /* from libgpvm */

int
main(argc, argv)
int argc;
char *argv[];
{
    int gstid;
    int i, j, ng; 
    GROUP_STRUCT group;
        group.len = 0; group.name = (char *) NULL;
        group.tids = (int *) NULL; group.btids = (int *) NULL;
        pvm_mytid();
	if((gstid = gs_getgstid()) < 0) 
        {
            fprintf(stderr, "%s: no groups server\n", argv[0]);
            return(-1);
        }
        pvm_setcontext( SYSCTX_DG );
        pvm_initsend(PvmDataDefault);
        pvm_send(gstid, GSLS);
        if ( pvm_trecv(gstid, GSLS, pvmgs_tmout) <= 0) 
        {
             fprintf(stderr, "Error receiving message from group server\n");
             exit (-1);
        }
       
        /* Unpack the information for each group                           */
        pvm_upkint(&ng, 1, 1);
        if (ng == 0)
            fprintf(stdout, "no groups exist\n");
        for (i = 0; i < ng; i++) 
        {
            pvm_upkint(&(group.len),1,1);
            NEWMEM(group.name, group.len+1,char);
            pvm_upkstr(group.name);
            pvm_upkint(&(group.ntids), 1, 1);
            pvm_upkint(&(group.maxntids), 1,1);
            pvm_upkint(&(group.barrier_count), 1, 1);
            pvm_upkint(&(group.barrier_reached),1,1);
            NEWMEM(group.tids, group.maxntids, int);
            NEWMEM(group.btids, group.barrier_reached, int);
            pvm_upkint(group.tids, group.maxntids, 1);
            if (group.barrier_reached > 0)
                pvm_upkint(group.btids, group.barrier_reached, 1);
            fprintf(stdout, 
                "group: %s, size: %d,  barrier_count %d, barrier_reached %d\n",
                 group.name, group.ntids, group.barrier_count, 
                 group.barrier_reached);
            fputs("tids:\n", stdout);
            for (j = 0; j < group.maxntids; j++)
                if (group.tids[j] > 0)
                    fprintf(stdout, "%d 0x%x\t", j, group.tids[j]);
            fputs("\n", stdout);
            if (group.barrier_reached > 0) 
            {
                fputs("tids waiting on barrier:\n", stdout);
                for (j = 0; j < group.barrier_reached; j++)
                     fprintf(stdout, "0x%x\t", group.btids[j]);
                fputs("\n", stdout);
            }
        }
	pvm_exit();
}
