
static char rcsid[] = 
	"$Id: trccompatglob.c,v 4.1 1996/10/31 20:47:48 pvmsrc Exp $";

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


/* Tracer Library Header */

#include "trclib.h"

#include "trccompat.h"


/* Old PVM 3.3 Trace Event Descriptors */

char *TRC_OLD_TRACE_DESCRIPTORS[TRC_OLD_TRACE_MAX] =
{

"\"addhosts0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Count\" \"Number of Hosts to Add\"\n\
	int \"count\";\n\
	// \"Names\" \"Names of Hosts to Add\"\n\
	char \"names\"[][];\n\
};;\n",

"\"addhosts1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"barrier0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
	// \"Count\" \"Number of Members to Wait\"\n\
	int \"count\";\n\
};;\n",

"\"barrier1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"bcast0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
};;\n",

"\"bcast1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"bufinfo0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"MID\" \"Message Buffer\"\n\
	int \"mid\";\n\
};;\n",

"\"bufinfo1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
	// \"Length\" \"Number of Bytes\"\n\
	int \"length\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
	// \"Src\" \"Message Source TID\"\n\
	int \"src\";\n\
};;\n",

"\"config0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"config1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
	// \"Nhost\" \"Number of Hosts\"\n\
	int \"nhost\";\n\
	// \"Narch\" \"Number of Architectures\"\n\
	int \"narch\";\n\
};;\n",

"\"delete0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Name\" \"Class Name\"\n\
	char \"name\"[];\n\
	// \"Req\" \"Class Index\"\n\
	int \"req\";\n\
};;\n",

"\"delete1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"delhosts0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Count\" \"Number of Hosts to Delete\"\n\
	int \"count\";\n\
	// \"Names\" \"Names of Hosts to Delete\"\n\
	char \"names\"[][];\n\
};;\n",

"\"delhosts1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"exit0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"exit1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"freebuf0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"MID\" \"Message Buffer\"\n\
	int \"mid\";\n\
};;\n",

"\"freebuf1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"getfds0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"getfds1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"NFDS\" \"# of File Descriptors\"\n\
	int \"nfds\";\n\
	// \"FDS\" \"File Descriptors\"\n\
	int \"fds\"[];\n\
};;\n",

"\"getinst0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
	// \"Instance\" \"Instance Number in Group\"\n\
	int \"instance\";\n\
};;\n",

"\"getinst1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"getopt0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Option\" \"Option Code\"\n\
	int \"option\";\n\
};;\n",

"\"getopt1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"getrbuf0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"getrbuf1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"getsbuf0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"getsbuf1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"gettid0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
	// \"Instance\" \"Instance Number in Group\"\n\
	int \"instance\";\n\
};;\n",

"\"gettid1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"gsize0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
};;\n",

"\"gsize1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"halt0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"halt1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"initsend0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Encoding\" \"Message Encoding\"\n\
	int \"encoding\";\n\
};;\n",

"\"initsend1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"insert0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Name\" \"Class Name\"\n\
	char \"name\"[];\n\
	// \"Req\" \"Class Index\"\n\
	int \"req\";\n\
	// \"Data\" \"Data for Insert\"\n\
	int \"data\";\n\
};;\n",

"\"insert1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"joingroup0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
};;\n",

"\"joingroup1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"kill0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Dst\" \"Destination Task ID\"\n\
	int \"dst\";\n\
};;\n",

"\"kill1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"lookup0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Name\" \"Class Name\"\n\
	char \"name\"[];\n\
	// \"Req\" \"Class Index\"\n\
	int \"req\";\n\
};;\n",

"\"lookup1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"lvgroup0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
};;\n",

"\"lvgroup1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"mcast0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Count\" \"Number of Tasks\"\n\
	int \"count\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
	// \"TIDs\" \"Destination Task IDs\"\n\
	int \"tids\"[];\n\
};;\n",

"\"mcast1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"mkbuf0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Encoding\" \"Message Encoding\"\n\
	int \"encoding\";\n\
};;\n",

"\"mkbuf1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"mstat0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Host\" \"Name of Host\"\n\
	char \"host\"[];\n\
};;\n",

"\"mstat1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"mytid0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"mytid1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"notify0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"What\" \"Specific Notify Event\"\n\
	int \"what\";\n\
	// \"Code\" \"Notify Code\"\n\
	int \"code\";\n\
	// \"Count\" \"Notify Count\"\n\
	int \"count\";\n\
	// \"Val\" \"Notify Values\"\n\
	int \"vals\"[];\n\
};;\n",

"\"notify1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"nrecv0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Src\" \"Message Source TID\"\n\
	int \"src\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
};;\n",

"\"nrecv1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
	// \"Nbytes\" \"Number of Bytes Received\"\n\
	int \"nbytes\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
	// \"Src\" \"Message Source TID\"\n\
	int \"src\";\n\
};;\n",

"\"parent0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"parent1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"perror0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"LocStr\" \"Error Location String\"\n\
	char \"locstr\"[];\n\
};;\n",

"\"perror1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"pkbyte0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkbyte1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkcplx0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkcplx1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkdcplx0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkdcplx1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkdouble0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkdouble1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkfloat0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkfloat1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkint0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkint1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkuint0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkuint1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pklong0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pklong1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkulong0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkulong1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkshort0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkshort1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkushort0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Pack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"pkushort1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pkstr0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
};;\n",

"\"pkstr1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"probe0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Src\" \"Message Source TID\"\n\
	int \"src\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
};;\n",

"\"probe1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"pstat0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Dst\" \"TID of Task to Pstat\"\n\
	int \"dst\";\n\
};;\n",

"\"pstat1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"recv0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Src\" \"Message Source TID\"\n\
	int \"src\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
};;\n",

"\"recv1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
	// \"Nbytes\" \"Number of Bytes Received\"\n\
	int \"nbytes\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
	// \"Src\" \"Message Source TID\"\n\
	int \"src\";\n\
};;\n",

"\"recvf0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"recvf1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"send0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Dst\" \"Message Destination TID\"\n\
	int \"dst\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
};;\n",

"\"send1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"sendsig0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Dst\" \"Signal Destination TID\"\n\
	int \"dst\";\n\
	// \"Sig\" \"Signal Number to Raise\"\n\
	int \"sig\";\n\
};;\n",

"\"sendsig1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"setopt0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"What\" \"Option to Set\"\n\
	int \"what\";\n\
	// \"Value\" \"Value to Set Option\"\n\
	int \"val\";\n\
};;\n",

"\"setopt1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"setrbuf0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"MID\" \"Message Buffer\"\n\
	int \"mid\";\n\
};;\n",

"\"setrbuf1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"setsbuf0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"MID\" \"Message Buffer\"\n\
	int \"mid\";\n\
};;\n",

"\"setsbuf1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"spawn0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"File\" \"Executable File\"\n\
	char \"file\"[];\n\
	// \"Flags\" \"Spawn Flags\"\n\
	int \"flags\";\n\
	// \"Where\" \"Host or Arch for Spawn\"\n\
	char \"where\"[];\n\
	// \"Count\" \"Number of Tasks to Spawn\"\n\
	int \"count\";\n\
};;\n",

"\"spawn1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"NTID\" \"Number of TIDs Spawned\"\n\
	int \"cc\";\n\
	// \"TIDs\" \"Task IDs of Spawned Tasks\"\n\
	int \"tids\"[];\n\
};;\n",

"\"start_pvmd0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Argc\" \"Number of Args\"\n\
	int \"argc\";\n\
	// \"Argv\" \"Argument Strings\"\n\
	char \"argv\"[][];\n\
	// \"Block\" \"Block Flag\"\n\
	int \"block\";\n\
};;\n",

"\"start_pvmd1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"tasks0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Where\" \"Which Hosts to Check\"\n\
	int \"where\";\n\
};;\n",

"\"tasks1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
	// \"Ntask\" \"Number of Tasks Found\"\n\
	int \"ntask\";\n\
};;\n",

"\"tickle0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Narg\" \"Number of Arguments\"\n\
	int \"narg\";\n\
	// \"Args\" \"Argument Array\"\n\
	int \"args\"[];\n\
};;\n",

"\"tickle1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
	// \"Nres\" \"Number of Responses\"\n\
	int \"nres\";\n\
};;\n",

"\"tidtohost0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"tidtohost1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"trecv0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Src\" \"Message Source TID\"\n\
	int \"src\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
};;\n",

"\"trecv1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
	// \"Nbytes\" \"Number of Bytes Received\"\n\
	int \"nbytes\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
	// \"Src\" \"Message Source TID\"\n\
	int \"src\";\n\
};;\n",

"\"upkbyte0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkbyte1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkcplx0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkcplx1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkdcplx0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkdcplx1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkdouble0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkdouble1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkfloat0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkfloat1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkint0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkint1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkuint0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkuint1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upklong0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upklong1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkulong0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkulong1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkshort0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkshort1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkushort0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
	// \"Count\" \"Number of Items to Unpack\"\n\
	int \"count\";\n\
	// \"Stride\" \"Stride Among Items\"\n\
	int \"stride\";\n\
};;\n",

"\"upkushort1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"upkstr0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Address\" \"Address of Stuff to Unpack\"\n\
	int \"addr\";\n\
};;\n",

"\"upkstr1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"version0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"version1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Version\" \"PVM Version String\"\n\
	char \"version\"[];\n\
};;\n",

"\"reg_hoster0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"reg_hoster1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"reg_rm0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"reg_rm1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"reg_tasker0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"reg_tasker1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"newtask\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"PTID\" \"Parent Task ID\"\n\
	int \"ptid\";\n\
	// \"Host\" \"PVMD TID of Host\"\n\
	int \"host\";\n\
	// \"Flags\" \"Task Spawn Flags\"\n\
	int \"flags\";\n\
	// \"Aout\" \"Executable File\"\n\
	char \"aout\"[];\n\
};;\n",

"",

"\"endtask\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Status\" \"Task Exit Status\"\n\
	int \"status\";\n\
	// \"Usec\" \"User Usage Secs\"\n\
	int \"usersec\";\n\
	// \"Uusec\" \"User Usage Usecs\"\n\
	int \"userusec\";\n\
	// \"Ssec\" \"System Usage Secs\"\n\
	int \"syssec\";\n\
	// \"Susec\" \"System Usage Usecs\"\n\
	int \"sysusec\";\n\
};;\n",

"",

"\"spntask\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Parent\" \"Parent Task ID\"\n\
	int \"parent\";\n\
};;\n",

"",

"\"archcode0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"archcode1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"catchout0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"catchout1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"getmwid0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"MID\" \"Message Buffer\"\n\
	int \"mid\";\n\
};;\n",

"\"getmwid1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"gettmask0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"gettmask1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"hostsync0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"hostsync1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"packf0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Format\" \"Format String\"\n\
	char \"fmt\"[];\n\
};;\n",

"\"packf1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"precv0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Src\" \"Message Source TID\"\n\
	int \"src\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Length\" \"Number of Bytes\"\n\
	int \"length\";\n\
	// \"Type\" \"Data Type to Which Buffer Points\"\n\
	int \"type\";\n\
};;\n",

"\"precv1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"psend0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Dst\" \"Message Destination TID\"\n\
	int \"dst\";\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
	// \"Address\" \"Address of Stuff to Pack\"\n\
	int \"addr\";\n\
	// \"Length\" \"Number of Bytes\"\n\
	int \"length\";\n\
	// \"Type\" \"Data Type to Which Buffer Points\"\n\
	int \"type\";\n\
};;\n",

"\"psend1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"reduce0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
};;\n",

"\"reduce1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"setmwid0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"MID\" \"Message Buffer\"\n\
	int \"mid\";\n\
	// \"WID\" \"Wait Message ID\"\n\
	int \"wid\";\n\
};;\n",

"\"setmwid1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"settmask0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"settmask1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
};;\n",

"\"unpackf0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Format\" \"Format String\"\n\
	char \"fmt\"[];\n\
};;\n",

"\"unpackf1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"gather0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
};;\n",

"\"gather1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"scatter0\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Group\" \"Name of Group\"\n\
	char \"group\"[];\n\
	// \"Code\" \"Message Type\"\n\
	int \"code\";\n\
};;\n",

"\"scatter1\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"CC\" \"Condition Code\"\n\
	int \"cc\";\n\
};;\n",

"\"output\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Task ID\"\n\
	int \"tid\";\n\
	// \"Out\" \"Task Output\"\n\
	char \"out\"[];\n\
};;\n",

"\"host_add\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Host PVMD Task ID\"\n\
	int \"tid\";\n\
	// \"Name\" \"Host Name\"\n\
	char \"name\"[];\n\
	// \"Alias\" \"Host Alias\"\n\
	char \"alias\"[];\n\
	// \"Arch\" \"Host Arch\"\n\
	char \"arch\"[];\n\
	// \"Speed\" \"Relative Host Speed\"\n\
	int \"speed\";\n\
};;\n",

"\"host_del\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Host PVMD Task ID\"\n\
	int \"tid\";\n\
	// \"Name\" \"Host Name\"\n\
	char \"name\"[];\n\
};;\n",

"\"host_sync\"\n\
{\n\
	// \"Time\" \"Seconds\"\n\
	int \"sec\";\n\
	// \"Time\" \"Micro Seconds\"\n\
	int \"usec\";\n\
	// \"TID\" \"Host PVMD Task ID\"\n\
	int \"tid\";\n\
	// \"DSec\" \"Host Clock Synchronization Delta Seconds\"\n\
	int \"dsec\";\n\
	// \"DUSec\" \"Host Clock Synchronization Delta Microseconds\"\n\
	int \"dusec\";\n\
};;\n"

};


/* Old PVM 3.3 Trace Event Names */

char *TRC_OLD_TEV_TRACE_NAMES[TRC_OLD_TRACE_MAX] =
{
	"addhosts0",
	"addhosts1",
	"barrier0",
	"barrier1",
	"bcast0",
	"bcast1",
	"bufinfo0",
	"bufinfo1",
	"config0",
	"config1",
	"delete0",
	"delete1",
	"delhosts0",
	"delhosts1",
	"exit0",
	"exit1",
	"freebuf0",
	"freebuf1",
	"getfds0",
	"getfds1",
	"getinst0",
	"getinst1",
	"getopt0",
	"getopt1",
	"getrbuf0",
	"getrbuf1",
	"getsbuf0",
	"getsbuf1",
	"gettid0",
	"gettid1",
	"gsize0",
	"gsize1",
	"halt0",
	"halt1",
	"initsend0",
	"initsend1",
	"insert0",
	"insert1",
	"joingroup0",
	"joingroup1",
	"kill0",
	"kill1",
	"lookup0",
	"lookup1",
	"lvgroup0",
	"lvgroup1",
	"mcast0",
	"mcast1",
	"mkbuf0",
	"mkbuf1",
	"mstat0",
	"mstat1",
	"mytid0",
	"mytid1",
	"notify0",
	"notify1",
	"nrecv0",
	"nrecv1",
	"parent0",
	"parent1",
	"perror0",
	"perror1",
	"pkbyte0",
	"pkbyte1",
	"pkcplx0",
	"pkcplx1",
	"pkdcplx0",
	"pkdcplx1",
	"pkdouble0",
	"pkdouble1",
	"pkfloat0",
	"pkfloat1",
	"pkint0",
	"pkint1",
	"pkuint0",
	"pkuint1",
	"pklong0",
	"pklong1",
	"pkulong0",
	"pkulong1",
	"pkshort0",
	"pkshort1",
	"pkushort0",
	"pkushort1",
	"pkstr0",
	"pkstr1",
	"probe0",
	"probe1",
	"pstat0",
	"pstat1",
	"recv0",
	"recv1",
	"recvf0",
	"recvf1",
	"send0",
	"send1",
	"sendsig0",
	"sendsig1",
	"setopt0",
	"setopt1",
	"setrbuf0",
	"setrbuf1",
	"setsbuf0",
	"setsbuf1",
	"spawn0",
	"spawn1",
	"start_pvmd0",
	"start_pvmd1",
	"tasks0",
	"tasks1",
	"tickle0",
	"tickle1",
	"tidtohost0",
	"tidtohost1",
	"trecv0",
	"trecv1",
	"upkbyte0",
	"upkbyte1",
	"upkcplx0",
	"upkcplx1",
	"upkdcplx0",
	"upkdcplx1",
	"upkdouble0",
	"upkdouble1",
	"upkfloat0",
	"upkfloat1",
	"upkint0",
	"upkint1",
	"upkuint0",
	"upkuint1",
	"upklong0",
	"upklong1",
	"upkulong0",
	"upkulong1",
	"upkshort0",
	"upkshort1",
	"upkushort0",
	"upkushort1",
	"upkstr0",
	"upkstr1",
	"version0",
	"version1",
	"reg_hoster0",
	"reg_hoster1",
	"reg_rm0",
	"reg_rm1",
	"reg_tasker0",
	"reg_tasker1",
	"newtask",
	"",
	"endtask",
	"",
	"spntask",
	"",
	"archcode0",
	"archcode1",
	"catchout0",
	"catchout1",
	"getmwid0",
	"getmwid1",
	"gettmask0",
	"gettmask1",
	"hostsync0",
	"hostsync1",
	"packf0",
	"packf1",
	"precv0",
	"precv1",
	"psend0",
	"psend1",
	"reduce0",
	"reduce1",
	"setmwid0",
	"setmwid1",
	"settmask0",
	"settmask1",
	"unpackf0",
	"unpackf1",
	"gather0",
	"gather1",
	"scatter0",
	"scatter1",
	"output",
	"host_add",
	"host_del",
	"host_sync",
};


/* Old PVM 3.3 Trace Event Formats */

struct trc_old_tevfmt trc_old_tev_formats[] =
{
	{ "pvm_addhosts0",		"(%*d, [%$0S])" },
	{ "pvm_addhosts1",		"=%R" },
	{ "pvm_barrier0",		"(%S, %d)" },
	{ "pvm_barrier1",		"=%R" },
	{ "pvm_bcast0",			"(%S, %d)" },
	{ "pvm_bcast1",			"=%R" },
	{ "pvm_bufinfo0",		"(%d)" },
	{ "pvm_bufinfo1",		"=%R, %d, %d, %d" },
	{ "pvm_config0",		"()" },
	{ "pvm_config1",		"=%R, %d, %d" },
	{ "pvm_delete0",		"(%S, %d)" },
	{ "pvm_delete1",		"=%R" },
	{ "pvm_delhosts0",		"(%*d, [%$0S])" },
	{ "pvm_delhosts1",		"=%R" },
	{ "pvm_exit0",			"()" },
	{ "pvm_exit1",			0 },
	{ "pvm_freebuf0",		"(%d)" },
	{ "pvm_freebuf1",		"=%R" },
	{ "pvm_getfds0",		"()" },
	{ "pvm_getfds1",		"=%*R [%$0d]" },
	{ "pvm_getinst0",		"(%S, %d)" },
	{ "pvm_getinst1",		"=%R" },
	{ "pvm_getopt0",		"(%d)" },
	{ "pvm_getopt1",		"=%R" },
	{ "pvm_getrbuf0",		"()" },
	{ "pvm_getrbuf1",		"=%R" },
	{ "pvm_getsbuf0",		"()" },
	{ "pvm_getsbuf1",		"=%R" },
	{ "pvm_gettid0",		"(%S, %d)" },
	{ "pvm_gettid1",		"=%R" },
	{ "pvm_gsize0",			"(%S)" },
	{ "pvm_gsize1",			"=%R" },
	{ "pvm_halt0",			"()" },
	{ "pvm_halt1",			0 },
	{ "pvm_initsend0",		"(%d)" },
	{ "pvm_initsend1",		"=%R" },
	{ "pvm_insert0",		"(%S, %d, %x)" },
	{ "pvm_insert1",		"=%R" },
	{ "pvm_joingroup0",		"(%S)" },
	{ "pvm_joingroup1",		"=%R" },
	{ "pvm_kill0",			"(%x)" },
	{ "pvm_kill1",			"=%R" },
	{ "pvm_lookup0",		"(%S, %d)" },
	{ "pvm_lookup1",		"=%R" },
	{ "pvm_lvgroup0",		"(%S)" },
	{ "pvm_lvgroup1",		"=%R" },
	{ "pvm_mcast0",			"(%*d, %d, [%$0x])" },
	{ "pvm_mcast1",			"=%R" },
	{ "pvm_mkbuf0",			"(%d)" },
	{ "pvm_mkbuf1",			"=%R" },
	{ "pvm_mstat0",			"(%S)" },
	{ "pvm_mstat1",			"=%R" },
	{ "pvm_mytid0",			"()" },
	{ "pvm_mytid1",			"=%r" },
	{ "pvm_notify0",		"(%d, %d, %d, ...)" },
	{ "pvm_notify1",		"=%R" },
	{ "pvm_nrecv0",			"(%x, %d)" },
	{ "pvm_nrecv1",			"=%R, ..." },
	{ "pvm_parent0",		"()" },
	{ "pvm_parent1",		"=%r" },
	{ "pvm_perror0",		"(%S %d)" },
	{ "pvm_perror1",		0 },
	{ "pvm_pkbyte0",		"(%x, %d, %d)" },
	{ "pvm_pkbyte1",		"=%R" },
	{ "pvm_pkcplx0",		"(%x, %d, %d)" },
	{ "pvm_pkcplx1",		"=%R" },
	{ "pvm_pkdcplx0",		"(%x, %d, %d)" },
	{ "pvm_pkdcplx1",		"=%R" },
	{ "pvm_pkdouble0",		"(%x, %d, %d)" },
	{ "pvm_pkdouble1",		"=%R" },
	{ "pvm_pkfloat0",		"(%x, %d, %d)" },
	{ "pvm_pkfloat1",		"=%R" },
	{ "pvm_pkint0",			"(%x, %d, %d)" },
	{ "pvm_pkint1",			"=%R" },
	{ "pvm_pkuint0",		"(%x, %d, %d)" },
	{ "pvm_pkuint1",		"=%R" },
	{ "pvm_pklong0",		"(%x, %d, %d)" },
	{ "pvm_pklong1",		"=%R" },
	{ "pvm_pkulong0",		"(%x, %d, %d)" },
	{ "pvm_pkulong1",		"=%R" },
	{ "pvm_pkshort0",		"(%x, %d, %d)" },
	{ "pvm_pkshort1",		"=%R" },
	{ "pvm_pkushort0",		"(%x, %d, %d)" },
	{ "pvm_pkushort1",		"=%R" },
	{ "pvm_pkstr0",			"(%x)" },
	{ "pvm_pkstr1",			"=%R" },
	{ "pvm_probe0",			"(%x, %d)" },
	{ "pvm_probe1",			"=%R" },
	{ "pvm_pstat0",			"(%x)" },
	{ "pvm_pstat1",			"=%R" },
	{ "pvm_recv0",			"(%x, %d)" },
	{ "pvm_recv1",			"=%R, %d, ..." },
	{ "pvm_recvf0",			"(...)" },
	{ "pvm_recvf1",			0 },
	{ "pvm_send0",			"(%x, %d)" },
	{ "pvm_send1",			"=%R" },
	{ "pvm_sendsig0",		"(%x, %d)" },
	{ "pvm_sendsig1",		"=%R" },
	{ "pvm_setopt0",		"(%d, %d)" },
	{ "pvm_setopt1",		"=%R" },
	{ "pvm_setrbuf0",		"(%d)" },
	{ "pvm_setrbuf1",		"=%R" },
	{ "pvm_setsbuf0",		"(%d)" },
	{ "pvm_setsbuf1",		"=%R" },
	{ "pvm_spawn0",			"(%S, ?, %d, %S, %d, ...)" },
	{ "pvm_spawn1",			"=%*R [%$0r]" },
	{ "pvm_start_pvmd0","%d, %d, ..." },
	{ "pvm_start_pvmd1","=%R" },
	{ "pvm_tasks0",			"(%x)" },
	{ "pvm_tasks1",			"=%R, %d" },
	{ "pvm_tickle0",		"(%*d, [%$0d])" },
	{ "pvm_tickle1",		"=%R, %d" },
	{ "pvm_tidtohost0",		"(%x)" },
	{ "pvm_tidtohost1",		"=%r" },
	{ "pvm_trecv0",			"(%x, %d, [%d %d])" },
	{ "pvm_trecv1",			"=%R, %d, ..." },
	{ "pvm_upkbyte0",		"(%x, %d, %d)" },
	{ "pvm_upkbyte1",		"=%R" },
	{ "pvm_upkcplx0",		"(%x, %d, %d)" },
	{ "pvm_upkcplx1",		"=%R" },
	{ "pvm_upkdcplx0",		"(%x, %d, %d)" },
	{ "pvm_upkdcplx1",		"=%R" },
	{ "pvm_upkdouble0",		"(%x, %d, %d)" },
	{ "pvm_upkdouble1",		"=%R" },
	{ "pvm_upkfloat0",		"(%x, %d, %d)" },
	{ "pvm_upkfloat1",		"=%R" },
	{ "pvm_upkint0",		"(%x, %d, %d)" },
	{ "pvm_upkint1",		"=%R" },
	{ "pvm_upkuint0",		"(%x, %d, %d)" },
	{ "pvm_upkuint1",		"=%R" },
	{ "pvm_upklong0",		"(%x, %d, %d)" },
	{ "pvm_upklong1",		"=%R" },
	{ "pvm_upkulong0",		"(%x, %d, %d)" },
	{ "pvm_upkulong1",		"=%R" },
	{ "pvm_upkshort0",		"(%x, %d, %d)" },
	{ "pvm_upkshort1",		"=%R" },
	{ "pvm_upkushort0",		"(%x, %d, %d)" },
	{ "pvm_upkushort1",		"=%R" },
	{ "pvm_upkstr0",		"(%x)" },
	{ "pvm_upkstr1",		"=%R" },
	{ "pvm_version0",		"()" },
	{ "pvm_version1",		"=%S" },
	{ "pvm_reg_hoster0",	"()" },
	{ "pvm_reg_hoster1",	"=%R" },
	{ "pvm_reg_rm0",		"()" },
	{ "pvm_reg_rm1",		"=%R" },
	{ "pvm_reg_tasker0",	"()" },
	{ "pvm_reg_tasker1",	"=%R" },
	{ "newtask",			" %x, %d, %S" },
	{ "",					0 },
	{ "endtask",			" %x u %d,%d s %d,%d" },
	{ "",					0 },
	{ "spntask",			" %x" },
	{ "",					0 },
	{ "pvm_archcode0",		0 /*"(%S)"*/ },
	{ "pvm_archcode1",		0 /*"=%R"*/ },
	{ "pvm_catchout0",		0 /*"(...)"*/ },
	{ "pvm_catchout1",		0 /*"=%R"*/ },
	{ "pvm_getmwid0",		"(%d)" },
	{ "pvm_getmwid1",		"=%R" },
	{ "pvm_gettmask0",		0 /*"(%d, )"*/ },
	{ "pvm_gettmask1",		0 /*"=%R"*/ },
	{ "pvm_hostsync0",		0 /*"(%d, ...)"*/ },
	{ "pvm_hostsync1",		0 /*"=%R"*/ },
	{ "pvm_packf0",			"(%S, ...)" },
	{ "pvm_packf1",			"=%R" },
	{ "pvm_precv0",			"(%x, %d, %x, %d, %d, ...)" },
	{ "pvm_precv1",			"=%R" },
	{ "pvm_psend0",			"(%x, %d, %x, %d, %d)" },
	{ "pvm_psend1",			"=%R" },
	{ "pvm_reduce0",		0 /*"(...)"*/ },
	{ "pvm_reduce1",		0 /*"=%R"*/ },
	{ "pvm_setmwid0",		"(%d, %d)" },
	{ "pvm_setmwid1",		"=%R" },
	{ "pvm_settmask0",		0 /*"(%d, %S)"*/ },
	{ "pvm_settmask1",		0 /*"=%R"*/ },
	{ "pvm_unpackf0",		"(%S, ...)" },
	{ "pvm_unpackf1",		"=%R" },
	{ "pvm_gather0",		0 },
	{ "pvm_gather1",		0 },
	{ "pvm_scatter0",		0 },
	{ "pvm_scatter1",		0 }
};


/* Old PVM 3.3 Trace Event Descriptor Flags */

int TRC_OLD_TEV_TRACE[TRC_OLD_TRACE_MAX];


/* Old PVM 3.3 Error Messages for -pvm_errno */

char *trc_old_pvm_errlist[] =
{
		"Error 0",
		"Error 1",
	"Bad parameter",
	"Count mismatch",
	"Value too large",
	"End of buffer",
	"No such host",
	"No such file",
		"Error 8",				/* not used */
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
	"No such entry",
	"Duplicate entry",
};


/* Old PVM 3.3 Exported Num of Errors */

int trc_old_pvm_nerr =
	sizeof(trc_old_pvm_errlist) / sizeof(trc_old_pvm_errlist[0]);


#ifdef USE_PVM_33


/* Reverse Compatibility Declarations for PVM 3.3 */


struct Pvmtevinfo pvmtevinfo[] =
{
	"dummy", 0, { 0, 0 }, { 0, 0 }, 0
};


struct Pvmtevdid pvmtevdidlist[] =
{
	"XXX", "Dummy"
};


#endif

