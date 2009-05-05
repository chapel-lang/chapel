
/* $Id: host.h,v 1.6 2001/09/26 23:35:21 pvmsrc Exp $ */

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
 *	host.h
 *
 *	Host table data structures.
 *
 * $Log: host.h,v $
 * Revision 1.6  2001/09/26 23:35:21  pvmsrc
 * Added new hd_vmid to hostd struct.
 * 	- use to override local PVM_VMID settings with hostfile "id=" option
 * (Spanker=kohl)
 *
 * Revision 1.5  1999/07/08 19:00:20  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.4  1997/06/25  22:08:45  pvmsrc
 * Markus adds his frigging name to the author list of
 * 	every file he ever looked at...
 *
 * Revision 1.3  1997/04/30  21:25:57  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.2  1997/01/28  19:27:56  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:17  pvmsrc
 * Initial revision
 *
 * Revision 1.4  1995/05/17  16:10:01  manchek
 * added HF_OVERLOAD
 *
 * Revision 1.3  1994/06/21  18:30:47  manchek
 * added HF_SPEED flag
 *
 * Revision 1.2  1994/06/03  20:38:14  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:26:48  manchek
 * Initial revision
 *
 */


/* Host descriptor */

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
	struct sockaddr_in hd_sad;	/* UDP address/port */
	int hd_rxseq;				/* expected next seq num from host */
	int hd_txseq;				/* next tx seq num to host */
	struct pkt *hd_txq;			/* not-yet-sent packet queue to host */
	struct pkt *hd_opq;			/* outstanding packets to host */
	int hd_nop;					/* length of opq */
	struct pkt *hd_rxq;			/* packet reordering queue from host */
	struct pmsg *hd_rxm;		/* to-us msg reassembly from host */
	struct timeval hd_rtt;		/* estd round-trip time to host */
	int hd_speed;				/* cpu relative speed */
	struct mca *hd_mcas;		/* from-host mca cache */
	char *hd_aname;				/* name to use for network address */
	char *hd_vmid;				/* virtual machine id */
};

/* hd_flag values */

/* for advisory host table */
#define	HF_NOSTART	0x100		/* don't start pvmd */
#define	HF_SPEED	0x200		/* hd_speed is set */
#define	HF_OVERLOAD	0x400		/* allow overloaded (>1 pvmd) host */


/* Host table */

struct htab {
	int ht_serial;				/* serial number */
	int ht_last;				/* highest entry */
	int ht_cnt;					/* number of entries (not incl [0]) */
	int ht_master;				/* master host */
	int ht_cons;				/* console host */
	int ht_local;				/* this host */
	int ht_narch;				/* count of different data reps */
	struct hostd **ht_hosts;	/* hosts */
};

struct hostd *hd_new __ProtoGlarp__(( int ));
void hd_free __ProtoGlarp__(( struct hostd * ));
void hd_unref __ProtoGlarp__(( struct hostd * ));
void hd_dump __ProtoGlarp__(( struct hostd * ));

struct hostd *nametohost __ProtoGlarp__(( struct htab *, char * ));
struct hostd *indtohost __ProtoGlarp__(( struct htab *, int ));
struct hostd *tidtohost __ProtoGlarp__(( struct htab *, int ));
struct htab *ht_new __ProtoGlarp__(( int ));
void ht_insert __ProtoGlarp__(( struct htab *, struct hostd * ));
void ht_delete __ProtoGlarp__(( struct htab *, struct hostd * ));
void ht_free __ProtoGlarp__(( struct htab * ));
struct htab *readhostfile __ProtoGlarp__(( char * ));
int ht_merge __ProtoGlarp__(( struct htab *, struct htab * ));
void ht_dump __ProtoGlarp__(( struct htab * ));

