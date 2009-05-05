
/* $Id: trccompat.h,v 4.1 1996/10/31 20:47:45 pvmsrc Exp $ */

/*
 *         Tracer version 1.0:  A Trace File Generator for PVM
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *           Authors:  James Arthur Kohl and G. A. Geist
 *                   (C) 1994 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear
 * in supporting documentation.
 *
 * Neither the Institution, Oak Ridge National Laboratory, nor the
 * Authors make any representations about the suitability of this
 * software for any purpose.  This software is provided ``as is''
 * without express or implied warranty.
 *
 * Tracer was funded by the U.S. Department of Energy.
 */


/* Old PVM 3.3 Libpvm tracing includes. */


/* Trace Event Constants */

#define	TRC_OLD_TEV_FIRST			0
#define	TRC_OLD_TEV_ADDHOSTS0		0
#define	TRC_OLD_TEV_ADDHOSTS1		1
#define	TRC_OLD_TEV_BARRIER0		2
#define	TRC_OLD_TEV_BARRIER1		3
#define	TRC_OLD_TEV_BCAST0			4
#define	TRC_OLD_TEV_BCAST1			5
#define	TRC_OLD_TEV_BUFINFO0		6
#define	TRC_OLD_TEV_BUFINFO1		7
#define	TRC_OLD_TEV_CONFIG0			8
#define	TRC_OLD_TEV_CONFIG1			9
#define	TRC_OLD_TEV_DELETE0			10
#define	TRC_OLD_TEV_DELETE1			11
#define	TRC_OLD_TEV_DELHOSTS0		12
#define	TRC_OLD_TEV_DELHOSTS1		13
#define	TRC_OLD_TEV_EXIT0			14
#define	TRC_OLD_TEV_EXIT1			15
#define	TRC_OLD_TEV_FREEBUF0		16
#define	TRC_OLD_TEV_FREEBUF1		17
#define	TRC_OLD_TEV_GETFDS0			18
#define	TRC_OLD_TEV_GETFDS1			19
#define	TRC_OLD_TEV_GETINST0		20
#define	TRC_OLD_TEV_GETINST1		21
#define	TRC_OLD_TEV_GETOPT0			22
#define	TRC_OLD_TEV_GETOPT1			23
#define	TRC_OLD_TEV_GETRBUF0		24
#define	TRC_OLD_TEV_GETRBUF1		25
#define	TRC_OLD_TEV_GETSBUF0		26
#define	TRC_OLD_TEV_GETSBUF1		27
#define	TRC_OLD_TEV_GETTID0			28
#define	TRC_OLD_TEV_GETTID1			29
#define	TRC_OLD_TEV_GSIZE0			30
#define	TRC_OLD_TEV_GSIZE1			31
#define	TRC_OLD_TEV_HALT0			32
#define	TRC_OLD_TEV_HALT1			33
#define	TRC_OLD_TEV_INITSEND0		34
#define	TRC_OLD_TEV_INITSEND1		35
#define	TRC_OLD_TEV_INSERT0			36
#define	TRC_OLD_TEV_INSERT1			37
#define	TRC_OLD_TEV_JOINGROUP0		38
#define	TRC_OLD_TEV_JOINGROUP1		39
#define	TRC_OLD_TEV_KILL0			40
#define	TRC_OLD_TEV_KILL1			41
#define	TRC_OLD_TEV_LOOKUP0			42
#define	TRC_OLD_TEV_LOOKUP1			43
#define	TRC_OLD_TEV_LVGROUP0		44
#define	TRC_OLD_TEV_LVGROUP1		45
#define	TRC_OLD_TEV_MCAST0			46
#define	TRC_OLD_TEV_MCAST1			47
#define	TRC_OLD_TEV_MKBUF0			48
#define	TRC_OLD_TEV_MKBUF1			49
#define	TRC_OLD_TEV_MSTAT0			50
#define	TRC_OLD_TEV_MSTAT1			51
#define	TRC_OLD_TEV_MYTID0			52
#define	TRC_OLD_TEV_MYTID1			53
#define	TRC_OLD_TEV_NOTIFY0			54
#define	TRC_OLD_TEV_NOTIFY1			55
#define	TRC_OLD_TEV_NRECV0			56
#define	TRC_OLD_TEV_NRECV1			57
#define	TRC_OLD_TEV_PARENT0			58
#define	TRC_OLD_TEV_PARENT1			59
#define	TRC_OLD_TEV_PERROR0			60
#define	TRC_OLD_TEV_PERROR1			61
#define	TRC_OLD_TEV_PKBYTE0			62
#define	TRC_OLD_TEV_PKBYTE1			63
#define	TRC_OLD_TEV_PKCPLX0			64
#define	TRC_OLD_TEV_PKCPLX1			65
#define	TRC_OLD_TEV_PKDCPLX0		66
#define	TRC_OLD_TEV_PKDCPLX1		67
#define	TRC_OLD_TEV_PKDOUBLE0		68
#define	TRC_OLD_TEV_PKDOUBLE1		69
#define	TRC_OLD_TEV_PKFLOAT0		70
#define	TRC_OLD_TEV_PKFLOAT1		71
#define	TRC_OLD_TEV_PKINT0			72
#define	TRC_OLD_TEV_PKINT1			73
#define	TRC_OLD_TEV_PKUINT0			74
#define	TRC_OLD_TEV_PKUINT1			75
#define	TRC_OLD_TEV_PKLONG0			76
#define	TRC_OLD_TEV_PKLONG1			77
#define	TRC_OLD_TEV_PKULONG0		78
#define	TRC_OLD_TEV_PKULONG1		79
#define	TRC_OLD_TEV_PKSHORT0		80
#define	TRC_OLD_TEV_PKSHORT1		81
#define	TRC_OLD_TEV_PKUSHORT0		82
#define	TRC_OLD_TEV_PKUSHORT1		83
#define	TRC_OLD_TEV_PKSTR0			84
#define	TRC_OLD_TEV_PKSTR1			85
#define	TRC_OLD_TEV_PROBE0			86
#define	TRC_OLD_TEV_PROBE1			87
#define	TRC_OLD_TEV_PSTAT0			88
#define	TRC_OLD_TEV_PSTAT1			89
#define	TRC_OLD_TEV_RECV0			90
#define	TRC_OLD_TEV_RECV1			91
#define	TRC_OLD_TEV_RECVF0			92
#define	TRC_OLD_TEV_RECVF1			93
#define	TRC_OLD_TEV_SEND0			94
#define	TRC_OLD_TEV_SEND1			95
#define	TRC_OLD_TEV_SENDSIG0		96
#define	TRC_OLD_TEV_SENDSIG1		97
#define	TRC_OLD_TEV_SETOPT0			98
#define	TRC_OLD_TEV_SETOPT1			99
#define	TRC_OLD_TEV_SETRBUF0		100
#define	TRC_OLD_TEV_SETRBUF1		101
#define	TRC_OLD_TEV_SETSBUF0		102
#define	TRC_OLD_TEV_SETSBUF1		103
#define	TRC_OLD_TEV_SPAWN0			104
#define	TRC_OLD_TEV_SPAWN1			105
#define	TRC_OLD_TEV_START_PVMD0		106
#define	TRC_OLD_TEV_START_PVMD1		107
#define	TRC_OLD_TEV_TASKS0			108
#define	TRC_OLD_TEV_TASKS1			109
#define	TRC_OLD_TEV_TICKLE0			110
#define	TRC_OLD_TEV_TICKLE1			111
#define	TRC_OLD_TEV_TIDTOHOST0		112
#define	TRC_OLD_TEV_TIDTOHOST1		113
#define	TRC_OLD_TEV_TRECV0			114
#define	TRC_OLD_TEV_TRECV1			115
#define	TRC_OLD_TEV_UPKBYTE0		116
#define	TRC_OLD_TEV_UPKBYTE1		117
#define	TRC_OLD_TEV_UPKCPLX0		118
#define	TRC_OLD_TEV_UPKCPLX1		119
#define	TRC_OLD_TEV_UPKDCPLX0		120
#define	TRC_OLD_TEV_UPKDCPLX1		121
#define	TRC_OLD_TEV_UPKDOUBLE0		122
#define	TRC_OLD_TEV_UPKDOUBLE1		123
#define	TRC_OLD_TEV_UPKFLOAT0		124
#define	TRC_OLD_TEV_UPKFLOAT1		125
#define	TRC_OLD_TEV_UPKINT0			126
#define	TRC_OLD_TEV_UPKINT1			127
#define	TRC_OLD_TEV_UPKUINT0		128
#define	TRC_OLD_TEV_UPKUINT1		129
#define	TRC_OLD_TEV_UPKLONG0		130
#define	TRC_OLD_TEV_UPKLONG1		131
#define	TRC_OLD_TEV_UPKULONG0		132
#define	TRC_OLD_TEV_UPKULONG1		133
#define	TRC_OLD_TEV_UPKSHORT0		134
#define	TRC_OLD_TEV_UPKSHORT1		135
#define	TRC_OLD_TEV_UPKUSHORT0		136
#define	TRC_OLD_TEV_UPKUSHORT1		137
#define	TRC_OLD_TEV_UPKSTR0			138
#define	TRC_OLD_TEV_UPKSTR1			139
#define	TRC_OLD_TEV_VERSION0		140
#define	TRC_OLD_TEV_VERSION1		141
#define	TRC_OLD_TEV_REG_HOSTER0		142
#define	TRC_OLD_TEV_REG_HOSTER1		143
#define	TRC_OLD_TEV_REG_RM0			144
#define	TRC_OLD_TEV_REG_RM1			145
#define	TRC_OLD_TEV_REG_TASKER0		146
#define	TRC_OLD_TEV_REG_TASKER1		147
#define	TRC_OLD_TEV_NEWTASK			148
#define	TRC_OLD_TEV_ENDTASK			150
#define	TRC_OLD_TEV_SPNTASK			152
#define	TRC_OLD_TEV_ARCHCODE0		154
#define	TRC_OLD_TEV_ARCHCODE1		155
#define	TRC_OLD_TEV_CATCHOUT0		156
#define	TRC_OLD_TEV_CATCHOUT1		157
#define	TRC_OLD_TEV_GETMWID0		158
#define	TRC_OLD_TEV_GETMWID1		159
#define	TRC_OLD_TEV_GETTMASK0		160
#define	TRC_OLD_TEV_GETTMASK1		161
#define	TRC_OLD_TEV_HOSTSYNC0		162
#define	TRC_OLD_TEV_HOSTSYNC1		163
#define	TRC_OLD_TEV_PACKF0			164
#define	TRC_OLD_TEV_PACKF1			165
#define	TRC_OLD_TEV_PRECV0			166
#define	TRC_OLD_TEV_PRECV1			167
#define	TRC_OLD_TEV_PSEND0			168
#define	TRC_OLD_TEV_PSEND1			169
#define	TRC_OLD_TEV_REDUCE0			170
#define	TRC_OLD_TEV_REDUCE1			171
#define	TRC_OLD_TEV_SETMWID0		172
#define	TRC_OLD_TEV_SETMWID1		173
#define	TRC_OLD_TEV_SETTMASK0		174
#define	TRC_OLD_TEV_SETTMASK1		175
#define	TRC_OLD_TEV_UNPACKF0		176
#define	TRC_OLD_TEV_UNPACKF1		177
#define	TRC_OLD_TEV_GATHER0			178
#define	TRC_OLD_TEV_GATHER1			179
#define	TRC_OLD_TEV_SCATTER0		180
#define	TRC_OLD_TEV_SCATTER1		181
#define	TRC_OLD_TEV_MAX				181

#define TRC_OLD_TRACE_OUTPUT		(TRC_OLD_TEV_MAX + 1)
#define TRC_OLD_TRACE_HOST_ADD		(TRC_OLD_TEV_MAX + 2)
#define TRC_OLD_TRACE_HOST_DEL		(TRC_OLD_TEV_MAX + 3)
#define TRC_OLD_TRACE_HOST_SYNC		(TRC_OLD_TEV_MAX + 4)
#define TRC_OLD_TRACE_MAX			(TRC_OLD_TEV_MAX + 5)


/* Old PVM 3.3 Event Format Structure */

struct trc_old_tevfmt
{
	char *name;
	char *fmt;
};


/* Globals */

extern char *TRC_OLD_TRACE_DESCRIPTORS[];

extern char *TRC_OLD_TEV_TRACE_NAMES[];

extern struct trc_old_tevfmt trc_old_tev_formats[];

extern int TRC_OLD_TEV_TRACE[TRC_OLD_TRACE_MAX];

extern char *trc_old_pvm_errlist[];

extern int trc_old_pvm_nerr;

