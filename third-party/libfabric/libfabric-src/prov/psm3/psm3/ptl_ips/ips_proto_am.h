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

#ifndef _IPS_PROTO_AM_H
#define _IPS_PROTO_AM_H

#include "psm_user.h"
#include "ips_scb.h"

struct ips_proto_am {
	struct ips_proto *proto;	/* back pointer */
	struct ips_scbctrl scbc_request;
	struct ips_scbctrl scbc_reply;
};

psm2_error_t
psm3_ips_am_get_parameters(psm2_ep_t ep, struct psm2_am_parameters *parameters);

psm2_error_t
psm3_ips_am_short_reply(psm2_am_token_t tok,
		   psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		   void *src, size_t len, int flags,
		   psm2_am_completion_fn_t completion_fn, void *completion_ctxt);

psm2_error_t
psm3_ips_am_short_request(psm2_epaddr_t epaddr,
		     psm2_handler_t handler, psm2_amarg_t *args, int nargs,
		     void *src, size_t len, int flags,
		     psm2_am_completion_fn_t completion_fn,
		     void *completion_ctxt);

psm2_error_t
MOCKABLE(psm3_ips_proto_am_init)(struct ips_proto *proto,
             int num_send_slots,
             uint32_t imm_size,
             struct ips_proto_am *proto_am);
MOCK_DCL_EPILOGUE(psm3_ips_proto_am_init);

psm2_error_t psm3_ips_proto_am_fini(struct ips_proto_am *proto_am);

#endif /* _IPS_PROTO_AM_H */
