/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2015 Intel Corporation.

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  Contact Information:
  Intel Corporation, www.intel.com

  BSD LICENSE

  Copyright(c) 2015 Intel Corporation.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/* Copyright (c) 2003-2014 Intel Corporation. All rights reserved. */

#include "psm_user.h"

#define PSMI_NOLOG  -1

struct psm2_error_token {
	psm2_ep_t ep;
	psm2_error_t error;
	char err_string[PSM2_ERRSTRING_MAXLEN];
};

static
psm2_error_t
psmi_errhandler_noop(psm2_ep_t ep, const psm2_error_t err,
		     const char *error_string, psm2_error_token_t token)
{
	return err;
}

static
psm2_error_t
psmi_errhandler_psm(psm2_ep_t ep,
		    const psm2_error_t err,
		    const char *error_string, psm2_error_token_t token)
{
	/* we want the error to be seen through ssh, etc., so we flush and then
	 * sleep a bit.   Not perfect, but not doing so means it almost never
	 * gets seen. */
	fprintf(stderr, "%s: %s\n", psm3_get_mylabel(), token->err_string);
	fflush(stdout);
	fflush(stderr);

	/* XXX Eventually, this will hook up to a connection manager, and we'll
	 * issue an upcall into the connection manager at shutdown time */
	sleep(3);

	/* We use this "special" ep internally to handle internal errors that are
	 * triggered from within code that is not expected to return to the user.
	 * Errors of this sort on not expected to be handled by users and always
	 * mean we have an internal PSM bug. */
	if (err == PSM2_INTERNAL_ERR)
		abort();
	else
		exit(-1);
}

psm2_ep_errhandler_t psm3_errhandler_global = psmi_errhandler_noop;

psm2_error_t psm3_error_defer(psm2_error_token_t token)
{
	psm2_error_t rv;
	PSM2_LOG_MSG("entering");
	rv = psmi_errhandler_psm(token->ep, token->error, token->err_string,
				   token);
	PSM2_LOG_MSG("leaving");
	return rv;
}

psm2_error_t
psm3_error_register_handler(psm2_ep_t ep, const psm2_ep_errhandler_t errhandler)
{
	psm2_ep_errhandler_t *errh;

	PSM2_LOG_MSG("entering");

	if (ep == NULL)
		errh = &psm3_errhandler_global;
	else
		errh = &ep->errh;

	if (errhandler == PSM2_ERRHANDLER_PSM_HANDLER)
		*errh = psmi_errhandler_psm;
	else if (errhandler == PSM2_ERRHANDLER_NO_HANDLER)
		*errh = psmi_errhandler_noop;
	else
		*errh = errhandler;

	PSM2_LOG_MSG("leaving");

	return PSM2_OK;
}

psm2_error_t
MOCKABLE (psm3_handle_error)(psm2_ep_t ep, psm2_error_t error, const char *buf, ...)
{
	va_list argptr;
	int syslog_level;
	int console_print = 0;
	psm2_error_t newerr;
	struct psm2_error_token token;
	char *c, fullmsg[PSM2_ERRSTRING_MAXLEN];
	token.error = error;
	snprintf(fullmsg, PSM2_ERRSTRING_MAXLEN - 1, "%s", buf);
	fullmsg[PSM2_ERRSTRING_MAXLEN - 1] = '\0';
	va_start(argptr, buf);
	vsnprintf(token.err_string, PSM2_ERRSTRING_MAXLEN - 1, fullmsg, argptr);
	va_end(argptr);
	token.err_string[PSM2_ERRSTRING_MAXLEN - 1] = '\0';

	/* Unless the user has set PSM3_NO_VERBOSE_ERRORS, always print errors to
	 * console */
	c = getenv("PSM3_NO_VERBOSE_ERRORS");
	console_print = 0;
	if (ep == PSMI_EP_LOGEVENT)
		console_print = 1;
	else if (!c || *c == '\0') {	/* no desire to prevent verbose errors */
		/* Remove the console print if we're internally handling the error */
		if (ep == PSMI_EP_NORETURN)
			console_print = 0;
		else if (ep == NULL
			 && psm3_errhandler_global != psmi_errhandler_psm)
			console_print = 1;
		else if (ep != NULL && ep->errh != psmi_errhandler_psm)
			console_print = 1;
	}

	/* Before we let the user even handle the error, send to syslog */
	syslog_level = psm3_error_syslog_level(error);
	if (syslog_level != PSMI_NOLOG || ep == PSMI_EP_LOGEVENT)
		psm3_syslog(ep, console_print,
			    ep == PSMI_EP_LOGEVENT ? LOG_NOTICE : syslog_level,
			    "%s (err=%d)", token.err_string, error);

	if (ep == PSMI_EP_LOGEVENT)	/* we're just logging */
		newerr = PSM2_OK;
	else if (ep == PSMI_EP_NORETURN)
		newerr =
		    psmi_errhandler_psm(NULL, error, token.err_string, &token);
	else if (ep == NULL)
		newerr =
		    psm3_errhandler_global(NULL, error, token.err_string,
					   &token);
	else
		newerr = ep->errh(ep, error, token.err_string, &token);

	return newerr;
}
MOCK_DEF_EPILOGUE(psm3_handle_error);

/* Returns the "worst" error out of errA and errB */
psm2_error_t psm3_error_cmp(psm2_error_t errA, psm2_error_t errB)
{
#define _PSMI_ERR_IS(err) if (errA == (err) || errB == (err)) return (err)

	/* Bad runtime or before initialization */
	_PSMI_ERR_IS(PSM2_NO_MEMORY);
	_PSMI_ERR_IS(PSM2_INTERNAL_ERR);
	_PSMI_ERR_IS(PSM2_INIT_NOT_INIT);
	_PSMI_ERR_IS(PSM2_INIT_BAD_API_VERSION);

	/* Before we cget an endpoint */
	_PSMI_ERR_IS(PSM2_EP_NO_DEVICE);
	_PSMI_ERR_IS(PSM2_EP_UNIT_NOT_FOUND);
	_PSMI_ERR_IS(PSM2_EP_DEVICE_FAILURE);
	_PSMI_ERR_IS(PSM2_EP_NO_PORTS_AVAIL);
	_PSMI_ERR_IS(PSM2_TOO_MANY_ENDPOINTS);

	/* As we open/close the endpoint */
	_PSMI_ERR_IS(PSM2_EP_NO_NETWORK);
	_PSMI_ERR_IS(PSM2_SHMEM_SEGMENT_ERR);
	_PSMI_ERR_IS(PSM2_EP_CLOSE_TIMEOUT);
	_PSMI_ERR_IS(PSM2_EP_INVALID_UUID_KEY);
	_PSMI_ERR_IS(PSM2_EP_NO_RESOURCES);

	/* In connect phase */
	_PSMI_ERR_IS(PSM2_EPID_NETWORK_ERROR);
	_PSMI_ERR_IS(PSM2_EPID_INVALID_NODE);
	_PSMI_ERR_IS(PSM2_EPID_INVALID_CONNECT);
	_PSMI_ERR_IS(PSM2_EPID_INVALID_PKEY);
	_PSMI_ERR_IS(PSM2_EPID_INVALID_VERSION);
	_PSMI_ERR_IS(PSM2_EPID_INVALID_UUID_KEY);
	_PSMI_ERR_IS(PSM2_EPID_INVALID_MTU);
	_PSMI_ERR_IS(PSM2_EPID_RV_CONNECT_ERROR);

	/* Timeout if nothing else */
	_PSMI_ERR_IS(PSM2_TIMEOUT);

	_PSMI_ERR_IS(PSM2_EPID_RV_CONNECT_RECOVERING);

	/* Last resort */
	return max(errA, errB);
}

struct psmi_error_item {
	int syslog_level;
	const char *error_string;
};

static
struct psmi_error_item psmi_error_items[] = {
	{PSMI_NOLOG, "Success"},	/*  PSM2_OK = 0, */
	{PSMI_NOLOG, "No events were progressed in psm_poll"},	/* PSM2_OK_NO_PROGRESS = 1 */
	{PSMI_NOLOG, "unknown 2"},
	{PSMI_NOLOG, "Error in a function parameter"},	/* PSM2_PARAM_ERR = 3 */
	{LOG_CRIT, "Ran out of memory"},	/* PSM2_NO_MEMORY = 4 */
	{PSMI_NOLOG, "PSM has not been initialized"},	/* PSM2_INIT_NOT_INIT = 5 */
	{LOG_INFO, "API version passed is incompatible"},	/* PSM2_INIT_BAD_API_VERSION = 6 */
	{PSMI_NOLOG, "PSM Could not set affinity"},	/* PSM2_NO_AFFINITY = 7 */
	{LOG_ALERT, "PSM Unresolved internal error"},	/* PSM2_INTERNAL_ERR = 8 */
	{LOG_CRIT, "PSM could not set up shared memory segment"},	/* PSM2_SHMEM_SEGMENT_ERR = 9 */
	{PSMI_NOLOG, "PSM option is a read-only option"},	/* PSM2_OPT_READONLY = 10 */
	{PSMI_NOLOG, "Operation timed out"},	/* PSM2_TIMEOUT = 11 */
	{LOG_INFO, "Exceeded supported amount of endpoints"},
	/* PSM2_TOO_MANY_ENDPOINTS = 12 */
	{PSMI_NOLOG, "PSM is in the finalized state"},	/* PSM2_IS_FINALIZED = 13 */
	{PSMI_NOLOG, "unknown 14"},
	{PSMI_NOLOG, "unknown 15"},
	{PSMI_NOLOG, "unknown 16"},
	{PSMI_NOLOG, "unknown 17"},
	{PSMI_NOLOG, "unknown 18"},
	{PSMI_NOLOG, "unknown 19"},
	{PSMI_NOLOG, "Endpoint was closed"},	/* PSM2_EP_WAS_CLOSED = 20 */
	{LOG_ALERT, "PSM Could not find an OPA Unit"},	/* PSM2_EP_NO_DEVICE = 21 */
	{PSMI_NOLOG, "User passed a bad unit number"},	/* PSM2_EP_UNIT_NOT_FOUND = 22 */
	{LOG_ALERT, "Failure in initializing endpoint"},	/* PSM2_EP_DEVICE_FAILURE = 23 */
	{PSMI_NOLOG, "Error closing the endpoing error"},	/* PSM2_EP_CLOSE_TIMEOUT = 24 */
	{PSMI_NOLOG, "No free contexts could be obtained"},	/* PSM2_EP_NO_PORTS_AVAIL = 25 */
	{LOG_ALERT, "Could not detect network connectivity"},	/* PSM2_EP_NO_NETWORK = 26 */
	{LOG_INFO, "Invalid Unique job-wide UUID Key"},	/* PSM2_EP_INVALID_UUID_KEY = 27 */
	{LOG_INFO, "Out of endpoint resources"},	/* PSM2_EP_NO_RESOURCES = 28 */
	{PSMI_NOLOG, "unknown 29"},
	{PSMI_NOLOG, "unknown 30"},
	{PSMI_NOLOG, "unknown 31"},
	{PSMI_NOLOG, "unknown 32"},
	{PSMI_NOLOG, "unknown 33"},
	{PSMI_NOLOG, "unknown 34"},
	{PSMI_NOLOG, "unknown 35"},
	{PSMI_NOLOG, "unknown 36"},
	{PSMI_NOLOG, "unknown 37"},
	{PSMI_NOLOG, "unknown 38"},
	{PSMI_NOLOG, "unknown 39"},
	{PSMI_NOLOG, "Unknown/unresolved connection status (other errors occurred)"},	/* PSM2_EPID_UNKNOWN = 40 */
	{PSMI_NOLOG, "Endpoint could not be reached"},	/* PSM2_EPID_UNREACHABLE = 41 */
	{PSMI_NOLOG, "unknown 42"},
	{LOG_CRIT, "Invalid node (mismatch in bit width 32/64 or byte order)"},	/* PSM2_EPID_INVALID_NODE = 43 */
	{LOG_CRIT, "Invalid MTU"},	/* PSM2_EPID_INVALID_MTU =  44 */
	{PSMI_NOLOG, "UUID key mismatch"},	/* PSM2_EPID_INVALID_UUID_KEY = 45 */
	{LOG_ERR, "Incompatible PSM version"},	/* PSM2_EPID_INVALID_VERSION = 46 */
	{LOG_CRIT, "Connect received garbled connection information"},	/* PSM2_EPID_INVALID_CONNECT = 47 */
	{PSMI_NOLOG, "Endpoint was already connected"},	/* PSM2_EPID_ALREADY_CONNECTED = 48 */
	{LOG_CRIT, "Two or more endpoints have the same network id (LID)"},	/* PSM2_EPID_NETWORK_ERROR = 49 */
	{LOG_CRIT, "Endpoint provided incompatible Partition Key"},
	{LOG_CRIT, "Unable to resolve network path. Check connectivity and routing between nodes"},
	{LOG_CRIT, "Unable to establish RV RC QP connection"}, /* PSM2_EPID_RV_CONNECT_ERROR */
	{LOG_INFO, "Recovering RV RC QP connection"}, /* PSM2_EPID_RV_CONNECT_RECOVERING */
	{PSMI_NOLOG, "unknown 54"},
	{PSMI_NOLOG, "unknown 55"},
	{PSMI_NOLOG, "unknown 56"},
	{PSMI_NOLOG, "unknown 57"},
	{PSMI_NOLOG, "unknown 58"},
	{PSMI_NOLOG, "unknown 59"},
	{PSMI_NOLOG, "MQ Non-blocking request is incomplete"},	/* PSM2_MQ_NO_COMPLETIONS = 60 */
	{PSMI_NOLOG, "MQ Message has been truncated at the receiver"},	/* PSM2_MQ_TRUNCATION = 61 */
	{PSMI_NOLOG, "unknown 62"},
	{PSMI_NOLOG, "unknown 63"},
	{PSMI_NOLOG, "unknown 64"},
	{PSMI_NOLOG, "unknown 65"},
	{PSMI_NOLOG, "unknown 66"},
	{PSMI_NOLOG, "unknown 67"},
	{PSMI_NOLOG, "unknown 68"},
	{PSMI_NOLOG, "unknown 69"},
	{PSMI_NOLOG, "Invalid AM reply"},
	{PSMI_NOLOG, "unknown 71"},
	{PSMI_NOLOG, "unknown 72"},
	{PSMI_NOLOG, "unknown 73"},
	{PSMI_NOLOG, "unknown 74"},
	{PSMI_NOLOG, "unknown 75"},
	{PSMI_NOLOG, "unknown 76"},
	{PSMI_NOLOG, "unknown 77"},
	{PSMI_NOLOG, "unknown 78"},
	{PSMI_NOLOG, "unknown 79"},
	{PSMI_NOLOG, "unknown 80"},
};

const char *psm3_error_get_string(psm2_error_t error)
{
	PSM2_LOG_MSG("entering");
	if (error >= PSM2_ERROR_LAST) {
		PSM2_LOG_MSG("leaving");
		return "unknown";
	}
	else {
		PSM2_LOG_MSG("leaving");
		return psmi_error_items[error].error_string;
	}
}

int psm3_error_syslog_level(psm2_error_t error)
{
	if (error >= PSM2_ERROR_LAST)
		return PSMI_NOLOG;
	else
		return psmi_error_items[error].syslog_level;
}
