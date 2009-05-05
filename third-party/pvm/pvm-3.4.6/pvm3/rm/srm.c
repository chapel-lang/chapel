
static char rcsid[] =
	"$Id: srm.c,v 1.6 2009/01/23 01:20:37 pvmsrc Exp $";

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, K. S. London, R. J. Manchek,
 *    P. Mucci, P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
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
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <sys/time.h>
#include <signal.h>
#include "pvm3.h"
#include "pvmproto.h"
#include "pvmtev.h"
#include "../src/global.h"
#include "srm.h"


#ifdef DEBUG_RM_MORE
#ifndef DEBUG_MORE
#define DEBUG_MORE
#endif
#endif

static void my_handler();

char *pvmnametag();

int
main(argc, argv)
int     argc;
char	*argv[];
{
  HOST_HEAD.next = NULL;
  HOST_HEAD.prev = NULL;
  TASK_HEAD.next = NULL;
  TASK_HEAD.prev = NULL;
  no_signal = TRUE;
  loop_init();
  return 0;
}

void
start_pvm()
{
	int			cc, i, narch, nhost, ntask, se;
	struct pvmhostinfo 	*hosts;
        struct pvmtaskinfo 	*tasks;

	pvm_setopt( PvmResvTids, 1 );
	pvm_setopt( PvmRoute, PvmDontRoute );
	se = pvm_setopt( PvmAutoErr, 0 );
	cc = pvm_start_pvmd( 0, (char **) "", FALSE );
	if ( cc < 0 )
        {
                if ( cc == PvmDupHost )
                {
                        printf( "Connecting to PVMD already running... \n" );
                        fflush( stdout );
                }

                else
                {
                        pvm_perror( "Can't Start PVM" );
                        exit( -1 );
                }
        }
        else
        {
                printf( "New PVMD started...\n" );
                fflush( stdout );
        }

        pvm_setopt( PvmResvTids, 1 );

        /* Get My TID */

        MYTID = pvm_mytid();

        if ( MYTID < 0 )
        {
                pvm_perror( "Error Joining PVM" );

                exit( -1 );
        }

        else
                printf( "SRM connected as TID=0x%x.\n", MYTID );
        if (pvm_reg_rm(&our_host) != PvmOk)
        {
                pvm_perror( "Error registering as Resource Manager" );
                exit( -1 );
        }
        else
                printf( "SRM registered as Resource Manager.\n");

		/* signal(SIGTERM, SIG_IGN); */
        signal(SIGINT, SIG_IGN);

        /* if (signal(SIGINT, my_handler) == SIG_ERR) */
			/* printf ("Oops problem\n"); */

		/* Need to do a config otherwise the first host gets messed up 
		if we start the daemon */
         pvm_config ( &nhost, &narch, &hosts );
         for ( i = 0; i < nhost; i++ )
                  add_host ( &(hosts[i]) );
         pvm_tasks(0, &ntask, &tasks);
         for ( i = 0; i < ntask; i++ )
                  add_task( &(tasks[i]));
}


int
loop_init()
{
	int buf, id, found, handled, i, msgtag, msgtid;
	struct pvmminfo info;


	start_pvm ( );

	while ( no_signal ) {
		handled = 0;
		buf = pvm_recv(-1, -1);
		pvm_bufinfo(buf, 0, &msgtag, &msgtid);
		pvm_getminfo(buf, &info);

		for ( i=0; i < MAX_MESSAGE; i++ )
			if (msgtag == Messages[i].msg_tag) {
#ifdef MESSAGE_ON
			  printf ("PVMd message is %s from t%x\n",
				pvmnametag(msgtag,&found), msgtid );
#endif
			Messages[i].code(msgtid, &info);
			handled = 1;
			break;
			}
#ifdef DEBUG_RM
		if ( !handled )
			printf ("Unexpected message with tag %d from t%x\n",
				msgtag, msgtid );
#endif
	}
	return 1;
}
	

int	
sm_spawn(who, info)
int	who;
struct	pvmminfo *info;
{
        char    arch[100], buf[1000], new_buf[1000], where[100];
        int     arg_count, count, dest, env_count, flag, i, j, new_tid;
        int	ret_count, sbuf, trc_ctx, trc_tag, trc_tid, out_ctx, out_tag;
	int	out_tid, one = 1;
        char    **argv, **env;
        struct  pvmtaskinfo             tinfo;

	pvm_upkstr ( buf );
	pvm_upkint ( &flag, 1, 0 );
	pvm_upkstr ( where );
	pvm_upkint ( &count, 1, 0 );

	arg_count =	unpack_list (&argv);

	pvm_upkint ( &out_tid, 1, 1 );
	pvm_upkint ( &out_ctx, 1, 1 );
	pvm_upkint ( &out_tag, 1, 1 );
	pvm_upkint ( &trc_tid, 1, 1 );
	pvm_upkint ( &trc_ctx, 1, 1 );
	pvm_upkint ( &trc_tag, 1, 1 );
			
	env_count  =	unpack_list (&env);
#ifdef DEBUG_RM_MORE
	printf ("Buf %s, Flag %d, Where %s, Out_Tid t%x, Out_ctx %x\n",
		buf, flag, where, out_tid, out_ctx);
	printf ("Out_tag %x, Trc_tid %x, Trc_ctx %x, Trc_tag %x\n",
		out_tag, trc_tid, trc_ctx, trc_tag );
#endif
	pvm_initsend(PvmDataDefault);
	pvm_pkint ( &count, 1, 1);
	pvm_setminfo ( pvm_getsbuf(), info );
	
	for ( i = 0; i < count; i++ ) {

		if ( flag & PvmTaskArch )
			dest = select_host ( flag, arch );
		else 
			dest = select_host ( flag, where );
	
		if ( dest ) {
			sbuf = pvm_setsbuf (pvm_mkbuf(PvmDataDefault));
			pvm_pkint(&who, 1, 1);
                        pvm_pkstr(buf);
                        pvm_pkint(&flag, 1, 1);
                        pvm_pkint(&one, 1, 1);
			pvm_pkint (&arg_count, 1, 1);
			for (j = 0; j < arg_count; j++) 
				pvm_pkstr ( argv[j] );
			pvm_pkint(&out_tid, 1, 1);
			pvm_pkint(&out_ctx, 1, 1);
                        pvm_pkint(&out_tag, 1, 1);
                        pvm_pkint(&trc_tid, 1, 1);
			pvm_pkint(&trc_ctx, 1, 1);
                        pvm_pkint(&trc_tag, 1, 1);
                        pvm_pkint(&env_count, 1, 1);
			sprintf(new_buf, "PVMTMASK=%s", pvmctrc.tmask);
                        pvm_pkstr(new_buf);
                        sprintf(new_buf, "PVMTRCBUF=%d", pvmctrc.trcbuf);
                        pvm_pkstr(new_buf);
                        sprintf(new_buf, "PVMTRCOPT=%d", pvmctrc.trcopt);
                        pvm_pkstr(new_buf);
                        sprintf(new_buf, "PVMCTX=0x%x", pvmmyctx);
                        pvm_pkstr(new_buf);

                        for (j = 0; j < env_count; j++) 
                                pvm_pkstr( env[j] );
			pvm_setminfo ( pvm_getsbuf(), info );
                        pvm_send(dest | TIDPVMD, SM_EXEC);

                        pvm_freebuf(pvm_setsbuf(sbuf));
                        pvm_recv(-1, SM_EXECACK);
                        pvm_upkint(&ret_count, 1, 1);
                        if (ret_count != 1) {
                                printf("sm_spawn: unexpected return count: %d\n"
, ret_count);
                        }
                        pvm_upkint(&new_tid, 1, 1);
                        pvm_pkint(&new_tid, 1, 1);

                        tinfo.ti_tid = new_tid;
                        tinfo.ti_ptid = who;
                        tinfo.ti_host = dest;
/*
                        tinfo.ti_flag = 0x00000000;
*/
                        tinfo.ti_flag = flag;
                        tinfo.ti_a_out = buf;
                        tinfo.ti_pid = 0; 
			add_task(&tinfo);
                } else {
                        new_tid = PvmNoHost;
                        pvm_pkint(&new_tid, 1, 1);
                }
        }
        pvm_send(who, SM_SPAWN);
        free_list(argv, arg_count - 2);
        free_list(env, env_count);
	return 0;
}

int
sm_exec(who, info)
int	who;
struct	pvmminfo	*info;
{
#ifdef DEBUG_RM
	printf("sm_exec: Should mot be called!\n");
#endif
	return 0;
}

int
sm_execack(who, info)
int 	who;
struct	pvmminfo	*info;
{
	int	count, i;
	struct  pvmtaskinfo	task;

	pvm_upkint ( &count, 1, 1 );
        pvm_upkint(&(task.ti_tid), 1, 1);
        task.ti_ptid = 0;
        task.ti_host = pvm_tidtohost( task.ti_tid );
        task.ti_flag = 0x00000000;
        task.ti_a_out = "\0";
        task.ti_pid = 0;
        add_task( &task );
	return 0;
}

int
sm_task(who, info)
int	who;
struct	pvmminfo	*info;
{
        int error = 0;
		int where;
	task_type       *task;
	host_type	*host;


        pvm_upkint(&where, 1, 1);
        pvm_initsend(PvmDataDefault);
	pvm_setminfo ( pvm_getsbuf(), info );
        pvm_pkint(&error, 1, 1 );
	if ( where == 0 )
	{
		task = find_task ( who );
		if ( task )
		{
#ifdef DEBUG_RM_MORE
			printf("Sm_task calling find_host\n");
			fflush(stdout);
#endif
			host = find_host ( task->entry.ti_host );
			where = host->entry.hi_tid;
		}
	}
        pack_task_list(where);
        pvm_send(who, SM_TASK);
	return 0;
}

int
sm_config(who, info)
int	who;
struct pvmminfo *info;
{
        pvm_initsend(PvmDataDefault);
	pvm_setminfo ( pvm_getsbuf(), info );
        pack_host_list();
        pvm_send(who, SM_CONFIG);
	return 0;
}

int
sm_addhost(who, info)
int	who;
struct pvmminfo	*info;
{
        int             	count,  i, narches;
        char    		new_host_arch[100], new_host_name[100];
        char    		**host_names;
        struct  pvmhostinfo     newhost;


        count = unpack_list(&host_names);
        pvm_initsend(PvmDataDefault);
	pvm_setminfo ( pvm_getsbuf(), info );
        pvm_pkint(&count, 1, 1);

        for (i = 0; i < count; i++) 
                pvm_pkstr( host_names[i] );

        pvm_send(our_host->hi_tid | TIDPVMD, SM_ADD);
	pvm_recv(-1, SM_ADDACK);
	

        pvm_upkint(&count, 1, 1);
        pvm_upkint(&narches, 1, 1);

        pvm_initsend(PvmDataDefault);
	pvm_setminfo ( pvm_getsbuf(), info );

        pvm_pkint(&count, 1, 1);
        pvm_pkint(&narches, 1, 1);


#ifdef DEBUG_RM_MORE
	printf ("Count %d: Arch: %d  Tid %d\n", count, narches, newhost.hi_tid);
#endif
        if ( count > 0 ) {
                newhost.hi_name = new_host_name;
                newhost.hi_arch = new_host_arch;
                for (i = 0; i < count; i++) {
                        pvm_upkint(&newhost.hi_tid, 1, 1);
#ifdef DEBUG_RM
			printf ("Newhost hi_tid t%x\n", newhost.hi_tid);
#endif
			if ( newhost.hi_tid > 0 ) /* Error */
			{
                        	pvm_upkstr( new_host_name );
                        	pvm_upkstr( new_host_arch );
                        	pvm_upkint(&newhost.hi_speed, 1, 1);
				pvm_upkint(&newhost.hi_dsig, 1, 1);
#ifdef DEBUG_RM_MORE
				printf ("%s %s %d sig%x\n",
				new_host_name,new_host_arch,newhost.hi_speed,
				newhost.hi_dsig);
#endif
                        	add_host( &newhost );
			}
                        pvm_pkint(&newhost.hi_tid, 1, 1);
                        pvm_pkstr( new_host_name );
                        pvm_pkstr( new_host_arch );
                        pvm_pkint(&newhost.hi_speed, 1, 1);
			pvm_pkint(&newhost.hi_dsig, 1, 1);
                }
        }

        pvm_send(who, SM_ADDHOST);
        free_list(host_names, count);
	return 0;
}

int
sm_delhost(who, info)
int	who;
struct  pvmminfo	*info;
{
        char    		**host_names;
        int             	count, i, rc;
        int             	*status;
	struct	pvmminfo	temp_info;

	temp_info = *info;

        count = unpack_list(&host_names);
        status = (int *) malloc( count * sizeof(int) );

	/* Can't just use rc since we recursively call pvm_delhosts KSL */

        rc = pvm_delhosts( host_names, count, status );
	pvm_initsend(PvmDataDefault);
	pvm_setminfo ( pvm_getsbuf(), &temp_info );

        if ( rc == 0 ) /* XXX Look just don't ask KSL */
		pvm_pkint(&count, 1, 1);
	else
        	pvm_pkint(&rc, 1, 1);
	
	pvm_pkint(status, 1, 1);
	pvm_send ( who, SM_DELHOST);
        free(status);
        free_list(host_names, count);
	return 0;
}

int 
sm_add(who, info)
int	who;
struct	pvmminfo	*info;
{
#ifdef DEBUG_RM
	printf("sm_add: Should not be called!\n");
#endif
	return 0;
}

int 
sm_addack(who, info)
int	who;
struct	pvmminfo	*info;
{
#ifdef DEBUG_RM
	printf("sm_addack: Should not be called!\n");
#endif
	return 0;
}

int 
sm_notify(who, info)
int	who;
struct	pvmminfo	*info;
{
        int             count, ctx, flags, code, i, notify_tid, vals[200];

        pvm_upkint(&flags, 1, 1);
	pvm_upkint(&ctx, 1, 1);
        pvm_upkint(&code, 1, 1);
        pvm_upkint(&count, 1, 1);

#ifdef DEBUG_RM_MORE
	printf ("Flags %x, Who t%x, Code %d, Vals %x, Ctx %d, Count %d\n",
		flags, who, code, vals[0], ctx, count);
#endif
	if ( flags & PvmNotifyCancel ) {
        	for ( i = 0; i < num_notifys; i++ )
            		if ( notifylist[i].for_who == who )
                		del_notification(notifylist[i].for_who,
                   		    notifylist[i].on_tid);
	}
        else if ( flags == PvmHostAdd ) {
                new_notification(flags, who, code, count, ctx);
        } 
	else {
                for (; count > 0; count--) {
                        pvm_upkint(&notify_tid, 1, 1);
#ifdef DEBUG_RM_MORE
		        printf("Setting up notification with flags %d, who t%x, code %d, notify_tid %d, ctx %d", flags, who, code,count, ctx);
#endif
                        new_notification(flags, who, code, notify_tid, ctx);
                }
        }
	return 0;
}

int 
sm_taskx(who, info)
int	who;
struct	pvmminfo	*info;
{
	int	i, status, tid, error;

	pvm_upkint (&tid, 1, 1);

	error = delete_task ( tid );

        send_notification( PvmTaskExit, tid );
	pvm_upkint (&status, 1, 1);
#ifdef DEBUG_RM
	printf ("sm_taskx: Task t%x, exit with status 0x%x\n", tid, status);
#endif
	return 0;
}

int 
sm_hostx(who, info)
int	who;
struct	pvmminfo	*info;
{
	int	error, i, tid;
	
	pvm_upkint (&tid, 1, 1);
	tid &= ~TIDPVMD; /*Quick and Dirty fix */
#ifdef DEBUG_RM_MORE
	printf ("Attepmpting to delete host t%x\n", tid );
#endif
	send_notification(PvmHostDelete, tid);
	error = delete_host ( tid );
#ifdef DEBUG_RM
	if ( error != -1 )
		printf ("sm_hostx: Host t%x deleted.\n", tid);
#endif
	return 0;
}

int
sm_handoff(who, info)
int	who;
struct	pvmminfo	*info;
{
#ifdef DEBUG_RM
	printf ("sm_handoff: This should not be called!\n");
#endif
	return 0;
}

int
sm_sched(who, info)
int	who;
struct	pvmminfo	*info;
{
	int		error;

	pvm_initsend ( PvmDataDefault );
	pvm_setminfo ( pvm_getsbuf(), info );
	pvm_pkint ( &error, 1, 1 );
	pvm_send (who, SM_SCHED);
	return 0;
}

int
sm_sthost(who, info)
int	who;
struct	pvmminfo	*info;
{
#ifdef DEBUG_RM
	printf ("sm_sthost: This should not be called!\n");
#endif
	return 0;
}

int
sm_sthostack(who, info)
int	who;
struct	pvmminfo	*info;
{
#ifdef DEBUG_RM
	printf ("sm_sthostack: This should not be called!\n");
#endif
	return 0;
}

int
add_host (host)
struct pvmhostinfo	*host;
{
	host_type	*next_host, *new_host, *temp_host;
	bool		found_arch = FALSE;
	bool		found_host = FALSE;
	
	temp_host = &(HOST_HEAD);
	for (next_host=HOST_HEAD.next;next_host;next_host=next_host->next)
	{
	     if(!found_arch&&!strcmp( next_host->entry.hi_arch,host->hi_arch))
			found_arch = TRUE;
		temp_host = next_host;
		if ( next_host->entry.hi_tid == host->hi_tid )
		{
			found_host = TRUE;
			break;
		}
	}

	if ( found_host )
	{
#ifdef DEBUG_RM
		printf ("Host t%x already in the list.\n", host->hi_tid);
#endif
		return -1;
	}
	
	if ( !found_arch )
		num_arches++;
	
	new_host = (host_type *) malloc (sizeof (host_type) );
	temp_host->next = new_host;
	new_host->prev = temp_host;
	new_host->next = NULL;
	new_host->load = 0;
	new_host->entry.hi_tid	=	host->hi_tid;
	new_host->entry.hi_name	=	(char *) strdup(host->hi_name);
	new_host->entry.hi_arch =	(char *) strdup(host->hi_arch);
	new_host->entry.hi_speed=	host->hi_speed;
	new_host->entry.hi_dsig	=	host->hi_dsig;
	send_notification(PvmHostAdd, host->hi_tid);
        num_hosts++;
#ifdef DEBUG_RM
	printf ( "Adding host t%x\n", host->hi_tid );
#endif
#ifdef DEBUG_RM_HOSTS
	print_host_contents();
#endif
	return 0;
}

int
add_task (task)
struct pvmtaskinfo	*task;
{
	task_type	*next_task, *new_task, *temp_task;
	host_type	*host;

	temp_task = &TASK_HEAD;
	for(next_task=TASK_HEAD.next;next_task;next_task=next_task->next)
	{
		if (next_task->entry.ti_tid == task->ti_tid)
		{ 
#ifdef DEBUG_RM
			printf ("Task already added\n");
#endif
			return -1;
		}
		temp_task = next_task;
	}
	new_task = (task_type *) malloc (sizeof (task_type) );
	temp_task->next = new_task;
	new_task->prev = temp_task;
	new_task->next = NULL;
	new_task->entry.ti_tid	=	task->ti_tid;
	new_task->entry.ti_ptid	=	task->ti_ptid;
	new_task->entry.ti_host =	task->ti_host;
	new_task->entry.ti_flag =	task->ti_flag;
	new_task->entry.ti_a_out=	(char *) strdup( task->ti_a_out );
	new_task->entry.ti_pid  =	task->ti_pid;
#ifdef DEBUG_RM_MORE
	printf("add_task calling find_host\n");
	fflush(stdout);
#endif
	host = find_host( task->ti_host );
	if ( host )
		host->load++;
#ifdef DEBUG_RM
	else
		printf ("Hmmmm no host found for t%x", task->ti_host );
	printf ("Adding task t%x\n", task->ti_tid );
#endif
#ifdef DEBUG_RM_TASKS
	print_task_contents();
#endif
	return 0;
}

int
select_host (flag, where)
int	flag;
char *	where;
{
	host_type  *best_host, *next_host;

	/* Put load balancing code here */
	next_host = HOST_HEAD.next;
	best_host = HOST_HEAD.next;
	for ( ; next_host ; next_host = next_host->next ) {
		if ( (flag & PvmTaskArch) && 
			strcmp(where, next_host->entry.hi_name) ) 
				continue;
		if ( (flag & PvmTaskHost) &&
			strcmp(where, next_host->entry.hi_name) )
				continue;
		if ( next_host->load < best_host->load )
			best_host = next_host;
	}
#ifdef MESSAGE_ON
	printf ("Host selected: %s\n", best_host->entry.hi_name );
#endif
#ifdef DEBUG_RM_MORE
	printf ("Host tid: t%x\n", best_host->entry.hi_tid);
	fflush(stdout);
#endif
	return best_host->entry.hi_tid;
}

int
delete_host ( tid )
int	tid;
{
	host_type	*next, *next_host;	
	bool		found = FALSE;
	
	for ( next = HOST_HEAD.next; next; next = next->next )
		if ( next->entry.hi_tid == tid )
		{
			found = TRUE;
			break;
		}
	
	if ( !found )
	{
#ifdef DEBUG_RM
		printf ( "delete_host:  Host t%x not found.\n\r", tid );
#endif
		return -1;
	}

	found = FALSE;
	for (next_host=HOST_HEAD.next;next_host;next_host=next_host->next)
		if (!strcmp (next_host->entry.hi_arch, next->entry.hi_arch) )
		{
			found = TRUE;
			break;
		}
	if ( !found )
		num_arches--;
	next->prev->next = next->next;
	if ( next->next )
		next->next->prev = next->prev;
	free ( next );
	num_hosts--;
        return 0;
}

int
delete_task ( tid )
int	tid;
{
	host_type	*host;
	task_type	*next;
	bool		found = FALSE;
	
	for (	next = TASK_HEAD.next; next; next = next->next)
		if ( next->entry.ti_tid == tid )
		{
			found = TRUE;
			break;
		}

	if ( !found )
	{
#ifdef DEBUG_RM
		printf ("delete_task: Task t%x not found.\n", tid );
#endif
		return -1;
	}
#ifdef DEBUG_RM_MORE
	printf("delete_task calling find_host\n");
	fflush(stdout);
#endif
	host = find_host ( next->entry.ti_host );
	if ( host )
		host->load--;
#ifdef DEBUG_RM
	else
		printf ("Hmmm host t%x not found\n", next->entry.ti_host );
#endif
	next->prev->next = next->next;
	if ( next->next )
		next->next->prev = next->prev;
	free ( next );
	return 0;
}

int
unpack_list(str_list)
char    ***str_list;
{
        int             i, count = 0;
        char    	temp_str[1000];

        pvm_upkint( &count, 1, 1 );
        *str_list = (char **) malloc( (count + 1) * sizeof(char *));
        for (i = 0 ; i < count; i++ ) {
                pvm_upkstr( temp_str );
                (*str_list)[i] = (char *) strdup( temp_str );
        }
        (*str_list)[i] = 0;
        return count;
}

int
free_list(str_list, count)
char    **str_list;
int             count;
{
        int             i;

        for (i = 0; i < count; i++) {
                free(str_list[i]);
        }
        free(str_list);
	return 0;
}

int
pack_host(host)
struct pvmhostinfo      *host;
{
        pvm_pkint(&(host->hi_tid), 1, 1);
        pvm_pkstr(host->hi_name);
        pvm_pkstr(host->hi_arch ? host->hi_arch : "" );
        pvm_pkint(&(host->hi_speed), 1, 1);
	pvm_pkint(&(host->hi_dsig), 1, 1);
	return 0;
}


int
pack_host_list()
{
        host_type             *next_host;

        pvm_pkint(&num_hosts, 1, 1);
        pvm_pkint(&num_arches, 1, 1);
	for ( next_host = HOST_HEAD.next;next_host;next_host=next_host->next)
                pack_host(&(next_host->entry));
	return 0;
}

int
pack_task(task)
task_type         *task;
{
        pvm_pkint(&(task->entry.ti_tid), 1, 1);
        pvm_pkint(&(task->entry.ti_ptid), 1, 1);
        pvm_pkint(&(task->entry.ti_host), 1, 1);
        pvm_pkint(&(task->entry.ti_flag), 1, 1);
        pvm_pkstr(task->entry.ti_a_out);
        pvm_pkint(&(task->entry.ti_pid), 1, 1);
	return 0;
}


int
pack_task_list(where)
int             where;
{
	task_type	*next_task;
	host_type	*host;

#ifdef DEBUG_RM_MORE
	printf("pack_task_list calling find host\n");
	fflush(stdout);
#endif
        host = find_host(where);
	for (next_task=TASK_HEAD.next; next_task; next_task = next_task->next)
	{
		if ( where == 0 || (host != NULL && 
			next_task->entry.ti_host == host->entry.hi_tid ) ||
			( next_task->entry.ti_tid == where ) )
                        	pack_task(next_task);
        }
	return 0;
}

host_type *
find_host(tid)
int             tid;
{
	host_type	*next_host;

	for ( next_host = HOST_HEAD.next;next_host;next_host=next_host->next )
	{
		if ( next_host->entry.hi_tid == tid )
			return next_host;
	}
	return HOST_HEAD.next; /* XXX Needs to be fixed */
}

task_type *
find_task(tid)
int	tid;
{
	task_type 	*next_task;
	for ( next_task = TASK_HEAD.next;next_task;next_task=next_task->next)
        {
                if ( next_task->entry.ti_tid == tid )
                        return next_task;
        }
        return NULL; /* XXX Needs to be fixed */
}

int
send_notification(kind, on_tid)
int             kind, on_tid;
{
        int             	i, sbuf;
        struct  pvmhostinfo     *notify_host;
	host_type		*host;

        for (i = 0; i < num_notifys; i++) {
                if (notifylist[i].kind == kind && ((kind == PvmHostAdd) ||
   			(notifylist[i].on_tid == on_tid))) {
                        sbuf = pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
                        if (kind == PvmTaskExit) {
                                pvm_pkint(&on_tid, 1, 1);
                        } 
			else {
#ifdef DEBUG_RM_MORE
				printf ("Send_notification calling find host.\n");
				fflush(stdout);
#endif
                                host = find_host(on_tid);
                                notify_host = &(host->entry);
                                pack_host(notify_host);
                        }
                        pvm_send(notifylist[i].for_who, notifylist[i].msg_tag);
			if ( kind == PvmHostAdd && notifylist[i].on_tid == 1 )
			{
				del_notification(notifylist[i].for_who,
					notifylist[i].on_tid );
				i--;
			}
			else if ( kind == PvmHostAdd )
				notifylist[i].on_tid --;
			else if ( kind == PvmHostDelete || kind == PvmTaskExit)
			{
				del_notification(notifylist[i].for_who,
					notifylist[i].on_tid );
				i--;
			}
                        pvm_freebuf(pvm_setsbuf(sbuf));
                }
        }
	return 0;
}


int
del_notification(for_who, on_tid)
int             for_who, on_tid;
{
        int             i, j;

        for (i = 0; i < num_notifys; i++) 
                if (notifylist[i].for_who == for_who &&
                        notifylist[i].on_tid == on_tid) {
                        break;
                }

        if (i >= num_notifys) {
#ifdef DEBUG_RM
                fprintf(stderr, "del_notification: Unable to find notify struct\n");
#endif
                return -1;
        }

	for ( j = (i+1); j < num_notifys; j++,i++ )
        	notifylist[i] = notifylist[j];
#ifdef DEBUG_RM
	printf ("Deleteing notify\n");
#endif
#ifdef DEBUG_RM_NOTIFY
	print_notify_contents();
#endif
        num_notifys--;
        return 0;
}

int
new_notification(kind, for_who, msg_tag, on_tid, ctx)
int kind, for_who, msg_tag, on_tid, ctx;
{
	int i;
#ifdef DEBUG_RM_MORE
	printf ("For_whom t%x, On_tid t%x\n", for_who, on_tid);
#endif
        if (notify_list_size == 0) {
                notify_list_size = 10;
                notifylist = (struct notification *) malloc(notify_list_size *

                 sizeof(struct notification));
        }
        if (num_notifys + 1 >= notify_list_size) {
                notify_list_size *= 2;
                notifylist = (struct notification *) realloc(notifylist,
                         notify_list_size *
                  sizeof(struct notification));
        }

        	notifylist[num_notifys].kind = kind;
        	notifylist[num_notifys].for_who = for_who;
        	notifylist[num_notifys].msg_tag = msg_tag;
        	notifylist[num_notifys].on_tid = on_tid;
		notifylist[num_notifys].m_ctx = ctx;
        	num_notifys++;
#ifdef DEBUG_RM
	printf ("Setting up notify with kind %d\n", kind);
#endif
#ifdef DEBUG_RM_NOTIFY
	print_notify_contents();
#endif
        return 0;
}

int
print_task_contents()
{
	task_type *task;
	int	i = 0;

	for ( task= TASK_HEAD.next; task; task = task->next)
	{
		printf("\n");
		printf("Entry %d\n", i);
		printf("Task Tid t%x\n",task->entry.ti_tid);
		printf("Task PTid t%x\n",task->entry.ti_ptid);
		printf("Task Host t%x\n",task->entry.ti_host);
		printf("Task Flag %d\n",task->entry.ti_flag);
		printf("Task A_Out %s\n",task->entry.ti_a_out);
		printf("Task Pid t%x\n",task->entry.ti_pid);
		i++;
	}
	return 1;
}

int 
print_notify_contents()
{
	int i = 0;

  printf("Here\n");
	for ( i = 0; i < num_notifys; i++ )
	{
		printf("\n");
		printf("Kind %d\n", notifylist[i].kind );
		printf("For who t%x\n", notifylist[i].for_who );
		printf("Msg tag %d\n", notifylist[i].msg_tag );
		printf("On_tid t%x\n", notifylist[i].on_tid );
		printf("Context %d\n", notifylist[i].m_ctx );
	}
  fflush(stdout);
  return 1;
}	
	
int
print_host_contents()
{
	host_type *host;
	int	i = 0;

	for ( host= HOST_HEAD.next; host; host = host->next)
	{
		printf("\n");
		printf("Entry %d\n", i);
		printf("Load %d\n",host->load);
		printf("Host Tid t%x\n",host->entry.hi_tid);
		printf("Host name %s\n",host->entry.hi_name);
		printf("Host arch %s\n",host->entry.hi_arch);
		printf("Host speed %d\n",host->entry.hi_speed);
		printf("Host signature %x\n",host->entry.hi_dsig);
		i++;
	}
	return 1;
}

static void
my_handler( signo )
int signo;
{
	int cc,sbf, rbf;
	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataDefault));
	rbf = pvm_setrbuf(0);
	cc = (msendrecv(TIDPVMD, TM_HALT, SYSCTX_TM) < 0) ? 0 : PvmSysErr;
	pvm_freebuf(pvm_setsbuf(sbf));
        pvm_setrbuf(rbf);
	no_signal = FALSE;

	printf ("Pvm halted....\n");
	fflush(stdout);
        return;
}
