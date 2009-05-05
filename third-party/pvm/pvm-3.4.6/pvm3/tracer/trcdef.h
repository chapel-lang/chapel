
/* $Id: trcdef.h,v 4.1 1996/10/31 20:47:54 pvmsrc Exp $ */

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


/* Tracer Data Types Header */

#include "trctype.h"


#ifdef USE_PVM_33

/* Backwards Compatibility Header for PVM 3.3 Usage */

#include "trccompatrev.h"

#endif


/* Routine Prototypes */

#include "trcproto.h"


/* External Variables */

extern	struct timeval	TRC_TRACE_TIME;

extern	TRC_TEVREC		TRC_TMP_TR;

extern	TRC_TRIE		TRC_OLD_EVENT_TRIE;

extern	TRC_TRIE		TRC_HANDLE_TRIE;

extern	TRC_TRIE		TRC_EVENT_TRIE;

extern	TRC_TRIE		TRC_DID_TRIE;

extern	TRC_HOST		TRC_HOST_LIST;

extern	char			*TRC_HOST_NAME;
extern	char			*TRC_HOST_ALIAS;

extern	char			*TRC_TMP_HOST;

extern	char			*TRC_VERSION;

extern	char			*TRC_NAME;

extern	int				TRC_HOST_ADD_NOTIFY_CODE;
extern	int				TRC_HOST_DEL_NOTIFY_CODE;

extern	int				TRC_TMP_CMP;

extern	int				TRC_TID;

