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

#ifndef _IPS_PROTO_INTERNAL_H
#define _IPS_PROTO_INTERNAL_H

#include "ips_expected_proto.h"
#include "ips_proto_help.h"
#include "ips_proto_connect.h"

#define INC_TIME_SPEND(timer)

// return 1 if seq is between first and last inclusive.  Accounts for possible
// wraparound where numerically first >= last
PSMI_INLINE(
int
between(int first_seq, int last_seq, int seq))
{
	if (last_seq >= first_seq) {
		if (seq < first_seq || seq > last_seq) {
			return 0;
		}
	} else {
		if (seq > last_seq && seq < first_seq) {
			return 0;
		}
	}
	return 1;
}

/*
 * Connect protocol.
 *
 * On receive, handled by upcalling into the connect interface.
 * On send, handled by ips_proto by having connect compose the message.
 */
psm2_error_t psm3_ips_proto_process_connect(struct ips_proto *proto,
				      uint8_t opcode,
				      struct ips_message_header *p_hdr,
				      void *payload, uint32_t paylen);
psm2_error_t psm3_ips_proto_timer_ack_callback(struct psmi_timer *, uint64_t);
psm2_error_t psm3_ips_proto_timer_send_callback(struct psmi_timer *, uint64_t);
psm2_error_t psm3_ips_proto_timer_ctrlq_callback(struct psmi_timer *, uint64_t);
psm2_error_t psm3_ips_proto_timer_pendq_callback(struct psmi_timer *, uint64_t);
void psm3_ips_proto_rv_scbavail_callback(struct ips_scbctrl *scbc, void *context);

psm2_error_t psm3_ips_proto_recv_init(struct ips_proto *proto);
psm2_error_t psm3_ips_proto_recv_fini(struct ips_proto *proto);

int psm3_ips_proto_process_err_chk(struct ips_recvhdrq_event *rcv_ev);
int psm3_ips_proto_connect_disconnect(struct ips_recvhdrq_event *rcv_ev);
int psm3_ips_proto_process_unknown_opcode(struct ips_recvhdrq_event *rcv_ev);

#endif /* _IPS_PROTO_INTERNAL_H */
