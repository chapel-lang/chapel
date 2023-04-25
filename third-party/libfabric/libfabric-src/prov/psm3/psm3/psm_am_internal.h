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

#ifndef _PSM2_AM_INTERNAL_H
#define _PSM2_AM_INTERNAL_H

#define PSMI_AM_MAX_ARGS     10
#define PSMI_AM_NUM_HANDLERS 256	/* must be power of 2 */

#define PSMI_AM_ARGS_DEFAULT psm2_am_token_t token,			\
			     psm2_amarg_t *args, int nargs,		\
			     void *src, uint32_t len,			\
			     void *hctx

enum psm2_am_handler_version
{
	PSM2_AM_HANDLER_V1 = 0,
	PSM2_AM_HANDLER_V2,
};

struct psm2_ep_am_handle_entry
{
	void *hfn;
	void *hctx;
	enum psm2_am_handler_version version;
};

struct psmi_am_token {
	psm2_epaddr_t epaddr_incoming;
	uint32_t flags;
	/* Can handler reply? i.e. Not OPCODE_AM_REQUEST_NOREPLY request */
	uint32_t can_reply;

	/* PTLs may add other stuff here */
};

/* AM capabilities parameters are initialized once in psm3_am_init_internal
   and copied out in psm3_am_get_parameters.  When debugging is enabled,
   various assertions reference these parameters for sanity checking. */
extern struct psm2_am_parameters psm3_am_parameters;

PSMI_ALWAYS_INLINE(struct psm2_ep_am_handle_entry *
		   psm_am_get_handler_function(psm2_ep_t ep,
					       psm2_handler_t handler_idx))
{
	int hidx = handler_idx & (PSMI_AM_NUM_HANDLERS - 1);
	struct psm2_ep_am_handle_entry *hentry = &ep->am_htable[hidx];
	psmi_assert_always(hentry != NULL);
	return hentry;
}

/* PSM internal initialization */
psm2_error_t psm3_am_init_internal(psm2_ep_t ep);
void psm3_am_fini_internal(psm2_ep_t ep);

#endif
