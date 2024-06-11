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
#include "psm2_hal.h"
#include "ips_proto.h"
#include "ips_expected_proto.h"
#include "ips_proto_help.h"

// the buffer mode for stdout is _IOLBF (line buffered) if it connects to an interactive device,
// otherwise the buffer mode is _IOFBF (fully buffered). When we pipe stdout to a file, we are
// using fully buffered mode, we need to call fflush to ensure proper print.
void psm3_ips_proto_dump_frame(void *frame, int length, char *message)
{
	uint8_t *raw_frame = frame;
	int counter, print_len = 0;
	char default_message[] = "<UNKNOWN>";
	char tmp[128] = {};

	if (!message)
		message = default_message;

	printf("\nHex dump of %i bytes at %p from %s\n", length, frame,
	       message);

	for (counter = 0; counter < length; counter++) {
		if ((counter % 16) == 0) {
			printf("%s\n", tmp);
			fflush(stdout);
			print_len = 0;
		}

		if ((counter % 4) == 0 && print_len < sizeof(tmp) - 1)
			print_len += snprintf(tmp + print_len, sizeof(tmp) - print_len, "   ");

		if (print_len < sizeof(tmp) - 1)
			print_len += snprintf(tmp + print_len, sizeof(tmp) - print_len, "%02X ", raw_frame[counter]);
	}
	printf("%s\n", tmp);
	fflush(stdout);
}

void psm3_ips_proto_dump_data(void *data, int data_length)
{
	int counter, print_len = 0;
	uint8_t *payload = (uint8_t *) data;
	char tmp[128] = {};

	printf("\nHex dump of data, length = %i\n", data_length);

	for (counter = 0; counter < data_length; counter++) {
		if ((counter % 16) == 0) {
			printf("%s\n", tmp);
			fflush(stdout);
			print_len = snprintf(tmp, sizeof(tmp), " %04d: ", counter);
		}

		if ((counter % 4) == 0 && print_len < sizeof(tmp) - 1)
			print_len += snprintf(tmp + print_len, sizeof(tmp) - print_len, "   ");

		if (print_len < sizeof(tmp) - 1)
			print_len += snprintf(tmp + print_len, sizeof(tmp) - print_len, "%02X ", payload[counter]);
	}
	printf("%s\n", tmp);
	fflush(stdout);
}

void psm3_ips_proto_show_header(struct ips_message_header *p_hdr, char *msg)
{
	psmi_seqnum_t ack_seq_num;

	printf("\nHeader decoding in hex: %s\n", msg ? msg : "");

	printf("LRH: VL4-LVer4-SL4-Res2-LNH2: %x\n",
	       __be16_to_cpu(p_hdr->lrh[0]));
	printf("LRH: DLID %x\n", __be16_to_cpu(p_hdr->lrh[1]));
	printf("LRH: Res4-PktLen12 %x\n", __be16_to_cpu(p_hdr->lrh[2]));
	printf("LRH: SLID %x\n", __be16_to_cpu(p_hdr->lrh[3]));

	printf("BTH: OpCode8-SE1-M1-PC2-TVer4-Pkey16 %x\n",
	       __be32_to_cpu(p_hdr->bth[0]));
	printf("BTH: Res24-Flow8 %x\n", __be32_to_cpu(p_hdr->bth[1]));
	printf("BTH: A1-PSN31 %x\n", __be32_to_cpu(p_hdr->bth[2]));

	printf("IPH: jkey-hcrc %x\n", __le32_to_cpu(p_hdr->khdr.kdeth1));
	printf("IPH: kver-sh-intr-tidctrl-tid-om-offset %x\n",
	       __le32_to_cpu(p_hdr->khdr.kdeth0));

	printf("opcode %x\n", _get_proto_hfi_opcode(p_hdr));

	ack_seq_num.psn_num = p_hdr->ack_seq_num;
		printf("ack_seq_num %x\n", ack_seq_num.psn_num);

	printf("src_rank/connidx %x\n", p_hdr->connidx);
	printf("flags %x\n", p_hdr->flags);
}
