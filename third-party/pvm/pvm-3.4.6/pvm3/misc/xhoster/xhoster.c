
/*
*	xhoster.c
*
*	Example X-Window hoster.  Lets user type passwords without having
*	master pvmd running in foreground.
*	From hoster.c example.
*
*	21 Sep 1994  Manchek
*/

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/AsciiText.h>
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <netdb.h>
#ifdef NEEDSSELECTH
#include <sys/select.h>
#endif
#ifdef	SYSVSTR
#include <string.h>
#define	CINDEX(s,c)	strchr(s,c)
#else
#include <strings.h>
#define	CINDEX(s,c)	index(s,c)
#endif
#include <pvm3.h>
#include <pvmproto.h>
#include "hostc.h"
#include "bfunc.h"

#ifndef	RSHTIMEOUT
#define	RSHTIMEOUT	60
#endif

#ifndef	RSHNPLL
#define	RSHNPLL	5
#endif

#ifndef	max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif

#define	TALLOC(n,t,g)	(t*)malloc((n)*sizeof(t))
#define	FREE(p)	free((char *)p)
#define	STRALLOC(s)			strcpy(TALLOC(strlen(s)+1,char,"str"),s)

#define	LISTPUTAFTER(o,n,f,r) \
	{ (n)->f=(o)->f; (n)->r=(o); (o)->f->r=(n); (o)->f=(n); }
#define	LISTPUTBEFORE(o,n,f,r) \
	{ (n)->r=(o)->r; (n)->f=(o); (o)->r->f=(n); (o)->r=(n); }
#define	LISTDELETE(e,f,r) \
	{ (e)->f->r=(e)->r; (e)->r->f=(e)->f; (e)->r=(e)->f=0; }

#define	TVCLEAR(tvp)	((tvp)->tv_sec = (tvp)->tv_usec = 0)

#define	TVISSET(tvp)	((tvp)->tv_sec || (tvp)->tv_usec)

#define	TVXLTY(xtv, ytv) \
	((xtv)->tv_sec < (ytv)->tv_sec || \
		((xtv)->tv_sec == (ytv)->tv_sec && (xtv)->tv_usec < (ytv)->tv_usec))

#define	TVXADDY(ztv, xtv, ytv)	\
	if (((ztv)->tv_usec = (xtv)->tv_usec + (ytv)->tv_usec) < 1000000) {	\
		(ztv)->tv_sec = (xtv)->tv_sec + (ytv)->tv_sec;	\
	} else {	\
		(ztv)->tv_usec -= 1000000;	\
		(ztv)->tv_sec = (xtv)->tv_sec + (ytv)->tv_sec + 1;	\
	}

#define	TVXSUBY(ztv, xtv, ytv)	\
	if ((xtv)->tv_usec >= (ytv)->tv_usec) {	\
		(ztv)->tv_sec = (xtv)->tv_sec - (ytv)->tv_sec;	\
		(ztv)->tv_usec = (xtv)->tv_usec - (ytv)->tv_usec;	\
	} else {	\
		(ztv)->tv_sec = (xtv)->tv_sec - (ytv)->tv_sec - 1;	\
		(ztv)->tv_usec = (xtv)->tv_usec + 1000000 - (ytv)->tv_usec;	\
	}

/*
*	for keeping state assoc. with a host
*/

struct hst {
	int h_tid;
	char *h_name;
	char *h_login;
	char *h_sopts;
	int h_flag;
#define	HST_PASSWORD	1		/* ask for a password */
#define	HST_MANUAL		2		/* do manual startup */
	char *h_pwd;
	char *h_cmd;
	char *h_result;
};

#define	STA_IDLE	0
#define	STA_NEEDPW	1
#define	STA_START	2

#define	HostAdded	1		/* for notify */
#define	HostDeleted	2

struct slot {
	struct slot *s_link, *s_rlink;		/* free/active list */
	struct hst *s_hst;					/* host table entry */
	struct timeval s_bail;				/* timeout time */
	int s_rfd, s_wfd, s_efd;			/* slave stdin/out/err */
	char s_buf[256];					/* config reply line */
	char s_ebuf[256];					/* stderr from host */
	int s_len;							/* length of s_buf */
	int s_elen;							/* length of s_ebuf */
};

int debugmask = 0;
char *username = 0;
Display *xDisp = 0;
XtAppContext context = 0;
Widget topLevel = 0;		/* main widget */
int xScrn = 0;
Window xRootW = 0;
char pwbuffer[30];
Widget pwdWindow = 0;
Widget logWindow = 0;
Widget okButton = 0;
Widget okallButton = 0;
Widget hostsLabel = 0;
int mystate = STA_IDLE;			/* state of widgy */
struct hst **hostlist = 0;		/* hosts being added */
int numhost = 0;				/* number of hosts */
int fromtid = 0;				/* source tid of add request */
int fromwid = 0;				/* wait id in request */
int curhost = 0;				/* waiting for password for this host */
char scribble[1024];

static struct slot slots[RSHNPLL+2];	/* state var/context for each slot */
static struct slot *slfree = 0;			/* free list of slots */
static Arg args[16];
static XtCallbackRec callback[2] = { { 0, 0 }, { 0, 0 } };

static char *fallbacks[] = {
	"*titleLabel.label: PVM Xhoster",
	"*titleLabel.borderWidth: 0",
	"*hostsLabel.borderWidth: 0",
	"*pwdLabel.label: Password:",
	"*pwdLabel.borderWidth: 0",
	"*quitButton.label: Exit",
	"*hbox.borderWidth: 0",
	"*okButton.label: Ok",
	"*okallButton.label: Ok-for-all",
	"*logWindow.width: 400",
	"*logWindow.height: 90",
	"*logWindow.scrollHorizontal: Always",
	"*logWindow.scrollVertical: Always",
	0
};

typedef struct {
	Bool mono;			/* force monochrome display */
} app_res_t, *app_resp_t;

static app_res_t app_res;

static XtResource res_list[] = {
	{ "monochrome", "Monochrome", XtRBool, sizeof(Bool),
		XtOffset(app_resp_t, mono), XtRString, "off" },
};

static XrmOptionDescRec knownargs[] = {
	{ "-mono", ".monochrome", XrmoptionNoArg, "on" },
	{ "+mono", ".monochrome", XrmoptionNoArg, "off" },
};

char *pvmgetrsh();

void pvm_cb();
void quit_cb();


hostcount()
{
	int nh = 0;
	struct hostc *hp = 0;

	while (hp = host_next(hp))
		nh++;
	sprintf(scribble, "Hosts:%3d", nh);
	XtSetArg(args[0], XtNlabel, scribble);
	XtSetValues(hostsLabel, args, 1);
}


host_was_added(tid)
	int tid;
{
	return 0;
}


host_was_deleted(tid)
	int tid;
{
	struct hostc *hp;

	if (hp = host_findtid(tid)) {
		sprintf(scribble, "Host %s deleted\n", hp->name);
		log_this(scribble);
	}
	return 0;
}


main(argc, argv)
	int argc;
	char **argv;
{
	struct passwd *pe;
	int uid;
	int n;
	int *fds;
	int didstart = 0;

	n = 0;
	topLevel = XtAppInitialize(&context, "xhoster",
		knownargs, XtNumber(knownargs),
		&argc, argv,
		fallbacks,
		args, n);

/*
	XtGetApplicationResources(topLevel, (caddr_t)&app_res,
		res_list, XtNumber(res_list), 0, 0);
*/

	pvm_setopt(PvmRoute, PvmDontRoute);

	if (pvm_mytid() < 0) {
		if (pvm_start_pvmd(0, (char **)0, 1) < 0)
			exit(1);
		didstart = 1;
	}

/*
	XtAppAddActions(context, actbl, XtNumber(actbl));
*/

	xDisp = XtDisplay(topLevel);
	xScrn = DefaultScreen(xDisp);
	xRootW = RootWindow(xDisp, xScrn);

	create_widget();

	if ((uid = getuid()) == -1) {
		fprintf(stderr, "main() can't getuid()\n");
		exit(1);
	}
	pvmchkuid( uid );
	if (pe = getpwuid(uid))
		username = STRALLOC(pe->pw_name);
	else
		fprintf(stderr, "main() can't getpwuid\n");
	endpwent();

	pvm_setopt(PvmResvTids, 1);
	if (pvm_reg_hoster()) {
		pvm_exit();
		exit(1);
	}

	host_init(HostAdded, HostDeleted, host_was_added, host_was_deleted);

	pvm_getfds(&fds);
	XtAppAddInput(context, fds[0], (XtPointer)XtInputReadMask,
			pvm_cb, (XtPointer)0);

	if (didstart)
		log_this("No pvmd found, started one");
	log_this("PVM Xhoster ready");
	XtAppMainLoop(context);

	pvm_exit();	/* not reached */
	exit(0);
}


void
pvm_cb(cli, src, id)
	XtPointer cli;
	int *src;
	XtInputId *id;
{
	int tag;

	while (pvm_nrecv(-1, -1) > 0) {
		pvm_bufinfo(pvm_getrbuf(), (int *)0, (int *)&tag, (int *)0);
		if (tag == SM_STHOST)
			hoster();
		if (tag == HostAdded) {
			host_add();
			hostcount();
		}
		if (tag == HostDeleted) {
			host_delete();
			hostcount();
		}
	}
}


void
ok_cb(wgt, cli, cd)
	Widget wgt;
	caddr_t cli;
	caddr_t cd;
{
	int n;

	if (mystate == STA_NEEDPW)
		hostlist[curhost]->h_pwd = STRALLOC(pwbuffer);
/*
	else
		TextAppend(logWindow, "what?\n", 6);
*/
	BZERO(pwbuffer, sizeof(pwbuffer));
	n = 0;
	XtSetArg(args[n], XtNstring, pwbuffer); n++;
	XtSetValues(pwdWindow, args, n);
	if (mystate == STA_NEEDPW)
		next_pwd();
}


void
okall_cb(wgt, cli, cd)
	Widget wgt;
	caddr_t cli;
	caddr_t cd;
{
	int n;

	if (mystate == STA_NEEDPW) {
		while (curhost < numhost) {
			if (hostlist[curhost]->h_flag & HST_PASSWORD)
				hostlist[curhost]->h_pwd = STRALLOC(pwbuffer);
			curhost++;
		}
		log_this("Using password for all hosts\n");
	}
	BZERO(pwbuffer, sizeof(pwbuffer));
	n = 0;
	XtSetArg(args[n], XtNstring, pwbuffer); n++;
	XtSetValues(pwdWindow, args, n);
	if (mystate == STA_NEEDPW)
		hoster2();
}


void
quit_cb(wgt, cli, cd)
	Widget wgt;
	caddr_t cli;
	caddr_t cd;
{
	pvm_exit();
	exit(0);
}


log_this(s)
	char *s;
{
/*
	int n;

	n = 0;
	XtSetArg(args[n], XtNlabel, s); n++;
	XtSetValues(stateLabel, args, n);
*/
	TextAppend(logWindow, s, strlen(s));
	if (s[strlen(s) - 1] != '\n')
		TextAppend(logWindow, "\n", 1);
}


long
TextLength(w)
	Widget w;
{
	return XawTextSourceScan(XawTextGetSource(w),
			(XawTextPosition)0, XawstAll, XawsdRight, 1, TRUE);
}


TextReplace(w, start, end, block)
	Widget w;
	XawTextBlock *block;
{
	Arg arg;
	Widget source;
	XawTextEditType edit_mode;

	source = XawTextGetSource(w);
	XtSetArg(arg, XtNeditType, &edit_mode);
	XtGetValues(source, &arg, 1);
	XtSetArg(arg, XtNeditType, XawtextEdit);
	XtSetValues(source, &arg, 1);
	XawTextReplace(w, start, end, block);
	XtSetArg(arg, XtNeditType, edit_mode);
	XtSetValues(source, &arg, 1);
}


TextAppend(w, s, len)
	Widget w;
	char *s;
{
	long last, current;
	XawTextBlock block;

	current = XawTextGetInsertionPoint(w);
	last = TextLength(w);
	block.ptr = s;
	block.firstPos = 0;
	block.length = len;
	block.format = FMT8BIT;
	TextReplace(w, last, last, &block);
	if (current == last)
		XawTextSetInsertionPoint(w, last + block.length);
}


create_widget()
{
	Widget box, hbox;
	Widget w, w2;
	int n;
/*
	Pixmap pm;
*/
	XtAccelerators atbl;
	int nh;

	atbl = XtParseAcceleratorTable("#override\n<Key>Return:set()notify()reset()\n");

	n = 0;
	box = XtCreateManagedWidget("", formWidgetClass, topLevel, args, n);

	n = 0;
	XtSetArg(args[n], XtNtop, XawChainTop); n++;
	XtSetArg(args[n], XtNbottom, XawChainTop); n++;
	XtSetArg(args[n], XtNleft, XawChainLeft); n++;
	XtSetArg(args[n], XtNright, XawChainLeft); n++;
	hbox = XtCreateManagedWidget("hbox", formWidgetClass, box, args, n);

	n = 0;
	sprintf(scribble, "PVM Xhoster (task id =t%x)", pvm_mytid());
	XtSetArg(args[n], XtNlabel, scribble); n++;
	w = XtCreateManagedWidget("titleLabel", labelWidgetClass,
			hbox, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	pvm_config(&nh, (int *)0, (struct pvmhostinfo **)0);
	sprintf(scribble, "Hosts:%3d", nh);
	XtSetArg(args[n], XtNlabel, scribble); n++;
	w = hostsLabel = XtCreateManagedWidget("hostsLabel", labelWidgetClass,
			hbox, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	callback[0].callback = (XtCallbackProc)quit_cb;
	callback[0].closure = (caddr_t)0;
	XtSetArg(args[n], XtNcallback, callback); n++;
/*
	pm = XCreatePixmapFromBitmapData(xDisp, xRootW,
			quit_bits, quit_width, quit_height, 1, 0, 1);
	XtSetArg(args[n], XtNbitmap, (XtArgVal)pm); n++;
*/
	w = XtCreateManagedWidget("quitButton", commandWidgetClass,
			hbox, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromVert, hbox); n++;
	XtSetArg(args[n], XtNtop, XawChainTop); n++;
	XtSetArg(args[n], XtNbottom, XawChainBottom); n++;
	XtSetArg(args[n], XtNleft, XawChainLeft); n++;
	XtSetArg(args[n], XtNright, XawChainRight); n++;
	hbox = logWindow = XtCreateManagedWidget("logWindow", asciiTextWidgetClass,
			box, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromVert, hbox); n++;
	XtSetArg(args[n], XtNtop, XawChainBottom); n++;
	XtSetArg(args[n], XtNbottom, XawChainBottom); n++;
	XtSetArg(args[n], XtNleft, XawChainLeft); n++;
	XtSetArg(args[n], XtNright, XawChainLeft); n++;
	hbox = XtCreateManagedWidget("hbox", formWidgetClass, box, args, n);

	n = 0;
	XtSetArg(args[n], XtNeditType, XawtextEdit); n++;
	w = XtCreateManagedWidget("pwdLabel", labelWidgetClass,
			hbox, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	XtSetArg(args[n], XtNeditType, XawtextEdit); n++;
	XtSetArg(args[n], XtNstring, pwbuffer); n++;
	XtSetArg(args[n], XtNlength, sizeof(pwbuffer)-1); n++;
	XtSetArg(args[n], XtNuseStringInPlace, True); n++;
	XtSetArg(args[n], XtNecho, False); n++;
	w = pwdWindow = XtCreateManagedWidget("pwdWindow", asciiTextWidgetClass,
			hbox, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	callback[0].callback = (XtCallbackProc)ok_cb;
	callback[0].closure = (caddr_t)0;
	XtSetArg(args[n], XtNcallback, callback); n++;
	XtSetArg(args[n], XtNaccelerators, atbl); n++;
	w = okButton = XtCreateManagedWidget("okButton", commandWidgetClass,
			hbox, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	callback[0].callback = (XtCallbackProc)okall_cb;
	callback[0].closure = (caddr_t)0;
	XtSetArg(args[n], XtNcallback, callback); n++;
	XtSetArg(args[n], XtNaccelerators, atbl); n++;
	w = okallButton = XtCreateManagedWidget("okallButton", commandWidgetClass,
			hbox, args, n);

	XtRealizeWidget(topLevel);
	XtInstallAccelerators(pwdWindow, okButton);
}


/*	hoster()
*
*	Unpack host table from message, attempt to start 'em up,
*	send reply message.
*/

hoster()
{
	int i;
	struct hst *hp;
	char *p;
	struct pvmminfo minfo;
	char sopts[64];
	char lognam[256];
	char cmd[512];

	/*
	* unpack the startup message
	*/

	pvm_bufinfo(pvm_getrbuf(), (int *)0, (int *)0, &fromtid);
	pvm_unpackf("%d", &numhost);
	pvm_getminfo(pvm_getrbuf(), &minfo);
	fromwid = minfo.wid;

	if (numhost > 0) {
		log_this("Ready to add hosts:\n");
		hostlist = TALLOC(numhost, struct hst *, "xxx");
		for (i = 0; i < numhost; i++) {
			hp = TALLOC(1, struct hst, "xxx");
			BZERO((char *)hp, sizeof(struct hst));
			hostlist[i] = hp;
			if (pvm_unpackf("%d %s %s %s", &hp->h_tid, sopts, lognam, cmd)) {
				log_this("received message with bad format\n");
				return 1;
			}
			hp->h_sopts = STRALLOC(sopts);
			hp->h_login = STRALLOC(lognam);
			hp->h_cmd = STRALLOC(cmd);
			sprintf(scribble, "%2d. %s\n", i + 1, hp->h_login);
			log_this(scribble);
			if (debugmask)
				fprintf(stderr, "%d. t%x %s so=\"%s\"\n", i,
						hp->h_tid,
						hp->h_login,
						hp->h_sopts);

			if (p = CINDEX(hp->h_login, '@')) {
				hp->h_name = STRALLOC(p + 1);
				*p = 0;
				p = STRALLOC(hp->h_login);
				FREE(hp->h_login);
				hp->h_login = p;

			} else {
				hp->h_name = hp->h_login;
				hp->h_login = 0;
			}
			if (!strcmp(hp->h_sopts, "pw"))
				hp->h_flag |= HST_PASSWORD;
			if (!strcmp(hp->h_sopts, "ms"))
				hp->h_flag |= HST_MANUAL;
		}
	}
	curhost = -1;
	next_pwd();
	return 0;
}


next_pwd()
{
	char buf[256];

	while (1) {
		curhost++;
		if (curhost >= numhost) {
			hoster2();
			break;
	
		} else
			if (hostlist[curhost]->h_flag & HST_PASSWORD) {
				mystate = STA_NEEDPW;
				sprintf(buf, "Enter password for %s:\n",
						hostlist[curhost]->h_name);
				log_this(buf);
				break;
			}
	}
}


hoster2()
{
	int i;
	char *p;
	struct hst *hp;
	struct pvmminfo minfo;

	/*
	* do it
	*/

	mystate = STA_START;
	log_this("Starting...");
	XFlush(xDisp);

	pl_startup(numhost, hostlist);

	/*
	* send results back to pvmd
	*/

	pvm_packf("%+ %d", PvmDataFoo, numhost);
	for (i = 0; i < numhost; i++) {
		pvm_packf("%d", hostlist[i]->h_tid);
		pvm_packf("%s", hostlist[i]->h_result
				? hostlist[i]->h_result : "PvmDSysErr");
	}

	pvm_getminfo(pvm_getsbuf(), &minfo);
	minfo.wid = fromwid;
	pvm_setminfo(pvm_getsbuf(), &minfo);
	pvm_send(fromtid, SM_STHOSTACK);

	mystate = STA_IDLE;
	log_this("...Done");

	for (i = 0; i < numhost; i++) {
		hp = hostlist[i];
		if (hp->h_name)
			FREE(hp->h_name);
		if (hp->h_login)
			FREE(hp->h_login);
		if (hp->h_sopts)
			FREE(hp->h_sopts);
		if (hp->h_pwd) {
			for (p = hp->h_pwd; *p; p++)
				*p = 0;
			FREE(hp->h_pwd);
		}
		if (hp->h_cmd)
			FREE(hp->h_cmd);
		if (hp->h_result)
			FREE(hp->h_result);
		FREE(hp);
	}
	FREE(hostlist);
	hostlist = 0;
	numhost = 0;

	return 0;
}


close_slot(sp)
	struct slot *sp;
{
	if (sp->s_wfd != -1)
		(void)close(sp->s_wfd);
	if (sp->s_rfd != -1)
		(void)close(sp->s_rfd);
	if (sp->s_efd != -1)
		(void)close(sp->s_efd);
	LISTDELETE(sp, s_link, s_rlink);
	LISTPUTBEFORE(slfree, sp, s_link, s_rlink);
	return 0;
}


pl_startup(num, hostlist)
	int num;
	struct hst **hostlist;
{
	int nxth = 0;						/* next host in list to start */
	struct slot *slact = 0;				/* active list of slots */
	struct hst *hp;
	struct slot *sp, *sp2;
	struct timeval tnow;
	struct timeval tout;
#ifdef FDSETNOTSTRUCT
	fd_set rfds;
#else
	struct fd_set rfds;
#endif
	int nfds;
	int i;
	int n;
	char *p;

	/* init slot free list */

	slfree = &slots[RSHNPLL+1];
	slfree->s_link = slfree->s_rlink = slfree;
	slact = &slots[RSHNPLL];
	slact->s_link = slact->s_rlink = slact;
	for (i = RSHNPLL; i-- > 0; ) {
		LISTPUTAFTER(slfree, &slots[i], s_link, s_rlink);
	}

	/*
	* keep at this until all hosts in table are completed
	*/

	for (; ; ) {

		/*
		* if empty slots, start on new hosts
		*/

		for (; ; ) {

			/* find a host for slot */

			if (slfree->s_link != slfree && nxth < num)
				hp = hostlist[nxth++];
			else
				break;

			sp = slfree->s_link;
			LISTDELETE(sp, s_link, s_rlink);
			sp->s_hst = hp;
			sp->s_len = 0;
			sp->s_elen = 0;
			if (debugmask) {
				fprintf(stderr, "pl_startup() trying %s\n", hp->h_name);
			}
			phase1(sp);
			if (hp->h_result) {
				/* error or fully started (manual startup) */

				LISTPUTBEFORE(slfree, sp, s_link, s_rlink);

			} else {
				/* partially started */

				LISTPUTBEFORE(slact, sp, s_link, s_rlink);
				gettimeofday(&sp->s_bail, (struct timezone*)0);
				tout.tv_sec = RSHTIMEOUT;
				tout.tv_usec = 0;
				TVXADDY(&sp->s_bail, &sp->s_bail, &tout);
			}
		}

		/* if no hosts in progress, we are finished */

		if (slact->s_link == slact)
			break;

		/*
		* until next timeout, get output from any slot
		*/

		FD_ZERO(&rfds);
		nfds = 0;
		TVCLEAR(&tout);
		gettimeofday(&tnow, (struct timezone*)0);
		for (sp = slact->s_link; sp != slact; sp = sp->s_link) {
			if (TVXLTY(&sp->s_bail, &tnow)) {
				sprintf(scribble, "Giving up on host %s after %d secs\n",
						sp->s_hst->h_name, RSHTIMEOUT);
				log_this(scribble);
				sp->s_hst->h_result = STRALLOC("PvmCantStart");
				sp2 = sp->s_rlink;
				close_slot(sp);
				sp = sp2;
				continue;
			}

			if (!TVISSET(&tout) || TVXLTY(&sp->s_bail, &tout))
				tout = sp->s_bail;
			if (sp->s_rfd >= 0)
				FD_SET(sp->s_rfd, &rfds);
			if (sp->s_rfd > nfds)
				nfds = sp->s_rfd;
			if (sp->s_efd >= 0)
				FD_SET(sp->s_efd, &rfds);
			if (sp->s_efd > nfds)
				nfds = sp->s_efd;
		}

		if (slact->s_link == slact)
			break;

		nfds++;

		if (TVXLTY(&tnow, &tout)) {
			TVXSUBY(&tout, &tout, &tnow);
		} else {
			TVCLEAR(&tout);
		}
		if (debugmask) {
			fprintf(stderr, "pl_startup() select timeout is %d.%06d\n",
					tout.tv_sec, tout.tv_usec);
		}
		if ((n = select(nfds,
#ifdef	FDSETISINT
				(int *)&rfds, (int *)0, (int *)0,
#else
				(fd_set *)&rfds, (fd_set *)0, (fd_set *)0,
#endif
				&tout)) == -1) {
			if (errno != EINTR) {
				pvmlogperror("work() select");
				pvmbailout(0);
			}
		}
		if (debugmask) {
			(void)fprintf(stderr, "pl_startup() select returns %d\n", n);
		}
		if (n < 1) {
			if (n == -1 && errno != EINTR) {
				pvmlogperror("pl_startup() select");
				pvmbailout(0);	/* XXX this is too harsh */
			}
			continue;
		}

		/*
		* check for response on stdout or stderr of any slave.
		*/

		for (sp = slact->s_link; sp != slact; sp = sp->s_link) {

			/*
			* stderr ready.  log output with remote's host name.
			*/
			if (sp->s_efd >= 0 && FD_ISSET(sp->s_efd, &rfds)) {
				n = read(sp->s_efd, sp->s_ebuf + sp->s_elen,
						sizeof(sp->s_ebuf) - sp->s_elen - 1);
				if (n > 0) {
					sp->s_elen += n;
					sp->s_ebuf[sp->s_elen] = 0;
					while (p = CINDEX(sp->s_ebuf, '\n')) {
						*p = 0;
						sprintf(scribble, "stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						log_this(scribble);
						p++;
						sp->s_elen -= p - sp->s_ebuf;
						if (sp->s_elen > 0) {
							BCOPY(p, sp->s_ebuf, sp->s_elen);
							sp->s_ebuf[sp->s_elen] = 0;
						}
					}
					if (sp->s_elen == sizeof(sp->s_ebuf) - 1) {
						sprintf(scribble, "stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						log_this(scribble);
						sp->s_elen = 0;
					}

				} else {
					if (sp->s_elen > 0) {
						sprintf(scribble, "stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						log_this(scribble);
						sp->s_elen = 0;
					}
					(void)close(sp->s_efd);
					sp->s_efd = -1;
				}
			}

			/*
			* stdout ready.
			* look for a complete line starting with "ddpro".
			*/
			if (sp->s_rfd >= 0 && FD_ISSET(sp->s_rfd, &rfds)) {
				n = read(sp->s_rfd, sp->s_buf + sp->s_len,
						sizeof(sp->s_buf) - sp->s_len - 1);
				if (n > 0) {
					sp->s_len += n;
					sp->s_buf[sp->s_len] = 0;
					while (p = CINDEX(sp->s_buf, '\n')) {
						*p = 0;
						if (!strncmp(sp->s_buf, "ddpro", 5)) {
							if (debugmask) {
								sprintf(scribble, "stdout@%s: %s\n",
										sp->s_hst->h_name, sp->s_buf);
								log_this(scribble);
							}
							sp->s_hst->h_result = STRALLOC(sp->s_buf);
							break;

						} else {
							sprintf(scribble, "stdout@%s: %s\n",
									sp->s_hst->h_name, sp->s_buf);
							log_this(scribble);
							p++;
							sp->s_len -= p - sp->s_buf;
							if (sp->s_len > 0) {
								BCOPY(p, sp->s_buf, sp->s_len);
								sp->s_buf[sp->s_len] = 0;
							}
						}
					}
					if (sp->s_len == sizeof(sp->s_buf) - 1) {
						sprintf(scribble, "stdout@%s: %s\n",
								sp->s_hst->h_name, sp->s_buf);
						log_this(scribble);
						sp->s_len = 0;
					}

				} else {
					if (sp->s_len > 0) {
						sprintf(scribble, "stdout@%s: %s\n",
								sp->s_hst->h_name, sp->s_buf);
						log_this(scribble);
						sp->s_len = 0;
					}
					if (n) {
						sprintf(scribble, "stdout@%s",
								sp->s_hst->h_name);
						pvmlogperror(scribble);
					} else {
						sprintf(scribble, "stdout@%s: EOF\n",
								sp->s_hst->h_name);
						log_this(scribble);
					}
					sp->s_hst->h_result = STRALLOC("PvmCantStart");
					if (sp->s_elen > 0) {
						sprintf(scribble, "stderr@%s: %s\n",
								sp->s_hst->h_name, sp->s_ebuf);
						log_this(scribble);
						sp->s_elen = 0;
					}
				}
				if (sp->s_hst->h_result) {
					sp2 = sp->s_rlink;
					close_slot(sp);
					sp = sp2;
					continue;
				}
			}

		}
	}
	return 0;
}


phase1(sp)
	struct slot *sp;
{
	struct hst *hp;
	char *hn;
	char *av[16];			/* for rsh args */
	int ac;
	char buf[512];
	int pid = -1;			/* pid of rsh */
	char *p;

#ifndef NOREXEC
	struct servent *se;
	static u_short execport = 0;

	if (!execport) {
		if (!(se = getservbyname("exec", "tcp"))) {
			fprintf(stderr, "phase1() can't getservbyname(): %s\n", "exec");
			pvmbailout(0);
		}
		execport = se->s_port;
		endservent();
	}
#endif

	hp = sp->s_hst;
	hn = hp->h_name;
	sp->s_rfd = sp->s_wfd = sp->s_efd = -1;

	/*
	* XXX manual startup hack... this is if we can't use rexec or rsh
	*/

	if (hp->h_flag & HST_MANUAL) {
		sprintf(scribble, "Can't start %s, manual startup specified\n",
				hp->h_name);
		log_this(scribble);
		hp->h_result = STRALLOC("PvmNotImpl");
#if 0
		fprintf(stderr, "*** Manual startup ***\n");
		fprintf(stderr, "Login to \"%s\" and type:\n", hn);
		fprintf(stderr, "%s\n", hp->h_cmd);

	/* get version */

		fprintf(stderr, "Type response: ");
		fflush(stderr);
		if (!(fgets(buf, sizeof(buf), stdin))) {
			fprintf(stderr, "host %s read error\n", hn);
			goto oops;
		}
		p = buf + strlen(buf) - 1;
		if (*p == '\n')
			*p = 0;
		hp->h_result = STRALLOC(buf);
		fprintf(stderr, "Thanks\n");
		fflush(stderr);
#endif
		return 0;
	}

	/*
	* XXX end manual startup hack
	*/

	if (!(hp->h_flag & HST_PASSWORD)) {		/* use rsh to start */
		int wpfd[2], rpfd[2], epfd[2];
		int i;

		if (debugmask) {
			fprintf(stderr, "phase1() trying rsh to %s\n", hn);
		}

	/* fork an rsh to startup the slave pvmd */

#ifdef	IMA_TITN
		if (socketpair(AF_UNIX, SOCK_STREAM, 0, wpfd) == -1
		|| socketpair(AF_UNIX, SOCK_STREAM, 0, rpfd) == -1
		|| socketpair(AF_UNIX, SOCK_STREAM, 0, epfd) == -1) {
			pvmlogperror("phase1() socketpair");
			goto oops;
		}
#else
		if (pipe(wpfd) == -1 || pipe(rpfd) == -1 || pipe(epfd) == -1) {
			pvmlogperror("phase1() pipe");
			goto oops;
		}
#endif

		if (debugmask) {
			fprintf(stderr, "phase1() pipes: %d %d %d %d %d %d\n",
					wpfd[0], wpfd[1], rpfd[0], rpfd[1], epfd[0], epfd[1]);
		}

		if ((pid = fork()) == -1) {
			pvmlogperror("phase1() fork");
			pvmbailout(0);
		}
		if (!pid) {
			(void)dup2(wpfd[0], 0);
			(void)dup2(rpfd[1], 1);
			(void)dup2(epfd[1], 2);
			for (i = getdtablesize(); --i > 2; )
				(void)close(i);
			ac = 0;
			av[ac++] = pvmgetrsh();
			av[ac++] = hn;
			if (hp->h_login) {
				av[ac++] = "-l";
				av[ac++] = hp->h_login;
			}
			av[ac++] = hp->h_cmd;
			av[ac++] = 0;
			if (debugmask) {
				for (ac = 0; av[ac]; ac++)
					fprintf(stderr, "av[%d]=\"%s\" ", ac, av[ac]);
				fputc('\n', stderr);
			}
			execvp(av[0], av);
			fputs("phase1() execvp failed\n", stderr);
			fflush(stderr);
			_exit(1);
		}
		(void)close(wpfd[0]);
		(void)close(rpfd[1]);
		(void)close(epfd[1]);
		sp->s_wfd = wpfd[1];
		sp->s_rfd = rpfd[0];
		sp->s_efd = epfd[0];

	} else {		/* use rexec to start */

#ifdef NOREXEC
		log_this("Sorry, xhoster has been compiled without rexec()\n");
		log_this("and can't take passwords\n");
		goto oops;
#else
		if (debugmask) {
			fprintf(stderr, "phase1() rexec \"%s\"\n", hp->h_cmd);
		}
		if ((sp->s_wfd = sp->s_rfd = rexec(&hn, execport,
				(hp->h_login ? hp->h_login : username),
				(hp->h_flag & HST_PASSWORD ? hp->h_pwd : (char *)0),
				hp->h_cmd, &sp->s_efd))
		== -1) {
			sprintf(scribble, "Permission denied for %s\n", hn);
			log_this(scribble);
			goto oops;
		}
#endif
	}
	return 0;

oops:
	hp->h_result = STRALLOC("PvmCantStart");
	if (sp->s_wfd != -1)
		close(sp->s_wfd);
	if (sp->s_rfd != -1)
		close(sp->s_rfd);
	if (sp->s_efd != -1)
		close(sp->s_efd);
	sp->s_wfd = sp->s_rfd = sp->s_efd = -1;
	return 1;
}


