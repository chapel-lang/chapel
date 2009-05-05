
static char rcsid[] =
	"$Id: cmds.c,v 1.48 2009/01/30 21:22:23 pvmsrc Exp $";

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
 *	cmds.c
 *
 *	PVM console commands.
 *
 * $Log: cmds.c,v $
 * Revision 1.48  2009/01/30 21:22:23  pvmsrc
 * Fixed up insecure usage of fopen() for creating/writing/appending files.
 * 	- replace with open(...O_EXCL...) and fdopen()...
 * (Spanker=kohl)
 *
 * Revision 1.47  2009/01/29 20:57:14  pvmsrc
 * Replaced use of PVMTMPNAM() and temporary files for auto-diagnostics...
 * 	- just use popen() instead, no file race security holes...  :-D
 * (Spanker=kohl)
 *
 * Revision 1.46  2006/08/15 15:03:57  pvmsrc
 * Extended help text for host add commands...
 * 	- make note of host option tags...
 * (Spanker=kohl)
 *
 * Revision 1.45  2004/01/14 18:46:20  pvmsrc
 * Added new AIX5* arches.
 * (Spanker=kohl)
 *
 * Revision 1.44  2001/09/26 23:37:22  pvmsrc
 * Made hd_vmid additions to hostd mirror struct...
 * 	- don't know if this is really necessary, but wtf.
 * (Spanker=kohl)
 *
 * Revision 1.43  2001/09/25 21:18:13  pvmsrc
 * Minor TMPNAMFUN()/tmpnam() cleanup.
 * 	- moved macro def to pvm3.h, renamed PVMTNPMAN().
 * 	- same for LEN_OF_TMP_NAM -> PVMTMPNAMLEN.
 * 	- mostly a huge waste of time, since *both* tmpnam() & mktemp()
 * 		produce the same "dangerous" warning message in Linux/gcc...
 * 	- damn.
 * (Spanker=kohl)
 *
 * Revision 1.42  2001/08/17 20:18:32  pvmsrc
 * Added --host= option to spawn command.
 * 	- allows passing IP address in place of host name.
 * 	- used gnu-like syntax at Paul Gray's suggestion... :-)
 * (Spanker=kohl)
 *
 * Revision 1.41  2001/07/30 17:46:08  pvmsrc
 * O.K., Paul's bug was legit.  Some goofy (Mandrake) Linux passes
 * 	stderr back through rsh...
 * 	- weed out lines with stuff like "/bin/ls: ... No such file..."
 * (Spanker=kohl)
 *
 * Revision 1.40  2001/07/30 16:52:00  pvmsrc
 * Minor cleanup to auto-diag routine...
 * 	- slightly more solid checking for leftover /tmp/pvmd.* files.
 * 	- inspired by a pseudo-bug report from Paul Gray, could be bogus...
 * 	:-)
 * (Spanker=kohl)
 *
 * Revision 1.39  2001/07/30 16:02:31  pvmsrc
 * epm -> csm...
 * (Spanker=kohl)
 *
 * Revision 1.38  2001/04/23 14:41:41  pvmsrc
 * Tweaked spawn_cmd() parsing to allow -:wd working directory spec.
 * 	- i.e. without a host or arch specified...
 * 	- fixed help spawn text.
 * (Spanker=kohl)
 *
 * Revision 1.37  2001/02/02 17:05:50  pvmsrc
 * Fixed typo in Win32 diagnostic message output.
 * (Spanker=kohl)
 *
 * Revision 1.36  2000/03/29 20:01:40  pvmsrc
 * Stoopid typo...  :-Q
 * (Spanker=kohl)
 *
 * Revision 1.35  2000/02/18 22:01:42  pvmsrc
 * Cleaned up Windows side of diagnose_cant_start_pvm()...
 * 	- lose any '\r' at end of rsh output.
 * 	- use "chdir" instead of "pwd" for Windows O.S. check.
 * 	- minor typos...
 * (Spanker=kohl)
 *
 * Revision 1.34  2000/02/15 18:06:20  pvmsrc
 * Modified jobs_cmd() to dump task ids for traced jobs...
 * 	- can't use output buffers, must use TRC_ID->tevtask_list list.
 * 	- snagged necessary state constants from trclib.h, can't just
 * 		#include it, as it #includes fricking everything...  :-Q
 * (Spanker=kohl)
 *
 * Revision 1.33  2000/02/15 17:06:42  pvmsrc
 * Added new getopt / setopt console commands.
 * 	- for setting PVM console task's PVM options (like PvmShowTids).
 * Fixed debug flags in tickle help.
 * (Spanker=kohl)
 *
 * Revision 1.32  2000/02/14 20:30:59  pvmsrc
 * Lose #define-d RSHCOMMAND usage.
 * 	- use new pvmgetrsh() routine, which checks for PVM_RSH or else
 * 		uses old RSHCOMMAND interface.
 * (Spanker=kohl)
 *
 * Revision 1.31  2000/02/10 22:17:38  pvmsrc
 * Cleaned up diagnose_cant_start_pvmd() test routine a bunch.
 * 	- replaced gobs of inline code with new do_rsh_cmd() routine.
 * 	- added new check_env_var() routine to sort through extra
 * 		garbage in rsh commands when checking env vars...
 * (Spanker=kohl)
 *
 * Revision 1.30  2000/02/10 20:43:37  pvmsrc
 * Added checking for new PVM_TMP env var on remote host.
 * 	- in diagnose_cant_start_pvm()...
 * 	- use in place of hard-coded /tmp or \TEMP...
 * (Spanker=kohl)
 *
 * Revision 1.29  2000/01/26 23:34:17  pvmsrc
 * *** Added Killer Routine of All Time - diagnose_cant_start_pvm() ***
 * Nearly 800 lines of pure joy!  :-D
 * 	- on "Can't start pvmd" error, tries to rsh to host and checks
 * 		everything from rhosts to $PVM_ROOT to /tmp/pmvd.* crap files.
 * 	- should theoretically work on both Unix and Windows machines,
 * 		for both Unix and Windows remote hosts (tested for Unix :-).
 * 	- with any luck this will cut bug mail in half...  yeah right.
 * (Spanker=kohl)
 *
 * Revision 1.28  1999/07/08 18:59:39  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.27  1999/06/11 17:27:35  pvmsrc
 * When breaking up multiline commands into subcommands,
 * 	dump out each command as you execute it, along with a fresh prompt.
 * (Spanker=kohl)
 * 
 * Revision 1.26  1999/06/10 16:57:16  pvmsrc
 * Fixed docmd() to handle commands with '\n's in them...
 * 	- a la file input redirection, cut & paste, etc.
 * 	- break string into individual commands and execute.
 * (Spanker=kohl)
 * 
 * Revision 1.25  1999/02/05  20:36:17  pvmsrc
 * More contributions from Edward Vigmond <vigmonde@IGB.UMontreal.CA>.
 * 	- better readline interface, with command completion & help.
 * (Spanker=kohl)
 *
 * Revision 1.24  1998/11/20  19:43:07  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * source for win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.23  1998/01/28  23:03:39  pvmsrc
 * Fixed tracing bogusness!
 * 	- host add / del notifies for tracing were getting scrunched by
 * 		console host add notify, etc...
 * 	- moved message / notify tag constants to job.h and USED THEM.
 * 	- initialized nextjob properly...
 * 	- added new "joboffset" global in cons.c for dumping more
 * 		reasonable job numbers to user.
 * (Spanker=kohl)
 *
 * Revision 1.22  1997/12/31  22:13:15  pvmsrc
 * Updated "xpvm" trace mask settings.
 * (Spanker=kohl)
 *
 * Revision 1.21  1997/08/29  16:56:53  pvmsrc
 * Added TEV_USER_DEFINED to "xpvm" default trace mask settings...
 * (Spanker=kohl)
 *
 * Revision 1.20  1997/07/09  13:21:03  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.19  1997/05/13  14:37:38  pvmsrc
 * Changed header file $includes:
 * 	- ../src/listmac.h -> listmac.h
 * 	- ../src/bfunc.h -> bfunc.h
 * 	- use -I$(PVMDIR)/src in Makefile.aimk instead.
 *
 * Revision 1.18  1997/05/02  20:18:51  pvmsrc
 * D-Oh.
 *
 * Revision 1.17  1997/05/01  15:41:14  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.16  1997/04/30  21:15:35  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.15  1997/04/17  12:53:15  pvmsrc
 * rename of pvm_mboxinfo() to include word "get"
 *
 * Revision 1.14  1997/04/10  20:34:35  pvmsrc
 * "CVS": Modified Files:
 * Typo...
 *
 * Revision 1.13  1997/04/10  20:09:33  pvmsrc
 * Rewrote names_cmd() for new pvm_mboxinfo() syntax.
 * 	- pvm_getnames() is history.
 *
 * Revision 1.12  1997/04/09  20:18:13  pvmsrc
 * Minor fixes (like being able to actually pass args to "reset"... :-).
 *
 * Revision 1.11  1997/04/09  19:53:12  pvmsrc
 * Merged resetinfo_cmd() command into reset_cmd() routine.
 * 	- check for args, adjust calling parms to pvmreset()...
 *
 * Revision 1.10  1997/04/09  18:31:58  pvmsrc
 * Added new "resetinfo" command.
 * 	- like a reset, but without all the task bloodshed...  :-)~
 *
 * Revision 1.9  1997/04/08  17:08:46  pvmsrc
 * Extracted guts of reset_cmd() routine:
 * 	- created new system pvmreset() routine for general usage.
 *
 * Revision 1.8  1997/04/01  19:35:44  pvmsrc
 * Corrected flag to PvmMbox...
 *
 * Revision 1.7  1997/04/01  17:04:45  pvmsrc
 * Updated mbox stuff:
 * 	- changed args of pvm_putinfo().
 * 	- renamed pvm_getinfo() (again) -> pvm_recvinfo(), w/ new syntax.
 *
 * Revision 1.6  1997/03/27  20:09:51  pvmsrc
 * Added PvmNoSpawnParent to pvm_spawn() calls in spawn_cmd().
 *
 * Revision 1.5  1997/02/17  16:29:04  pvmsrc
 * Added checking of pvm_getnoresets() list in reset_cmd().
 *
 * Revision 1.4  1997/01/28  19:13:17  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/24  23:03:39  pvmsrc
 * Updated to new tracing facilty.
 * 	- new trace_cmd() options:
 * 		* "xpvm" mask mimick.
 * 		* "buf" trace buffer size query & set.
 * 		* "opt" trace options query & set.
 * 	- new job creation handling for spawn command,
 * 		tracing vs. output vs. both...
 * 	- installed new trc_print_tmask() library call for trace mask dump.
 *
 * Revision 1.2  1996/10/08  18:29:39  pvmsrc
 * Renamed routines:
 * 	- pvm_put() -> pvm_putinfo().
 * 	- pvm_get() -> pvm_getinfo().
 *
 * Revision 1.1  1996/09/23  20:25:35  pvmsrc
 * Initial revision
 *
 * Revision 1.8  1995/11/02  15:11:40  manchek
 * added to tickle help
 *
 * Revision 1.7  1995/09/05  19:06:52  manchek
 * help text lowercase
 *
 * Revision 1.6  1995/07/03  18:59:51  manchek
 * help text
 *
 * Revision 1.5  1995/07/03  18:57:29  manchek
 * added ps 's' flag, tickle help 100.
 * ps doesn't print tasks with tid 0 unless -x flag used
 *
 * Revision 1.4  1995/05/17  15:26:17  manchek
 * added "return 0" to lots of commands.
 * added idump command
 *
 * Revision 1.3  1994/10/15  18:38:56  manchek
 * updated task flags and ps list headers
 *
 * Revision 1.2  1994/06/03  20:01:51  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:30:32  manchek
 * Initial revision
 *
 */

#include <stdio.h>
#include <fcntl.h>
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <ctype.h>
#include <signal.h>
#include <sys/stat.h>
#include <pvm3.h>
#include <pvmtev.h>
#include "cmd.h"
#include "myalloc.h"
#include "listmac.h"
#include "bfunc.h"
#include "job.h"

#ifndef WIN32
#include <pwd.h>
#endif

#define	PVMERRMSG(n)	((n) <= 0 && (n) > -pvm_nerr \
						? pvm_errlist[-(n)] : "Unknown Error")

char *getenv();
void i_dump();

extern char **environ;

extern char *pvm_errlist[];
extern int pvm_nerr;

extern struct Pvmtevinfo pvmtevinfo[];

extern char *prompt;

int add_cmd();
int alias_cmd();
int conf_cmd();
int delete_cmd();
int echo_cmd();
int export_cmd();
int getopt_cmd();
int halt_cmd();
int help_cmd();
int id_cmd();
int idump_cmd();
/*
int insert_cmd();
*/
int jobs_cmd();
int kill_cmd();
/*
int lookup_cmd();
*/
int mstat_cmd();
int names_cmd();
int quit_cmd();
int pstat_cmd();
int put_cmd();
/*
int remove_cmd();
*/
int reset_cmd();
int setenv_cmd();
int setopt_cmd();
int sig_cmd();
int spawn_cmd();
int start_cmd();
int tasks_cmd();
int tickle_cmd();
int trace_cmd();
int unalias_cmd();
int unexport_cmd();
int version_cmd();

extern struct alias *aliases;			/* from cons.c */
extern struct pvmhostinfo *hostlist;	/* from cons.c */
extern struct job *joblist;				/* from job.c */
extern int mytid;						/* from cons.c */
extern int narchs;						/* from cons.c */
extern int joboffset;					/* from cons.c */
extern int nextjob;						/* from cons.c */
extern int nhosts;						/* from cons.c */


struct cmdsw commands[] = {
	{ "add",     2, 0, add_cmd },
	{ "alias",   1, 0, alias_cmd },
	{ "conf",    1, 1, conf_cmd },
	{ "delete",  2, 0, delete_cmd },
	{ "echo",    1, 0, echo_cmd },
	{ "export",  1, 0, export_cmd },
	{ "getopt",  1, 2, getopt_cmd },
	{ "halt",    1, 1, halt_cmd },
	{ "help",    1, 2, help_cmd },
	{ "id",      1, 1, id_cmd },
	{ "idump",   1, 2, idump_cmd },
/*
	{ "insert",  4, 4, insert_cmd },
*/
	{ "jobs",    1, 2, jobs_cmd },
	{ "kill",    2, 0, kill_cmd },
/*
	{ "lookup",  3, 3, lookup_cmd },
*/
	{ "mstat",   2, 0, mstat_cmd },
	{ "names",   1, 2, names_cmd },
	{ "ps",      1, 2, tasks_cmd },
	{ "pstat",   2, 0, pstat_cmd },
	{ "put",     4, 4, put_cmd },
	{ "quit",    1, 1, quit_cmd },
/*
	{ "remove",  3, 3, remove_cmd },
*/
	{ "reset",   1, 4, reset_cmd },
	{ "setenv",  1, 0, setenv_cmd },
	{ "setopt",  3, 3, setopt_cmd },
	{ "sig",     3, 0, sig_cmd },
	{ "spawn",   2, 0, spawn_cmd },
	{ "trace",   1, 0, trace_cmd },
	{ "tickle",  2, 11, tickle_cmd },
	{ "unalias", 2, 0, unalias_cmd },
	{ "unexport", 1, 0, unexport_cmd },
	{ "version", 1, 1, version_cmd },
	{ 0, 0, 0, 0 }
};


/* Host descriptor (pieces lifted from pvm3/src/host.h - pvmd stuff) */

struct hostd {
	int hd_ref;					/* num refs to this struct */
	int hd_hostpart;			/* host tid base */
	char *hd_name;				/* name */
	char *hd_arch;				/* cpu arch class */
	char *hd_login;				/* loginname  [used master only] */
	char *hd_dpath;				/* daemon executable */
	char *hd_epath;				/* task exec search path */
	char *hd_bpath;				/* debugger executable */
	char *hd_wdir;				/* pvmd working dir */
	char *hd_sopts;				/* hoster options */
	int hd_flag;
	int hd_dsig;				/* data signature */
	int hd_err;					/* error code */
	int hd_mtu;					/* max snd/rcv length */
	/* struct sockaddr_in hd_sad;	/^ UDP address/port */
	int hd_rxseq;				/* expected next seq num from host */
	int hd_txseq;				/* next tx seq num to host */
	/* struct pkt *hd_txq;		/^ not-yet-sent packet queue to host */
	/* struct pkt *hd_opq;		/^ outstanding packets to host */
	int hd_nop;					/* length of opq */
	/* struct pkt *hd_rxq;		/^ packet reordering queue from host */
	/* struct pmsg *hd_rxm;		/^ to-us msg reassembly from host */
	/* struct timeval hd_rtt;	/^ estd round-trip time to host */
	int hd_speed;				/* cpu relative speed */
	/* struct mca *hd_mcas;		/^ from-host mca cache */
	char *hd_aname;				/* name to use for network address */
	char *hd_vmid;				/* virtual machine id for host */
};


freealias(ap)
	struct alias *ap;
{
	LISTDELETE(ap, a_link, a_rlink);
	MY_FREE(ap->a_name);
	while (ap->a_num-- > 0)
		MY_FREE(ap->a_args[ap->a_num]);
	MY_FREE(ap->a_args);
	MY_FREE(ap);
	return 0;
}


struct alias *
newalias(name, num, args)
	char *name;
	int num;
	char **args;
{
	struct alias *ap, *ap2;

	ap = TALLOC(1, struct alias, "alias");
	ap->a_name = STRALLOC(name);
	ap->a_args = TALLOC(num + 1, char *, "aargs");
	ap->a_num = num;
	while (num-- > 0)
		ap->a_args[num] = STRALLOC(args[num]);
	for (ap2 = aliases->a_link; ap2 != aliases; ap2 = ap2->a_link)
		if (strcmp(ap2->a_name, name) > 0)
			break;
	LISTPUTBEFORE(ap2, ap, a_link, a_rlink);
	return ap;
}


struct alias *
findalias(name)
	char *name;
{
	struct alias *ap;

	for (ap = aliases->a_link; ap != aliases; ap = ap->a_link)
		if (!strcmp(ap->a_name, name))
			return ap;
	return (struct alias*)0;
}


docmd(cmd)
	char *cmd;
{
	char *p;
	struct cmdsw *csp;
	struct alias *ap;
	int i;
	int ac;
	char *av[128];
	int self;
	char *nextcmd = NULL;
	int first = 1;

	/*
	* check for '\n's in command, separate commands
	*/

	nextcmd = cmd;

	do
	{
		/*
		* look for a newline...
		*/

		cmd = nextcmd;

		while ( *nextcmd != '\0' && *nextcmd != '\n' )
			nextcmd++;

		if ( *nextcmd == '\n' )
		{
			*nextcmd++ = '\0';

			if ( !first )
				printf( prompt );
			
			else
				first = 0;

			printf( "%s\n", cmd );
		}

		/*
		* parse command
		*/

		ac = sizeof(av)/sizeof(av[0]) - 1;
		if (acav(cmd, &ac, av)) {
			fputs("command too long\n", stdout);
			return 0;
		}
		if (!ac)
			return 0;

		/*
		* resolve aliases
		*/

		for (ap = aliases->a_link; ap != aliases; ap = ap->a_link)
			ap->a_flag = 0;

		while (ap = findalias(av[0])) {
			if (ap->a_flag) {
				printf("alias loop\n");
				return 0;
			}
			ap->a_flag = 1;
			if (ap->a_num > 1) {
				if (ac + ap->a_num > sizeof(av)/sizeof(av[0])) {
					fputs("command too long\n", stdout);
					return 0;
				}
				for (i = ac; --i > 0; )
					av[i + ap->a_num - 1] = av[i];
				ac += ap->a_num - 1;
			}
			self = !strcmp(av[0], ap->a_args[0]);
			for (i = ap->a_num; i-- > 0; )
				av[i] = ap->a_args[i];
			if (self)
				break;
		}

		/*
		* find command and call it
		*/

		av[ac] = 0;
		p = av[0];
		for (csp = commands; csp->cmd; csp++) {
			if (!strcmp(csp->cmd, p)) {
				if (ac >= csp->a1 && (ac <= csp->a2 || !csp->a2))
					(csp->fun)(ac, av);
				else
					printf("%s: wrong #args\n", p);
				break;
			}
		}
		if (!csp->cmd)
			printf("%s: not found\n", p);
	}
	while ( *nextcmd != '\0' );

	return 0;
}


/****************
 **  Commands  **
 **            **
 ****************/

add_cmd(ac, av)
	int ac;
	char **av;
{
	int diagnose = 0;
	int cc;
	int *sv;
	int i;

	av++;
	ac--;
	sv = TALLOC(ac, int, "int");
	if ((cc = pvm_addhosts(av, ac, sv)) >= 0) {
		if (cc > 0)
			pvm_recv(-1, HostsAddedTag); /* waste the notify message */

		printf("%d successful\n", cc);
		fputs("                    HOST     DTID\n", stdout);
		for (i = 0; i < ac; i++)
			if ((cc = sv[i]) < 0) {
				printf("%24s %8s\n", av[i], PVMERRMSG(cc));
				if (cc == PvmCantStart)
					diagnose++;
			}
			else
				printf("%24s %8x\n", av[i], cc);

		/* Try to Further Diagnose "PvmCantStart" Cases... */
		/* Otherwise, sure to generate MORE BUG MAIL!!!  :-Q */
		if ( diagnose ) {
			printf( "\nAuto-Diagnosing Failed Hosts...\n" );
			for (i = 0; i < ac; i++) {
				if (sv[i] == PvmCantStart) {
					printf( "%s...\n", av[i] );
					diagnose_cant_start_pvm( av[i] );
					printf( "\n" );
				}
			}
		}
	}
	MY_FREE(sv);
	pvm_config(&nhosts, &narchs, &hostlist);
	return 0;
}


diagnose_cant_start_pvm( hostline )
char *hostline;
{
	FILE *fp;

#ifndef WIN32
	struct passwd *pw;
#endif

	struct hostd hp;

	struct stat statck;

	char lhost[1024];
	char pvmarch[1024];
	char pvmroot[1024];
	char pvmtmp[1024];
	char result[1024];
	char result2[1024];
	char tcmd[1024];
	char tmp[255];
	char user[1024];

	int remote_os = -1;
	int len;
	int i;

	/* Extract Raw Host Name from Add Command */

	if ( parsehost(hostline, &hp) < 0 ) {
		printf( "Error Parsing Host String!\n" );
		printf( "Cannot Auto-Diagnose!\n" );
		return( 0 );
	}

	/* Get Local Hostname */

	gethostname( lhost, 1024 );

	/* Get Local User */

#ifndef WIN32
	if ( (pw = getpwuid( getuid() )) != NULL )
		strcpy( user, pw->pw_name );
	else
		strcpy( user, "<your_user_name>" );
#else
	strcpy( user, MyGetUserName() );
#endif

	/* Ping / Timeout Test */

	/*
	 * Maybe later - for now rsh returns "Connection timed out"
	 *
	printf( "Pinging Host \"%s\"...\n", hp.hd_name );

	sprintf( tcmd, "ping -c 1 %s 1>%s 2>&1", hp.hd_name, tfile );

	system( tcmd );

	fp = fopen( tfile, "r" );

	if ( fp != NULL ) {
		while ( fgets( result, 1024, fp ) != NULL ) {
			printf( "%s\n", result );
		}
		fclose( fp );
	}
	else
		printf( "Ping Command Failed - Test Skipped...\n" );

	unlink( tfile );
	*/

	/* Verify Rsh Path */

	printf( "Verifying Local Path to \"rsh\"...\n" );

	strcpy( tcmd, pvmgetrsh() );

	if ( stat( tcmd, &statck ) < 0 ) {
		printf( "\nError - File %s Not Found!\n", tcmd );
#ifndef WIN32
		printf( "Use \"whereis rsh\" or \"which rsh\" to determine\n" );
		printf( "where \"rsh\" is on your system and modify the\n" );
		printf( "$PVM_ROOT/conf/$PVM_ARCH.def configuration file\n" );
#else
		printf( "Determine the path to the \"rsh\" command on your\n" );
		printf( "system, and edit %PVM_ROOT%\\conf\\WIN32.def\n" );
#endif
		printf( "to adjust the path for the -DRSHCOMMAND=\\\"\\\"\n" );
		printf( "flag.  Then recompile PVM and your applications.\n" );

		return( 0 );
	}
	else
		printf( "Rsh found in %s - O.K.\n", tcmd );

	/* Rsh Test */

	if ( do_rsh_cmd( &hp, "Testing Rsh/Rhosts Access to",
			"echo YES", result, 1024 ) ) {

		/* check result */
		if ( !strncmp( result, "YES", 3 ) )
			printf( "Rsh/Rhosts Access is O.K.\n" );
		else {
			printf( "\nRsh/Rhosts Access FAILED - \"%s\"\n", result );
			printf( "Make sure host %s is up and connected to\n",
					hp.hd_name );
			printf( "a network and check its DNS / IP address.\n" );
			printf( "Also verify that %s is allowed\n", lhost );
			printf( "rsh access on %s\n", hp.hd_name );
			printf( "Add this line to the $HOME/.rhosts on %s:\n",
					hp.hd_name );
			printf( "%s %s\n", lhost, user );

			return( 0 );
		}
	}
	else
		return( 0 );

	/* Is Remote Host O.S. Windows or Unix? */

	if ( do_rsh_cmd( &hp, "Checking O.S. Type (Unix test) on",
			"pwd", result, 1024 ) ) {

		/* Unix-land */
		if ( result[0] == '/' ) {
			printf( "Host %s is Unix-based.\n", hp.hd_name );
			remote_os = 0;
		}
		else if ( do_rsh_cmd( &hp, "Checking O.S. Type (Win test) on",
				"chdir", result2, 1024 ) ) {

			/* Windows-land */
			len = strlen( result2 );
			if ( len >= 3 && result2[0] >= 'A' && result2[0] <= 'Z'
					&& result2[1] == ':' && result2[2] == '\\' ) {
				printf( "Host %s is Windows-based.\n", hp.hd_name );
				remote_os = 1;
			}
			/* Hmmm... Go Figure. */
			else {
				len = strlen( result );
				for ( i=0 ; i < len - 1 && remote_os < 0 ; i++ ) {
					if ( result[i] == '/' ) {
						printf( "Assuming Host %s to be Unix-based.\n",
								hp.hd_name );
						remote_os = 0;
					}
					else if ( result[0] == '\\' ) {
						printf(
							"Assuming Host %s to be Windows-based.\n",
								hp.hd_name );
						remote_os = 1;
					}
				}
				if ( remote_os < 0 ) {
					len = strlen( result2 );
					if ( !len ) {
						printf( "Assuming Host %s to be Unix-based.\n",
								hp.hd_name );
						remote_os = 0;
					}
					else {
						for ( i=0 ; i < len - 1 && remote_os < 0 ;
								i++ ) {
							if ( result2[i] == '/' ) {
								printf(
								"Assuming Host %s to be Unix-based.\n",
										hp.hd_name );
								remote_os = 0;
							}
							else if ( result2[0] == '\\' ) {
								printf(
							"Assuming Host %s to be Windows-based.\n",
										hp.hd_name );
								remote_os = 1;
							}
						}
					}
				}
				if ( remote_os < 0 ) {
					printf( "Error - Cannot Determine Remote O.S.!\n" );
					printf( "Can't Auto-Diagnose.\n" );
					return( 0 );
				}
			}
		}
	}
	else
		return( 0 );

	/* PVM_ROOT Test */

	if ( do_rsh_cmd( &hp,
			( remote_os == 0 ) ?
				"Checking $PVM_ROOT on" : "Checking %PVM_ROOT% on",
			( remote_os == 0 ) ?
				"echo $PVM_ROOT" : "echo %PVM_ROOT%",
			result, 1024 ) ) {

		/* check for undefined variable (Unix) */
		if ( remote_os == 0 && !check_env_var( "PVM_ROOT", result ) ) {
			printf(
				"\nThe $PVM_ROOT Environment Variable is Not Set!\n" );
			printf( "Use one of the shell *.stub files in\n" );
			printf( "the pvm3/lib directory to set up the\n" );
			printf( "PVM environment on host %s.\n", hp.hd_name );

			return( 0 );
		}
		/* check for undefined variable (Windows) */
		else if ( remote_os == 1 && !strncmp( result, "ECHO", 4 ) ) {
			printf(
			"\nThe %%PVM_ROOT%% Environment Variable is Not Set!\n" );
			printf( "Check your PVM installation on host %s or\n",
				hp.hd_name );
			printf( "set the %%PVM_ROOT%% environment variable\n" );
			printf( "in the C:\\AUTOEXEC.BAT file to point to\n" );
			printf( "PVM3.4 directory:\n" );
			printf( "set PVM_ROOT = C:\\PVM3.4\n" );

			return( 0 );
		}
		/* check for bogus relative path (Unix) */
		else if ( remote_os == 0 && result[0] != '/' ) {
			printf( "\nThe value of the $PVM_ROOT environment\n" );
			printf( "variable on %s is invalid (\"%s\").\n",
				hp.hd_name, result );
			printf( "Use the absolute path to the pvm3/ directory.\n" );

			return( 0 );
		}
		/* check for bogus relative path (Windows) */
		else if ( remote_os == 1 && ( result[1] != ':'
				|| result[2] != '\\' ) ) {
			printf( "\nThe value of the %%PVM_ROOT%% environment\n" );
			printf( "variable on %s is invalid (\"%s\").\n",
				hp.hd_name, result );
			printf(
				"Use the absolute path to the PVM3.4 directory.\n" );

			return( 0 );
		}
		/* appears to be O.K., fall through to next test */
		else {
			strcpy( pvmroot, result );
			printf( "%sPVM_ROOT%s on %s Appears O.K. (\"%s\")\n",
				( remote_os == 0 ) ? "$" : "%",
				( remote_os == 0 ) ? "" : "%",
				hp.hd_name, pvmroot );
		}
	}
	else
		return( 0 );

	/* Pvmd Script Existence Test (a.k.a. $PVM_ROOT Validity Test) */

	if ( do_rsh_cmd( &hp, "Verifying Location of PVM Daemon Script on",
			( remote_os == 0 ) ? "\\ls $PVM_ROOT/lib/pvmd"
				: "dir /B /L %PVM_ROOT%\\lib\\pvmd.bat",
			result, 1024 ) ) {

		/* expected pvmd script path */
		if ( remote_os == 0 )
			sprintf( tmp, "%s/lib/pvmd", pvmroot );
		else
			sprintf( tmp, "pvmd.bat" );
		/* check existence of pvmd script */
		if ( strcmp( result, tmp ) ) {
			printf( "\nPVM Daemon Script \"%s\"\n", tmp );
			printf( "Was Not Found on %s\n", hp.hd_name );
			printf( "Please check the setting of %sPVM_ROOT%s...\n",
				( remote_os == 0 ) ? "$" : "%",
				( remote_os == 0 ) ? "" : "%" );

			return( 0 );
		}
		else
			printf( "PVM Daemon Script Found (\"%s\")\n", tmp );
	}
	else
		return( 0 );

	/* Determine Remote Host Architecture */

	if ( do_rsh_cmd( &hp, "Determining PVM Architecture on",
			( remote_os == 0 ) ? "echo $PVM_ARCH" : "echo %PVM_ARCH%",
			result, 1024 ) ) {

		/* check for undefined variable */
		if ( ( remote_os == 0 && check_env_var( "PVM_ARCH", result ) )
				|| ( remote_os == 1
					&& strncmp( result, "ECHO", 4 ) ) ) {
			printf( "%sPVM_ARCH%s on %s set to %s\n",
				( remote_os == 0 ) ? "$" : "%",
				( remote_os == 0 ) ? "" : "%",
				hp.hd_name, result );
			strcpy( pvmarch, result );
		}
		else {
			printf( "%sPVM_ARCH%s not set on %s\n",
				( remote_os == 0 ) ? "$" : "%",
				( remote_os == 0 ) ? "" : "%",
				hp.hd_name );
			if ( remote_os == 1 ) {
				printf( "Assuming WIN32.\n" );
				strcpy( pvmarch, "WIN32" );
			}
			else
				pvmarch[0] = '\0';
		}
	}
	else
		return( 0 );

	/* Manually Determine Remote Host Architecture (Unix Only) */

	if ( remote_os == 0 && pvmarch[0] == '\0' ) {

		if ( do_rsh_cmd( &hp,
				"Manually Determining PVM Architecture on",
				"$PVM_ROOT/lib/pvmgetarch", result, 1024 ) ) {

			len = strlen( result );
			/* sanity check */
			if ( len > 1 ) {
				printf( "$PVM_ARCH for %s is %s.\n",
					hp.hd_name, result );
				strcpy( pvmarch, result );
			}
			else {
				printf( "Could Not Determine $PVM_ARCH.\n" );
				printf( "Can't Further Auto-Diagnose.\n" );

				return( 0 );
			}
		}
		else
			return( 0 );
	}

	/* Existence of pvmd3 Executable? */

	if ( remote_os == 0 )
		sprintf( tmp, "\\ls $PVM_ROOT/lib/%s/pvmd3", pvmarch );
	else
		sprintf( tmp, "dir /B /L %%PVM_ROOT%%\\lib\\%s\\pvmd3.exe",
			pvmarch );

	if ( do_rsh_cmd( &hp,
			"Verifying Existence of PVM Daemon Executable on",
			tmp, result, 1024 ) ) {

		/* expected pvmd executable path */
		if ( remote_os == 0 )
			sprintf( tmp, "%s/lib/%s/pvmd3", pvmroot, pvmarch );
		else
			sprintf( tmp, "pvmd3.exe" );
		/* check existence of pvmd executable */
		if ( strcmp( result, tmp ) ) {
			printf( "\nPVM Daemon Executable \"%s\"\n", tmp );
			printf( "Was Not Found on %s!\n", hp.hd_name );
			printf( "Has PVM been compiled there?\n" );

			return( 0 );
		}
		else
			printf( "PVM Daemon Executable Found (\"%s\")\n", tmp );
	}
	else
		return( 0 );

	/* Determine Remote Host Temp Directory (if set) */

	if ( do_rsh_cmd( &hp, "Determining PVM Temporary Directory on",
			( remote_os == 0 ) ? "echo $PVM_TMP" : "echo %PVM_TMP%",
			result, 1024 ) ) {

		/* check for undefined variable */
		if ( ( remote_os == 0 && check_env_var( "PVM_TMP", result ) )
				|| ( remote_os == 1
					&& strncmp( result, "ECHO", 4 ) ) ) {
			printf( "%sPVM_TMP%s on %s set to %s\n",
				( remote_os == 0 ) ? "$" : "%",
				( remote_os == 0 ) ? "" : "%",
				hp.hd_name, result );
			strcpy( pvmtmp, result );
		}
		else {
			printf( "%sPVM_TMP%s not set on %s\n",
				( remote_os == 0 ) ? "$" : "%",
				( remote_os == 0 ) ? "" : "%",
				hp.hd_name );
			if ( remote_os == 1 ) {
				printf( "Assuming C:\\TEMP.\n" );
				strcpy( pvmtmp, "C:\\TEMP" );
			}
			else {
				printf( "Assuming /tmp.\n" );
				strcpy( pvmtmp, "/tmp" );
			}
		}
	}
	else
		return( 0 );

	/* Pvmd Already Running There / Leftover Pvmd Files? */

	if ( remote_os == 0 )
		sprintf( tmp, "\\ls -l %s/pvmd.* | grep %s",
			pvmtmp, ( hp.hd_login != NULL ) ? hp.hd_login : user );
	else
		/* should really also check %TEMP% */
		sprintf( tmp, "dir /B /L %s\\pvmd.%s",
			pvmtmp, ( hp.hd_login != NULL ) ? hp.hd_login : user );

	if ( do_rsh_cmd( &hp, "Checking for Leftover PVM Daemon Files on",
			tmp, result, 1024 ) ) {

		len = strlen( result );
		/* Find Any? (Unix) */
		if ( remote_os == 0 ) {
			/* search for a '/pvmd', if found assume a hit */
			for ( i=0 ; i < len - 1 ; i++ ) {
				/* bail out if "/bin/ls: ... No such file..." */
				if ( !strncmp( result + i, "ls:", 3 ) )
					break;
				else if ( !strncmp( result + i, "/pvmd.", 6 ) ) {
					printf( "\nPVM Daemon Files Found on %s!\n",
						hp.hd_name );
					printf(
						"PVM Could Already Be Running or else it\n" );
					printf( "crashed and left behind a %s/pvmd.<uid>\n",
						pvmtmp );
					printf( "daemon file.\n" );
					printf(
						"Halt PVM if it is running on %s, or else\n",
						hp.hd_name );
					printf( "remove any leftover %s/pvmd.* files.\n",
						pvmtmp );

					return( 0 );
				}
			}
			printf( "No PVM Daemon Files Found.\n" );
		}
		/* Find Any? (Windows) */
		else if ( remote_os == 1 && !strncmp( result, "pvmd", 4 ) ) {
			printf( "\nPVM Daemon Files Found on %s!\n", hp.hd_name );
			printf( "Either PVM is Already Running or else it\n" );
			printf( "crashed and left behind a \\TEMP\\pvmd.%s\n",
				( hp.hd_login != NULL ) ? hp.hd_login : user );
			printf( "daemon file.\n" );
			printf( "Halt PVM if it is running on %s, or else\n",
				hp.hd_name );
			printf( "remove any leftover \\TEMP\\pvmd.* files.\n" );

			return( 0 );
		}
		/* No Pvmd Files, O.K.? */
		else
			printf( "No PVM Daemon Files Found.\n" );
	}
	else
		return( 0 );

	/* Hmmm...  Must Be O.K. (ha ha ha) */

	printf( "\nHost %s Appears to Be Correctly Configured.\n",
		hp.hd_name );
#ifndef WIN32
	printf( "Please check your local %s/pvml.%d log file\n",
		pvmgettmp(), getuid() );
#else
	printf( "Please check your local %s\\pvml.%s log file\n",
		pvmgettmp(), user );
#endif
	printf( "for error messages, or else email \"%s\" for\n",
		"pvm@msr.csm.ornl.gov" );
	printf( "further assistance.\n" );

	return( 1 );
}


do_rsh_cmd( hp, label, cmd, result, rsize )
struct hostd *hp;
char *label;
char *cmd;
char *result;
int rsize;
{
	FILE *fp;

	char tcmd[1024];
	char tmp[255];

	int ret = 1;
	int len;

	/* Dump Test Label */

	printf( "%s Host \"%s\"...\n", label, hp->hd_name );

	/* Construct Rsh Command */

	sprintf( tcmd, "%s ", pvmgetrsh() );

	if ( hp->hd_login != NULL ) {
		sprintf( tmp, "-l %s ", hp->hd_login );
		strcat( tcmd, tmp );
	}

	sprintf( tmp, "%s '%s' ", hp->hd_name, cmd );
	strcat( tcmd, tmp );

#ifndef WIN32
	sprintf( tmp, "2>&1" );
	strcat( tcmd, tmp );
#endif

	/* Collect Rsh Command Results */

	fp = popen( tcmd, "r" );

	if ( fp != NULL ) {
		if ( fgets( result, rsize, fp ) != NULL ) {
			/* strip off '\n' */
			len = strlen( result );
			if ( len > 0 && result[ len - 1 ] == '\n' )
				result[ len - 1 ] = '\0';
			/* strip off any '\r' */
			len = strlen( result );
			if ( len > 0 && result[ len - 1 ] == '\r' )
				result[ len - 1 ] = '\0';
		}
		else {
			printf( "Error Parsing Output - Can't Auto-Diagnose.\n" );
			ret = 0;
		}
		pclose( fp );
	}
	else {
		printf( "Rsh Command Failed - Check Your Execution Path!\n" );
		ret = 0;
	}

	return( ret );
}


check_env_var( name, result )
char *name;
char *result;
{
	char *ptr;

	int nlen = strlen( name );

	/* Standard "FOO - Undefined variable" Response? */

	if ( !strncmp( result, name, nlen ) )
		return( 0 );

	/* Check for preceding garbage before standard response */

	ptr = result + 1;

	while ( *ptr != '\0' && strlen( ptr ) >= nlen )
	{
		if ( !strncmp( ptr, name, nlen ) )
			return( 0 );
		
		ptr++;
	}

	/* Look for "[Uu]ndefined" in result... */

	ptr = result;

	while ( *ptr != '\0' && strlen( ptr ) >= 9 )
	{
		if ( !strncmp( ptr, "Undefined", nlen )
				|| !strncmp( ptr, "undefined", nlen ) )
			return( 0 );
		
		ptr++;
	}

	/* Must Be O.K. */

	return( 1 );
}


/*	parsehost()
*
*	Parse hostfile line into hostd.
*	(lifted from pvm3/src/host.c - pvmd code... hacked a little :-)
*/

int
parsehost(buf, hp)
	char *buf;
	struct hostd *hp;
{
	char *av[10];		/* parsed words */
	int ac;
	int err = 0;

	ac = sizeof(av)/sizeof(av[0]);
	if (acav(buf, &ac, av)) {
		printf("parsehost(): line too long\n");
		goto bad;
	}
	if (!ac)
		goto bad;

	/* Initialize Host Struct Fields */

	hp->hd_name = (char *) NULL;
	hp->hd_arch = (char *) NULL;
	hp->hd_login = (char *) NULL;
	hp->hd_dpath = (char *) NULL;
	hp->hd_epath = (char *) NULL;
	hp->hd_bpath = (char *) NULL;
	hp->hd_wdir = (char *) NULL;
	hp->hd_sopts = (char *) NULL;
	hp->hd_vmid = (char *) NULL;

	/* add options to host descriptor */

	while (--ac > 0) {
		if (!strncmp(av[ac], "lo=", 3)) {
			if (hp->hd_login)
				MY_FREE(hp->hd_login);
			hp->hd_login = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "dx=", 3)) {
			if (hp->hd_dpath)
				MY_FREE(hp->hd_dpath);
			hp->hd_dpath = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "ep=", 3)) {
			if (hp->hd_epath)
				MY_FREE(hp->hd_epath);
			hp->hd_epath = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "sp=", 3)) {
			hp->hd_speed = atoi(av[ac] + 3);
			/* hp->hd_flag |= HF_SPEED; */
			continue;
		}
		if (!strncmp(av[ac], "bx=", 3)) {
			if (hp->hd_bpath)
				MY_FREE(hp->hd_bpath);
			hp->hd_bpath = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "wd=", 3)) {
			if (hp->hd_wdir)
				MY_FREE(hp->hd_wdir);
			hp->hd_wdir = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "so=", 3)) {
			if (hp->hd_sopts)
				MY_FREE(hp->hd_sopts);
			hp->hd_sopts = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "ip=", 3)) {
			if (hp->hd_aname)
				MY_FREE(hp->hd_aname);
			hp->hd_aname = STRALLOC(av[ac] + 3);
			continue;
		}
		if (!strncmp(av[ac], "id=", 3)) {
			if (hp->hd_vmid)
				MY_FREE(hp->hd_vmid);
			hp->hd_vmid = STRALLOC(av[ac] + 3);
			continue;
		}
		printf("parsehost(): unknown option \"%s\"\n", av[ac]);
		err++;
	}
	if (err)
		goto bad;

	if (hp->hd_name)
		MY_FREE(hp->hd_name);
	hp->hd_name = STRALLOC(av[0]);
	return 0;

bad:
	return -1;
}


alias_cmd(ac, av)
	int ac;
	char **av;
{
	struct alias *ap;
	char *p;
	int i;

	if (ac < 3) {
		for (ap = aliases->a_link; ap != aliases; ap = ap->a_link) {
			fputs(ap->a_name, stdout);
			p = "\t";
			for (i = 0; i < ap->a_num; i++) {
				printf("%s%s", p, ap->a_args[i]);
				p = " ";
			}
			fputs("\n", stdout);
		}

	} else {
		if (ap = findalias(av[1]))
			freealias(ap);
		newalias(av[1], ac - 2, av + 2);
	}
	return 0;
}


conf_cmd(ac, av)
	int ac;
	char **av;
{
	int i;

	ac = ac;
	av = av;
	if (!pvm_config(&nhosts, &narchs, &hostlist)) {
		printf("%d host%s, %d data format%s\n",
			nhosts, (nhosts > 1 ? "s" : ""), narchs, (narchs > 1 ? "s" : ""));
		fputs("                    HOST     DTID     ARCH   SPEED       DSIG\n", stdout);
		for (i = 0; i < nhosts; i++)
			printf("%24s %8x %8s%8d 0x%08x\n",
					hostlist[i].hi_name,
					hostlist[i].hi_tid,
					hostlist[i].hi_arch,
					hostlist[i].hi_speed,
					hostlist[i].hi_dsig);
	}
	return 0;
}


delete_cmd(ac, av)
	int ac;
	char **av;
{
	int cc;
	int i;
	int *sv;

	av++;
	ac--;
	sv = TALLOC(ac, int, "int");
	if ((cc = pvm_delhosts(av, ac, sv)) >= 0) {
		printf("%d successful\n", cc);
		fputs("                    HOST  STATUS\n", stdout);
		for (i = 0; i < ac; i++) {
			printf("%24s  ", av[i]);
			if ((cc = sv[i]) < 0)
				printf("%8s\n", PVMERRMSG(cc));
			else
				printf("deleted\n");
		}
	}
	MY_FREE(sv);
	return 0;
}


echo_cmd(ac, av)
	int ac;
	char **av;
{
	int i;
	char *p = "";

	for (i = 1; i < ac; i++) {
		printf("%s%s", p, av[i]);
		p = " ";
	}
	printf("\n");
	return 0;
}


halt_cmd(ac, av)
	int ac;
	char **av;
{
	ac = ac;
	av = av;
	if (!pvm_halt()) {
		pvmendtask();
		exit(0);
	}
	return 0;
}


char *helptx[] = {

	"add add         Add hosts to virtual machine",
	"add Syntax:  add hostname ...",
	"add   or     add 'hostname <option>=<value>...' ...",
	"add (See man page for 'pvmd3' for valid host (file) options.)",

	"alias alias       Define/list command aliases",
	"alias Syntax:  alias [name command args ...]",

	"conf conf        List virtual machine configuration",
	"conf Syntax:  conf",
	"conf Output fields:",
	"conf   HOST    host name",
	"conf   DTID    tid base of pvmd",
	"conf   ARCH    xhost architecture",
	"conf   SPEED   host relative speed",

	"delete delete      Delete hosts from virtual machine",
	"delete Syntax:  delete hostname ...",

	"echo echo        Echo arguments",
	"echo Syntax:  echo [ arg ... ]",

	"export export      Add environment variables to spawn export list",
	"export Syntax:  export [ varname ... ]",

	"getopt getopt      Display PVM options for the console task",
	"getopt Syntax:  getopt [ what ]",

	"halt halt        Stop pvmds",
	"halt Syntax:  halt",

	"help help        Print helpful information about a command",
	"help Syntax:  help [ command ]",

	"id id          Print console task id",
	"id Syntax:  id",

	"idump-idump          Call i_dump to display heap contents",
	"idump-Syntax:  idump [ how ]",

/*
	"insert-insert      Add entry to database",
	"insert-Syntax:  insert name index|-1 hexvalue",
*/

	"jobs jobs        Display list of running jobs",
	"jobs Syntax:  jobs [ options ]",
	"jobs Options:  -l   give long listing",

	"kill kill        Terminate tasks",
	"kill Syntax:  kill [ options ] tid ...",
	"kill Options:  -c   kill children of tid",

/*
	"lookup-lookup      Find entry in database",
	"lookup-Syntax:  lookup name index|-1",
*/

	"names names       List message mailbox names",
	"names Syntax:  names [ pattern ]",

	"mstat mstat       Show status of hosts",
	"mstat Syntax:  mstat name ...",

	"ps ps          List tasks",
	"ps Syntax:  ps [ -axh ]",
	"ps Options:  -a       all hosts (default is local)",
	"ps           -hhost   specific host tid",
	"ps           -nhost   specific host name",
	"ps           -l       long (show process id)",
	"ps           -x       show all tasks (e.g. console and nulls)",
	"ps Output fields:",
	"ps   HOST    host name",
	"ps   TID     task id",
	"ps   PTID    parent task id",
	"ps   PID     task process id",
	"ps   FLAG    status",
	"ps   COMMAND executable name",
	"ps FLAG values:",
	"ps   f   task process is child of pvmd",
	"ps   c   task connected to pvmd",
	"ps   a   task waiting authorization",
	"ps   o   task connection being closed",
	"ps   s   task needs too many shared pages, is deadlocked",
	"ps   H   hoster task",
	"ps   R   resource manager task",
	"ps   T   tasker task",

	"pstat pstat       Show status of tasks",
	"pstat Syntax:  pstat tid ...",

	"put put         Add entry to message mailbox",
	"put Syntax:  put name flags",

	"quit quit        Exit console",
	"quit Syntax:  quit",

/*
	"remove-remove      Delete entry from database",
	"remove-Syntax:  remove name index",
*/

	"reset reset       Kill all tasks, delete leftover mboxes",
	"reset Syntax:  reset\n",
	"reset reset info    Delete orphaned persistent mbox entries",
	"reset Syntax:  reset info [ class [ index ] ]",

	"setenv setenv      Display or set environment variables",
	"setenv Syntax:  setenv [ name [ value ] ]",

	"setopt setopt      Set PVM options - for the console task *only*!",
	"setopt Syntax:  setopt what value",

	"sig sig         Send signal to task",
	"sig Syntax:  sig signum task ...",

	"spawn spawn       Spawn task",
	"spawn Syntax:  spawn [ options ] file [ arg ... ]",
	"spawn Options:  -(count)         number of tasks, default is 1",
	"spawn           -(host)          spawn on host, default is any",
	"spawn           -(host):(wd)     spawn on host, in directory 'wd'",
	"spawn           --host=(IP)      spawn on host with given IP addr",
	"spawn           --host=(IP):(wd) spawn on IP, in directory 'wd'",
	"spawn           -(ARCH)          spawn on hosts of ARCH",
	"spawn           -(ARCH):(wd)     spawn on hosts of ARCH, in 'wd'",
	"spawn           -:(wd)           spawn in working directory 'wd'",
	"spawn           -?               enable debugging",
	"spawn           ->               redirect job output to console",
	"spawn           ->(file)         redirect output of job to file",
	"spawn           ->>(file)        append output of job to file",
	"spawn           -@               trace job, output to terminal",
	"spawn           -@(file)         trace job, output to file",

	"tickle-tickle      Tickle pvmd",
	"tickle-Syntax:  tickle how [ arg ... ]",
	"tickle-How:",
	"tickle-  0   dump heap",
	"tickle-  1   dump host table",
	"tickle-  2   dump local task table",
	"tickle-  3   dump waitc list",
	"tickle-  4   dump message mailbox",
	"tickle-  5   get debugmask",
	"tickle-  6   (mask) set debugmask",
	"tickle-        mask is the sum of the following bits for information about",
	"tickle-           1  Packet routing",
	"tickle-           2  Message routing and entry points",
	"tickle-           4  Task state",
	"tickle-           8  Slave pvmd startup",
	"tickle-          16  Host table updates",
	"tickle-          32  Select loop",
	"tickle-          64  IP network",
	"tickle-         128  Multiprocessor nodes",
	"tickle-         256  Resource manager interface",
	"tickle-         512  Application warnings (scrapped messages etc.)",
	"tickle-        1024  Wait contexts",
	"tickle-        2048  Shared memory operations",
	"tickle-        4096  Semaphores",
	"tickle-        8192  Locks",
	"tickle-       16384  Message routing",
	"tickle-       32768  Message mailbox",
	"tickle-       65536  Tracer tracing",
	"tickle-      131072  Multicast tracking",
	"tickle-  7   (num) set nopax",
	"tickle-  8   (dtid) trigger hostfail",
	"tickle-  9   (rst) dump pvmd statistics, clear if rst true",
	"tickle-  100 dump shared memory data structures",

	"trace trace       Set/display trace event mask",
	"trace Syntax:  trace",
	"trace          trace [+] name ...",
	"trace          trace - name ...",
	"trace          trace [+] *",
	"trace          trace - *",
	"trace          trace xpvm (default mask for XPVM display)",
	"trace          trace buf nbytes",
	"trace          trace buf",
	"trace          trace opt < full | time | count >",
	"trace          trace opt",

	"unalias unalias     Undefine command alias",
	"unalias Syntax:  unalias name ...",

	"unexport unexport    Remove environment variables from spawn export list",
	"unexport Syntax:  unexport [ varname ... ]",

	"version version     Show libpvm version",
	"version Syntax:  version",
	0
};


help_cmd(ac, av)
	int ac;
	char **av;
{
	char **p;
	char *topic;
	int l;
	struct cmdsw *csp;

	/* if not specified, topic = help */
	if (ac > 1)
		topic = av[1];
	else
		topic = "help";

	l = strlen(topic);

	/* search through messages for ones matching topic */
	for (p = helptx; *p; p++) {
		if (!strncmp(topic, *p, l) && ((*p)[l] == ' ' || (*p)[l] == '-'))
			printf("%s\n", (*p) + l + 1);
	}

	if (!strcmp(topic, "help")) {
		printf("Commands are:\n");
		for (csp = commands; csp->cmd; csp++) {
			l = strlen(csp->cmd);
			for (p = helptx; *p; p++)
				if (!strncmp(csp->cmd, *p, l) && (*p)[l] == ' ') {
					printf("  %s\n", (*p) + l + 1);
					break;
				}
		}
	}
	return 0;
}


id_cmd(ac, av)
	int ac;
	char **av;
{
	ac = ac;
	av = av;
	printf("t%x\n", mytid);
	return 0;
}


idump_cmd(ac, av)
	int ac;
	char **av;
{
	int how = 0;

	if (ac > 1)
		how = atoi(av[1]);
	i_dump(how);
	return 0;
}


/*
insert_cmd(ac, av)
	int ac;
	char **av;
{
	int cc;
	int data;

	ac = ac;
	data = axtoi(av[3]);
	if ((cc = pvm_insert(av[1], axtoi(av[2]), data)) >= 0)
		printf("%s, %d = 0x%08x\n", av[1], cc, data);
	else
		if (cc == PvmDupEntry)
			printf("already exists\n");
	return 0;
}
*/


jobs_cmd(ac, av)
	int ac;
	char **av;
{
	struct job *jp;
	struct obuf *op;
	int l = 0;
	int ntask;
	struct pvmtaskinfo *tip;
	TRC_TEVTASK tt;

	if (ac > 1 && !strcmp(av[1], "-l")) {
		l = 1;
		print_task_hdr(0);
		if (!hostlist)
			pvm_config(&nhosts, &narchs, &hostlist);
	}

	for (jp = joblist->j_link; jp != joblist; jp = jp->j_link) {
		printf("%c%d:%s", (jp->j_flag & JOB_TRACE ? 'T' : ' '),
				jp->j_jid - joboffset, (l ? "\n" : ""));
		if (jp->j_flag & JOB_TRACE) {
			tt = jp->j_trcid->tevtask_list;
			while ( tt ) {
				if ( tt->tevstatus == TRC_TASK_ALIVE
						|| tt->outstatus == TRC_TASK_OUT ) {
					if (l) {
						if (!pvm_tasks(tt->tid, &ntask, &tip)
								&& ntask == 1)
							print_task_rec(&tip[0], 0);
						else
							printf(" t%x", tt->tid);
						printf("\n");
					} else
						printf(" t%x", tt->tid);
				}
				tt = tt->next;
			}
			if (!l)
				printf("\n");
		}
		else if (jp->j_obufs) {
			for (op = jp->j_obufs->o_link; op != jp->j_obufs;
					op = op->o_link)
				if (l) {
					if (!pvm_tasks(op->o_tid, &ntask, &tip)
							&& ntask == 1)
						print_task_rec(&tip[0], 0);
					else
						printf(" t%x", op->o_tid);
					printf("\n");
				} else
					printf(" t%x", op->o_tid);
			if (!l)
				printf("\n");
		}
	}
	return 0;
}


kill_cmd(ac, av)
	int ac;
	char **av;
{
	int i;
	int tid, tid2;
	char *p;
	int host = 0;
	int cflg = 0;
	struct pvmtaskinfo *tip;
	int ntask;

	if (ac > 1 && av[1][0] == '-') {
		ac--;
		for (p = *++av; *p; p++)
			switch (*p) {

			case 'c':
				cflg = 1;
				break;

			case '-':
				break;

			default:
				printf("unknown flag -%c\n", *p);
				break;
			}
	}

	if (ac < 2) {
		fputs("incorrect arg count\n", stdout);
		return 1;
	}

	if (cflg && pvm_tasks(host, &ntask, &tip) < 0)
		return 1;

	while (ac > 1) {
		ac--;
		tid = tidtoi(*++av);

		if (cflg) {
			for (i = 0; i < ntask; i++)
				if (tip[i].ti_ptid == tid) {
					tid2 = tip[i].ti_tid;
					if (tid2 != mytid)
						pvm_kill(tid2);
				}

		} else {
			if (tid == mytid)
				printf("t%x: that's me.\n", tid);
			else
				pvm_kill(tid);
		}
	}
	return 0;
}


/*
lookup_cmd(ac, av)
	int ac;
	char **av;
{
	int cc, data = 0;

	ac = ac;
	if ((cc = pvm_lookup(av[1], axtoi(av[2]), &data)) >= 0)
		printf("%s, %d = 0x%08x\n", av[1], cc, data);
	else
		if (cc == PvmNoEntry)
			printf("no such entry\n");
	return 0;
}
*/


mstat_cmd(ac, av)
	int ac;
	char **av;
{
	int i;
	int cc;

	for (i = 1; i < ac; i++) {
		cc = pvm_mstat(av[i]);
		printf("%24s  %s\n", av[i], (cc < 0 ? PVMERRMSG(cc) : "ok"));
	}
	return 0;
}


pstat_cmd(ac, av)
	int ac;
	char **av;
{
	int i;
	int tid;
	int cc;

	for (i = 1; i < ac; i++) {
		tid = tidtoi(av[i]);
		cc = pvm_pstat(tid);
		printf("t%8x  %s\n", tid, (cc < 0 ? PVMERRMSG(cc) : "run"));
	}
	return 0;
}


quit_cmd()
{
	printf("\n");
	if (mytid > 0) {
		pvm_exit();
		printf("pvmd still running.\n");
	}
#ifdef WIN32
	WSACleanup();
#endif
	exit(0);
	return 0;  /* for stupid SGI compiler :-Q */
}


/*
remove_cmd(ac, av)
	int ac;
	char **av;
{
	int cc;

	ac = ac;
	if ((cc = pvm_delete(av[1], axtoi(av[2]))) > 0)
		printf("deleted %s, %d\n", av[1], cc);
	else
		if (cc == PvmNoEntry)
			printf("no such entry\n");
	return 0;
}
*/


reset_cmd(ac, av)
	int ac;
	char **av;
{
	char *class = (char *) NULL;

	int killtasks = 1;
	int index = -1;

	/* Check for Info (mbox) Reset */

	if ( ac > 1 ) {
		if ( !strcmp( av[1], "info" ) ) {
			killtasks = 0;
			if ( ac > 2 ) {
				class = av[2];
				if ( ac > 3 )
					index = atoi( av[3] );
			}
		} else {
			printf( "\"%s\" arg to reset not valid\n", av[1] );
			return 1;
		}
	}

	/* Call the universal VM-spanker routine...  :-) */
	pvmreset( mytid, killtasks, class, index );

	return 0;
}


setenv_cmd(ac, av)
	int ac;
	char **av;
{
	char **pp;
	char *p;
	char *sep;
	int n;
	int i;

	switch (ac) {

	case 1:
		for (pp = environ; *pp; pp++)
			printf("%s\n", *pp);
		break;

	case 2:
		if (p = getenv(av[1]))
			printf("%s\n", p);
		break;

	default:
		if (ac > 2) {
			n = 0;
			for (i = ac; i-- > 1; )
				n += strlen(av[i]) + 1;
			p = TALLOC(n, char, "env");
			strcpy(p, av[1]);
			strcat(p, "=");
			sep = "";
			for (i = 2; i < ac; i++) {
				strcat(p, sep);
				strcat(p, av[i]);
				sep = " ";
			}
			pvmputenv(p);
		}
		break;
	}
	return 0;
}


static char *setop_strs[] = {
	"PvmRoute",
	"PvmDebugMask",
	"PvmAutoErr",
	"PvmOutputTid",
	"PvmOutputCode",
	"PvmTraceTid",
	"PvmTraceCode",
	"PvmTraceBuffer",
	"PvmTraceOptions",
	"PvmFragSize",
	"PvmResvTids",
	"PvmSelfOutputTid",
	"PvmSelfOutputCode",
	"PvmSelfTraceTid",
	"PvmSelfTraceCode",
	"PvmSelfTraceBuffer",
	"PvmSelfTraceOptions",
	"PvmShowTids",
#if defined(IMA_AIX4MP) || defined(IMA_AIX5MP) \
		|| defined(IMA_ALPHAMP) || defined(IMA_CSPP) \
		|| defined(IMA_HPPAMP) || defined(IMA_RS6KMP) \
		|| defined(IMA_SGIMP) || defined(IMA_SGIMP6) \
		|| defined(IMA_SGIMP64) || defined(IMA_SUNMP)
	"PvmPollType",
	"PvmPollTime",
#else
	"",
	"",
#endif
	"PvmOutputContext",
	"PvmTraceContext",
	"PvmSelfOutputContext",
	"PvmSelfTraceContext",
	"PvmNoReset",
	NULL
};


setopt_cmd(ac, av)
	int ac;
	char **av;
{
	char **name;
	int what = -1;
	int value;
	int cc;
	int i;

	name = setop_strs;

	i = 1;

	while ( *name != NULL && what < 0 ) {
		if ( !strcmp( *name, av[1] ) )
			what = i;
		else {
			name++; i++;
		}
	}

	if ( what > 0 ) {
		value = atoi( av[2] );
		cc = pvm_setopt( what, value );
		printf("%s Changed from %d to %d\n", av[1], cc, value );
	}
	else
		printf("PVM Option \"%s\" Unknown.\n", av[1] );

	return 0;
}


getopt_cmd(ac, av)
	int ac;
	char **av;
{
	char **name;
	int what = -1;
	int value;
	int cc;
	int i;

	switch( ac ) {

	case 1:
		name = setop_strs;
		i = 1;
		while ( *name != NULL ) {
			if ( strcmp( *name, "" ) ) {
				cc = pvm_getopt( i );
				printf("%s Currently Set to %d\n", *name, cc );
			}
			name++; i++;
		}
		break;

	case 2:
		name = setop_strs;
		i = 1;
		while ( *name != NULL && what < 0 ) {
			if ( !strcmp( *name, av[1] ) )
				what = i;
			else {
				name++; i++;
			}
		}
		if ( what > 0 ) {
			cc = pvm_getopt( what );
			printf("%s Currently Set to %d\n", av[1], cc );
		}
		else
			printf("PVM Option \"%s\" Unknown.\n", av[1] );
		break;
	}

	return 0;
}


sig_cmd(ac, av)
	int ac;
	char **av;
{
	int i;
	int signum;
	int tid;

	signum = atoi(av[1]);
	for (i = 2; i < ac; i++) {
		tid = tidtoi(av[i]);
		pvm_sendsig(tid, signum);
	}
	return 0;
}


spawn_cmd(ac, av)
	int ac;
	char **av;
{
	int *tids = 0;
	char *where = 0;
	int flags = PvmNoSpawnParent;
	int count = 1;
	int i;
	int oflg = 0;
	int tflg = 0;
	int app;
	char *ofn = 0;
	char *tfn = 0;
	struct job *jp, *jp2;
	void status_msg();
	void event_dump_hdr();
	void output_dump_hdr();
	int fd;

	while (av[1][0] == '-') {
		if (ac < 3) {
			fputs("incorrect arg count\n", stdout);
			return 1;
		}
		if (av[1][1] == '~') {
			flags |= PvmHostCompl;
			av[1]++;
		}
		if (av[1][1] == '.' || islower(av[1][1])) {
			where = av[1] + 1;
			flags |= PvmTaskHost;
		}
		if (av[1][1] == ':')
			where = av[1] + 1;
		if (isupper(av[1][1])) {
			where = av[1] + 1;
			flags |= PvmTaskArch;
		}
		if (av[1][1] == '?')
			flags |= PvmTaskDebug;
		if (isdigit(av[1][1]))
			count = atoi(av[1] + 1);
		if (av[1][1] == '>') {
			oflg = 1;
			app = 0;
			ofn = av[1] + 2;
			if (av[1][2] == '>') {
				app = 1;
				ofn++;
			}
			if (!*ofn)
				ofn = 0;
/*
			printf("%s to %s\n", (app ? "Append" : "Write"),
					(ofn ? ofn : "(console)"));
*/
		}
		if (av[1][1] == '@') {
			tflg = 1;
			tfn = av[1] + 2;
			if (!*tfn)
				tfn = "";
		}
		/* gnu-like host (where) arg, allows IP's to be used... */
		if (av[1][1] == '-') {
			if ( !strncmp( &(av[1][2]), "host=", 5 ) ) {
				where = av[1] + 7;
				flags |= PvmTaskHost;
			}
		}
		av++;
		ac--;
	}
	if (tflg) {
		jp2 = job_new(nextjob);
		jp2->j_flag |= JOB_TRACE;
		jp2->j_trcid = trc_get_tracer_id();
		jp2->j_trcid->status_msg = status_msg;
		if (tfn && !strcmp(tfn,""))
			jp2->j_trcid->event_dump_hdr = event_dump_hdr;
		if (oflg && !ofn) ofn = "";
		if (ofn)
			jp2->j_trcid->output_dump_hdr = output_dump_hdr;
		jp2->j_trcid->event_ctx = pvm_getcontext();
		jp2->j_trcid->event_tag = nextjob;
		jp2->j_trcid->output_ctx = pvm_getcontext();
		jp2->j_trcid->output_tag = nextjob + 1;
		trc_set_tracing_codes( jp2->j_trcid );
		printf("[%d]\n", nextjob - joboffset);
		nextjob += 2;
		trc_set_trace_file( jp2->j_trcid, tfn );
		if (!trc_reset_trace_file( jp2->j_trcid )) {
			job_free(jp2);
			return 1;
		}
		trc_save_host_status_events( jp2->j_trcid );
		if (ofn) {
			trc_set_output_file( jp2->j_trcid, ofn );
			if (!trc_open_output_file( jp2->j_trcid )) {
				job_free(jp2);
				return 1;
			}
		}
		oflg = 0;
	}
	else {
		pvm_setopt(PvmTraceTid, 0);

		if (oflg) {
			pvm_setopt(PvmOutputTid, mytid);
			pvm_setopt(PvmOutputContext, pvm_getcontext());
			pvm_setopt(PvmOutputCode, nextjob);
			jp = job_new(nextjob);
			printf("[%d]\n", nextjob - joboffset);
			nextjob++;
			if (ofn) {
				if ( app ) {
					fd = open( ofn,
						O_RDWR|O_APPEND|O_CREAT|O_EXCL|O_TRUNC, 0600 );
				}
				else {
					fd = open( ofn, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600 );
				}
				if (fd < 0) {
					perror(ofn);
					job_free(jp);
					return 1;
				}
				jp->j_ff = fdopen( fd, (app ? "a" : "w") );
				if (!jp->j_ff) {
					perror(ofn);
					job_free(jp);
					return 1;
				}
			}
		} else
			pvm_setopt(PvmOutputTid, 0);
	}

	tids = TALLOC(count > 1 ? count : 1, int, "int");
	if ((i = pvm_spawn(av[1], &av[2], flags, where, count, tids)) >= 0) {
		if (oflg & !i)
			job_free(jp);
		if (tflg & !i)
			job_free(jp2);
		printf("%d successful\n", i);
		for (i = 0; i < count; i++)
			if (tids[i] < 0)
				printf("%s\n", PVMERRMSG(tids[i]));
			else
				printf("t%x\n", tids[i]);
	}
	MY_FREE(tids);
	return 0;
}


static char *tflgs[] = {
	0, "f", "c", "a", "o", "s", 0, 0,
	"R", "H", "T"
};

char *
task_flags(f)
	int f;
{
	static char buf[64];
	int bit, i;

	sprintf(buf, "%x/", f);
	i = sizeof(tflgs)/sizeof(tflgs[0]) - 1;
	bit = 1 << i;
	while (i >= 0) {
		if ((f & bit) && tflgs[i]) {
			strcat(buf, tflgs[i]);
			strcat(buf, ",");
		}
		bit /= 2;
		i--;
	}
	buf[strlen(buf) - 1] = 0;
	return buf;
}


print_task_hdr(lflg)
	int lflg;
{
	if (lflg)
		fputs("                    HOST      TID     PTID    PID   FLAG 0x COMMAND\n",
			stdout);
	else
		fputs("                    HOST      TID   FLAG 0x COMMAND\n",
			stdout);
	return 0;
}


print_task_rec(tip, lflg)
	struct pvmtaskinfo *tip;
	int lflg;
{
	struct pvmhostinfo *hip = 0;

	if (hostlist) {
		for (hip = hostlist + nhosts - 1; hip >= hostlist; hip--)
			if (hip->hi_tid == tip->ti_host)
				break;
		if (hip < hostlist)
			hip = 0;
	}
	if (hip)
		printf("%24s", hip->hi_name);
	else
		printf("%24x", tip->ti_host);
	if (tip->ti_tid == mytid)
		printf("   (cons)");
	else
		printf(" %8x", tip->ti_tid);
	if (lflg) {
		if (tip->ti_ptid == mytid)
			printf("   (cons)");
		else
			if (tip->ti_ptid)
				printf(" %8x", tip->ti_ptid);
			else
				printf("        -");
		printf(" %6d", tip->ti_pid);
	}
	printf(" %9s", task_flags(tip->ti_flag));
	printf(" %-12s", tip->ti_a_out[0] ? tip->ti_a_out : "-");
	return 0;
}


tasks_cmd(ac, av)
	int ac;
	char **av;
{
	struct pvmtaskinfo *tip;
	int ntask;
	int i;
	struct pvmhostinfo *hip = 0;
	int xflg = 0;
	char *p;
	int host = pvm_tidtohost(mytid);
	int lflg = 0;

	if (!hostlist)
		pvm_config(&nhosts, &narchs, &hostlist);

	if (ac > 1) {
		for (p = av[1]; *p; p++)
			switch (*p) {

			case 'n':
				p++;
				if (!strcmp(p, "."))
					pvm_tidtohost(mytid);
				else {
					for (hip = hostlist + nhosts; --hip >= hostlist; )
						if (!(strcmp(p, hip->hi_name))) {
							host = hip->hi_tid;
							break;
						}
					if (hip < hostlist)
						printf("unknown host %s\n", p);
				}
				while (*p)
					p++;
				p--;
				break;

			case 'h':
				host = tidtoi(++p);
				while (*p)
					p++;
				p--;
				break;

			case 'a':
				host = 0;
				break;

			case 'l':
				lflg = 1;
				break;

			case 'x':
				xflg = 1;
				break;

			case '-':
				break;

			default:
				printf("unknown flag -%c\n", *p);
				break;
			}
	}

	if (!pvm_tasks(host, &ntask, &tip)) {
		print_task_hdr(lflg);
		for (i = 0; i < ntask; i++) {
			if (!xflg && (tip[i].ti_tid == mytid || tip[i].ti_tid == 0))
				continue;
			print_task_rec(&tip[i], lflg);
			printf("\n");
		}
	}
	return 0;
}


tickle_cmd(ac, av)
	int ac;
	char **av;
{
	int nar;
	int arg[10];
	int i;

	ac--;
	av++;
	for (nar = 0; nar < ac; nar++)
		arg[nar] = axtoi(av[nar]);

	if (!pvm_tickle(nar, arg, &nar, arg)) {
		printf("(");
		for (i = 0; i < nar; i++)
			printf(" %d", arg[i]);
		printf(" )\n");
	}
	return 0;
}


printtm(who)
	int who;
{
	Pvmtmask tm;

	pvm_gettmask(who, tm);

	trc_print_tmask(tm);

	return 0;
}


trace_cmd(ac, av)
	int ac;
	char **av;
{
	static int topt = PvmTraceFull;
	static int tbuf = 0;
	int i;
	int onoff = 1;
	int e;
	int l;
	char buf[64];
	char *optstr;
	Pvmtmask tm;
	int settm;

	if (ac == 1) {
		printtm(PvmTaskChild);
		return 0;
	}

	pvm_gettmask(PvmTaskChild, tm);

	settm = 0;

	for (i = 1; i < ac; i++) {
		if (!strcmp(av[i], "+")) {
			onoff = 1;
		} else if (!strcmp(av[i], "-")) {
			onoff = 0;
		} else if (!strcmp(av[i], "*")) {
			for (e = TEV_FIRST; e <= TEV_MAX; e++) {
				if (onoff)
					TEV_MASK_SET(tm, e);
				else
					TEV_MASK_UNSET(tm, e);
			}
			settm++;
		} else if (!strcmp(av[i], "buf")) {
			if ( i + 1 < ac ) {
				tbuf = atoi(av[++i]);
				pvm_setopt( PvmTraceBuffer, tbuf );
			} else {
				printf("\nTrace Buffering set to %d bytes\n\n", tbuf);
			}
		} else if (!strcmp(av[i], "opt")) {
			if ( i + 1 < ac ) {
				i++;
				if ( !strcmp( av[i], "full" ) )
					topt = PvmTraceFull;
				else if ( !strcmp( av[i], "time" ) )
					topt = PvmTraceTime;
				else if ( !strcmp( av[i], "count" ) )
					topt = PvmTraceCount;
				else {
					printf( "\nError:  Unknown Trace Option \"%s\"\n\n",
						av[i] );
				}
				pvm_setopt( PvmTraceOptions, topt );
			} else {
				switch ( topt )
				{
					case PvmTraceFull:		optstr = "full";	break;
					case PvmTraceTime:		optstr = "time";	break;
					case PvmTraceCount:		optstr = "count";	break;
					default: {
						sprintf( buf, "<Unknown=%d>", topt );
						optstr = buf;
					}
				}
				printf("\nTrace Options set to \"%s\"\n\n", optstr );
			}
		} else if (!strcmp(av[i], "xpvm")) {
			TEV_MASK_INIT( tm );
			TEV_MASK_SET( tm, TEV_BARRIER );
			TEV_MASK_SET( tm, TEV_BCAST );
			TEV_MASK_SET( tm, TEV_JOINGROUP );
			TEV_MASK_SET( tm, TEV_LVGROUP );
			TEV_MASK_SET( tm, TEV_MYTID );
			TEV_MASK_SET( tm, TEV_PARENT );
			TEV_MASK_SET( tm, TEV_SIBLINGS );
			TEV_MASK_SET( tm, TEV_SPAWN );
			TEV_MASK_SET( tm, TEV_SENDSIG );
			TEV_MASK_SET( tm, TEV_KILL );
			TEV_MASK_SET( tm, TEV_EXIT );
			TEV_MASK_SET( tm, TEV_MCAST );
			TEV_MASK_SET( tm, TEV_SEND );
			TEV_MASK_SET( tm, TEV_RECV );
			TEV_MASK_SET( tm, TEV_RECVF );
			TEV_MASK_SET( tm, TEV_NRECV );
			TEV_MASK_SET( tm, TEV_PSEND );
			TEV_MASK_SET( tm, TEV_PRECV );
			TEV_MASK_SET( tm, TEV_TRECV );
			TEV_MASK_SET( tm, TEV_PROBE );
			TEV_MASK_SET( tm, TEV_ADDMHF );
			TEV_MASK_SET( tm, TEV_DELMHF );
			TEV_MASK_SET( tm, TEV_MHF_INVOKE );
			TEV_MASK_SET( tm, TEV_USER_DEFINED );
			settm++;
		} else {
			if (!strncmp(av[i], "pvm_", 4))
				strcpy(buf, av[i] + 4);
			else
				strcpy(buf, av[i]);
			for (e = TEV_FIRST; e <= TEV_MAX; e++)
				if (!strcmp(pvmtevinfo[e].name, buf)
				|| !strcmp(pvmtevinfo[e].name, av[i]))
					break;
			if ( e <= TEV_MAX ) {
				if (onoff)
					TEV_MASK_SET(tm, e);
				else
					TEV_MASK_UNSET(tm, e);
				settm++;
			}
			else
				printf("no such event \"%s\"\n", av[i]);
		}
	}

	if ( settm )
		pvm_settmask(PvmTaskChild, tm);

	return 0;
}


unalias_cmd(ac, av)
	int ac;
	char **av;
{
	struct alias *ap;
	int i;

	for (i = 1; i < ac; i++)
		if (ap = findalias(av[i]))
			freealias(ap);
	return 0;
}


version_cmd(ac, av)
	int ac;
	char **av;
{
	ac = ac;
	av = av;
	printf("%s\n", pvm_version());
	return 0;
}


names_cmd(ac, av)
	int ac;
	char **av;
{
	struct pvmmboxinfo *classes;
	int nclasses;
	int i, j;
	int cc;

	pvm_getmboxinfo((ac > 1 ? av[1] : "*"), &nclasses, &classes);
	for ( i=0 ; i < nclasses ; i++ )
	{
		printf("<%s>:\n", classes[i].mi_name);
		for ( j=0 ; j < classes[i].mi_nentries ; j++ )
			printf("\tindex=%d owner=0x%x/%d flags=0x%x\n",
				classes[i].mi_indices[j],
				classes[i].mi_owners[j], classes[i].mi_owners[j],
				classes[i].mi_flags[j] );
		printf("\n");
	}
	return 0;
}


put_cmd(ac, av)
	int ac;
	char **av;
{
	int cc;

	pvm_packf("%+ %d", PvmDataDefault, 0);
	cc = pvm_putinfo(av[1], pvm_getsbuf(), (int)atoi(av[2]));
	if (cc < 0)
		printf("%s\n", pvm_errlist[-cc]);
	return 0;
}


export_cmd(ac, av)
	int ac;
	char **av;
{
	int i;
	char *p;

	if (ac == 1) {
		p = getenv("PVM_EXPORT");
		printf("PVM_EXPORT=%s\n", p ? p : "");

	} else {
		for (i = 1; i < ac; i++)
			pvm_export(av[i]);
	}
	return 0;
}


unexport_cmd(ac, av)
	int ac;
	char **av;
{
	int i;

	for (i = 1; i < ac; i++)
		pvm_unexport(av[i]);
	return 0;
}


