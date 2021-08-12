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

#ifndef _PSMI_IN_USER_H
#error psm_context.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSM_CONTEXT_H
#define _PSM_CONTEXT_H

typedef
struct psmi_context {

	/* The following three member variables are used for sharing contexts among
	   subcontexts and they have the following common properties:

	   a. They are all initialized below HAL layer when the context is opened.
	   b. If they are NULL that means no context is being shared among subcontexts,
	   non-NULL means a context is being shared among some number of subcontexts.
	   c. The initialization code is currently found in the gen1 hal instance.
	*/

	void *spio_ctrl;
	void *tid_ctrl;
	void *tf_ctrl;

	/* end of shared context member variables. */

	psmi_hal_hw_context psm_hw_ctxt;

	psm2_ep_t ep;		/* psm ep handle */
	psm2_epid_t epid;	/* psm integral ep id */
	psm2_error_t status_lasterr;
	time_t networkLostTime;
} psmi_context_t;

psm2_error_t
psmi_context_open(const psm2_ep_t ep, long unit_id, long port,
		  psm2_uuid_t const job_key,
		  int64_t timeout_ns, psmi_context_t *context);

psm2_error_t psmi_context_close(psmi_context_t *context);

/* Check status of context */
psm2_error_t psmi_context_check_status(const psmi_context_t *context);

psm2_error_t psmi_context_interrupt_set(psmi_context_t *context, int enable);
int psmi_context_interrupt_isenabled(psmi_context_t *context);

/*
 * round robin contexts across HFIs, then
 * ports; this is the default.
 * This option spreads the HFI selection within the local socket.
 * If it is preferred to spread job over over entire set of
 * HFIs within the system, see ALG_ACROSS_ALL below.
 */
#define PSMI_UNIT_SEL_ALG_ACROSS     PSM_HAL_ALG_ACROSS

#define PSMI_UNIT_SEL_ALG_ACROSS_ALL PSM_HAL_ALG_ACROSS_ALL

/*
 * use all contexts on an HFI (round robin
 * active ports within), then next HFI
 */
#define PSMI_UNIT_SEL_ALG_WITHIN     PSM_HAL_ALG_WITHIN

#endif /* PSM_CONTEXT_H */
