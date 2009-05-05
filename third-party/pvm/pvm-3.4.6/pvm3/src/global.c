
static char rcsid[] =
	"$Id: global.c,v 1.19 2002/07/30 15:35:04 pvmsrc Exp $";

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
 *	global.c
 *
 *	General PVM Globals.
 *
 * $Log: global.c,v $
 * Revision 1.19  2002/07/30 15:35:04  pvmsrc
 * Enhanced PvmIPLoopback error text - "pvm3/"Readme file...  Duh...
 * (Spanker=kohl)
 *
 * Revision 1.18  2001/02/07 23:14:03  pvmsrc
 * First Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.17  2000/02/10 23:53:34  pvmsrc
 * Added new PvmIPLoopback error code:
 * 	- Master Host IP Address tied to Loopback.
 * 	- check for this case in addhosts(), don't even TRY to add hosts...
 * (Spanker=kohl)
 *
 * Revision 1.16  1999/07/08 18:59:52  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.15  1998/11/20  20:03:55  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.14  1997/12/31  22:14:23  pvmsrc
 * Renamed TEV_REMOVE -> TEV_DELINFO.  D-Oh.
 * (Spanker=kohl)
 *
 * Revision 1.13  1997/12/23  20:23:47  pvmsrc
 * Added new TEV_MHF_INVOKE trace event.
 * 	- include message handler src/tag/ctx settings.
 * 	- include actual message bufid, len, and src/tag/ctx.
 * 	- use new TEV_PUSHTOP, TEV_AMPUSHED and TEV_POPTOP macros to
 * 		fake top level tracing for PVM calls in message handler.
 * (Spanker=kohl)
 *
 * Revision 1.12  1997/12/17  22:15:34  pvmsrc
 * Added new mhf trace stuff.
 * 	- TEV_DID_MHI definition.
 * 	- TEV_ADDMHF & TEV_DELMHF trace event slots.
 * (Spanker=kohl)
 *
 * Revision 1.11  1997/10/24  15:17:42  pvmsrc
 * Added TEV_DID_RCX to trace events for Receive Message Context.
 * 	- in pvm_recv(), pvm_trecv(), pvm_nrecv(), and pvm_precv().
 * (Spanker=kohl)
 *
 * Revision 1.10  1997/10/24  14:20:51  pvmsrc
 * Added TEV_DID_MCX to pvmtevdidinfo[].
 * 	- for Message Context.
 * (Spanker=kohl)
 *
 * Revision 1.9  1997/06/27  17:32:19  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.8  1997/06/26  19:38:48  pvmsrc
 * 		Add PvmParentNotSet
 *
 * Revision 1.7  1997/06/23  21:26:14  pvmsrc
 * Added new PvmHostrNMstr error code.
 * 	- Hoster must run on master host.
 *
 * Revision 1.6  1997/05/01  20:17:38  pvmsrc
 * Renamed TEV_MBOXINFO -> TEV_GETMBOXINFO.
 *
 * Revision 1.5  1997/04/10  20:13:45  pvmsrc
 * Replaced trace event for getnames -> mboxinfo.
 *
 * Revision 1.4  1997/04/08  19:57:49  pvmsrc
 * Promoted mbox static "classes" to public global "pvmmboxclasses".
 * 	- so pvmd can spank mboxes in ddpro.c...  :-Q
 * 	- renamed everywhere, moved decl / extern to global.[ch].
 *
 * Revision 1.3  1997/03/06  20:57:36  pvmsrc
 * 	Add host.h on its include list
 *
 * Revision 1.2  1997/01/28  19:26:16  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/10/24  19:43:02  pvmsrc
 * Initial revision
 *
 *
 */


#include <stdio.h>

#include <pvm3.h>

#if defined(WIN32) || defined(CYGWIN)
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#include "lpvm.h"
#include "pmsg.h"
#include "host.h"
#include "waitc.h"
#include <pvmtev.h>
#include "tevmac.h"
#include "msgbox.h"
#include "global.h"


/***************
 **  Globals  **
 **           **
 ***************/


/* error messages for -pvm_errno */

char *pvm_errlist[] = {
		"Error 0",
		"Error 1",
	"Bad parameter",
	"Parameter mismatch",
	"Value too large",
	"End of buffer",
	"No such host",
	"No such file",
	"Permission denied",
		"Error 9",				/* not used */
	"Malloc failed",
		"Error 11",				/* not used */
	"Can't decode message",
		"Error 13",				/* not used */
	"Can't contact local daemon",
	"No current buffer",
	"No such buffer",
	"Null group name",
	"Already in group",
	"No such group",
	"Not in group",
	"No such instance",
	"Host failed",
	"No parent task",
	"Not implemented",
	"Pvmd system error",
	"Version mismatch",
	"Out of resources",
	"Duplicate host",
	"Can't start pvmd",
	"Already in progress",
	"No such task",
	"Not Found",
	"Already Exists",
	"Hoster must run on master host",
	"Spawning parent set PvmNoParent",
	"Master Host IP Address is Loopback! (See pvm3/Readme)",
};

int pvm_nerr =							/* exported num of errors */
	sizeof(pvm_errlist) / sizeof(pvm_errlist[0]);

int pvm_errno = 0;						/* last err in PVM API funct */

struct waitc *waitlist = 0;				/* wait list */

struct pvmmclass *pvmmboxclasses = 0;	/* the mbox database */



/* Trace Data Identifier Structures */

struct Pvmtevdid pvmtevdidlist[] =
{
	/* TEV_DID_AN */	"AN",	"PVM Architecture Name",
	/* TEV_DID_AC */	"AC",	"PVM Architecture Code",
	/* TEV_DID_AS */	"AS",	"Argument Strings (Argc/Argv)",
	/* TEV_DID_BF */	"BF",	"Block Flag",
	/* TEV_DID_CC */	"CC",	"Condition Code",
	/* TEV_DID_CN */	"CN",	"Class Name",
	/* TEV_DID_CI */	"CI",	"Class Index",
	/* TEV_DID_CF */	"CF",	"Class Flags",
	/* TEV_DID_CD */	"CD",	"Class Data",
	/* TEV_DID_CR */	"CR",	"Returned Class Data",
	/* TEV_DID_CL */	"CL",	"Returned Class Names",
	/* TEV_DID_CDA */	"CDA",	"Collective Data Address",
	/* TEV_DID_CDT */	"CDT",	"Collective Data Type",
	/* TEV_DID_CDX */	"CDX",	"Collective Data",
	/* TEV_DID_CDC */	"CDC",	"Collective Count",
	/* TEV_DID_CGI */	"CGI",	"Collective Group Result Instance",
	/* TEV_DID_CRF */	"CRF",	"Collective Reduce Function",
	/* TEV_DID_CXC */	"CXC",	"Current Message Context",
	/* TEV_DID_CXF */	"CXF",	"Freed Message Context",
	/* TEV_DID_CXN */	"CXN",	"New Message Context",
	/* TEV_DID_CXS */	"CXS",	"Set Message Context",
	/* TEV_DID_ERI */	"ERI",	"PVM Error Identifier",
	/* TEV_DID_ERR */	"ERR",	"PVM Error String",
	/* TEV_DID_FDS */	"FDS",	"File Descriptors",
	/* TEV_DID_FP */	"FP",	"File Pointer",
	/* TEV_DID_GN */	"GN",	"Group Name",
	/* TEV_DID_GI */	"GI",	"Group Instance Number",
	/* TEV_DID_GT */	"GT",	"Group Task ID",
	/* TEV_DID_GS */	"GS",	"Group Size",
	/* TEV_DID_GBC */	"GBC",	"Group Barrier Count",
	/* TEV_DID_HN */	"HN",	"Host Name",
	/* TEV_DID_HNL */	"HNL",	"Host Name List",
	/* TEV_DID_HNA */	"HNA",	"Host Name Alias",
	/* TEV_DID_HPT */	"HPT",	"Host PVMD Task ID",
	/* TEV_DID_HA */	"HA",	"Host Architecture",
	/* TEV_DID_HSP */	"HSP",	"Host Speed",
	/* TEV_DID_HIS */	"HIS",	"Host Information Structures",
	/* TEV_DID_HCS */	"HCS",	"Host Clock Seconds",
	/* TEV_DID_HCU */	"HCU",	"Host Clock Microseconds",
	/* TEV_DID_HDS */	"HDS",	"Host Delta Seconds",
	/* TEV_DID_HDU */	"HDU",	"Host Delta Microseconds",
	/* TEV_DID_HS */	"HS",	"Host Status",
	/* TEV_DID_INB */	"INB",	"Info Number of Message Bytes",
	/* TEV_DID_IMC */	"IMC",	"Info Message Code",
	/* TEV_DID_IST */	"IST",	"Info Message Source Task",
	/* TEV_DID_MC */	"MC",	"Message Code",
	/* TEV_DID_MCX */	"MCX",	"Message Context",
	/* TEV_DID_MB */	"MB",	"Message Buffer",
	/* TEV_DID_MNB */	"MNB",	"Message Number of Bytes",
	/* TEV_DID_SRC */	"SRC",	"Message Source Task ID",
	/* TEV_DID_DST */	"DST",	"Message Destination Task ID",
	/* TEV_DID_MDL */	"MDL",	"Message Destination Task ID List",
	/* TEV_DID_MTS */	"MTS",	"Message Timeout Seconds",
	/* TEV_DID_MTU */	"MTU",	"Message Timeout Microseconds",
	/* TEV_DID_MRF */	"MRF",	"Message Receive Function",
	/* TEV_DID_MRB */	"MRB",	"Message Receive Buffer",
	/* TEV_DID_MSB */	"MSB",	"Message Send Buffer",
	/* TEV_DID_ME */	"ME",	"Message Encoding",
	/* TEV_DID_MHI */	"MHI",	"Message Handler Index",
	/* TEV_DID_AST */	"AST",	"Actual Message Sender Task ID",
	/* TEV_DID_AMC */	"AMC",	"Actual Message Code",
	/* TEV_DID_AML */	"AML",	"Actual Message Length",
	/* TEV_DID_NE */	"NE",	"Notify Event ID",
	/* TEV_DID_NMC */	"NMC",	"Notify Message Code",
	/* TEV_DID_NCT */	"NCT",	"Notify Count",
	/* TEV_DID_NTL */	"NTL",	"Notify Task ID List",
	/* TEV_DID_NH */	"NH",	"Number of Hosts",
	/* TEV_DID_NA */	"NA",	"Number of Architectures",
	/* TEV_DID_OPT */	"OPT",	"PVM Option ID",
	/* TEV_DID_OPV */	"OPV",	"PVM Option Value",
	/* TEV_DID_OS */	"OS",	"Output String",
	/* TEV_DID_PDA */	"PDA",	"Pack Data Address",
	/* TEV_DID_PDT */	"PDT",	"Pack Data Type",
	/* TEV_DID_PC */	"PC",	"Pack Count",
	/* TEV_DID_PSD */	"PSD",	"Pack Stride",
	/* TEV_DID_PSG */	"PSG",	"Pack String",
	/* TEV_DID_PF */	"PF",	"Pack Format",
	/* TEV_DID_PRF */	"PRF",	"Profiling Count",
	/* TEV_DID_RST */	"RST",	"Receive Msg Source Task / Wildcard",
	/* TEV_DID_RMC */	"RMC",	"Receive Msg Code / Wildcard",
	/* TEV_DID_RCX */	"RCX",	"Receive Msg Context",
	/* TEV_DID_SN */	"SN",	"Signal Number",
	/* TEV_DID_SE */	"SE",	"Spawn Executable File",
	/* TEV_DID_SF */	"SF",	"Spawn Flags",
	/* TEV_DID_SW */	"SW",	"Spawn Where",
	/* TEV_DID_SC */	"SC",	"Spawn Count",
	/* TEV_DID_STL */	"STL",	"Spawned Task ID List",
	/* TEV_DID_SIB */	"SIB",	"Sibling Task ID List",
	/* TEV_DID_TID */	"TID",	"Task ID",
	/* TEV_DID_PT */	"PT",	"Parent Task ID",
	/* TEV_DID_MT */	"MT",	"My Task ID",
	/* TEV_DID_TF */	"TF",	"Task Flags",
	/* TEV_DID_TN */	"TN",	"Task Name",
	/* TEV_DID_PID */	"PID",	"Task Process ID",
	/* TEV_DID_TT */	"TT",	"Target Task ID",
	/* TEV_DID_TW */	"TW",	"Tasks Where",
	/* TEV_DID_TIS */	"TIS",	"Task Information Structures",
	/* TEV_DID_TNT */	"TNT",	"Task Info - Number of Tasks",
	/* TEV_DID_TST */	"TST",	"Task Status",
	/* TEV_DID_TUS */	"TUS",	"Task User Usage Seconds",
	/* TEV_DID_TUU */	"TUU",	"Task User Usage Microseconds",
	/* TEV_DID_TSS */	"TSS",	"Task System Usage Seconds",
	/* TEV_DID_TSU */	"TSU",	"Task System Usage Microseconds",
	/* TEV_DID_TES */	"TES",	"Task Exit Status",
	/* TEV_DID_TA */	"TA",	"Tickle Arguments",
	/* TEV_DID_TR */	"TR",	"Tickle Responses",
	/* TEV_DID_TS */	"TS",	"Time Seconds",
	/* TEV_DID_TU */	"TU",	"Time Microseconds",
	/* TEV_DID_TGS */	"TGS",	"Timing Seconds",
	/* TEV_DID_TGU */	"TGU",	"Timing Microseconds",
	/* TEV_DID_TMW */	"TMW",	"Trace Mask Who",
	/* TEV_DID_TM */	"TM",	"Trace Mask",
	/* TEV_DID_TRC */	"TRC",	"Tracer Message Trace Context",
	/* TEV_DID_TRT */	"TRT",	"Tracer Message Trace Tag",
	/* TEV_DID_TRX */	"TRX",	"Tracer Message Output Context",
	/* TEV_DID_TRO */	"TRO",	"Tracer Message Output Tag",
	/* TEV_DID_TRM */	"TRM",	"Tracer Trace Mask",
	/* TEV_DID_TRB */	"TRB",	"Tracer Trace Buffering",
	/* TEV_DID_TRL */	"TRL",	"Tracer Trace Options",
	/* TEV_DID_VER */	"VER",	"PVM Version",
	/* TEV_DID_VCT */	"VCT",	"Trace Event Count",
	/* TEV_DID_VID */	"VID",	"Trace Event ID",
	/* TEV_DID_WID */	"WID",	"Wait Identifier Number"
};


/* Trace Event Info */

struct Pvmtevinfo pvmtevinfo[] =
{
	/* TEV_ADDHOSTS */		"addhosts",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_BARRIER */		"barrier",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_BCAST */			"bcast",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_BUFINFO */		"bufinfo",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_CONFIG */		"config",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_DELETE */		"delete",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_DELHOSTS */		"delhosts",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_EXIT */			"exit",			0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_FREEBUF */		"freebuf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETFDS */		"getfds",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETINST */		"getinst",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETOPT */		"getopt",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETRBUF */		"getrbuf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETSBUF */		"getsbuf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETTID */		"gettid",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GSIZE */			"gsize",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_HALT */			"halt",			0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_INITSEND */		"initsend",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_INSERT */		"insert",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_JOINGROUP */		"joingroup",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_KILL */			"kill",			0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_LOOKUP */		"lookup",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_LVGROUP */		"lvgroup",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_MCAST */			"mcast",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_MKBUF */			"mkbuf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_MSTAT */			"mstat",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_MYTID */			"mytid",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_NOTIFY */		"notify",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_NRECV */			"nrecv",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PARENT */		"parent",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PERROR */		"perror",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKBYTE */		"pkbyte",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKCPLX */		"pkcplx",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKDCPLX */		"pkdcplx",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKDOUBLE */		"pkdouble",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKFLOAT */		"pkfloat",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKINT */			"pkint",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKUINT */		"pkuint",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKLONG */		"pklong",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKULONG */		"pkulong",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKSHORT */		"pkshort",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKUSHORT */		"pkushort",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PKSTR */			"pkstr",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PROBE */			"probe",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PSTAT */			"pstat",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_RECV */			"recv",			0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_RECVF */			"recvf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SEND */			"send",			0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SENDSIG */		"sendsig",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SETOPT */		"setopt",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SETRBUF */		"setrbuf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SETSBUF */		"setsbuf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SPAWN */			"spawn",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_START_PVMD */	"start_pvmd",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_TASKS */			"tasks",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_TICKLE */		"tickle",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_TIDTOHOST */		"tidtohost",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_TRECV */			"trecv",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKBYTE */		"upkbyte",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKCPLX */		"upkcplx",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKDCPLX */		"upkdcplx",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKDOUBLE */		"upkdouble",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKFLOAT */		"upkfloat",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKINT */		"upkint",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKUINT */		"upkuint",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKLONG */		"upklong",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKULONG */		"upkulong",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKSHORT */		"upkshort",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKUSHORT */		"upkushort",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UPKSTR */		"upkstr",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_VERSION */		"version",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_REG_HOSTER */	"reg_hoster",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_REG_RM */		"reg_rm",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_REG_TASKER */	"reg_tasker",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_REG_TRACER */	"reg_tracer",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_NEWTASK */		"newtask",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_ENDTASK */		"endtask",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SPNTASK */		"spntask",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_ARCHCODE */		"archcode",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_CATCHOUT */		"catchout",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETMWID */		"getmwid",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETTMASK */		"gettmask",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_HOSTSYNC */		"hostsync",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PACKF */			"packf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PRECV */			"precv",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PSEND */			"psend",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_REDUCE */		"reduce",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SETMWID */		"setmwid",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SETTMASK */		"settmask",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_UNPACKF */		"unpackf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GATHER */		"gather",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SCATTER */		"scatter",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PUTINFO */		"putinfo",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETINFO */		"getinfo",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_DELINFO */		"delinfo",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETMBOXINFO */	"getmboxinfo",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_NEWCONTEXT */	"newcontext",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_FREECONTEXT */	"freecontext",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SETCONTEXT */	"setcontext",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETCONTEXT */	"getcontext",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SIBLINGS */		"siblings",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_GETMINFO */		"getminfo",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_SETMINFO */		"setminfo",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_ADDMHF */		"addmhf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_DELMHF */		"delmhf",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_MHF_INVOKE */	"mhf_invoke",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_TIMING */		"timing",		0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_PROFILING */		"profiling",	0,	{ 0, 0 }, { 0, 0 }, 0,
	/* TEV_USER_DEFINED */	"user_defined",	0,	{ 0, 0 }, { 0, 0 }, 0
};

