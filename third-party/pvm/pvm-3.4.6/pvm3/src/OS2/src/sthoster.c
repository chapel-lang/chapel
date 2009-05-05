#include<stdlib.h>
#include<process.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include <rpc/types.h>
#include <rpc/xdr.h>

#include <pvm3.h>
#include <pvmproto.h> 
#include "../../pmsg.h"
#include "../../task.h"
#include "../../pvmalloc.h"

extern int hostertid;				/* from pvmd.c */
extern int pvmmytid;				/* from pvmd.c */

static int
hosterforkexec(tid,name, tpp)
	int tid;			/* tid given by hosterexectask */
	char *name;			/* filename */

	struct task **tpp;		/* return task context */
{
	int pid=-1;			/* task pid */
	char *argv[2];
	struct task *tp;		/* new task context */
	char *expected_pid;
	char buf[32];
	char *myenv[100];
	char **p, **q;
	struct stat sb;	
	extern int *ptr_nfp;		/* XXX fix this */

	if (stat(name, &sb) == -1) 
		return PvmNoFile;

	tp = task_new(tid);		

	p = myenv;
	q = environ;
	while (*q) {
		*p++ = *q++;   	
	}
			/* copy all the environment for 
				 	socket stuff and more */
	expected_pid=malloc(20 * sizeof(char));
	sprintf(expected_pid, "PVMEPID=%d", *ptr_nfp);
		
	*p++ = expected_pid;
	*p=0;
	pvmputenv(expected_pid);
	argv[0]=name;
	argv[1]=0;

	pid = spawnve(P_NOWAIT,name,argv,myenv); 
	if (pid == -1) {
			pvmlogperror("forkexec_hoster() _spawnve");
			/* task_free(&tp); */
			pvmbailout(0);
			return PvmOutOfRes;
	}
		
	task_setpid(tp,*ptr_nfp);
	*ptr_nfp=*ptr_nfp + 1;

	tp->t_flag |= TF_FORKD;

	tp->t_a_out = STRALLOC(name);
	 
	*tpp = tp;
	return 0;

}		


static int
hostexectasker(file, tid)
	char *file;
	int tid;
{
	int tids = 0;			/* tid from hosterforkexec */
	
	struct pmsg *rmp;
	struct task *tp = 0;
	int err = 0;			/* error code from forkexec */

	rmp = mesg_new(0);
	rmp->m_dst = pvmmytid;
	rmp->m_src = pvmmytid;

	rmp->m_tag = DM_EXECACK;

	if (err = hosterforkexec(tid,file, &tp)) {
		tids = err;
	} else {
			tp->t_ptid = 0;
			tp->t_outtid = 0;
			tp->t_trctid = 0;
			tp->t_sched = 0;
			tids = tp->t_tid;
	}

	pkint(rmp, 1);
	pkint(rmp, tids);
	sendmessage(rmp);
	
	return err;
}


/*	start_hoster()
*	forkexec the hoster process which
*	will start other pvmd's.
*	acts like pvmd'
***/

int start_hoster(int reserved_tid)
{
	char hosterpath[128];
	strcpy(hosterpath,(char*)pvmgetroot());
	strcat(hosterpath,"/bin/OS2/hoster.exe");

	if (hostexectasker(hosterpath,reserved_tid)) {
		
		return -1;
	}
	return 0;
}

