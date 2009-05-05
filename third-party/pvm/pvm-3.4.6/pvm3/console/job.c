
static char rcsid[] =
	"$Id: job.c,v 1.11 2005/04/22 15:16:20 pvmsrc Exp $";

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
 *	job.c
 *
 *	Job descriptor / output gathering code.
 *
 * $Log: job.c,v $
 * Revision 1.11  2005/04/22 15:16:20  pvmsrc
 * Fixed long-standing console catchout error!
 * 	- newline search & dump code omitted key bounds check...
 * 	(leftover '\n' beyond current op->o_len could cause negative o_len!)
 * (Test case provided by Michael PARKER <michael.parker@st.com>. Thanks!)
 * (Spanker=kohl)
 *
 * Revision 1.10  2000/02/15 17:16:47  pvmsrc
 * *Really* removed old checktrace() routine.
 * 	- hasn't been called for years...  duh...
 * (Spanker=kohl)
 *
 * Revision 1.9  2000/02/15 17:06:20  pvmsrc
 * Modified dumping of task output / trace events.
 * 	- key off of PvmShowTids option, leave off task id prefix if set.
 * (Spanker=kohl)
 *
 * Revision 1.8  1999/07/08 18:59:41  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.7  1998/01/28  23:03:41  pvmsrc
 * Fixed tracing bogusness!
 * 	- host add / del notifies for tracing were getting scrunched by
 * 		console host add notify, etc...
 * 	- moved message / notify tag constants to job.h and USED THEM.
 * 	- initialized nextjob properly...
 * 	- added new "joboffset" global in cons.c for dumping more
 * 		reasonable job numbers to user.
 * (Spanker=kohl)
 *
 * Revision 1.6  1997/07/09  13:21:07  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.5  1997/05/13  14:37:41  pvmsrc
 * Changed header file $includes:
 * 	- ../src/listmac.h -> listmac.h
 * 	- ../src/bfunc.h -> bfunc.h
 * 	- use -I$(PVMDIR)/src in Makefile.aimk instead.
 *
 * Revision 1.4  1997/05/01  15:41:17  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.3  1997/01/28  19:13:19  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/10/24  23:04:30  pvmsrc
 * Updated for new tracing facility:
 * 	- removed old checktrace() stuff...
 *
 * Revision 1.1  1996/09/23  20:25:38  pvmsrc
 * Initial revision
 *
 * Revision 1.2  1994/06/03  20:01:52  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:30:32  manchek
 * Initial revision
 *
 */


#include <stdio.h>
#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif
#include <pvm3.h>
#include <pvmtev.h>
#include "myalloc.h"
#include "job.h"
#include "listmac.h"
#include "bfunc.h"

extern char *pvm_errlist[];
extern int pvm_nerr;

extern int mytid;						/* from cons.c */
extern int joboffset;					/* from cons.c */

struct job *joblist = 0;


struct job *
job_new(jid)
	int jid;
{
	struct job *jp = joblist->j_link;
	struct job *jp2;
	struct obuf *op;

	while (jp != joblist && jp->j_jid < jid)
		jp = jp->j_link;
	if (jp->j_jid != jid) {
		jp2 = TALLOC(1, struct job, "job");
		BZERO((char*)jp2, sizeof(struct job));
		jp2->j_jid = jid;
		op = TALLOC(1, struct obuf, "obuf");
		BZERO((char*)op, sizeof(struct obuf));
		op->o_link = op->o_rlink = op;
		jp2->j_obufs = op;
		LISTPUTBEFORE(jp, jp2, j_link, j_rlink);
		jp = jp2;
	}
	return jp;
}


job_init()
{
	if (!joblist) {
		joblist = TALLOC(1, struct job, "job");
		BZERO((char*)joblist, sizeof(struct job));
		joblist->j_link = joblist->j_rlink = joblist;
	}
	return 0;
}


struct job *
job_find(jid)
	int jid;
{
	struct job *jp = joblist->j_link;

	while (jp != joblist && jp->j_jid < jid)
		jp = jp->j_link;
	if (jp->j_jid == jid)
		return jp;
	return (struct job*)0;
}


job_free(jp)
	struct job *jp;
{
	LISTDELETE(jp, j_link, j_rlink);
	if (jp->j_obufs)
		MY_FREE(jp->j_obufs);
	if (jp->j_ff)
		(void)fclose(jp->j_ff);
	MY_FREE(jp);
	return 0;
}


struct obuf *
obuf_find(jp, tid)
	struct job *jp;
	int tid;
{
	struct obuf *op = jp->j_obufs->o_link;

	while (op != jp->j_obufs && op->o_tid < tid)
		op = op->o_link;
	if (op->o_tid == tid)
		return op;
	return (struct obuf*)0;
}


struct obuf *
obuf_new(jp, tid)
	struct job *jp;
	int tid;
{
	struct obuf *op = jp->j_obufs->o_link;
	struct obuf *op2;

	while (op != jp->j_obufs && op->o_tid < tid)
		op = op->o_link;
	if (op->o_tid != tid) {
		op2 = TALLOC(1, struct obuf, "obuf");
		BZERO((char*)op2, sizeof(struct obuf));
		op2->o_tid = tid;
		LISTPUTBEFORE(op, op2, o_link, o_rlink);
		op = op2;
	}
	return op;
}


obuf_free(jp, op)
	struct job *jp;
	struct obuf *op;
{
	LISTDELETE(op, o_link, o_rlink);
	if (op->o_buf)
		MY_FREE(op->o_buf);
	MY_FREE(op);
	if (jp->j_obufs->o_link == jp->j_obufs) {
		printf("[%d] finished\n", jp->j_jid - joboffset);
		job_free(jp);
	}
	return 0;
}


obuf_dump(jp)
	struct job *jp;
{
	struct obuf *op;

	for (op = jp->j_obufs->o_link; op != jp->j_obufs; op = op->o_link) {
		printf("obuf_dump() t%x\n", op->o_tid);
	}
	return 0;
}


checkoutput(jp, cc, len, cod, src)
	struct job *jp;
	int cc;					/* message mid */
	int len;
	int cod;
	int src;
{
	int tid;					/* task */
	int n;						/* length or event code */
	char *p;
	int ptid;
	struct obuf *op;
	int show;

	show = pvm_getopt( PvmShowTids );

	pvm_upkint(&tid, 1, 1);
	pvm_upkint(&n, 1, 1);

	if (n > 0) {
		if (!(op = obuf_find(jp, tid))) {
			printf("Bogus message, job %d has no task t%x\n",
					jp->j_jid - joboffset, tid);
			goto hork;
		}
		if (n + op->o_len >= op->o_maxl) {
			op->o_maxl = op->o_len + n + 1;
/*
			printf("REALLOC t%x buf to %d\n", tid, op->o_maxl);
*/
			if (op->o_buf)
				op->o_buf = TREALLOC(op->o_buf, op->o_maxl, char);
			else
				op->o_buf = TALLOC(op->o_maxl, char, "");
		}
		pvm_upkbyte(op->o_buf + op->o_len, n, 1);
		op->o_buf[op->o_len + n] = 0;
/*
		printf("UNPACK t%x {%s}\n", tid, op->o_buf + op->o_len);
*/
		p = op->o_buf + op->o_len;
		op->o_len += n;
		while ( (p = CINDEX(p, '\n'))
				&& (p - op->o_buf <= op->o_len) ) {
			*p++ = 0;
			if ( show )
				fprintf((jp->j_ff ? jp->j_ff : stdout),
						"[%d:t%x] ", jp->j_jid - joboffset, tid);
			fprintf((jp->j_ff ? jp->j_ff : stdout),
					"%s\n", op->o_buf);
			op->o_len -= p - op->o_buf;
			if ( !(op->o_len) ) break;
			BCOPY(p, op->o_buf, op->o_len);
			p = op->o_buf;
		}

	} else {
		switch (n) {

		case 0:
			if (!(op = obuf_find(jp, tid))) {
				printf("Bogus message, job %d has no task t%x\n",
						jp->j_jid - joboffset, tid);
				goto hork;
			}
			if (op->o_len > 0) {
				if ( show )
					fprintf((jp->j_ff ? jp->j_ff : stdout),
							"[%d:t%x] ", jp->j_jid - joboffset, tid);
				fprintf((jp->j_ff ? jp->j_ff : stdout),
						"%s\n", op->o_buf);
				op->o_len = 0;
			}
			op->o_flag |= GOTEOF;
			if (op->o_flag == (GOTSPAWN|GOTEOF)) {
				if ( show )
					fprintf((jp->j_ff ? jp->j_ff : stdout),
							"[%d:t%x] EOF\n", jp->j_jid - joboffset,
							tid);
				obuf_free(jp, op);
			}
			break;

		case -1:
			if (!(op = obuf_find(jp, tid)))
				op = obuf_new(jp, tid);
/*
			pvm_upkint(&ptid, 1, 1);
*/
			op->o_flag |= GOTSPAWN;
			if (op->o_flag == (GOTSPAWN|GOTEOF)) {
				if ( show )
					fprintf((jp->j_ff ? jp->j_ff : stdout),
							"[%d:t%x] EOF\n", jp->j_jid - joboffset,
							tid);
				obuf_free(jp, op);
			}

			break;

		case -2:
			if (!(op = obuf_find(jp, tid)))
				op = obuf_new(jp, tid);
/*
			pvm_upkint(&ptid, 1, 1);
*/
			break;

		default:
			printf("Bogus message from job %d task t%x\n",
				jp->j_jid - joboffset, tid);
			break;
		}
	}

hork:
	pvm_freebuf(cc);
	return 0;
}


