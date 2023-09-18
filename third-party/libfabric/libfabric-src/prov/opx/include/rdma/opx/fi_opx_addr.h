/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 * Copyright (C) 2021 Cornelis Networks.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _FI_PROV_OPX_ADDR_H_
#define _FI_PROV_OPX_ADDR_H_

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>		/* only for fi_opx_addr_dump ... */

#include "rdma/fabric.h"	/* only for 'fi_addr_t' ... which is a typedef to uint64_t */



typedef uint32_t fi_opx_uid_t;

union fi_opx_uid {

	fi_opx_uid_t			fi;
	struct {
		uint16_t		endpoint_id;/* node-scoped endpoint identifier */
		uint16_t		lid;		/* fabric-scoped node identifier (big-endian) */
	} __attribute__((__packed__));
} __attribute__((__packed__));

static inline void
fi_opx_uid_dump (char * prefix, const union fi_opx_uid * const uid) {

	fprintf(stderr, "%s [%p]: %08x\n", prefix, uid, uid->fi);
	fprintf(stderr, "%s opx uid dump at %p (0x%08x)\n", prefix, (void*)uid, uid->fi);
	fprintf(stderr, "%s   .endpoint_id ......... %u (0x%04x)\n", prefix, uid->endpoint_id, uid->endpoint_id);
	fprintf(stderr, "%s   .lid ............. %u (0x%04x)\n", prefix, uid->lid, uid->lid);

	fflush(stderr);
}

#define FI_OPX_UID_DUMP(uid)							\
({										\
	char prefix[1024];							\
	snprintf(prefix, 1023, "%s:%s():%d", __FILE__, __func__, __LINE__);	\
	fi_opx_uid_dump(prefix, (uid));					\
})

union fi_opx_addr {
	fi_addr_t			fi;
	uint64_t			raw64b;
	uint32_t			raw32b[2];
	uint8_t				raw8b[8];
	struct {
		uint8_t			hfi1_rx;
		uint8_t			hfi1_unit;
		uint8_t			reliability_rx;	/* hfi1 rx id of reliability service */
		union fi_opx_uid	uid;
		uint8_t		    rx_index;
	} __attribute__((__packed__));
} __attribute__((__packed__));

struct fi_opx_extended_addr {
	union fi_opx_addr	addr;
	uint32_t			rank;
	uint32_t			rank_inst;
} __attribute__((__packed__));

extern union fi_opx_addr opx_default_addr;

static inline void
fi_opx_addr_dump (char * prefix, const union fi_opx_addr * const addr) {

	fprintf(stderr, "%s [%p]: %08x %08x\n", prefix, addr, addr->raw32b[0], addr->raw32b[1]);
	fprintf(stderr, "%s opx addr dump at %p (0x%016lx)\n", prefix, (void*)addr, addr->raw64b);
	fprintf(stderr, "%s   .raw8b[8] = { %02x %02x %02x %02x  %02x %02x %02x %02x }\n", prefix, addr->raw8b[0], addr->raw8b[1], addr->raw8b[2], addr->raw8b[3], addr->raw8b[4], addr->raw8b[5], addr->raw8b[6], addr->raw8b[7]);

	fprintf(stderr, "%s   .hfi1_rx ....................................... %u\n", prefix, addr->hfi1_rx);
	fprintf(stderr, "%s   .hfi1_unit ..................................... %u\n", prefix, addr->hfi1_unit);
	fprintf(stderr, "%s   .reliability_rx ................................ %u\n", prefix, addr->reliability_rx);
	fprintf(stderr, "%s   .uid.endpoint_id ............................... %u\n", prefix, addr->uid.endpoint_id);
	fprintf(stderr, "%s   .uid.lid (big endian) .......................... %u (le: 0x%04hx, be: 0x%04hx)\n", prefix, addr->uid.lid, ntohs(addr->uid.lid), addr->uid.lid);
	fprintf(stderr, "%s   .rx_index ...................................... %u\n", prefix, addr->rx_index);

	fflush(stderr);
}

#define FI_OPX_ADDR_DUMP(addr)						\
({										\
	char prefix[1024];							\
	snprintf(prefix, 1023, "%s:%s():%d", __FILE__, __func__, __LINE__);	\
	fi_opx_addr_dump(prefix, (addr));					\
})

#define FI_OPX_ADDR_TO_HFI1_LRH_DLID(fi_addr)					\
	((fi_addr & 0x00FFFF0000000000ul) >> 24)


#define FI_OPX_HFI1_LRH_DLID_TO_LID(hfi1_lrh_dlid)				\
	(hfi1_lrh_dlid >> 16)

#endif /* _FI_PROV_OPX_ADDR_H_ */
