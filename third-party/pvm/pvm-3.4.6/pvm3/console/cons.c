
static char rcsid[] =
	"$Id: cons.c,v 1.32 2007/04/19 23:14:46 pvmsrc Exp $";

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
 *	cons.c
 *
 *	PVM-console task.  Gives the user a shell to manage the virtual
 *	machine and other tasks.
 *
 * $Log: cons.c,v $
 * Revision 1.32  2007/04/19 23:14:46  pvmsrc
 * Added cast of completion_matches() to char ** to eliminate gcc warning.
 * (Spanker=kohl)
 *
 * Revision 1.31  2001/02/07 23:10:09  pvmsrc
 * CYGWIN Changes.
 * (Spanker=kohl)
 *
 * Revision 1.30  2000/02/15 18:05:54  pvmsrc
 * Modified console to intialize trace mask at startup.
 * 	- use "trace xpvm" command via trace_cmd() interface.
 * (Spanker=kohl)
 *
 * Revision 1.29  1999/07/08 18:02:48  pvmsrc
 * Fixed up crummy readline garbage.
 * 	- added omitted "char *line" argument to rl_got_line(),
 * 		use instead of rl_line_buffer global, else miss EOF condition.
 * 	- in command_completion(), keep track of last line buffer,
 * 		only dump help syntax once, then let readline dump files
 * 		instead.
 * (Spanker=kohl)
 *
 * Revision 1.28  1999/06/11 19:40:07  pvmsrc
 * Another (re-)update of the readline stuff from Edward Vigmond...
 * (Spanker=kohl)
 * 
 * Revision 1.27  1999/06/11 17:27:34  pvmsrc
 * When breaking up multiline commands into subcommands,
 * 	dump out each command as you execute it, along with a fresh prompt.
 * (Spanker=kohl)
 * 
 * Revision 1.26  1999/05/18  22:29:55  pvmsrc
 * More hacks on the readline stuff.
 * 	- from Edward Vigmond <vigmonde@IGB.UMontreal.CA>.
 * (Spanker=kohl)
 *
 * Revision 1.25  1999/03/25  21:49:31  pvmsrc
 * Readline Support Update.
 * 	- from Edward Vigmond <vigmonde@IGB.UMontreal.CA>.
 * (Spanker=kohl)
 *
 * Revision 1.24  1999/02/05  20:36:15  pvmsrc
 * More contributions from Edward Vigmond <vigmonde@IGB.UMontreal.CA>.
 * 	- better readline interface, with command completion & help.
 * (Spanker=kohl)
 *
 * Revision 1.23  1999/02/03  19:57:14  pvmsrc
 * Added support for system that have readline().
 * 	- patch submitted by Edward Vigmond <vigmonde@IGB.UMontreal.CA>.
 * (Spanker=kohl)
 *
 * Revision 1.22  1998/11/20  19:43:09  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * source for win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.21  1998/10/05  21:12:06  pvmsrc
 * One last uncomment from Markus...  :-)
 * (Spanker=kohl)
 *
 * Revision 1.20  1998/10/02  16:23:36  pvmsrc
 * Single source code merge of Win32 and Unix code.
 * oops - last one missed some things...
 * (Spanker=sscott)
 *
 * Revision 1.19  1998/01/28  23:03:40  pvmsrc
 * Fixed tracing bogusness!
 * 	- host add / del notifies for tracing were getting scrunched by
 * 		console host add notify, etc...
 * 	- moved message / notify tag constants to job.h and USED THEM.
 * 	- initialized nextjob properly...
 * 	- added new "joboffset" global in cons.c for dumping more
 * 		reasonable job numbers to user.
 * (Spanker=kohl)
 *
 * Revision 1.18  1997/12/29  19:38:17  pvmsrc
 * 	Deleted linux/time.h include.  Don't need it on Redhat 4.2 or 5.0.
 * (Spanker=phil)
 *
 * Revision 1.17  1997/12/01  19:21:25  pvmsrc
 * Replaced #ifdef IMA_OS2 fd_set declarations:
 * 	- new #ifdef FDSETNOTSTRUCT.
 * 	- choose between "fd_set foo" and "struct fd_set foo"...
 * (Spanker=kohl)
 *
 * Revision 1.16  1997/11/04  23:14:45  pvmsrc
 * Cleaned up fd_set stuff (hopefully).
 * (Spanker=kohl)
 *
 * Revision 1.15  1997/07/09  13:21:04  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.14  1997/06/25  18:03:58  pvmsrc
 * WIN32 changes from Markus.
 *
 * Revision 1.13  1997/05/13  14:37:39  pvmsrc
 * Changed header file $includes:
 * 	- ../src/listmac.h -> listmac.h
 * 	- ../src/bfunc.h -> bfunc.h
 * 	- use -I$(PVMDIR)/src in Makefile.aimk instead.
 *
 * Revision 1.12  1997/05/01  15:49:44  pvmsrc
 * Oops...  exit_handler() must be int funct, not void.
 * 	- for pvm_addmhf()...
 *
 * Revision 1.11  1997/05/01  15:41:16  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.10  1997/04/29  19:37:41  pvmsrc
 * Commented out signal( SIGTERM, SIGIGN ).
 * 	- not needed with PvmNoReset setopt now...
 *
 * Revision 1.9  1997/04/24  12:37:04  pvmsrc
 * Added NEEDSSELECTH switch to simplify ifdefs on IBM machines.
 *
 * Revision 1.8  1997/04/08  20:22:31  pvmsrc
 * Sonofabitch.  Typo.
 *
 * Revision 1.7  1997/04/08  20:15:28  pvmsrc
 * Spanked sscott's spank...  :-}
 *
 * Revision 1.6  1997/03/06  14:52:48  pvmsrc
 * Yanked out spank.
 * 	- removed Bob's test code for mbox & context...
 *
 * Revision 1.5  1997/02/17  19:51:54  pvmsrc
 * Added setting of PvmNoReset for pvm console.
 *
 * Revision 1.4  1997/01/28  19:13:18  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/24  23:04:05  pvmsrc
 * Updated for new tracing facility.
 * 	- new init, select handling with checkmsgs(),
 * 		and notify event / trace / output handling.
 *
 * Revision 1.2  1996/10/08  18:29:40  pvmsrc
 * Renamed routines:
 * 	- pvm_put() -> pvm_putinfo().
 * 	- pvm_get() -> pvm_getinfo().
 *
 * Revision 1.1  1996/09/23  20:25:36  pvmsrc
 * Initial revision
 *
 * Revision 1.6  1995/05/30  16:51:30  manchek
 * added SP2MPI architecture
 *
 * Revision 1.5  1995/05/17  15:28:06  manchek
 * added FDSETISINT switch.
 * allow pvm_getfds() to fail silently (e.g. for shared memory)
 *
 * Revision 1.4  1994/10/15  18:40:09  manchek
 * don't compare add-host notify source to 0x80000000
 *
 * Revision 1.3  1994/06/03  20:01:52  manchek
 * version 3.3.0
 *
 * Revision 1.2  1993/09/16  21:33:40  manchek
 * moved notify(HostAdd) to before we read script file.
 * added include for linux/time.h
 *
 * Revision 1.1  1993/08/30  23:30:32  manchek
 * Initial revision
 *
 */

#include <stdio.h>
#include <sys/types.h>
/* #ifdef IMA_LINUX */
/* #include <linux/time.h> */
/* #endif */
#ifdef NEEDSSELECTH
#include <sys/select.h>
#endif
#ifdef	IMA_I860
#include <sys/socket.h>
#endif
#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
#include <ctype.h>
#include <signal.h>
#include <pvm3.h>
#include <pvmtev.h>
#include "cmd.h"
#include "myalloc.h"
#include "listmac.h"
#include "bfunc.h"
#include "job.h"

#ifdef HASREADLINE

#include <readline/readline.h>

#ifndef CYGWIN
#include <readline/history.h>
#endif

char **command_completion();

void rl_got_line();

#endif

#define	PVMERRMSG(n)	((n) <= 0 && (n) > -pvm_nerr \
						? pvm_errlist[-(n)] : "Unknown Error")

char *getenv();

#ifdef WIN32
extern int nAlert;
WSADATA WSAData;
#endif			 		

extern char *pvm_errlist[];
extern int pvm_nerr;

extern struct job *joblist;				/* from job.c */

extern struct cmdsw commands[];
extern char *helptx[];

int mytid = -1;
int nhosts = 0;
int narchs = 0;
struct pvmhostinfo *hostlist = 0;
struct alias *aliases = 0;

int joboffset = FirstJobTag - 1;
int nextjob = FirstJobTag;

#ifdef WIN32
int system_loser_win = FALSE;
#endif

char *prompt = "pvm> ";


int
exit_handler(mid)
	int mid;
{
	printf("Console: exit handler called\n");
	pvm_freebuf(mid);
	return 0;
}


main(argc, argv)
	int argc;
	char **argv;
{
	static struct timeval more_msgs = { 0, 0 };
	char cmd[1024];
	int cc;
	int i;
	int src, cod, len;
	char *p;
	FILE *ff;
#ifdef FDSETNOTSTRUCT
	fd_set rfds, fds;
#else
	struct fd_set rfds, fds;
#endif
	int nfds;
	int n;
	int *np = 0;
	struct pvmminfo minfo;
	struct timeval *timeout;
	int more;
	int ac;
	char *av[128];

#ifdef WIN32

	/* WSAStartup has to be called before any socket command */
	/* can be executed. Why ? Ask Bill */

	HANDLE hStdin;
	struct timeval zerotimeout = { 0, 0 };

/*
	if ( WSAStartup( 0x0101, &WSAData ) != 0 ) {
		printf("\nWSAStartup() failed\n");
		ExitProcess( 1 );
	}
*/

#endif

#ifdef CYGWIN

	/*
		To run properly, the PVM console has to be run
		in CYGWIN's tty mode
	*/

	{
		char *cygwin_env;
		int cygwin_tty = 0;
	
		cygwin_env = getenv("CYGWIN");
		if (cygwin_env==NULL) 
			cygwin_env = getenv("CYGWIN32");
		if (cygwin_env!=NULL) {
			cygwin_tty  = (strstr(cygwin_env, "tty") == NULL);
			cygwin_tty |= (strstr(cygwin_env, "TTY") == NULL);
		}
		if (!cygwin_tty) {
			fprintf(stderr, 
		 		"PVM will not run properly if not in tty mode.\n");
			fprintf(stderr, 
		 		"Please set the following before calling PVM again:\n");
			fprintf(stderr, 
		 		"SET CYGWIN=tty   (Cygwin B20)\n");
			fprintf(stderr, 
		 		"SET CYGWIN32=tty (Cygwin B19)\n");
			exit(1);
		}
	}		

#endif

	pvm_setopt(PvmResvTids, 1);

	aliases = TALLOC(1, struct alias, "alias");
	BZERO((char*)aliases, sizeof(struct alias));
	aliases->a_link = aliases->a_rlink = aliases;

	if (argc > 1 && argv[1][0] == '-' && argv[1][1] == 'd')
		pvm_setopt(PvmDebugMask, pvmstrtoi(argv[1] + 2));

	pvm_setopt(PvmRoute, PvmDontRoute);

	pvm_setopt(PvmSelfTraceTid, -1);
	pvm_setopt(PvmSelfOutputTid, -1);

	i = pvm_setopt(PvmAutoErr, 0);
	cc = pvm_start_pvmd(argc - 1, argv + 1, 1);
	if (cc < 0) {
		if (cc != PvmDupHost) {
			pvm_perror("Console");
			exit(1);
		}
		printf("pvmd already running.\n");
	}
	pvm_setopt(PvmAutoErr, i);

	if ((mytid = pvm_mytid()) < 0)
		exit(1);

	(void)signal(SIGINT, SIG_IGN);
	/* (void)signal(SIGTERM, SIG_IGN); now PvmNoReset */

#ifndef WIN32	/* NT console problem - original NT codes had commented out */
	pvm_setopt( PvmNoReset, 1 );
	pvm_setopt( PvmTraceOptions, PvmTraceFull );
	pvm_setopt( PvmTraceBuffer, 0 );
#endif
	job_init();

	/* set default trace mask to xpvm */
	sprintf(cmd, "trace xpvm");
	ac = sizeof(av)/sizeof(av[0]) - 1;
	if (!acav(cmd, &ac, av) && ac)
		trace_cmd(ac, av);

	pvm_notify(PvmHostAdd, HostsAddedTag, -1, (int*)0);

	BZERO(&minfo, sizeof(minfo));
	minfo.src = -1;
	minfo.ctx = -1;
	minfo.tag = MyExitTag;

#ifndef WIN32	/* NT console problem - original NT codes had commented out */
	pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, exit_handler);
	pvm_notify(PvmTaskExit, MyExitTag, 1, &mytid);
#endif

	if (!(p = getenv("HOME")))
		p = ".";
	sprintf(cmd, "%s/.pvmrc", p);
	if (ff = fopen(cmd, "r")) {
		while (fgets(cmd, sizeof(cmd)-1, ff))
			docmd(cmd);
		(void)fclose(ff);
	}

	FD_ZERO(&rfds);

#ifndef WIN32
	FD_SET(0, &rfds);
#endif

	nfds = 1;
	i = pvm_setopt(PvmAutoErr, 0);
	if (pvm_getfds(&np) > 0) {
		FD_SET(np[0], &rfds);
		nfds = np[0] + 1;
	}
	pvm_setopt(PvmAutoErr, i);

	trc_init();

#ifdef HASREADLINE
	rl_attempted_completion_function =
			(CPPFunction *) command_completion;
	rl_callback_handler_install( prompt, rl_got_line );
#else
	printf(prompt);
	fflush(stdout);
#endif

#ifdef WIN32
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
#endif

	while (1) {

	/*
	* flush task stdout and trace events
	*/

		if (mytid > 0)
			more = checkmsgs();
		else
			more = 0;

		if ( more )
			timeout = &more_msgs;

		else
#ifndef WIN32
			timeout = (struct timeval *) NULL;
#else
			timeout = &zerotimeout;
#endif

	/*
	* wait for command or more output
	*/
		fds = rfds;
		if ((n = select(nfds,
#ifdef	FDSETISINT
				(int *)&fds, (int *)0, (int *)0,
#else
				(fd_set *)&fds, (fd_set *)0, (fd_set *)0,
#endif
				timeout))
		== -1) {
			perror("select");
#ifndef WIN32
			continue;
#endif
		} 

#ifdef WIN32
		else 
			if (n != 0) printf("select: returned untouched data \n");
#endif

#ifdef WIN32
		if (WAIT_TIMEOUT == WaitForSingleObject(hStdin,2000))
			continue; /* no input */
#else	
		if (n > 0 && FD_ISSET(0, &fds)) {
#endif

#ifdef HASREADLINE
			rl_callback_read_char();
#else
			if ((n = read(0, cmd, sizeof(cmd)-1)) < 1) {
				printf("quit\n");
				quit_cmd();
			}
			cmd[n] = 0;
			docmd(cmd);
			printf(prompt);
			fflush(stdout);
#endif

#ifndef WIN32
		}
#endif

	}
}


/*	acav()
*
*	Parse a string into words separated by whitespace.
*	Max number of words is original value of *acp.
*
*	Trashes out the original string.
*	Returns 0 with av[0]..av[*acp - 1] pointing to the words.
*	Returns 1 if too many words.
*	Returns -1 if unbalanced quote.
*/

int
acav(s, acp, av)
	char *s;			/* the string to parse */
	int *acp;			/* max num words in, num words found out */
	char **av;			/* pointers to words */
{
	int ac = 0;			/* number of words found */
	char *p = s;		/* input scanner */
	char *q;			/* output */
	int n = *acp;		/* max number of words allowed */
	int mode = 0;		/* quote mode */

	while (*p) {
		while (isspace(*p)) p++;
		if (*p) {
			if (*p == '#')
				break;
			if (ac >= n) {
				*acp = ac;
				return 1;
			}
			q = p;
			av[ac++] = p;
			while (*p) {
				if (mode) {
					if (mode == '\\') {
						*q++ = *p;
						mode = 0;

					} else if (mode == *p) {
						mode = 0;

					} else
						*q++ = *p;

				} else {
					if (isspace(*p))
						break;

					switch (*p) {

					case '"':
					case '\'':
					case '\\':
						mode = *p;
						break;

					default:
						*q++ = *p;
						break;
					}
				}
				p++;
			}
			if (*p)
				p++;
			if (*q)
				*q = 0;
			if (mode) {
				printf("unmatched %c\n", (char)mode);
				return -1;
			}
		}
	}
	*acp = ac;
	return 0;
}


/*	xtoi()
*
*	Yet another version of ascii hex to integer
*/

xtoi(p)
	char *p;
{
	int i = 0;
	char c;

	while (isxdigit(c = *p++)) {
		i = (i << 4) + c - (isdigit(c) ? '0' : (isupper(c) ? 'A' : 'a') - 10);
	}
	return i;
}


/*	axtoi()
*
*	ascii hex or decimal to integer.
*/

axtoi(p)
	char *p;
{
	if (p[0] == '0' && p[1] == 'x')
		return xtoi(p + 2);
	else
		return atoi(p);
}


int
tidtoi(p)
	char *p;
{
	if (*p == 't')
		p++;
	return xtoi(p);
}


checkmsgs()
{
	int cc;
	int len, cod, src;
	int n;
	struct job *jp;
	int *dtids;
	int i, j;
	int more, tmp;
	struct job *jpnext;

	/* Shit, really need context here to keep old pvm_nrecv(-1,-1)... */
	/* Should use 2 contexts, one for original console msgs and one   */
	/*     for tracer stuff...                                        */

	/* Get Host Add Notifies */
	while ((cc = pvm_nrecv(-1, HostsAddedTag)) > 0) {
		pvm_upkint(&n, 1, 1);
		dtids = TALLOC(n, int, "");
		pvm_upkint(dtids, n, 1);
		printf("\nConsole: %d new host%s added\n", n, (n == 1 ? "" : "s"));
		pvm_freebuf(cc);
		if (!pvm_config(&nhosts, &narchs, &hostlist)) {
			fputs("                    HOST     DTID     ARCH   SPEED\n",
					stdout);
			for (j = n; j-- > 0; )
				for (i = nhosts; i-- > 0; ) {
					if (dtids[j] == hostlist[i].hi_tid) {
						printf("%24s %8x %8s%8d\n",
								hostlist[i].hi_name,
								hostlist[i].hi_tid,
								hostlist[i].hi_arch,
								hostlist[i].hi_speed);
						break;
					}
				}
		}
		MY_FREE(dtids);
	}

	/* Poll Jobs...  This Sux...  Needs Context... */

	jp = joblist->j_link;

	more = 0;

	while (jp != joblist) {

		jpnext = jp->j_link;

		/* Trace Job */
		if (jp->j_flag & JOB_TRACE) {
			if (trc_recv_messages(jp->j_trcid, 10, &tmp) > 0 ) {
				fflush(jp->j_trcid->trace_out);
			}
			if ( tmp )
				more++;
			if ( jp->j_trcid->complete ) {
				printf("[%d] finished\n", jp->j_jid - joboffset);
				job_free(jp);
			}
		}

		/* Output Job */
		else {
			while ((cc = pvm_nrecv(-1, jp->j_jid)) > 0) {
				pvm_bufinfo(cc, &len, &cod, &src);
				checkoutput(jp, cc, len, cod, src);
			}
		}

		jp = jpnext;
	}

	if (cc < 0)
		exit(1);

	return( more );
}


#ifdef HASREADLINE

/* Readline Support Routines */

/* Provided by Edward Vigmond <vigmonde@IGB.UMontreal.CA> */


char *command_generator __ProtoGlarp__(( char *, int ));
char *command_help_generator __ProtoGlarp__(( char *, int ));

/*
 * readline has detected a complete input line
 */
void rl_got_line( line )
char *line;
{
	char *expcmd;

	if ( line == NULL )
	{
		printf( "quit\n" );
		quit_cmd();
	}

	/* try and expand input, if so print expanded comnmand */
	if ( history_expand( line, &expcmd ) == 1 ) {
		printf( "%s\n", expcmd );
		fflush( stdout );
	}

	/* if the command is not the same as the previous, store it */
	if ( *line ) {
		if ( !history_length ||
				strcmp( history_get( history_length )->line, expcmd ) )
			add_history( expcmd );
	}

	docmd( expcmd );
	free( expcmd );
}


char **
command_completion( text, start, end )
char *text;
int start;
int end;
{
	static char *last_line = (char *) NULL;

	char **matches = (char **) NULL;
	char tchar[2];

	if ( start == 0 ) {

		if ( last_line ) {
			free( last_line );
			last_line = (char *) NULL;
		}

		matches = (char **) completion_matches( text, command_generator );

	}

	/* if only the command entered so far, give the syntax */
	else if ( start == end ) {

		if ( !last_line || strcmp( last_line, rl_line_buffer ) ) {

			matches = (char **) completion_matches( text,
				command_help_generator );

			if ( last_line ) free( last_line );
			last_line = strdup( rl_line_buffer );

		}
	}

	return( matches );
}


/* function to match partially completed command names */
char *
command_generator( text, state )
char *text;
int state;
{
	static int list_index, len;
	char *name;

	if ( !state ) {
		list_index = 0;
		len = strlen( text );
	}

	while( name = commands[ list_index ].cmd ) {

		list_index++;

		if ( !strncmp( name, text, len ) )
			return( strdup( name ) );
	}

	return( (char *) NULL );
}


/* 
 * this function outputs the help for the command but does not inform
 * readline of a match since readline sorts the command completion
 * alphabetically, thus mixing up the help message
 */
char *
command_help_generator( text, state )
char *text;
int state;
{
	static int list_index, len, found_one;
	char *syntax;

	if ( !state ) {
		list_index = 0;
		len = strchr( rl_line_buffer, ' ' ) - rl_line_buffer;
		found_one = 0;
	}

	while ( syntax = helptx[ list_index ] ) {

		list_index++;

		if ( !strncmp( syntax, rl_line_buffer, len ) ) {

			if ( !found_one ) {
				found_one = 1;
				printf( "\n" );
			}

			printf( "%s\n", strchr( syntax, ' ' ) + 1 );
			fflush( stdout );
		}
	}

	if ( found_one && !syntax )
		rl_forced_update_display();

	return( (char *) NULL );
}
	
#endif

