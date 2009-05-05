
static char rcsid[] =
	"$Id: pvmgs_func.c,v 1.10 2005/08/22 15:11:52 pvmsrc Exp $";

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
#include "pvm3.h"
#include "pvmalloc.h"
#include "pvmgsd.h"
#include "bfunc.h"
#include <pvmtev.h>
#include "global.h"
#include "pvmmimd.h"   /* for TIDISNODE, TIDONNODE  macros */
#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

/* ================ gs_realloc_int_array ============================      */
/* reallocate an integer array if the nelems > maxelems.   Copy the
   old array to the new array and initialize the remaining part of the
   new array to init.
*/

int * 
gs_realloc_int_array(nelems, maxelems, array, inc, init, caller)
int nelems;
int *maxelems;
int *array;
int inc;
int init;
char *caller;
{
	int i;
	int *newarray, *newvalue;
	int oldmax; 
	int *value; 

	if (nelems <= *maxelems) 
		return(array);

	newarray = array;
	
	oldmax = *maxelems;

	*maxelems += inc;

	if ( (newarray = (int *) PVM_ALLOC(sizeof(int) * (*maxelems), caller))
				== (int *) NULL)	
	{
		fprintf(stderr, "could not allocate memory: %s\n", caller);
		return(newarray);
	}

	/* copy the old array contents to the new array, initialize the    */
	/* new memory with the specified init value                        */
	if (oldmax > 0)
		BCOPY(array, newarray, oldmax*sizeof(int));

	newvalue = newarray+oldmax;

	for (i = oldmax;  i < *maxelems; i++)
		*newvalue++ = init;

	if (array != (int *) NULL)  /* initial array could be NULL */
		PVM_FREE(array);

	return(newarray);
}

/* ================ gs_hash_init () ==================================     */
/* Initialize the hash list data structure                                 */ 

int 
gs_hash_init(hash_list)
GROUP_LIST_PTR hash_list;
{
	int g;

	for (g = 0; g < HASHSIZE; g++) 
	{
		hash_list->prev = (GROUP_LIST_PTR) NULL;
		hash_list->next = (GROUP_LIST_PTR) NULL;
		hash_list->group = (GROUP_STRUCT_PTR) NULL; 
		hash_list++;
	}

	return(PvmOk);
}

/* ================ gs_struct_init () ================================     */
/* Allocate and initialize a group structure                               */
/* Memory is allocated for the group name and the group name is copied     */
/* into the structure.                                                     */

GROUP_STRUCT_PTR
gs_struct_init(groupname)
char *groupname;
{

	GROUP_STRUCT_PTR rvalue;

	if ( (rvalue = (GROUP_STRUCT_PTR)PVM_ALLOC(sizeof(GROUP_STRUCT), GSNAME))
			==  (GROUP_STRUCT_PTR) NULL) 
		return (rvalue);    /* Out of resources                         */

	if ( (rvalue->name = 
				(char *) PVM_ALLOC(sizeof(char)*(strlen(groupname)+1), GSNAME))
				== (char *) NULL)
	{
		PVM_FREE(rvalue);

		fprintf(stderr, "gs_struct_init(), couldn't allocate memory\n");

		return (GROUP_STRUCT_PTR) NULL;
	}

	rvalue->len = strlen(groupname);
	strcpy(rvalue->name,groupname);	
	rvalue->ntids = 0;
	rvalue->tids = (int *) NULL; 
	rvalue->maxntids = 0;
	rvalue->barrier_count = -1;
	rvalue->barrier_reached = 0;
	rvalue->barrier_index = 0;
	rvalue->btids = (int *) NULL;
	rvalue->maxbtids = 0;
	rvalue->staticgroup = DYNAMIC;
	rvalue->sgroup_count = -1;
	rvalue->sgroup_reached = 0;
	rvalue->stids = (int *) NULL;
	rvalue->maxstids = 0;
	rvalue->ninfotids = 0;
	rvalue->infotids = (int *) NULL; 
	rvalue->maxinfotids = 0;
	rvalue->nhosts = 0;
	rvalue->np_onhost = (int *) NULL;
	rvalue->pcoord = (int *) NULL;
	rvalue->maxhosts = 0;

	return rvalue;
}      

/* ================ gs_struct_destroy () =============================     */
/*  Destroy a group structure                                              */
/* Memory is deallocated for tids, barrier tids, and infotids              */

int
gs_struct_destroy(group)
GROUP_STRUCT_PTR group;
{
	if (group != (GROUP_STRUCT_PTR) NULL ) 
	{
		if ( group->name != (char *) NULL)
			PVM_FREE(group->name);

		if ( group->btids != (int *) NULL)
			PVM_FREE(group->btids);

		if ( group->tids != (int *) NULL)
			PVM_FREE(group->tids);

		if ( group->infotids != (int *) NULL)
			PVM_FREE(group->infotids);

		if ( group->stids != (int *) NULL)
			PVM_FREE(group->stids);

		if ( group->np_onhost != (int *) NULL)
			PVM_FREE(group->np_onhost);

		if ( group->pcoord != (int *) NULL)
			PVM_FREE(group->pcoord);

		PVM_FREE(group);
	}

	return (PvmOk);

} 
/* ================ gs_hash_clear ()   ================================    */
/* Clear all group data held in the hash list                            */

int 
gs_hash_clear( hash_list, ngroups, size)
GROUP_LIST_PTR hash_list;
int *ngroups;
int size;
{
    int i;
	int j;

    GROUP_LIST_PTR current;
    GROUP_LIST_PTR prev;
    GROUP_LIST_PTR root;
    GROUP_STRUCT_PTR group;

	for (i=0; i < size; i++) /* go through entire list of groups   */
	{

		current = hash_list+i;

		root = current;

		current = current->next;

		root->next = (GROUP_LIST_PTR) NULL;  /* sever the list */

		while (current != (GROUP_LIST_PTR) NULL)
		{
			gs_struct_destroy(current->group);
			(*ngroups)--; 
			prev = current;
			current = current->next; 
			PVM_FREE(prev);
		} 
	}

	return (*ngroups);
}

/* ================ gs_hash_name () ===================================    */
/* Hash the group name to get the list entry for this group.               */
/*    optionally creates an element in the linked list                     */ 

GROUP_LIST_PTR
gs_hash_name(name, hash_list, ngroups, create)
char *name;
GROUP_LIST_PTR hash_list;
int *ngroups;
int create;
{
	char *key;

	int hash_index;
	int len = 0;
	int match; 
	int relation;
	int sum = 0; 

	GROUP_LIST_PTR current;
	GROUP_LIST_PTR listentry;
	GROUP_LIST_PTR prev;

	GROUP_STRUCT_PTR group;

	if (name == (char *) NULL || *name == '\0') 
		return( (GROUP_LIST_PTR) NULL);  /* Null group name error      */

	/* The following is the hash algorithm.                            */
	key = name; 
	while (*key != '\0')  /* sum all the characters in the string      */ 
	{    
		sum += INTERVAL(MINCHAR, *key, MAXCHAR);
		key++;
		len++;
	}

	hash_index = sum/len;
	hash_index = hash_index - MINCHAR;
  
	/* hash into the table and then do a linear search for the name    */
	/* if there is a collision                                         */
	/* names are kept in lexicographic order                           */

	prev = hash_list + hash_index;
	current = prev->next;
	listentry = (GROUP_LIST_PTR) NULL;

	while (current != (GROUP_LIST_PTR) NULL )
	{
		relation = strncmp(name, current->group->name, 
				MAX(current->group->len,len));    

		if ( relation == 0 )  /* strings are exactly equal             */
		{
			listentry = current;
			break; /* found the group name                             */
		}

		if (relation < 0) 
			break; /* group name does not exist                        */

		prev = current;             /* go to next entry in linked list */

		current = current->next;
	}

	if (create != CREATE || listentry != (GROUP_LIST_PTR) NULL)  
		return listentry;  /*return what we found, if anything         */

	/* we didn't find the group, but have been asked to create it      */

	if ( (group = gs_struct_init(name)) == (GROUP_STRUCT_PTR) NULL)
		return (GROUP_LIST_PTR) NULL;

	listentry = gs_list_insert(group, prev, current);

	if (listentry != (GROUP_LIST_PTR) NULL)
		(*ngroups)++;

	return(listentry);
}

/* ================ gs_list_insert () =================================    */
/* Insert the group structure into the hash list                           */

GROUP_LIST_PTR
gs_list_insert(group, prev, current)
GROUP_LIST_PTR prev;
GROUP_LIST_PTR current;
GROUP_STRUCT_PTR group;
{
    GROUP_LIST_PTR newentry; 
    GROUP_LIST_PTR vptr; 

	/* The member is inserted after prev                                */
	/*   to simplify logic: the head of the least always contains       */
	/*                     null group info, and is never destroyed      */

	newentry  = (GROUP_LIST_PTR) PVM_ALLOC(sizeof(GROUP_LIST),
			"gs_list_insert");

	if (newentry == (GROUP_LIST_PTR) NULL) /* error allocing memory     */
	{
		fprintf(stderr, "gs_list_insert failed to alloc memory \n");
		return (GROUP_LIST_PTR) NULL;
	}

	newentry->group = group;
	newentry->next = current;
	newentry->prev = prev;

	prev->next = newentry;  /* prev will always be non-null */

	if (current != (GROUP_LIST_PTR) NULL)
		current->prev = newentry;

	return (newentry);
}

/* ================ gs_group () =======================================    */
/* Return a pointer to a group structure, optionally create the group      */
/* structure.                                                              */

GROUP_STRUCT_PTR
gs_group(groupname, hash_list, ngroups, create)
char *groupname;
GROUP_LIST_PTR hash_list;
int *ngroups;
int create;
{

	GROUP_LIST_PTR listentry;

	listentry = gs_hash_name(groupname, hash_list, ngroups, create);

	if (listentry != (GROUP_LIST_PTR) NULL)
		return( listentry->group );
	else
		return (GROUP_STRUCT_PTR) NULL;
}

/* ================ gs_delete_group () ================================    */
/* Delete the group structure and free memory                              */

int
gs_delete_group(groupname, hash_list, ngroups)
char *groupname;
GROUP_LIST_PTR hash_list;
int *ngroups;
{

    GROUP_LIST_PTR listentry;

	listentry = gs_hash_name(groupname, hash_list, ngroups, NOCREATE);

	if (listentry == (GROUP_LIST_PTR) NULL)
		return (PvmNoGroup);

	listentry->prev->next = listentry->next;

	if (listentry->next != (GROUP_LIST_PTR) NULL)
		listentry->next->prev = listentry->prev;

	gs_struct_destroy(listentry->group);

	PVM_FREE(listentry);

	(*ngroups)--;

	return (PvmOk);
}
  

/* ================ gs_join() =======================================      */
int
gs_join(gname, tid, hash_list, ngroups) 
char *gname;
int tid;
GROUP_LIST_PTR hash_list;
int *ngroups;
{

	int hostindx;
	int inst;
	int thishost;

	GROUP_STRUCT_PTR group;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	if ( (group = gs_group(gname, hash_list, ngroups, CREATE))
			== (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);

	if (group->staticgroup == STATIC || group->staticgroup == STATICDEAD )
		return(PvmNotInGroup);   /* can't join a (dead) static group   */

	/* first check to see if the tid is already in the tid list        */
	for (inst = 0; inst < group->maxntids; inst++)
		if (group->tids[inst] == tid)
			return(PvmDupGroup); 

	/* not in the group; make sure there is room in the tid list to    */
	/* add the tid                                                     */

	group->tids = gs_realloc_int_array(group->ntids + 1, 
			&(group->maxntids), group->tids, DELTANTIDS, NOTID, "gs_join"); 

	if (group->tids == (int *) NULL)
		return(PvmNoMem);

	/* find the lowest available open slot to insert this tid.         */
	/* call to gs_realloc_int_array guarantees there is room.          */

	for (inst = 0; inst < group->maxntids; inst ++)
		if (group->tids[inst] == NOTID)
		{
			group->ntids++;
			group->tids[inst] = tid;
			break;
		}

	/* update the #procs on a particular host                          */

	thishost = gs_tidtohost(tid);

	hostindx = gs_binsearch(group->pcoord,group->nhosts,thishost,
			gs_tidtohost);

	if (hostindx < 0)  
		hostindx = gs_newhost(group,hostindx);

	if (tid < group->pcoord[hostindx] || group->pcoord[hostindx] == NOTID)
		group->pcoord[hostindx] = tid;     /*new coordinator       */

	group->np_onhost[hostindx] ++;        
         
	if (inst < group->maxntids) 
		return(inst);

	/* sanity check -- if here, something got screwed up               */
	fprintf(stderr, "gs_join: book keeping error, shouldn't happen\n");

	fprintf(stderr, "\t tid %d, ntids %d maxntids %d\n", tid, 
			group->ntids, group->maxntids);

	return(PvmMismatch);
}

/* ================ gs_leave() ======================================      */
/* 
	remove the tid from the group, remove the group if there are no
	more tids left in the group
*/
int
gs_leave(gname, tid, hash_list, ngroups)
char *gname;
int tid;
GROUP_LIST_PTR hash_list;
int *ngroups;
{
	int gid;
	int hostindx;
	int itmp, i;  
	int thishost; 

	GROUP_STRUCT_PTR group;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	if ( (group = gs_group(gname, hash_list, ngroups, NOCREATE))
			== (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);

	for (gid = 0; gid < group->maxntids; gid++)  /* find the tid       */
		if(group->tids[gid] == tid)
			break;

	if (gid == group->maxntids) /* if the tid is not there             */
		return(PvmNotInGroup);

	group->tids[gid] = NOTID;      /* delete it from the group         */

	group->ntids--;

	/* update the #procs on a particular host                          */
	thishost = gs_tidtohost(tid);

	hostindx = gs_binsearch(group->pcoord,group->nhosts,thishost, gs_tidtohost);
	if (hostindx >= 0)
	{
		if (-- group->np_onhost[hostindx] == 0)
			gs_delhost(group,hostindx);
		else if (group->pcoord[hostindx] == tid)  /* find new coordinator */
		{
			group->pcoord[hostindx] = NOTID;
			for (i = 0; i < group->maxntids; i++)
				if (gs_tidtohost(group->tids[i]) == thishost)
					if (group->tids[i] < group->pcoord[hostindx]
							|| group->pcoord[hostindx] == NOTID)
						group->pcoord[hostindx] = group->tids[i];
    
		}
	}

	if (group->staticgroup == STATIC) /* mark group as dead            */
	{

		group->staticgroup = STATICDEAD;

		/* release any tids waiting on a barrier.                      */
		pvm_initsend(PvmDataDefault);

		if (group->barrier_count > 0)
		{
			itmp = PvmNoGroup; /* send them an error                  */
			pvm_pkint(&itmp, 1, 1);
			pvm_mcast(group->btids, group->barrier_count, BARRIER);
			group->barrier_count = PvmNoGroup; 
		}
	}
 
	if( group->ntids <= 0)  /* delete the group if count is zero       */
		gs_delete_group(gname, hash_list, ngroups);

	return(PvmOk);
}


/* ================ gs_register() ===================================      */
/*     register the name with pvm so other processes can get it            */
/*       return -1 if can't register, 0 otherwise                          */

int
gs_register(name, tid)
char *name;
int tid;
{
	int cc;
	int sbuf,ssbuf;
 
	ssbuf = pvm_setsbuf(0);
 
	sbuf = pvm_initsend(PvmDataDefault);

	pvm_pkint(&tid,1,1);

	if ( (cc = pvm_putinfo(name, sbuf, PvmMboxDefault)) != 0 )
	{
		pvm_perror("Group Server already running?");
	}

	pvm_setsbuf(ssbuf);

	pvm_freebuf(sbuf);
 
	return(cc); /* Will be PvmOK if register succeeded, < 0 otherwise */
}

/* ================ gs_getinst() ====================================      */
/*    returns the instance associated with (tid,gname)                 */
int
gs_getinst(gname, tid, hash_list, ngroups, gstate)
char *gname;
int tid;
GROUP_LIST_PTR hash_list;
int *ngroups, *gstate;
{
	int gid;

	GROUP_STRUCT_PTR group;

	*gstate = DYNAMIC;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	group = gs_group(gname, hash_list, ngroups, NOCREATE);

	if (group == (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);

	for (gid = 0; gid < group->maxntids; gid++) /* find the tid        */
		if(group->tids[gid] == tid)
			break;
	if (gid == group->maxntids)      /* if the tid's not there         */
		return(PvmNotInGroup);

	if (group->staticgroup == STATIC)
		*gstate = STATIC;

	return(gid);
}

/* ================ gs_gettid() =====================================      */
/*    returns the tid associated with (inst, gname)                    */

int
gs_gettid(gname, inst, hash_list, ngroups, gstate)
char *gname;
int inst;
GROUP_LIST_PTR hash_list;
int *ngroups, *gstate;
{

	int rtid;

	GROUP_STRUCT_PTR group;

	*gstate = DYNAMIC;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	group = gs_group(gname, hash_list, ngroups, NOCREATE);

	if (group == (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);

	if (inst < 0 || inst > group->maxntids) /* check for valid inst    */
		return(PvmNoInst);

	if (group->staticgroup == STATIC)
		*gstate = STATIC;

	rtid = group->tids[inst];

	return( (rtid != NOTID ? rtid : PvmNoInst) );
}

/* ================ gs_gsize() =======================================      */
/*    returns the size of gname                                        */
int
gs_gsize(gname, hash_list, ngroups, gstate)
char *gname;
GROUP_LIST_PTR hash_list;
int *ngroups, *gstate;
{
	GROUP_STRUCT_PTR group;

	*gstate = DYNAMIC;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	group = gs_group(gname, hash_list, ngroups, NOCREATE);

	if (group == (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);

	if (group->staticgroup == STATIC)
		*gstate = STATIC;

	/* return the group size */

	return(group->ntids);
}

/* ================ gs_tidlist() ====================================      */
/*    returns the list of tids in the group                                */
int
gs_tidlist(gname, holes_not_allowed, hash_list, ngroups, gstate)
char *gname;
int holes_not_allowed;
GROUP_LIST_PTR hash_list;
int *ngroups, *gstate;
{
	int i;
	int cnt;
	int gi; 
	int hole_exists = 0;
	int ntids; 
	int *tids;  

	GROUP_STRUCT_PTR group;

	*gstate = DYNAMIC;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	group = gs_group(gname, hash_list, ngroups, NOCREATE);
	if (group == (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);

	if (group->staticgroup == STATICDEAD) /* no collectives allowed on */
		return(PvmNoGroup);               /* a dead static group       */

	ntids = group->ntids;

	if ( (tids = (int *) PVM_ALLOC(group->ntids * sizeof(int), "gs_tidlist()"))
			== (int *) NULL ) 
		return (PvmNoMem);

	for (i  = 0, cnt = 0; i < group->maxntids; i++)
	{
		if (group->tids[i] != -1)
			tids[cnt++] = group->tids[i];
		else
			if (i < ntids) hole_exists = 1;
				if (cnt > ntids) break;  /* don't want to overwrite mem  */
	}
	
	if (cnt != ntids)
	{
		fprintf(stderr, "gs_tidlist: cnt (%d) != ntids (%d) \n", cnt, 
				group->ntids);
		return(PvmSysErr);
	}
	
	if (holes_not_allowed && hole_exists) /* detected missing inst #s  */
		return(PvmNoInst); /* scatter and gather require no holes      */

	pvm_initsend(PvmDataDefault); /* pack 'em up                       */
	pvm_pkint(&ntids, 1, 1);
	pvm_pkint(tids,  cnt, 1);
	PVM_FREE(tids);

	if (group->staticgroup == STATIC)
		*gstate = STATIC;

	return (PvmOk);
}  
	
/* ================ gs_barrier() ====================================      */
/*    process a barrier request or a static group formation request        */
int
gs_barrier(gname, msgtag, cnt, tid, hash_list, ngroups)
char *gname;
int msgtag;
int cnt;
int tid;
GROUP_LIST_PTR hash_list;
int *ngroups;
{
	int i, info;
	int gstate;
	int pcoord;
	int nhosts, nmem_onhost; 
	
	GROUP_STRUCT_PTR group;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	group = gs_group(gname, hash_list, ngroups, NOCREATE);
	if (group == (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);
	
	if (group->staticgroup == STATICDEAD)
		return(PvmNoGroup);

	for (i = 0; i < group->maxntids; i++) /* tid is in group?          */
		if (group->tids[i] == tid) break;
			if (i == group->maxntids) 
				return(PvmNotInGroup);
	
	/* if it's the whole group the set the count to the # in the group */
	if (cnt == -1) 
		cnt = group->ntids;
	
	if (group->barrier_count == -1)  /* is it a new barrier ?       */
	{
		group->barrier_count = cnt;
		group->barrier_reached = 0;
		group->barrier_index = 0;
	}
	else 
	{
		if (cnt != group->barrier_count)  /* make sure counts match    */
			return(PvmMismatch);
	}
	
	/* store the tid */
	group->btids = gs_realloc_int_array(group->barrier_index + 1, 
			&(group->maxbtids), group->btids, DELTABTIDS, NOTID,
			"gs_barrier"); 

	group->btids[group->barrier_index++] = tid;

	if (msgtag == BARRIER)
		group->barrier_reached++;
	else  /* (msgtag == BARRIERV) */
	/* an on-host coordinator is representing nmem_onhost tasks in
	this barrier call */
	{
		gs_host_char(gname, hash_list, ngroups, gs_tidtohost(tid), 
				&pcoord, &nmem_onhost, &nhosts, &gstate);

		group->barrier_reached += nmem_onhost; 
	}
	
	/* if reached is bigger than count then there is a problem         */
	/* this would happen if count was zero                             */

	if (group->barrier_count < group->barrier_reached)
		return(PvmMismatch);
	
	/* if we're not at the barrier then return                         */
	if (group->barrier_count != group->barrier_reached)
		return(PvmOk);
	
	/* let everyone continue, send them the count                      */
	pvm_initsend(PvmDataDefault);
	pvm_pkint(&(group->barrier_count), 1, 1);
	pvm_mcast(group->btids, group->barrier_index, BARRIER);            
	
	group->barrier_count = -1;        /* reset the barrier             */

	return(PvmOk);
}
	
/* ================ gs_static() ====================================      */
/* Forms a static group based upon the tid list and size                  */
int 
gs_static(gname, size, tid, hash_list, ngroups)
char *gname;
int size;
int tid;
GROUP_LIST_PTR hash_list;
int *ngroups;
{
	int i, info;

	GROUP_STRUCT_PTR group;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	group = gs_group(gname, hash_list, ngroups, NOCREATE);
	if (group == (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);

	if (group->staticgroup == STATIC)
		return(PvmDupGroup);

	for (i = 0; i < group->maxntids; i++) /* tid is in group?          */
		if (group->tids[i] == tid) break;
			if (i == group->maxntids) 
				return(PvmNotInGroup);
	
	/* if it's the whole group then set the size to the # in the group */
	if (size == -1) 
		size = group->ntids;
	
	if (group->sgroup_count == -1)  /* first to form the static group  */
	{
		group->sgroup_count = size;
		group->sgroup_reached = 0;
	}
	else 
	{
		if (size != group->sgroup_count)  /* make sure counts match    */
			return(PvmMismatch);
	}
	
	/* store the tid */
	group->stids = gs_realloc_int_array(group->sgroup_reached + 1, 
			&(group->maxstids), group->stids, DELTABTIDS, NOTID, "gs_static"); 

	group->stids[group->sgroup_reached++] = tid;
	
	/* if reached is bigger than count then there is a problem         */
	/* this would happen if count was zero                             */

	if (group->sgroup_count < group->sgroup_reached)
		return(PvmMismatch);
	
	/* if we've not completed with every tid calling then return       */
	if (group->sgroup_count != group->sgroup_reached)
		return(PvmOk);
	
	/* let everyone continue, send them the count                      */
	group->staticgroup = STATIC;

	for (i=0; i < group->sgroup_count; i++)
		gs_newstaticcache(group->stids[i],gname, hash_list, ngroups,
				"gs_static");  /* record that tid has data   */

	pvm_initsend(PvmDataDefault);
	pvm_pkint(&(group->sgroup_count), 1, 1);
	pvm_pkint(&(group->staticgroup),1,1);
	gs_pkstaticinfo(group);      /* pack up the information */
	pvm_mcast(group->stids, group->sgroup_count, STATICGROUP);            
	PVM_FREE(group->stids); group->stids = (int *) NULL;

	return(PvmOk);
}
	
/* ================ gs_pkstaticinfo() =============================      */

int
gs_pkstaticinfo(group)
GROUP_STRUCT_PTR group;
{
	pvm_pkint(&(group->len),1,1);
	pvm_pkstr(group->name);
	pvm_pkint(&(group->ntids),1,1);
	pvm_pkint(&(group->maxntids),1,1);
	pvm_pkint(group->tids,group->maxntids,1);
	pvm_pkint(&(group->nhosts),1,1);
	pvm_pkint(group->np_onhost,group->nhosts,1);

	return(pvm_pkint(group->pcoord, group->nhosts,1));
	
}

/* ================ gs_newstaticcache() ============================      */

int
gs_newstaticcache(tid, groupname, hash_list, ngroups, caller)
int tid;
char *groupname;
GROUP_LIST_PTR hash_list;
int *ngroups;
char *caller;
{
	GROUP_STRUCT_PTR group;

	int i;

	group = gs_group(groupname, hash_list, ngroups, NOCREATE);
	if (group == (GROUP_STRUCT_PTR) NULL) 
		 return (0);

	group->infotids = gs_realloc_int_array( group->ninfotids + 1, 
			&(group->maxinfotids), group->infotids, DELTANTIDS, NOTID,
			caller ); 

	if (group->infotids != (int *)  NULL) 
	{
		for (i=0; i < group->ninfotids; i++)
			if (group->infotids[i] == tid) break;
				if (i == group->ninfotids) /* haven't sent info to this tid */
				{
					group->infotids[group->ninfotids] = tid;
					group->ninfotids++;
				}

		return (PvmgsNotCached);
	}

	return(PvmOk);
}
/* ================ gs_tidtohost() ============================      */
/* int hosttid = gs_tidtohost(int tid)                            
*     returns tid of host with following qualifications             
*       node tasks on mimd machines are considered to be on different
*       hosts that hosts programs. This is because the PGON internal
*       collective ops don't work across service and compute partitions.
*/

int
gs_tidtohost(tid)
int tid;
{
	int hostid;

	hostid = pvm_tidtohost(tid);

	if (TIDISNODE(tid))
		hostid = hostid | TIDONNODE;

	return (hostid);
} 
/* ================ gs_binsearch() ============================      */
/*  int index = gs_binsearch(array, nitems, target, mapfunc)
*              int *array;      integer array to search;
*              int nitems;      how many items in the array
*              int target;      value searching for;
*              int (*mapfunc)();   func to map array value before compare
*
*     returns
*              index >= 0     if found
*                       -n    -(index+1) where target should reside
*/ 

int
gs_binsearch(array,nitems,target,mapfunc)
int *array;
int  nitems; 
int target; 
int (*mapfunc)();
{
	int gs_retint();
	int maxidx, maxval, mididx, midval, minidx, minval;
	int tmpidx;

	if (nitems < 1) 
		return (-1);  /* make sure there is at least 1  */

	minidx = 0;

	maxidx = nitems-1; 

	if (!mapfunc)
		mapfunc = gs_retint;

	if ( target < (minval = (*mapfunc)(*(array+minidx))) ) 
		return -1;

	if ( target > (maxval = (*mapfunc)(*(array+maxidx))) )
		return -(nitems+1);

	mididx = minidx; midval = minval;

	if (midval == target) return(mididx); /* single element */ 
	{
		while (minidx != maxidx)
		{
			if (midval == target) 
				return(mididx); 
			if (maxval == target) 
				return(maxidx);

			if ((tmpidx = (minidx + maxidx)/2) != mididx)
				mididx = tmpidx;
			else
				return (-(mididx  + 2));

			midval = (*mapfunc)(*(array + mididx));

			if (midval > target)
			{ 
				maxidx = mididx;
				maxval = midval;
			}
			else
			{
				minidx = mididx;
				minval = midval; 
			}
		}
	}

	return -1;  /* not reached */
}

/* ================ gs_retint() ============================      */
/* return its integer argument, used by gs_binsearch when mapfunc is null */

int 
gs_retint(arg)
int arg;
{
	return(arg);
}

/* ================ gs_newhost() ============================      */
/* insert new host information in the group structure              
*     index used is the one returned by gs_binsearch
*/ 

int
gs_newhost(group,index)
GROUP_STRUCT_PTR group;
int index;
{
	int i;
	int oldmaxhosts; 

	if (index >= 0) 
		return(index);

	oldmaxhosts = group->maxhosts;

	group->np_onhost = gs_realloc_int_array(group->nhosts + 1, 
			&(group->maxhosts), group->np_onhost, DELTANTIDS, 0, "gs_newhost"); 

	group->maxhosts = oldmaxhosts;

	group->pcoord = gs_realloc_int_array(group->nhosts + 1, 
			&(group->maxhosts), group->pcoord, DELTANTIDS, NOTID, "gs_newhost"); 
	index = (-1 - index);     /* where new info should be stored */

	for (i = group->nhosts; i > index; i --) 
	{
		group->pcoord[i] = group->pcoord[i-1];
		group->np_onhost[i] = group->np_onhost[i-1]; 
	}

	group->pcoord[i] = NOTID;

	group->np_onhost[i] = 0;

	group->nhosts++;

	return(index);            /* return where this host info can be found */
}
/* ================ gs_delhost() ============================      */
/* delete host information from group structure              
*     index used is the one returned by gs_binsearch
*/ 

int 
gs_delhost(group,index)
GROUP_STRUCT_PTR group;
int index;
{
	int i;

	if (index < 0 )
		 return (0);

	for (i = index; i < group->nhosts -1; i++)
	{
		group->np_onhost[i] = group->np_onhost[i+1];
		group->pcoord[i] = group->pcoord[i+1];
	}

	group->nhosts--;

	return(0);
}
	
	
/* ================ gs_host_char() ============================      */
/* Characterize a host with the number of procs and the local coordinator
* of group functions
*/

int 
gs_host_char(gname, hash_list, ngroups, hosttid, 
pcoord, nmem_onhost, nhosts, gstate)
char *gname;
GROUP_LIST_PTR hash_list;
int *ngroups;
int hosttid; 
int *pcoord;
int *nmem_onhost; 
int *nhosts; 
int *gstate;
{
	int hostindx;

	GROUP_STRUCT_PTR group;

	*nmem_onhost = 0; 
	*nhosts = 0; 
	*gstate = DYNAMIC; 
	*pcoord = NOTID;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	group = gs_group(gname, hash_list, ngroups, NOCREATE);
	if (group == (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);
	
	hostindx = gs_binsearch(group->pcoord,group->nhosts,hosttid, gs_tidtohost);
	if (hostindx < 0)
		return(PvmNoGroup);

	*nmem_onhost = group->np_onhost[hostindx];

	*pcoord = group->pcoord[hostindx];

	*nhosts = group->nhosts;

	*gstate = group->staticgroup;

	return(PvmOk);
}
/* ================ gs_host_all() ============================      */
/* vectors of information about all hosts in a group 
*/

int 
gs_host_all(gname, hash_list, ngroups, pcoordv, nmem_onhostv, nhosts, gstate)
char *gname;
GROUP_LIST_PTR hash_list;
int *ngroups;
int **pcoordv; 
int **nmem_onhostv; 
int *nhosts; 
int *gstate;
{
	int hostindx;

	GROUP_STRUCT_PTR group;

	*nmem_onhostv = (int *) NULL; 
	*nhosts = 0; 
	*gstate = DYNAMIC; 
	*pcoordv = (int *) NULL;

	/* check for valid group name */
	if (gname == (char*) 0 || *gname == '\0')
		return(PvmNullGroup);

	group = gs_group(gname, hash_list, ngroups, NOCREATE);
	if (group == (GROUP_STRUCT_PTR) NULL)
		return(PvmNoGroup);

	*nmem_onhostv = group->np_onhost;
	*pcoordv = group->pcoord;
	*nhosts = group->nhosts;
	*gstate = group->staticgroup;

	return(PvmOk);
}
