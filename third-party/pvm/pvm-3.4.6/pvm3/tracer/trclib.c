
static char rcsid[] =
	"$Id: trclib.c,v 4.1 1996/10/31 20:48:01 pvmsrc Exp $";

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


/* Global Variables */

struct timeval	TRC_TRACE_TIME;

TRC_TEVDESC		TRC_OUTPUT_TEVDESC;
TRC_TEVDESC		TRC_HOST_ADD_TEVDESC;
TRC_TEVDESC		TRC_HOST_DEL_TEVDESC;
TRC_TEVDESC		TRC_HOST_SYNC_TEVDESC;

TRC_TEVDESC		*TRC_TEVDESC_LIST;

int				TRC_TEVDESC_SIZE;

TRC_TEVREC		TRC_TMP_TR;

TRC_TRIE		TRC_OLD_EVENT_TRIE;

TRC_TRIE		TRC_HANDLE_TRIE;

TRC_TRIE		TRC_EVENT_TRIE;

TRC_TRIE		TRC_DID_TRIE;

TRC_DID			TRC_LOCAL_DID[ TEV_DID_MAX - TEV_DID_FIRST + 1 ];

TRC_DID			TRC_DID_LIST;

TRC_ID			TRC_DEAD_ID_LIST;

TRC_ID			TRC_ID_LIST;

char			*TRC_TYPE_STRS[16] =
{
	"void",
	"char",
	"float",
	"double",
	"double",
	"float",
	"int",
	"int",
	"int",
	"int",
	"int",
	"int",
	"char",
	"void",
	"void",
	"void"
};

char			*TRC_VERSION;

char			*TRC_NAME;

char			*TRC_TMP_HOST;

TRC_HOST		TRC_HOST_LIST;

char			*TRC_HOST_NAME;

char			*TRC_HOST_ALIAS;

int				TRC_HOST_ADD_NOTIFY_CODE;
int				TRC_HOST_DEL_NOTIFY_CODE;

int				TRC_TID;

int				TRC_TMP_CMP;

int				TRC_TMP_CC;

