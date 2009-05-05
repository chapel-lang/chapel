
/* $Id: srm.h,v 1.2 1997/07/29 18:25:51 pvmsrc Exp $ */

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

/* Dynamic resource code for PVM has been developed by many different
 * groups and people over the last 3 years.
 * Thanks goes to the following:
 *      Technische Universitat Munchen, University of Wisconsin Madison and
 *  the University of Reading UK.
 *
 *      Special thanks to the CoCheck team for being the first to attempt a
 *      real RM (we found almost all the bugs now, thanks)
 *
 *
 */


#if     !defined(FALSE)
#define FALSE   0
#endif
#if     !defined(TRUE)
#define TRUE    1
#endif


#define MAX_MESSAGE     16      /* Make sure this number stays right :) */

/* Structure definitions */


typedef int                     bool;
typedef struct HOST_TYPE        host_type;
typedef struct MESSAGE_TYPE     message_type;
typedef struct TASK_TYPE        task_type;

struct HOST_TYPE
  {
        struct pvmhostinfo      entry;
        host_type               *next;
        host_type               *prev;
	int			load;
  };

struct MESSAGE_TYPE
  {
        int     msg_tag;
        int     (*code)();
  };

struct TASK_TYPE
  {
        struct pvmtaskinfo      entry;
        task_type               *next;
        task_type               *prev;
  };

struct notification {
        int             kind;
        int             for_who;
        int             msg_tag;
        int             on_tid;
        int             m_ctx;
} *notifylist;



int 			MYTID;
int             	notify_list_size = 0;
int                     num_arches = 0;
int                     num_hosts = 0;
int             	num_notifys = 0;
int             	num_tasks = 0;
int             	task_list_size = 0;
host_type	        HOST_HEAD;
task_type  	        TASK_HEAD;
struct pvmhostinfo      *host;
struct pvmhostinfo 	*our_host;
struct pvmhostinfo 	*LOCAL_HOST;
extern struct Pvmtracer pvmctrc;
extern int		pvmmyctx;
extern struct Pvmtracer pvmtrc;

/*      Function Prototypes         */

int     add_host                (   );
int     add_task                (   );
int     delete_host             (   );
int     del_notification        (   );
int     delete_task             (   );
task_type * find_task	 	(   );
host_type * find_host  	        (   );
int     free_list 	        (   );
int     new_notification        (   );
int     pack_host_list          (   );
int     pack_task_list          (   );
int     select_host             (   );
int     send_notification       (   );
int     sm_spawn                (   );
int     sm_exec                 (   );
int     sm_execack              (   );
int     sm_task                 (   );
int     sm_config               (   );
int     sm_addhost              (   );
int     sm_delhost              (   );
int     sm_add                  (   );
int     sm_addack               (   );
int     sm_notify               (   );
int     sm_taskx                (   );
int     sm_hostx                (   );
int     sm_handoff              (   );
int     sm_sched                (   );
int     sm_sthost               (   );
int     sm_sthostack            (   );
int     loop_init               (   );
int     unpack_string 	        (   );


message_type Messages[MAX_MESSAGE] =
{
        /*      Message Tag             Message Code */

        {       SM_TASK,                sm_task         }, /* 0 */
        {       SM_TASKX,               sm_taskx        },
        {       SM_SPAWN,               sm_spawn        },
        {       SM_EXEC,                sm_exec         },
        {       SM_EXECACK,             sm_execack      },
        {       SM_CONFIG,              sm_config       }, /* 5 */
        {       SM_ADDHOST,             sm_addhost      },
        {       SM_DELHOST,             sm_delhost      },
        {       SM_ADD,                 sm_add          },
        {       SM_ADDACK,              sm_addack       },
        {       SM_NOTIFY,              sm_notify       }, /* 10 */
        {       SM_HOSTX,               sm_hostx        },
        {       SM_HANDOFF,             sm_handoff      },
        {       SM_SCHED,               sm_sched        },
        {       SM_STHOST,              sm_sthost       },
        {       SM_STHOSTACK,           sm_sthostack    }  /* 15 */
};

#ifdef DEBUG_RM_MORE
#ifndef DEBUG_MORE
#define DEBUG_MORE
#endif
#endif

bool no_signal;
