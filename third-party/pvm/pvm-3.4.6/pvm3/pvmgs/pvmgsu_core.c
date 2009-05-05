
static char rcsid[] =
	"$Id: pvmgsu_core.c,v 1.17 2001/11/30 20:33:12 pvmsrc Exp $";

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
*	pvmgsu_core.c - Core group library routines  
* Revision 1.4  1994/11/07  21:09:38  manchek
* include stdlib if available.
* function prototypes for SCO.
* remove spurious breaks after returns
*
* Revision 1.3  1994/10/15  21:57:34  manchek
* disable output collection and tracing while spawning group server
*
* Revision 1.2  1994/10/15  18:53:48  manchek
* added tids to BCAST1 trace message
*
*
*	05 Mar 1993	Adam Beguelin adamb@cs.cmu.edu
*	05 Mar 1993	Fixed malloc() in pvm_bcast()
*	05 Mar 1993	pvm_barrier now returns 0 or an error
*	20 Mar 1993	pvm_bcast does not send to itself, beguelin
*      01 Jun 1993     Fixed saving and restoring of mbufs, beguelin
*	30 Nov 1993     Fixed gs_getgstid() to check spawn return correctly,
*                      manchek
*       8 Mar 1994     optimized gs_getgstid(). 
*       8 Mar 1994     Added reduce & assoc routines. Donato & P.Papadopoulos
*      24 Apr 1994     Added scatter, gather, gs_get_datasize routines. Donato
*      15 Jun 1995     Add static groups, clear local tables if tid changes
* 
*/
	
	
#ifdef HASSTDLIB
#include <stdlib.h>
#endif

#include <stdio.h>
#include <pvm3.h>
#include "pvmalloc.h"
#include "bfunc.h"
#include "lpvm.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "pvmgsd.h"
#include "pvmproto.h"

#if defined(IMA_PGON)
#include <nx.h>
#endif

int gstid = -1;
static int myoldtid = -1;

extern int pvm_errno;
extern int pvmmytid;
extern int pvmtoplvl;
extern struct Pvmtracer pvmtrc;

/* Declarations for static group information                               */
	
static GROUP_LIST sgroup_list[HASHSIZE];
static int ngroups = -1;

/* Set PVM Group Server Timeout (if any), In Seconds */
#ifndef PVMGSTIMEOUT
struct timeval *pvmgs_tmout = (struct timeval *) NULL;
#else
struct timeval pvmgs_tmout_struct = { PVMGSTIMEOUT, 0 };
struct timeval *pvmgs_tmout = &pvmgs_tmout_struct;
#endif

/* ================ gs_getgstid() ====================================     */
/* int info = getgstid()                                                   */
/*   gs_getgstid returns the tid of the group server, starts the server    */
/*   if it is not running                                                  */

int
gs_getgstid()
{
	int info;
	int mytid;
	int otid; 
	int rbuf;
	int srbuf;
	int ttid;

	mytid = pvm_mytid();
	
	if (gstid >= 0 && mytid == myoldtid) 
		return (gstid);
	
	srbuf = pvm_setrbuf(0);

	if ( pvm_recvinfo(GSNAME, 0, PvmMboxDefault) == PvmNotFound )
	{
		/* Don't trace output from the spawned group server */

		otid = pvm_setopt(PvmOutputTid, 0);
		ttid = pvm_setopt(PvmTraceTid, 0);

		info = pvm_spawn("pvmgs", (char **)0, PvmMppFront,
				(char *)0, 1, &gstid);

		pvm_setopt(PvmOutputTid, otid);
		pvm_setopt(PvmTraceTid, ttid);

		if (info != 1) 
		{
			if (info == 0 && gstid < 0) 
			{
				pvm_errno = gstid;
				pvm_perror("gs_getgstid() failed to start group server");
			}
			return pvm_errno;
		}

		while( pvm_recvinfo(GSNAME, 0, PvmMboxDefault) == PvmNotFound )
			/* wait for it to register */ ;
	}
	
	/* unpack the group server tid */ 
	pvm_upkint(&gstid,1,1);
	pvm_setrbuf(srbuf);
	
	/* initialize storage for static group information */

	if (myoldtid == -1)
	{
		ngroups = 0;
		gs_hash_init(sgroup_list);
	}
	else
		gs_hash_clear(sgroup_list, &ngroups, HASHSIZE);

	myoldtid = mytid;

	return(gstid);
}

/* ================ pvm_joingroup()===================================     */
/*
	int inum = pvm_joingroup(char* group)

	Adds the calling tid to the named group and returns its instance number.
	Always adds the task in the first available slot such that
	if one task leaves a group and another later joins, then the later
	task will get the instance number freed by the earlier task.
	This allows users to keep a contiguous block of instance numbers [0,p-1].
*/

int
pvm_joingroup(group)
char *group;
{
	int gid;
	int x;

	TEV_DECLS;
	
	BGN_TRACE( TEV_JOINGROUP, group, 0, (int *) NULL );

	int_query_server(group, JOIN, "pvm_joingroup", &gid, 0);

	END_TRACE( TEV_JOINGROUP, TEV_DID_CC, &gid );

	if (gid < 0)
		pvm_errno = gid;

	return gid;
}
	
/* ================ pvm_lvgroup()===================================     */
/*
	int info = pvm_lvgroup(char* group)
	Removes the calling tid from the named group.
	Returns only after getting confirmation from server.
	This allows users to coordinate leaving and joining.
*/
	
int
pvm_lvgroup(group)
char *group;
{
	int rc;
	int x; 

	GROUP_STRUCT_PTR sgroup;

	TEV_DECLS;
	
	BGN_TRACE( TEV_LVGROUP, group, 0, (int *) NULL );

	/* delete any statically held group information */
	gs_delete_group(group, sgroup_list, &ngroups); 

	int_query_server(group, LEAVE, "pvm_lvgroup", &rc, 0);

	END_TRACE( TEV_LVGROUP, TEV_DID_CC, &rc );

	if (rc < 0)
		pvm_errno = rc;

	return(rc);
}
	

	
/* ================ pvm_getinst() ====================================     */
/* int inum = pvm_getinst(char* group, int tid)
	Returns the instance number of the specified tid in the named group.
	Can be called by any task.
*/
int
pvm_getinst(group, tid)
char *group;
int tid;
{
	int foo;
	int inst;
	int x;

	TEV_DECLS;
	
	BGN_TRACE( TEV_GETINST, group, TEV_DID_GT, &tid );

	inst = gs_getinst(group, tid, sgroup_list, &ngroups, &foo); 

	if (inst < 0)    /* didn't find the info in local memory */ 
		int_query_server(group, GETINST, "pvm_getinst", &inst, tid);

	END_TRACE( TEV_GETINST, TEV_DID_GI, &inst );

	if (inst < 0)
		pvm_errno = inst;

	return(inst);
}
	
/* ================ pvm_gettid() =====================================     */
/*
int tid = pvm_gettid(char * group, int inum)
	Returns the tid of the task defined by the group/inum pair.
	Can be called by any task.
*/
int
pvm_gettid(group, inst)
char *group;
int inst;
{
	int foo;
	int tid;
	int x;

	TEV_DECLS;
	
	BGN_TRACE( TEV_GETTID, group, TEV_DID_GI, &inst );

	tid = gs_gettid(group, inst, sgroup_list, &ngroups, &foo); 

	if (tid < 0)     /* didn't find the info in local memory */
		int_query_server(group, GETTID, "pvm_gettid", &tid, inst);

	END_TRACE( TEV_GETTID, TEV_DID_GT, &tid );

	if (tid < 0)
		pvm_errno = tid;

	return(tid);
}
	
/* ================ pvm_gsize() ======================================     */
/*
	int gsize = pvm_gsize(char* group)
	Returns the present size of the named group.
*/

int
pvm_gsize(group)
char *group;
{
	int foo;
	int size;
	int x; 

	TEV_DECLS;
	
	BGN_TRACE( TEV_GSIZE, group, 0, (int *) NULL );

	size= gs_gsize(group, sgroup_list, &ngroups, &foo); 

	if (size < 0)    /* didn't find the info in local memory */
		int_query_server(group, GSIZE, "pvm_gsize", &size, 0);

	END_TRACE( TEV_GSIZE, TEV_DID_GS, &size );

	if (size < 0)
		pvm_errno = size;
	return(size);
}
	
/* ================ pvm_bcast() =====================================     */
/*
int info = pvm_bcast(char* group, int msgtag)
	Broadcast message to all members presently in the named group
	(excluding yourself if you are in the group).
	Calling tid need not be in the group.
	*/
int
pvm_bcast(group, msgtag)
char *group;
int msgtag;
{
	int bailout=0;
	int cc;
	int i;
	int mytid;
	int ntids; 
	int rbuf;
	int sbuf;
	int *tids = (int *) NULL;
	int x; 

	TEV_DECLS;
	
	BGN_TRACE( TEV_BCAST, group, TEV_DID_MC, &msgtag );

	if ((cc = gs_get_tidlist(group, msgtag, &ntids, &tids, 0)) < 0)
		bailout = 1; 

	if ((mytid = pvm_mytid()) < 0 && ! bailout) 
	{
		pvm_perror("pvm_bcast");
		cc = PvmSysErr;
		bailout = 1;
	}
	
	/* if I'm the only one in the group */
	if ((ntids == 1) && (tids[0] == mytid ) && ! bailout) 
	{
		cc = PvmNoInst;
		bailout = 1;
	}
	
	if (! bailout ) 
	{
		for (i = 0; i < ntids; i++) /* remove my tid                   */
		{
			if (tids[i] == mytid) 
			{
				/* move the last tid to here and shorten the list          */
				tids[i] = tids[--ntids];
				break;
			}
		}

		if ((cc = pvm_mcast(tids, ntids, msgtag)) > 0)
			cc = 0;
	}

	if (cc < 0)
		pvm_errno = cc;
	
	if ( TEV_AMEXCL )     /* Special handling of tracing for bcast */
	{
		if (TEV_DO_TRACE(TEV_BCAST,TEV_EVENT_EXIT)) 
		{
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			if (cc < 0) 
			{
				TEV_PACK_INT( TEV_DID_MDL, TEV_DATA_ARRAY,
					(int *) NULL, 0, 1 );
			} 
			else 
			{
				TEV_PACK_INT( TEV_DID_MDL, TEV_DATA_ARRAY,
					tids, ntids, 1 );
			}
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (tids)
		PVM_FREE(tids);

	return(cc);
}
	
/* ================ pvm_barrier() ====================================     */
/*
	int info = pvm_barrier(char* group, int count)

	Calling task waits until count members of named group also
	call pvm_barrier. If user places -1 for count then the present
	size of the group is used. Note this option is not useful if
	the size of the group is changing.
	A process must be a member of a group to call pvm_barrier on
	that group
*/

int
pvm_barrier(group, cnt)
char  *group;
int cnt;
{
	int rc;
	int x; 
	
#if defined(IMA_PGON)

	int gstate;
	int mytid;
	int nhosts;
	int nmem;
	int pcoord;

	extern int pvmpgonpartsize;

#endif

	TEV_DECLS;
	
	BGN_TRACE( TEV_BARRIER, group, TEV_DID_GBC, &cnt);
	
/* use gsync when possible on the PGON. 
	1) The group must be static
	2) The size of the PGON partition must equal number pgon nodes in group
	3) If group has other hosts besides pgon, then 1 node from pgon 
	represents all the nodes to the group server. 
*/
#if defined(IMA_PGON)
	mytid = pvm_mytid();

	rc = gs_host_char(group,sgroup_list, &ngroups, 

	gs_tidtohost(mytid), &pcoord, &nmem, &nhosts, &gstate);

	if (rc == 0 && nmem == pvmpgonpartsize ) /*use gsync */
	{
		rc = _gsync();

		if ( rc >= 0 && nhosts > 1) /* sync'ed pgon, now sync w/others */
		{
			if (pcoord == mytid)
				int_query_server(group, BARRIERV, "pvm_barrier", &rc, cnt);

			_gsync();  
		}
	}
	else
#endif

	int_query_server(group, BARRIER,  "pvm_barrier", &rc, cnt);

	if (rc > 0) 
		rc = 0;

	END_TRACE( TEV_BARRIER, TEV_DID_CC, &rc );

	if (rc < 0)
		pvm_errno = rc;

	return(rc);
}
	
/* ================ pvm_freezegroup() ===============================     */
/*
	int info = pvm_freezegroup(char *group, int size)
	The named dynamic group is frozen.  Group information is cached by
	the local process.
*/   

int
pvm_freezegroup(group, size)
char *group;
int size;
{
	int rc;
	int x;

	/* BGN_TRACE( TEV_FREEZE, group, TEV_DID_GS, &size ); */

	int_query_server(group, STATICGROUP,  "pvm_freezegroup", &rc, size);

	/* END_TRACE( TEV_FREEZE, TEV_DID_CC, &rc ); */

	return(rc);
}
	
	
/* **************** ROUTINES THAT DIRECTLY CONTACT THE SERVER  ************/

/* ================ pvm_gsdump() ====================================     */
/*
* 	void pvm_gsdump()
*	ask pvmgs to dump it's state. Assumes pvmgs is running
*/
void
pvm_gsdump()
{
	int savectx;
	int sbuf;

	savectx = pvm_setcontext( SYSCTX_DG );

	sbuf = pvm_mkbuf(PvmDataDefault);

	sbuf = pvm_setsbuf(sbuf);

	pvm_send(gstid, DUMP);

	pvm_freebuf(pvm_setsbuf(sbuf));

	pvm_setcontext( savectx );
}
	
/* ================ int_query_server() ===============================     */
/* Query server to get a single integer of information. Single integer     */
/* int info = int_query_server(char *group, int request, char *caller, 
*	int *rvalue, int optarg)
*	group    - string to identify the group
*	request  - integer ID of request
*	requests are: JOIN, LEAVE, GETTID, GETINST, BARRIER, GSIZE  
*	STATICGROUP
*	caller   - string to use in error reporting
*	rvalue   - value returned by server
*	optarg   - optional argument used by some requests (e.g., barrier,
*	freezegroup)
*	
*	The request is made of the group server for group. It is possible
*	for the server to not only return the information, but to tack on
*	the instance map of the group. If this happens, then a new entry
*	is kept locally to cache the info for this group.
*/

int
int_query_server(group, request, caller, rvalue, optarg)
char *group;
int request;
char *caller;
int *rvalue;
int optarg;
{
	int len;
	int rbuf;
	int sbuf,stid;
	int state;
	int savectx;

	GROUP_STRUCT_PTR sgroup;

	pvm_mytid();

	if (group == (char*)0 || *group == '\0')
		return  (*rvalue = PvmNullGroup);

	if ( (stid = gs_getgstid()) < 0)  /* find the server's tid         */
		return  (*rvalue = PvmSysErr);

	/* set context for dynamic groups */
	savectx = pvm_setcontext( SYSCTX_DG );

	/* send the request to the group server.                           */
	if ((sbuf = pvm_mkbuf(PvmDataDefault)) < 0)
		pvm_perror(caller);

	if ((sbuf = pvm_setsbuf(sbuf)) < 0)
		pvm_perror(caller);

	if (pvm_pkstr(group) < 0)
		pvm_perror(caller);

	if (request == GETINST 
			|| request == GETTID 
			|| request == BARRIER 
			||  request == BARRIERV 
			|| request == STATICGROUP) 
	{
		/* pack optarg                                                 */
		if (pvm_pkint(&optarg, 1, 1 ) < 0)
			pvm_perror(caller);
	}
	
	if (pvm_send(stid, request) < 0)
		pvm_perror(caller);

	if ((rbuf = pvm_setrbuf(0)) < 0) /* get return value from  server  */
		pvm_perror(caller);
	
	if (request == BARRIERV) request = BARRIER; 
	{
		if (pvm_trecv(stid, request, pvmgs_tmout) <= 0)
			pvm_perror(caller);
	}

	if (pvm_upkint(rvalue, 1, 1) < 0)
		pvm_perror(caller);

	if (request == JOIN || request == LEAVE || request == BARRIER 
			|| *rvalue < 0 )
		state = DYNAMIC; /* no state data will be returned            */
	else
		pvm_upkint(&state,1,1);

	if (state == STATIC)            /* we've got new static group info */
		gs_cachegroup(sgroup_list, &ngroups, &sgroup);

	pvm_freebuf(pvm_setsbuf(sbuf)); /* restore the users mbufs */

	pvm_freebuf(pvm_setrbuf(rbuf));

	/* restore user context */
	pvm_setcontext( savectx );

	return (PvmOk);
}
	
/* ================ gs_get_tidlist() =================================     */
/*
int info = gs_get_tidlist(char *group, int msgtag, int *ntids, int **tids,
	int holes_not_allowed)
	group - groupname
	msgtag - msgtag to use for querying server
	ntids  - number of tids in the tidlist
	tids   - the tidlist itself. Malloc'ed
	holes_not_allowed - flag to indicate tid list must not have holes.
*/

int 
gs_get_tidlist(group, msgtag, ntids, tids, holes_not_allowed)
char *group;
int msgtag;
int *ntids; 
int **tids; 
int holes_not_allowed;
{
	int cc;
	int i;
	int len;
	int rbuf;
	int sbuf, state, stid;
	int savectx;

	GROUP_STRUCT_PTR sgroup;

	if ( group == (char*)0 || *group == '\0' ) 
		return(PvmNullGroup);

	/* look up to see if the information is held locally               */

	sgroup = gs_group(group, sgroup_list, &ngroups, NOCREATE);

	if (sgroup != (GROUP_STRUCT_PTR) NULL) 
	{
		if (holes_not_allowed)  
		{
			for (i=0; i < sgroup->ntids; i++)
				if (sgroup->tids[i] == NOTID)
				{
					*ntids = -1;
					return (*ntids);
				}
		}

		*tids = (int *) PVM_ALLOC(sgroup->maxntids*sizeof(int),"tidlist"); 

		for (i = 0; i < sgroup->maxntids; i++)
			(*tids)[i] = sgroup->tids[i];
	
		*ntids = sgroup->ntids;

		return (PvmOk);
	} 
	
	/* find out the server's tid, start the server if need be          */
	if ( (stid = gs_getgstid()) < 0 ) 
		return(PvmSysErr);
	
	sbuf = pvm_mkbuf(PvmDataDefault);  /* send rqst to server          */

	sbuf = pvm_setsbuf(sbuf);

	rbuf = pvm_setrbuf(0); 

	pvm_pkstr(group);

	/* set context for dynamic groups */
	savectx = pvm_setcontext( SYSCTX_DG );

	if (holes_not_allowed)  
	{
		pvm_send(stid, TIDLIST);       /* e.g. scatter, gather         */
		cc = pvm_trecv(stid, TIDLIST, pvmgs_tmout); 
	}
	else 
	{
		pvm_send(stid, BCAST);         /* e.g. bcast                   */
		cc = pvm_trecv(stid, BCAST, pvmgs_tmout); 
	} 

	/* Catch pvmgs timeout... */
	if ( cc <= 0 )
		return(PvmSysErr);

	/* restore user context */
	pvm_setcontext( savectx );

	pvm_upkint(ntids, 1, 1);
	
	if (*ntids < 0) /* check for number of tids in group               */
	{
		pvm_freebuf(pvm_setsbuf(sbuf));
		pvm_freebuf(pvm_setrbuf(rbuf));

		return(*ntids);
	}
	
	if (*ntids == 0) /* if there is no one in the group */
	{
		pvm_freebuf(pvm_setsbuf(sbuf));
		pvm_freebuf(pvm_setrbuf(rbuf));
	
		return(PvmNoInst);
	}
	
	/* make room for the tids */
	*tids = (int *)PVM_ALLOC((*ntids) * sizeof(int), "gs_get_tidlist");

	if ((*tids) == (int *) NULL)
	{
		pvm_freebuf(pvm_setsbuf(sbuf));
		pvm_freebuf(pvm_setrbuf(rbuf));
	
		return(PvmSysErr);
	}

	pvm_upkint(*tids, *ntids, 1);

	pvm_upkint(&state,1,1);

	if (state == STATIC)            /* we've got new static group info */
		gs_cachegroup(sgroup_list, &ngroups, &sgroup);

	pvm_freebuf(pvm_setsbuf(sbuf)); /* restore the users mbufs         */

	pvm_freebuf(pvm_setrbuf(rbuf));
	
	return(PvmOk);
} 
	
/* ================ gs_cachegroup() ==================================     */
/* int info = gs_cachegroup(GROUP_LIST_PTR sgroup_list, int *ngroups,
	GROUP_STRUCT_PTR *rsgroup);
	
	sgroup_list - hash table that holds any static group information
	ngroups     - number of groups in the hash table
	*rsgroup    - pointer to structure that holds static info
	
	unpack group information from a message and then create the group in   
	the group list;                                                       
*/ 
int
gs_cachegroup(sgroup_list, ngroups, rsgroup)
GROUP_LIST_PTR sgroup_list;
int *ngroups;
GROUP_STRUCT_PTR *rsgroup;
{
	char *newname = (char *) NULL; 

	int info;
	int len;

	GROUP_STRUCT_PTR sgroup; 

	*rsgroup = (GROUP_STRUCT_PTR) NULL;

	if ( (info = pvm_upkint(&len,1,1))  < 0 )
		DO_ERROR_RTN( info, "gs_cachegroup" );

	if (len < 0)                  /* didn't get a valid groupname     */
		return(-1);

	if ( (newname = (char *) PVM_ALLOC(sizeof(char)*(len + 1),
			"gs_cachegroup") ) == (char *) NULL) 
		DO_ERROR_RTN( PvmNoMem, "gs_cachegroup" );

	if ( (info = pvm_upkstr(newname))  < 0  )
	{
		PVM_FREE(newname);
		DO_ERROR_RTN( info, "gs_cachegroup" );
	}

	sgroup = gs_group(newname, sgroup_list, ngroups, CREATE);

	if (sgroup != (GROUP_STRUCT_PTR) NULL)
	{
		if ( (info = pvm_upkint(&(sgroup->ntids),1,1) ) < 0 )
		{
			PVM_FREE(newname);
			DO_ERROR_RTN( info, "gs_cachegroup" );
		}

		if ( (info = pvm_upkint(&(sgroup->maxntids),1,1) ) < 0 )
		{
			PVM_FREE(newname);
			DO_ERROR_RTN( info, "gs_cachegroup" );
		}

		sgroup->tids = (int *) PVM_ALLOC(sgroup->maxntids*sizeof(int),
				"gs_cachegroup");

		if (sgroup->tids == (int *) NULL)
			gs_delete_group(newname, sgroup_list, ngroups);
		else
		{
			if ( (info = pvm_upkint(sgroup->tids,sgroup->maxntids,1) ) < 0)
			{
				gs_delete_group(newname, sgroup_list, ngroups);
				PVM_FREE(newname);
				DO_ERROR_RTN( info, "gs_cachegroup" );
			}

			info = pvm_upkint(&(sgroup->nhosts),1,1);

			sgroup->np_onhost = (int *) PVM_ALLOC(sizeof(int) * 
					sgroup->nhosts,"gs_cachegroup");
	
			sgroup->pcoord = (int *) PVM_ALLOC(sizeof(int) * 
					sgroup->nhosts,"gs_cachegroup");

			pvm_upkint(sgroup->np_onhost,sgroup->nhosts,1);

			info = pvm_upkint(sgroup->pcoord,sgroup->nhosts,1);

			if (info < 0)
			{ 
				gs_delete_group(newname, sgroup_list, ngroups);
				PVM_FREE(newname);
				DO_ERROR_RTN( info, "gs_cachegroup" );
			}

			sgroup->maxhosts = sgroup->nhosts;
		}

		sgroup->staticgroup = STATIC;
		*rsgroup = sgroup;  /* set the pointer to the group struct     */
	}

	PVM_FREE(newname); /* free the malloc'ed memory */

	return (PvmOk);
}

/* ================ pvm_grphostinfo() ===============================     */
/* int info = pvm_grphostinfo(char *group, int hosttid, int *coord,
	int *nmem_onhost, int *nhosts)
	* return information about how many hosts are represented in the group and
	* which tid should be used to coordinate local (on-host) group operations
*/

int 
pvm_grphostinfo(group, hosttid, coord, nmem_onhost, nhosts) 
char *group;
int hosttid, *coord, *nmem_onhost, *nhosts;
{
	GROUP_STRUCT_PTR sgroup;
	int foo;
	int info;
	int rbuf, rc;
	int sbuf, state, stid;
	int x;
	int savectx;
	
	/* BGN_TRACE( TEV_GRPHOST, group, TEV_DID_HPT, &hosttid ); */

	info = gs_host_char(group, sgroup_list, &ngroups, hosttid,
			coord, nmem_onhost, nhosts, &foo); 

	if (info  != PvmOk )   /* info not local */
	{
		if ( (stid = gs_getgstid()) < 0) { /* find the server's ti */
			info = PvmSysErr;
			/* END_TRACE( TEV_GRPHOST, TEV_DID_CC, &info ); */
			return( info );
		}

		/* set context for dynamic groups */
		savectx = pvm_setcontext( SYSCTX_DG );

		sbuf = pvm_mkbuf(PvmDataDefault);

		rbuf = pvm_setrbuf(0);

		sbuf = pvm_setsbuf(sbuf); 

		if ((info =  pvm_pkstr(group)) < 0 )
			goto cleanup;
		
		if ((info = pvm_pkint(&hosttid,1,1)) < 0)
			goto cleanup;

		if ((info = pvm_send(stid,HOSTCHAR)) < 0)
			goto cleanup;

		if ((info = pvm_trecv(stid,HOSTCHAR,pvmgs_tmout)) <= 0)
			goto cleanup;

		if ((info = pvm_upkint(nhosts,1,1)) < 0) 
			goto cleanup;

		if ((info = pvm_upkint(nmem_onhost,1,1)) < 0) 
			goto cleanup;

		if ((info = pvm_upkint(coord,1,1)) < 0) 
			goto cleanup;
	
		if (( info = pvm_upkint(&state,1,1)) < 0) 
			goto cleanup;

		if (state == STATIC)       /* we've got new static group info */
			gs_cachegroup(sgroup_list, &ngroups, &sgroup);

cleanup:     

		sbuf = pvm_setsbuf(sbuf);

		rbuf = pvm_setrbuf(rbuf);

		pvm_freebuf(sbuf);

		pvm_freebuf(rbuf);

		/* restore user context */
		pvm_setcontext( savectx );
	}
	
	/* END_TRACE( TEV_GRPHOST, TEV_DID_CC, &info ); */

	return(info);
}

/* ================ pvm_grpvhostinfo() ===============================     */
/* int info = pvm_grpvhostinfo(char *group, int **coord,
int **nmem_onhost, int *nhosts)
* return #vectors# of information about how may hosts are represented in
* a group, the local coordinating tid for each host, and #procs on each host
*
* NOTE: for efficiency, the data in the vectors *nmem_host and 
*coord are valid only until the next call to pvm_grpvhostinfo 
*/

static int *gsu_coordv = (int *) NULL;
static int *gsu_onhostv = (int *) NULL;
static int gsu_maxhosts = 0;

int 
pvm_grpvhostinfo(group, coordv, nmem_onhostv, nhosts) 
char *group;
int **coordv, **nmem_onhostv, *nhosts;
{
	int foo;
	int info;
	int rbuf, rc;
	int sbuf, state, stid;
	int x;
	int savectx;

	GROUP_STRUCT_PTR sgroup;

	/* BGN_TRACE( TEV_GRPVHOST, group, TEV_DID_HPT, &hosttid ); */

	info = gs_host_all(group, sgroup_list, &ngroups, 
			coordv, nmem_onhostv, nhosts, &foo); 

	if (info  != PvmOk )   /* info not local */
	{
		if ( (stid = gs_getgstid()) < 0) { /* find the server's ti */
			info = PvmSysErr;
			/* END_TRACE( TEV_GRPHOST, TEV_DID_CC, &info ); */
			return( info );
		}

		/* set context for dynamic groups */
		savectx = pvm_setcontext( SYSCTX_DG );

		sbuf = pvm_mkbuf(PvmDataDefault);

		rbuf = pvm_setrbuf(0);

		sbuf = pvm_setsbuf(sbuf); 

		if ((info = pvm_pkstr(group)) < 0) 
			goto cleanup;

		if ((info = pvm_send(stid,HOSTCHARV)) < 0) 
			goto cleanup;

		if ((info = pvm_trecv(stid,HOSTCHARV,pvmgs_tmout)) <= 0) 
			goto cleanup;

		if ((info = pvm_upkint(nhosts,1,1)) < 0) 
			goto cleanup;

		if (*nhosts >  gsu_maxhosts)
		{
			if (gsu_coordv) 
				PVM_FREE(gsu_coordv);

			if (gsu_onhostv) 
				PVM_FREE(gsu_onhostv);

			gsu_coordv = (int *) PVM_ALLOC(*nhosts*sizeof(int), 
					"pvm_grp_hostv");

			gsu_onhostv = (int *) PVM_ALLOC(*nhosts*sizeof(int), 
					"pvm_grp_hostv");

			gsu_maxhosts = *nhosts;
		}

		if (gsu_onhostv == (int *) NULL || gsu_coordv == (int *) NULL)
		{
			info = PvmOutOfRes;
			goto cleanup;
		}

		if ((info =  pvm_upkint(gsu_onhostv,*nhosts,1)) < 0) 
			goto cleanup;

		if ((info =  pvm_upkint(gsu_coordv,*nhosts,1)) < 0) 
			goto cleanup;

		if ((info =  pvm_upkint(&state,1,1)) < 0) 
			goto cleanup;

		if (state == STATIC)       /* we've got new static group info */
			gs_cachegroup(sgroup_list, &ngroups, &sgroup);
cleanup:
	
		sbuf = pvm_setsbuf(sbuf);

		rbuf = pvm_setrbuf(rbuf);

		pvm_freebuf(sbuf);

		pvm_freebuf(rbuf);

		/* restore user context */
		pvm_setcontext( savectx );

		*nmem_onhostv = gsu_onhostv;

		*coordv = gsu_coordv;
	}
	
	/* END_TRACE( TEV_GRPVHOST, TEV_DID_CC, &info ); */

	return(info);
}
