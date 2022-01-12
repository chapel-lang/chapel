/*
 * Copyright (C) 2016 by Argonne National Laboratory.
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
#ifndef _FI_PROV_BGQ_MU_H_
#define _FI_PROV_BGQ_MU_H_

#include <stdlib.h>
#include <stdio.h>

#include "rdma/bgq/fi_bgq_hwi.h"
#include "rdma/bgq/fi_bgq_spi.h"

#include "rdma/fi_errno.h"	// only for FI_* errno return codes
#include "rdma/fabric.h" // only for 'fi_addr_t' ... which is a typedef to uint64_t

#include "rdma/bgq/fi_bgq_l2atomic.h"

#define FI_BGQ_MU_RECFIFO_BYTES		(0x01 << 20)	/* 1 MB == 32K entries */
#define FI_BGQ_MU_RECFIFO_TAGGED_BYTES	(0x01 << 20)	/* 1 MB == 32K entries */
#define FI_BGQ_MU_RECFIFO_OTHER_BYTES	(0x01 << 15)	/* 32 KB == 1K entries */

#define FI_BGQ_MU_BAT_SUBGROUP_GLOBAL (65)
#define FI_BGQ_MU_BAT_ID_GLOBAL (FI_BGQ_MU_BAT_SUBGROUP_GLOBAL * BGQ_MU_NUM_DATA_COUNTERS_PER_SUBGROUP)
#define FI_BGQ_MU_BAT_ID_COUNTER (FI_BGQ_MU_BAT_ID_GLOBAL+1)
#define FI_BGQ_MU_BAT_ID_ZERO (FI_BGQ_MU_BAT_ID_COUNTER+1)
#define FI_BGQ_MU_BAT_ID_ONE (FI_BGQ_MU_BAT_ID_ZERO+1)
#define FI_BGQ_MU_BAT_ID_BLACKHOLE (FI_BGQ_MU_BAT_ID_ONE+1)

#define FI_BGQ_MUHWI_DESTINATION_MASK (0x073CF3C1ul)

// #define FI_BGQ_TRACE 1


typedef uint32_t		fi_bgq_uid_t;

union fi_bgq_uid {
	fi_bgq_uid_t		fi;
	uint32_t		raw32b;
	uint16_t		raw16b[2];
	uint8_t			raw8b[4];
	MUHWI_Destination_t	muhwi;		/* see fi_bgq_uid_get_destination() */
	struct {
		uint32_t	rx_msb	:  4;	/* see fi_bgq_uid_get_rx(); see NOTE_MU_RECFIFO */
		uint32_t	a	:  4;	/* 4 bits needed for the A dimention of the torus on Mira and Sequoia */
		uint32_t	unused_1:  2;
		uint32_t	b	:  4;	/* 4 bits needed for the B dimention of the torus on Mira and Sequoia */
		uint32_t	unused_2:  2;
		uint32_t	c	:  4;	/* 4 bits needed for the C dimention of the torus on Mira and Sequoia */
		uint32_t	unused_3:  2;
		uint32_t	d	:  4;	/* 4 bits needed for the D dimention of the torus on Mira and Sequoia */
		uint32_t	rx_lsb	:  5;	/* see fi_bgq_uid_get_rx(); see NOTE_MU_RECFIFO */
		uint32_t	e	:  1;	/* 1 bit needed for the E dimention of the torus on all BG/Q systems */
	} __attribute__((__packed__));
} __attribute__((__packed__));

static inline void
fi_bgq_uid_dump (char * prefix, const fi_bgq_uid_t * const uid) {

	const union fi_bgq_uid tmp = {.fi=*uid};
	uint32_t * ptr = (uint32_t *)uid;
	fprintf(stderr, "%s [%p]: %08x\n", prefix, ptr, *(ptr));
	fprintf(stderr, "%s fi_bgq_uid_t dump at %p\n", prefix, (void*)uid);

	fprintf(stderr, "%s   .rx_msb .................................... %u\n", prefix, tmp.rx_msb);

	fprintf(stderr, "%s   .a ......................................... %u\n", prefix, tmp.a);
	fprintf(stderr, "%s   .unused_1 .................................. %u\n", prefix, tmp.unused_1);
	fprintf(stderr, "%s   .b ......................................... %u\n", prefix, tmp.b);
	fprintf(stderr, "%s   .unused_2 .................................. %u\n", prefix, tmp.unused_2);
	fprintf(stderr, "%s   .c ......................................... %u\n", prefix, tmp.c);
	fprintf(stderr, "%s   .unused_3 .................................. %u\n", prefix, tmp.unused_3);
	fprintf(stderr, "%s   .d ......................................... %u\n", prefix, tmp.d);
	fprintf(stderr, "%s   .rx_lsb .................................... %u\n", prefix, tmp.rx_lsb);
	fprintf(stderr, "%s   .e ......................................... %u\n", prefix, tmp.e);

	fflush(stderr);
}

#define FI_BGQ_UID_DUMP(uid)							\
({										\
	char prefix[1024];							\
	snprintf(prefix, 1023, "%s:%s():%d", __FILE__, __func__, __LINE__);	\
	fi_bgq_uid_dump(prefix, (uid));						\
})

static inline
fi_bgq_uid_t fi_bgq_uid_set_rx (const fi_bgq_uid_t uid, const uint32_t rx) {
	return (uid & 0x0FFFFFC1u) |		/* clear rx_msb and rx_lsb */
		((rx << 23) & 0xF0000000u) |	/* set rx_msb */
		((rx << 1) & 0x0000003Eu);	/* set rx_lsb */
}

static inline
uint32_t fi_bgq_uid_get_rx (const fi_bgq_uid_t uid) {
	return ((uid & 0xF0000000u) >> 23) | ((uid & 0x0000003Eu) >> 1);
}

static inline
fi_bgq_uid_t fi_bgq_uid_set_destination (const fi_bgq_uid_t uid, const MUHWI_Destination_t destination) {
	const union fi_bgq_uid tmp = {.muhwi=destination};
	return (uid & 0xF0C30C3Eu) | tmp.fi;	/* clear torus fields (a,b,c,d,e); then set */
}

static inline
MUHWI_Destination_t fi_bgq_uid_get_destination (const fi_bgq_uid_t uid) {
	/* clear all bits except the torus coordinates */
	const union fi_bgq_uid tmp = {.fi=(uid & 0x0F3CF3C1ul)};
	return tmp.muhwi;
}

static inline
fi_bgq_uid_t fi_bgq_uid_create (const MUHWI_Destination_t destination, const uint32_t rx) {
	const union fi_bgq_uid tmp = {.muhwi=destination};
	return fi_bgq_uid_set_rx(tmp.fi, rx);
}


union fi_bgq_addr {
	fi_addr_t			fi;
	uint64_t			raw64b;
	struct {
		union fi_bgq_uid	uid;
		uint16_t		unused_0;
		uint16_t		fifo_map;	/* only the 12 msb are used */
	} __attribute__((__packed__));
} __attribute__((__packed__));

static inline void
fi_bgq_addr_dump (char * prefix, fi_addr_t * addr) {

	const union fi_bgq_addr tmp = {.fi=*addr};
	uint32_t * ptr = (uint32_t *)addr;
	fprintf(stderr, "%s [%p]: %08x %08x\n", prefix, ptr, *(ptr), *(ptr+1));
	fprintf(stderr, "%s bgq addr dump at %p\n", prefix, (void*)addr);

	fprintf(stderr, "%s   .uid.rx_msb .................................... %u\n", prefix, tmp.uid.rx_msb);
	fprintf(stderr, "%s   .uid.a ......................................... %u\n", prefix, tmp.uid.a);
	fprintf(stderr, "%s   .uid.unused_1 .................................. %u\n", prefix, tmp.uid.unused_1);
	fprintf(stderr, "%s   .uid.b ......................................... %u\n", prefix, tmp.uid.b);
	fprintf(stderr, "%s   .uid.unused_2 .................................. %u\n", prefix, tmp.uid.unused_2);
	fprintf(stderr, "%s   .uid.c ......................................... %u\n", prefix, tmp.uid.c);
	fprintf(stderr, "%s   .uid.unused_3 .................................. %u\n", prefix, tmp.uid.unused_3);
	fprintf(stderr, "%s   .uid.d ......................................... %u\n", prefix, tmp.uid.d);
	fprintf(stderr, "%s   .uid.rx_lsb .................................... %u\n", prefix, tmp.uid.rx_lsb);
	fprintf(stderr, "%s   .uid.e ......................................... %u\n", prefix, tmp.uid.e);

	fprintf(stderr, "%s   .unused_0 ................................... %u\n", prefix, tmp.unused_0);
	fprintf(stderr, "%s   .fifo_map ................................... %u\n", prefix, tmp.fifo_map);

	fflush(stderr);
}

#define FI_BGQ_ADDR_DUMP(addr)							\
({										\
	char prefix[1024];							\
	snprintf(prefix, 1023, "%s:%s():%d", __FILE__, __func__, __LINE__);	\
	fi_bgq_addr_dump(prefix, (addr));					\
})

static inline
fi_bgq_uid_t fi_bgq_addr_uid (const fi_addr_t addr) {
	return ((union fi_bgq_addr*)&addr)->uid.fi;
}


static inline
uint64_t fi_bgq_addr_rec_fifo_id (const fi_addr_t addr) {
	return (uint64_t) fi_bgq_uid_get_rx(fi_bgq_addr_uid(addr));
}

static inline
uint64_t fi_bgq_addr_get_fifo_map (const fi_addr_t addr) {
	return addr & 0x000000000000FFFFu;
}

static inline
uint64_t fi_bgq_addr_is_local (const fi_addr_t addr) {
	return (addr & (MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0 | MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1)) != 0;
}


static inline
uint32_t fi_bgq_addr_calculate_base_rx (const uint32_t process_id, const uint32_t processes_per_node) {

	/* only one domain per process is currently supported */
	const uint32_t domain_id = 0;
	const uint32_t domains_per_process = 1;

	/* only one endpoint per domain is currently supported */
	const uint32_t endpoint_id = 0;
	const uint32_t endpoints_per_domain = 1;

	/* each rx uses one mu reception fifo; See NOTE_MU_RECFIFO */
	const uint32_t rx_per_node =
		((BGQ_MU_NUM_REC_FIFO_GROUPS-1) * BGQ_MU_NUM_REC_FIFOS_PER_GROUP);

	const uint32_t rx_per_process = rx_per_node / processes_per_node;
	const uint32_t rx_per_domain = rx_per_process / domains_per_process;
	const uint32_t rx_per_endpoint = rx_per_domain / endpoints_per_domain;

	return (rx_per_process * process_id) + (rx_per_domain * domain_id) + (rx_per_endpoint * endpoint_id);
}

static inline
fi_addr_t fi_bgq_addr_create (const MUHWI_Destination_t destination,
	const uint64_t fifo_map, const uint32_t rx) {

	union fi_bgq_addr tmp;
	tmp.uid = (union fi_bgq_uid) fi_bgq_uid_create(destination, rx);
	tmp.unused_0=0;
	tmp.fifo_map=fifo_map;
	return tmp.fi;
}



#define FI_BGQ_MU_PACKET_TYPE_TAG			(0x01ul<<1)
#define FI_BGQ_MU_PACKET_TYPE_UNUSED			(0x01ul<<2)
#define FI_BGQ_MU_PACKET_TYPE_EAGER			(0x01ul<<3)
#define FI_BGQ_MU_PACKET_TYPE_RENDEZVOUS		(0x01ul<<4)
#define FI_BGQ_MU_PACKET_TYPE_RMA			(0x01ul<<5)
#define FI_BGQ_MU_PACKET_TYPE_ATOMIC			(0x01ul<<6)
#define FI_BGQ_MU_PACKET_TYPE_ACK			(0x01ul<<7)

/**
 * \brief MU packet header
 *
 * The MU packet header is consumed in many places and sometimes overloaded
 * for cache and memory allocation reasons.
 */
union fi_bgq_mu_packet_hdr {

	/* The torus packet header is 32 bytes. see: hwi/include/bqc/MU_PacketHeader.h */
	MUHWI_PacketHeader_t		muhwi;

	struct {
		/* The point-to-point header occupies bytes 0-11 of the packet header
		 * see: MUHWI_Pt2PtNetworkHeader_t in hwi/include/bqc/MU_Pt2PtNetworkHeader.h */
		uint64_t		reserved_0;
		uint32_t		reserved_1;

		/* The message unit header occupies bytes 12-31 of the packet header
		 * see: MUHWI_MessageUnitHeader_t in hwi/include/bqc/MU_MessageUnitHeader.h */
		uint16_t		reserved_2	: 10;
		uint16_t		unused_0	:  6;
		uint8_t			unused_1[18];
	} __attribute__((__packed__)) raw;

	struct {
		uint64_t		reserved_0;
		uint32_t		reserved_1;
		uint16_t		reserved_2	: 10;
		uint16_t		unused_0	:  6;

		uint8_t			unused_1;
		uint8_t			packet_type;		/* FI_BGQ_MU_PACKET_TYPE_*; all 8 bits are needed */
		uint64_t		unused_2[2];
	} __attribute__((__packed__)) common;

	struct {
		uint64_t		reserved_0;
		uint64_t		reserved_1	: 32;
		uint64_t		reserved_2	: 10;
		uint64_t		unused_0	: 14;
		uint64_t		reserved_3	:  8;	/* a.k.a. common::packet_type */

		MUHWI_Destination_t	origin;
		uint32_t		cntr_paddr_rsh3b;	/* 34b paddr, 8 byte aligned; See: NOTE_MU_PADDR */
		uint64_t		is_local;		/* only 1 bit is needed */
	} __attribute__((__packed__)) completion;

	struct {
		uint64_t		reserved_0;
		uint32_t		reserved_1;

		union {
			struct {
				uint32_t		reserved_2	: 10;
				uint32_t		is_local	:  1;	/* used to specify fifo map; only needed for FI_BGQ_REMOTE_COMPLETION */
				uint32_t		unused_0	:  3;
				uint32_t		message_length	: 10;	/* 0..512 bytes of payload data */
				uint32_t		reserved_3	:  8;	/* a.k.a. common::packet_type */
			} __attribute__((__packed__)) send;

			struct {
				uint16_t		reserved_2	: 10;
				uint16_t		is_local	:  1;	/* used to specify fifo map */
				uint16_t		niov_minus_1	:  5;	/* 1..31 mu iov elements in payload data */
				uint8_t			rget_inj_fifo_id;	/* 0..255 */
				uint8_t			reserved_3;		/* a.k.a. common::packet_type */
			} __attribute__((__packed__)) rendezvous;
		};

		union fi_bgq_uid	uid;
		uint32_t		immediate_data;
		uint64_t		ofi_tag;

	} __attribute__((__packed__)) pt2pt;

	struct {
		uint64_t		reserved_0;
		uint32_t		reserved_1;
		uint16_t		reserved_2	: 10;
		uint16_t		unused_0	:  6;

		uint8_t			unused_1;
		uint8_t			reserved_3;		/* a.k.a. common::packet_type (FI_BGQ_MU_PACKET_TYPE_ACK) */
		uint64_t		unused_2;
		uintptr_t		context;
	} __attribute__((__packed__)) ack;

	struct {
		uint64_t		reserved_0;
		uint32_t		reserved_1;
		uint16_t		reserved_2	: 10;
		uint16_t		unused_0	:  6;

		uint8_t			ndesc;			/* 0..8 descriptors */
		uint8_t			reserved_3;		/* a.k.a. common::packet_type (FI_BGQ_MU_PACKET_TYPE_RMA) */
		uint64_t		nbytes		: 16;	/* 0..512 bytes */
		uint64_t		unused_2	: 11;
		uint64_t		offset		: 37;	/* FI_MR_BASIC uses virtual address as the offset */
		uint64_t		key;			/* only 16 bits needed for FI_MR_SCALABLE but need up to 34 for FI_MR_BASIC vaddr-paddr delta */
	} __attribute__((__packed__)) rma;

	struct {
		uint64_t		reserved_0;
		uint32_t		reserved_1;
		uint32_t		reserved_2	: 10;
		uint32_t		unused_0	:  5;
		uint32_t		cntr_bat_id	:  9;
		uint32_t		reserved_3	:  8;	/* a.k.a. common::packet_type (FI_BGQ_MU_PACKET_TYPE_ATOMIC) */
		union {
			uint32_t		origin_raw;
			MUHWI_Destination_t	origin;
			struct {
				uint32_t	is_fetch	:  1;
				uint32_t	dt		:  4;	/* enum fi_datatype */
				uint32_t	a		:  3;	/* only 3 bits are needed for Mira */
				uint32_t	is_local	:  1;
				uint32_t	do_cntr		:  1;
				uint32_t	b		:  4;	/* only 4 bits are needed for Mira */
				uint32_t	unused_1	:  2;
				uint32_t	c		:  4;	/* only 4 bits are needed for Mira */
				uint32_t	unused_2	:  2;
				uint32_t	d		:  4;	/* only 4 bits are needed for Mira */
				uint32_t	op		:  5;	/* enum fi_op */
				uint32_t	e		:  1;	/* only 1 bit is needed for Mira */
			} __attribute__((__packed__));
		};
		uint16_t		nbytes_minus_1;			/* only 9 bits needed */
		uint16_t		key;				/* only 16 bits needed for FI_MR_SCALABLE and not used for FI_MR_BASIC */
		uint64_t		offset;				/* FI_MR_BASIC needs 34 bits */
	} __attribute__((__packed__)) atomic;

} __attribute__((__aligned__(32)));

struct fi_bgq_mu_iov {
	uint64_t			message_length;
	uint64_t			src_paddr;
};

struct fi_bgq_mu_fetch_metadata {
	uint64_t			dst_paddr;
	uint64_t			cq_paddr;
	uint64_t			fifo_map;
	uint64_t			unused;
};

union fi_bgq_mu_packet_payload {
	uint8_t				byte[512];
	struct {
		uint32_t		unused;
		uint32_t		cntr_paddr_rsh3b;	/* 34b paddr, 8 byte aligned; See: NOTE_MU_PADDR */
		uint64_t		fifo_map;
		struct fi_bgq_mu_iov	mu_iov[31];
	} rendezvous;
	struct {
		struct fi_bgq_mu_fetch_metadata	metadata;
		uint8_t				data[512-sizeof(struct fi_bgq_mu_fetch_metadata)];
	} atomic_fetch;
} __attribute__((__aligned__(32)));

struct fi_bgq_mu_packet {
	union {
		struct fi_bgq_mu_packet		*next;	/* first 8 bytes of the header is unused */
		union fi_bgq_mu_packet_hdr	hdr;
	};
	union fi_bgq_mu_packet_payload		payload;
} __attribute__((__aligned__(32)));


static inline uint64_t
fi_bgq_mu_packet_type_get (struct fi_bgq_mu_packet * pkt) {
	return pkt->hdr.common.packet_type;
}

static inline void
fi_bgq_mu_packet_type_set (union fi_bgq_mu_packet_hdr * hdr, const uint64_t packet_type) {
	hdr->common.packet_type = (uint8_t)packet_type;
}

static inline uint64_t
fi_bgq_mu_packet_get_fifo_map (struct fi_bgq_mu_packet * pkt) {
	return pkt->payload.rendezvous.fifo_map;
}

static inline void
fi_bgq_mu_packet_set_fifo_map (struct fi_bgq_mu_packet * pkt, const uint64_t fifo_map) {
	pkt->payload.rendezvous.fifo_map = fifo_map;
	return;
}

#define FI_BGQ_MU_DESCRIPTOR_UPDATE_BAT_TYPE_NONE	(0)
#define FI_BGQ_MU_DESCRIPTOR_UPDATE_BAT_TYPE_DST	(1)
#define FI_BGQ_MU_DESCRIPTOR_UPDATE_BAT_TYPE_SRC	(2)

union fi_bgq_mu_descriptor {

	/* The mu descriptor is 64 bytes. see: hwi/include/bqc/MU_Descriptor.h */
	MUHWI_Descriptor_t				muhwi_descriptor;

	struct {
		uint16_t				key_msb;
		uint8_t					update_type;		/* FI_BGQ_MU_DESCRIPTOR_UPDATE_BAT_TYPE_* */
		uint8_t					unused_0	:  7;
		uint8_t					reserved_0	:  1;

		uint32_t				unused_1	: 31;
		uint32_t				reserved_1	:  1;

		uint64_t				Pa_Payload;		/* 37 lsb are used */
		uint64_t				Message_Length;		/* 37 lsb (really, 35) are used */
		uint64_t				key_lsb		: 48;
		uint64_t				reserved_2	: 16;	/* a.k.a. Torus_FIFO_Map */

		union {
			MUHWI_Pt2PtNetworkHeader_t	muhwi_pt2ptnetworkheader;
			uint32_t			reserved_3[3];
		};

		union {
			MUHWI_MessageUnitHeader_t	muhwi_messageunitheader;
			struct {
				uint64_t		rec_payload_base_address_id	: 10;
				uint64_t		reserved_4			:  1;
				uint64_t		put_offset			: 37;
				uint64_t		unused_2			:  6;
				uint64_t		rec_counter_base_address_id	: 10;
				uint32_t		reserved_5[3];
			} __attribute__((__packed__)) dput;
		};

	} __attribute__((__packed__)) rma;

} __attribute__((__aligned__(64)));


static inline void
dump_descriptor (char * prefix, MUHWI_Descriptor_t * desc) {

	uint32_t * ptr = (uint32_t *)desc;
	fprintf(stderr, "%s [%p]: %08x %08x %08x %08x\n", prefix, ptr, *(ptr), *(ptr+1), *(ptr+2), *(ptr+3)); ptr+=4;
	fprintf(stderr, "%s [%p]: %08x %08x %08x %08x\n", prefix, ptr, *(ptr), *(ptr+1), *(ptr+2), *(ptr+3)); ptr+=4;
	fprintf(stderr, "%s [%p]: %08x %08x %08x %08x\n", prefix, ptr, *(ptr), *(ptr+1), *(ptr+2), *(ptr+3)); ptr+=4;
	fprintf(stderr, "%s [%p]: %08x %08x %08x %08x\n", prefix, ptr, *(ptr), *(ptr+1), *(ptr+2), *(ptr+3)); ptr+=4;

	fprintf(stderr, "%s descriptor dump at %p\n", prefix, (void*)desc);
	fprintf(stderr, "%s   .Half_Word0.Prefetch_Only .................. %d\n", prefix, desc->Half_Word0.Prefetch_Only);
	fprintf(stderr, "%s   .Half_Word1.Interrupt ...................... %d\n", prefix, desc->Half_Word1.Interrupt);
	fprintf(stderr, "%s   .Pa_Payload ................................ 0x%016lx\n", prefix, desc->Pa_Payload);
	fprintf(stderr, "%s   .Message_Length ............................ %lu\n", prefix, desc->Message_Length);
	fprintf(stderr, "%s   .Torus_FIFO_Map ............................ 0x%016lx\n", prefix, desc->Torus_FIFO_Map);
	fprintf(stderr, "%s   .PacketHeader.NetworkHeader.pt2pt\n", prefix);
	fprintf(stderr, "%s     .Data_Packet_Type ........................ 0x%02x\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Data_Packet_Type);
	fprintf(stderr, "%s     .Hints ................................... 0x%02x\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Hints);
	fprintf(stderr, "%s     .Byte2.Hint_E_plus ....................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_plus);
	fprintf(stderr, "%s     .Byte2.Hint_E_minus ...................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte2.Hint_E_minus);
	fprintf(stderr, "%s     .Byte2.Route_To_IO_Node .................. %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte2.Route_To_IO_Node);
	fprintf(stderr, "%s     .Byte2.Return_From_IO_Node ............... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte2.Return_From_IO_Node);
	fprintf(stderr, "%s     .Byte2.Dynamic ........................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte2.Dynamic);
	fprintf(stderr, "%s     .Byte2.Deposit ........................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte2.Deposit);
	fprintf(stderr, "%s     .Byte2.Interrupt ......................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte2.Interrupt);
	fprintf(stderr, "%s     .Byte3.Virtual_channel ................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Virtual_channel);
	fprintf(stderr, "%s     .Byte3.Zone_Routing_Id ................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Zone_Routing_Id);
	fprintf(stderr, "%s     .Byte3.Stay_On_Bubble .................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte3.Stay_On_Bubble);
	fprintf(stderr, "%s     .Destination.Destination.Reserved2 ....... %u\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.Reserved2);
	fprintf(stderr, "%s     .Destination.Destination.A_Destination ... %u\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.A_Destination);
	fprintf(stderr, "%s     .Destination.Destination.B_Destination ... %u\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.B_Destination);
	fprintf(stderr, "%s     .Destination.Destination.C_Destination ... %u\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.C_Destination);
	fprintf(stderr, "%s     .Destination.Destination.D_Destination ... %u\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.D_Destination);
	fprintf(stderr, "%s     .Destination.Destination.E_Destination ... %u\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Destination.Destination.E_Destination);
	fprintf(stderr, "%s     .Byte8.Packet_Type ....................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Packet_Type);
	fprintf(stderr, "%s     .Byte8.Reserved3 ......................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Reserved3);
	fprintf(stderr, "%s     .Byte8.Size .............................. %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Size);
	fprintf(stderr, "%s     .Injection_Info.Reserved4 ................ %hu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Injection_Info.Reserved4);
	fprintf(stderr, "%s     .Injection_Info.Skip ..................... %hhu\n", prefix, desc->PacketHeader.NetworkHeader.pt2pt.Injection_Info.Skip);
	if (desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Packet_Type == 0) {
		fprintf(stderr, "%s   .PacketHeader.messageUnitHeader.Packet_Types\n", prefix);
		fprintf(stderr, "%s     .Memory_FIFO.Reserved1 ................... %hu\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Reserved1);
		fprintf(stderr, "%s     .Memory_FIFO.Rec_FIFO_Id ................. %hu\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Rec_FIFO_Id);
		fprintf(stderr, "%s     .Memory_FIFO.Unused1 ..................... %hu\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Unused1);
		fprintf(stderr, "%s     .Memory_FIFO.Put_Offset_MSB .............. 0x%08hx\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Put_Offset_MSB);
		fprintf(stderr, "%s     .Memory_FIFO.Put_Offset_LSB .............. 0x%08x\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Memory_FIFO.Put_Offset_LSB);
	} else if (desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Packet_Type == 1) {
		fprintf(stderr, "%s   .PacketHeader.messageUnitHeader.Packet_Types\n", prefix);
		fprintf(stderr, "%s     .Direct_Put.Rec_Payload_Base_Address_Id .. %hu\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Payload_Base_Address_Id);
		fprintf(stderr, "%s     .Direct_Put.Pacing ....................... %hu\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Pacing);
		fprintf(stderr, "%s     .Direct_Put.Put_Offset_MSB ............... 0x%08hx\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_MSB);
		fprintf(stderr, "%s     .Direct_Put.Put_Offset_LSB ............... 0x%08x\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Put_Offset_LSB);
		fprintf(stderr, "%s     .Direct_Put.Unused1 ...................... %hu\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Unused1);
		fprintf(stderr, "%s     .Direct_Put.Rec_Counter_Base_Address_Id .. %hu\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Rec_Counter_Base_Address_Id);
		fprintf(stderr, "%s     .Direct_Put.Counter_Offset ............... 0x%016lx\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Direct_Put.Counter_Offset);
	} else if (desc->PacketHeader.NetworkHeader.pt2pt.Byte8.Packet_Type == 2) {
		fprintf(stderr, "%s   .PacketHeader.messageUnitHeader.Packet_Types\n", prefix);
		fprintf(stderr, "%s     .Remote_Get.Rget_Inj_FIFO_Id ............. %hu\n", prefix, desc->PacketHeader.messageUnitHeader.Packet_Types.Remote_Get.Rget_Inj_FIFO_Id);
	}
	fflush(stderr);
}

#define DUMP_DESCRIPTOR(desc)							\
({										\
	char prefix[1024];							\
	snprintf(prefix, 1023, "%s:%s():%d", __FILE__, __func__, __LINE__);	\
	dump_descriptor(prefix, (desc));					\
})



#define FI_BGQ_MU_TORUS_INJFIFO_COUNT (10)
#define FI_BGQ_MU_LOCAL_INJFIFO_COUNT (6)




/* expensive .. not for critical path! */
static inline
uint32_t fi_bgq_mu_calculate_fifo_map(BG_CoordinateMapping_t local,
		BG_CoordinateMapping_t remote, Personality_t * personality,
		uint64_t dcr_value) {

	/* calculate the signed coordinate difference between the source and
	 * destination torus coordinates
	 */
	ssize_t dA = (ssize_t)remote.a - (ssize_t)local.a;
	ssize_t dB = (ssize_t)remote.b - (ssize_t)local.b;
	ssize_t dC = (ssize_t)remote.c - (ssize_t)local.c;
	ssize_t dD = (ssize_t)remote.d - (ssize_t)local.d;
	ssize_t dE = (ssize_t)remote.e - (ssize_t)local.e;

	/* select the fifo based on the t coordinate only if local */
	if ((dA | dB | dC | dD | dE) == 0) {
		return (remote.t & 0x01) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL0 : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_LOCAL1;
	}

	/* select either A- or A+ if communicating only along the A dimension */
	if ((dB | dC | dD | dE) == 0) {
		if (ND_ENABLE_TORUS_DIM_A & personality->Network_Config.NetFlags) {
			uint64_t cutoff;
			if (dA > 0) {
				cutoff = ND_500_DCR__CTRL_CUTOFFS__A_PLUS_get(dcr_value);
				return (remote.a > cutoff) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP;
			} else {
				cutoff = ND_500_DCR__CTRL_CUTOFFS__A_MINUS_get(dcr_value);
				return (remote.a < cutoff) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM;
			}
		} else {
			return (dA > 0) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM;
		}
	}

	/* select either B- or B+ if communicating only along the B dimension */
	if ((dA | dC | dD | dE) == 0) {
		if (ND_ENABLE_TORUS_DIM_B & personality->Network_Config.NetFlags) {
			uint64_t cutoff;
			if (dB > 0) {
				cutoff = ND_500_DCR__CTRL_CUTOFFS__B_PLUS_get(dcr_value);
				return (remote.b > cutoff) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP;
			} else {
				cutoff = ND_500_DCR__CTRL_CUTOFFS__B_MINUS_get(dcr_value);
				return (remote.b < cutoff) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM;
			}
		} else {
			return (dB > 0) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM;
		}
	}

	/* select either C- or C+ if communicating only along the C dimension */
	if ((dA | dB | dD | dE) == 0) {
		if (ND_ENABLE_TORUS_DIM_C & personality->Network_Config.NetFlags) {
			uint64_t cutoff;
			if (dC > 0) {
				cutoff = ND_500_DCR__CTRL_CUTOFFS__C_PLUS_get(dcr_value);
				return (remote.c > cutoff) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP;
			} else {
				cutoff = ND_500_DCR__CTRL_CUTOFFS__C_MINUS_get(dcr_value);
				return (remote.c < cutoff) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM;
			}
		} else {
			return (dC > 0) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM;
		}
	}

	/* select either D- or D+ if communicating only along the D dimension */
	if ((dA | dB | dC | dE) == 0) {
		if (ND_ENABLE_TORUS_DIM_D & personality->Network_Config.NetFlags) {
			uint64_t cutoff;
			if (dD > 0) {
				cutoff = ND_500_DCR__CTRL_CUTOFFS__D_PLUS_get(dcr_value);
				return (remote.d > cutoff) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP;
			} else {
				cutoff = ND_500_DCR__CTRL_CUTOFFS__D_MINUS_get(dcr_value);
				return (remote.d < cutoff) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM;
			}
		} else {
			return (dD > 0) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM;
		}
	}

	/* select either E- or E+ if communicating only along the E dimension */
	if ((dA | dB | dC | dD) == 0) {
		/* the maximum 'e' dimension size is 2 - and is a torus */
		return (remote.t & 0x01) ? MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EP : MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_EM;
	}

	/* communicating along diagonal */
	/* TODO - OPTIMIZE - round-robin the fifo picking based on destination */
	if (dA > 0) {
		return MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AP;
	} else if (dA < 0)
		return MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_AM;

	if (dB > 0) {
		return MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BP;
	} else if (dB < 0)
		return MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_BM;

	if (dC > 0) {
		return MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CP;
	} else if (dC < 0)
		return MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_CM;

	if (dD > 0) {
		return MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DP;
	} else if(dD < 0)
		return MUHWI_DESCRIPTOR_TORUS_FIFO_MAP_DM;

	assert(0);
	return 0xFFFFu;
}

static inline
uint32_t fi_bgq_mu_calculate_fifo_map_single (BG_CoordinateMapping_t local, BG_CoordinateMapping_t remote) {

	Personality_t personality;
	int rc = Kernel_GetPersonality(&personality, sizeof(Personality_t));
	if (rc) return 0;	/* error!? */

	uint64_t dcr_value = DCRReadUser(ND_500_DCR(CTRL_CUTOFFS));

	return fi_bgq_mu_calculate_fifo_map(local, remote, &personality, dcr_value);
}


#define FI_BGQ_DEBUG_MEMORY()					\
({								\
	fi_bgq_debug_memory(__FILE__, __func__, __LINE__);	\
})

static inline
void fi_bgq_debug_memory (char * file, const char * func, int line)
{
	uint64_t shared, persist, heapavail, stackavail, stack, heap, guard, mmap;

	Kernel_GetMemorySize(KERNEL_MEMSIZE_SHARED, &shared);
	Kernel_GetMemorySize(KERNEL_MEMSIZE_PERSIST, &persist);
	Kernel_GetMemorySize(KERNEL_MEMSIZE_HEAPAVAIL, &heapavail);
	Kernel_GetMemorySize(KERNEL_MEMSIZE_STACKAVAIL, &stackavail);
	Kernel_GetMemorySize(KERNEL_MEMSIZE_STACK, &stack);
	Kernel_GetMemorySize(KERNEL_MEMSIZE_HEAP, &heap);
	Kernel_GetMemorySize(KERNEL_MEMSIZE_GUARD, &guard);
	Kernel_GetMemorySize(KERNEL_MEMSIZE_MMAP, &mmap);

	fprintf(stderr, "%s:%s():%d Allocated heap: %.2f MB, avail. heap: %.2f MB\n", file, func, line, (double)heap/(1024*1024),(double)heapavail/(1024*1024));
	fprintf(stderr, "%s:%s():%d Allocated stack: %.2f MB, avail. stack: %.2f MB\n", file, func, line, (double)stack/(1024*1024), (double)stackavail/(1024*1024));
	fprintf(stderr, "%s:%s():%d Memory: shared: %.2f MB, persist: %.2f MB, guard: %.2f MB, mmap: %.2f MB\n", file, func, line, (double)shared/(1024*1024), (double)persist/(1024*1024), (double)guard/(1024*1024), (double)mmap/(1024*1024));

	return;
 }

static inline int fi_bgq_lock_if_required (struct l2atomic_lock *lock,
		int required)
{
	if (required) l2atomic_lock_acquire(lock);
	return 0;
}

static inline int fi_bgq_unlock_if_required (struct l2atomic_lock *lock,
		int required)
{
	if (required) l2atomic_lock_release(lock);
	return 0;
}

static inline uint64_t fi_bgq_cnk_vaddr2paddr(const void * vaddr, size_t len, uint64_t * paddr)
{
	Kernel_MemoryRegion_t cnk_mr;
	uint32_t cnk_rc;
	cnk_rc = Kernel_CreateMemoryRegion(&cnk_mr, (void *)vaddr, len);
	if (cnk_rc) return cnk_rc;

	*paddr = (uint64_t)cnk_mr.BasePa + ((uint64_t)vaddr - (uint64_t)cnk_mr.BaseVa);
	return 0;
}

enum fi_bgq_msync_type {
	FI_BGQ_MSYNC_TYPE_RW,
	FI_BGQ_MSYNC_TYPE_RO,
	FI_BGQ_MSYNC_TYPE_WO,
	FI_BGQ_MSYNC_TYPE_LAST
};

static inline void fi_bgq_msync(const enum fi_bgq_msync_type type)
{
	if (type == FI_BGQ_MSYNC_TYPE_RW || type == FI_BGQ_MSYNC_TYPE_WO) {
		/* this "l1p flush" hack is only needed to flush *writes*
		 * from a processor cache to the memory system */
		volatile uint64_t *mu_register =
			(volatile uint64_t *)(BGQ_MU_STATUS_CONTROL_REGS_START_OFFSET(0, 0) +
			0x030 - PHYMAP_PRIVILEGEDOFFSET);
		*mu_register = 0;
	}
	ppc_msync();
}

static inline void fi_bgq_mu_checks ()
{
	assert(sizeof(union fi_bgq_mu_packet_hdr) == sizeof(MUHWI_PacketHeader_t));
	assert(sizeof(union fi_bgq_addr) == sizeof(fi_addr_t));
	assert(sizeof(union fi_bgq_mu_descriptor) == sizeof(MUHWI_Descriptor_t));
}

/* ************************************************************************** *
 *
 * NOTE_MU_PADDR - The MU HWI documentation for MU descriptors states that
 * the physical address used for MU operations is 37 bits. However, the MSB
 * of this 37 bit physical address is used to indicate an atomic address and
 * will always be zero for normal physical addresses, and the maximum
 * physical address space depends on the amount of DRAM installed on the
 * compute nodes - which is only 16 GB. The physical addresses for main memory
 * begin at 0x0, and are contiguous until 64 GB, which means that the two
 * MSBs of the 36 bit physical address will always be zero.
 *
 * Unaligned non-atomic physical addresses can be safely specified using
 * only 34 bits in MU operations.
 *
 * Atomic physical addresses must be 8-byte-aligned which means that the
 * corresponding non-atomic physical address will always have the three
 * LSBs set to zero. A non-atomic physical address to be used for an atomic
 * physical address can be right-shifted 3 bits and can be safely specified
 * using only 31 bits when transferred as metadata. For MU operations the
 * physical address will be expanded to 37 bits as expected by the hardware.
 *
 * - MUHWI_Descriptor_t                 (hwi/include/bqc/MU_Descriptor.h)
 * - MUHWI_MessageUnitHeader_t          (hwi/include/bqc/MU_MessageUnitHeader.h)
 * - MUHWI_ATOMIC_ADDRESS_INDICATOR     (hwi/include/bqc/MU_Addressing.h)
 * - PHYMAP_MAXADDR_MAINMEMORY          (hwi/include/bqc/PhysicalMap.h)
 *
 * ************************************************************************** */

/* ************************************************************************** *
 *
 * NOTE_MU_RECFIFO - There are 16 "user" MU groups (not including the 17th MU
 * group which is normally used by cnk and agents) and there are 16 MU
 * reception fifos in each group (BGQ_MU_NUM_REC_FIFOS_PER_GROUP). There is
 * one MU reception fifo allocated to each ofi receive context. There are a
 * maximum of 256 ofi receive contexts on a compute node which must be
 * allocated between all processes, domains, and endpoints.
 *
 * When configured to support "scalable endpoints" multiple ofi receive
 * contexts will be associated with each endpoint.
 *
 * The ofi address contains 9 bits for the receive context (a.k.a. mu reception
 * fifo id) for the application agent (17th core) reception fifos.
 *
 * ************************************************************************** */

#endif /* _FI_PROV_BGQ_MU_H_ */
