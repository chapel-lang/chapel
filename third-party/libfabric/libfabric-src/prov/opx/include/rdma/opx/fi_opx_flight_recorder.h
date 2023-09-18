/*
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

#ifndef PROV_OPX_INCLUDE_FLIGHT_RECORDER_H
#define PROV_OPX_INCLUDE_FLIGHT_RECORDER_H

#include <time.h>
#include <sys/syscall.h>
#include <execinfo.h>
#include "ofi.h"
#include "fi_opx_reliability.h"
#include "fi_opx_hfi1_packet.h"
#include "fi_opx_timer.h"

#define FLIGHT_RECORDER_ENTRY_DATA_LEN (sizeof(union fi_opx_hfi1_packet_payload) + sizeof(union fi_opx_hfi1_packet_hdr))
#ifndef FLIGHT_RECORDER_ENTRY_COUNT
#define FLIGHT_RECORDER_ENTRY_COUNT (1024)
#endif
#define FLIGHT_RECORDER_INTERVAL_SECONDS (60)

#define FR_EVENT_EP_RX_RECV            0x0001
#define FR_EVENT_EP_RX_RECVMSG         0x0002
#define FR_EVENT_EP_RX_PROCESS_HEADER  0x0003
#define FR_EVENT_EP_RX_PROCESS_CONTENT 0x0004
#define FR_EVENT_EP_RX_POLL            0x0005
#define FR_EVENT_EP_TX_SEND            0x0006
#define FR_EVENT_EP_TX_INJECT          0x0007
#define FR_EVENT_HFI1_POLL_ONCE        0x0008

#define FR_ENTRY_TYPE_STRING     0x01
#define FR_ENTRY_TYPE_PACKET_HDR 0x02
#define FR_ENTRY_TYPE_PACKET     0x03

#ifdef FLIGHT_RECORDER_ENABLE

struct flight_recorder_entry {
	struct timespec	timestamp;
	uint64_t	cycles;
	uint16_t 	event;
	uint16_t 	data_size;
	uint8_t 	type;
	uint8_t 	pad[3];
	uint8_t		data[FLIGHT_RECORDER_ENTRY_DATA_LEN];
} __attribute__((packed));

struct flight_recorder {
	struct timespec			last_dump;
	uint64_t			last_dump_cycles;
	unsigned			count;
	pid_t				tid;
	uint8_t				initialized;
	uint8_t				pad[7];
	struct flight_recorder_entry	entry[FLIGHT_RECORDER_ENTRY_COUNT];
	union fi_opx_timer_state*	timer;
	union fi_opx_timer_stamp*	now;
} __attribute__((packed));

static inline
void flight_recorder_init (struct flight_recorder * fr)
{
	fr->tid = (long int)syscall(SYS_gettid);
	fr->count = 0;
	fr->timer = malloc(sizeof(union fi_opx_timer_state));
	fi_opx_timer_init(fr->timer);
	fr->now = malloc(sizeof(union fi_opx_timer_stamp));
	fi_opx_timer_now(fr->now, fr->timer);   
	fr->last_dump = fr->now->tp;
	fr->last_dump_cycles = fr->now->cycle_timer.cycles;
	fr->initialized = 1;
}

static inline
struct flight_recorder_entry *flight_recorder_get_next(struct flight_recorder *fr)
{
	struct flight_recorder_entry *next = &fr->entry[fr->count++];
	if (fr->count == FLIGHT_RECORDER_ENTRY_COUNT) fr->count = 0;
	return next;
}

static inline
struct flight_recorder_entry *flight_recorder_init_next_entry(struct flight_recorder *fr,
								uint16_t event_id, uint8_t entry_type)
{
	if (!fr->initialized) flight_recorder_init(fr);
	struct flight_recorder_entry *next = flight_recorder_get_next(fr);
	fi_opx_timer_now(fr->now, fr->timer);
	next->timestamp = fr->now->tp;
	next->cycles = fr->now->cycle_timer.cycles;
	next->event = event_id;
	next->type = entry_type;
	return next;
}

static inline void flight_recorder_dump_packet_payload(struct flight_recorder_entry *entry)
{
	uint32_t offset = 0;
	uint32_t end = sizeof(entry->data);

	fprintf(stderr, "### PAYLOAD DATA\n");
	fprintf(stderr, "offset | Data\n");
	while (offset < end) {
		uint32_t *dw = (uint32_t *) (entry->data + offset);

		fprintf(stderr, "+%08X | %08X %08X  %08X %08X  %08X %08X  %08X %08X\n",
			offset, dw[0], dw[1], dw[2], dw[3], dw[4], dw[5], dw[6], dw[7]);
		offset += 0x20;
	}
}

static inline
void flight_recorder_dump (struct flight_recorder * fr)
{
	const unsigned count = fr->count;
	struct timespec current_time;
	fi_opx_timer_now(fr->now, fr->timer);
	if (!fr->timer->cycle_timer.use_cycle_timer){
		current_time = fr->now->tp;
		fprintf(stderr, "#FLIGHT_RECORDER t%d |Last Dump Time: %ld.%ld\n"
				"#FLIGHT_RECORDER t%d |Current   Time: %ld.%ld\n"
				"#FLIGHT_RECORDER t%d |Entry Count   : %u\n",
				fr->tid, fr->last_dump.tv_sec, fr->last_dump.tv_nsec,
				fr->tid, current_time.tv_sec, current_time.tv_nsec,
				fr->tid, count);
		fr->last_dump = current_time;
		if (count == 0) {
                	fr->last_dump = current_time;
                	return;
        	}
	}
	else{
		fprintf(stderr, "#FLIGHT_RECORDER t%d |Last Dump Time: %0.9lf\n"
                                "#FLIGHT_RECORDER t%d |Current   Time: %0.9lf\n"
                                "#FLIGHT_RECORDER t%d |Entry Count   : %u\n",
                                fr->tid, fr->last_dump_cycles * fr->timer->cycle_timer.picos_per_cycle/1e12,
                                fr->tid, fr->now->cycle_timer.cycles * fr->timer->cycle_timer.picos_per_cycle/1e12,
                                fr->tid, count);
                fr->last_dump_cycles = fr->now->cycle_timer.cycles;
		if (count == 0) {
                        fr->last_dump_cycles = fr->now->cycle_timer.cycles;
                        return;
                }
	}

	fprintf(stderr, "#FLIGHT_RECORDER|TIME seconds.nseconds|THREAD ID|EVENT ID|DATA\n");

	struct flight_recorder_entry * entry = &fr->entry[0];
	unsigned i;
	for (i=MAX(0, count-5); i<count; ++i) {
		if (!fr->timer->cycle_timer.use_cycle_timer){
			fprintf(stderr, "#FLIGHT_RECORDER|%ld.%09ld|%10d|%04X|",
				entry[i].timestamp.tv_sec, entry[i].timestamp.tv_nsec,
				fr->tid, entry[i].event);
		}
		else{
			fprintf(stderr, "#FLIGHT_RECORDER|%0.9lf|%10d|%04X|",
                                entry[i].cycles * fr->timer->cycle_timer.picos_per_cycle/1e12,
                                fr->tid, entry[i].event);
		}
		if (entry[i].type == FR_ENTRY_TYPE_PACKET_HDR) {
			fprintf(stderr, "PACKET HDR|\n");
			fi_opx_hfi1_dump_packet_hdr((union fi_opx_hfi1_packet_hdr *)entry[i].data,
							"#FLIGHT_RECORDER", 0);
		} else if (entry[i].type == FR_ENTRY_TYPE_PACKET) {
			flight_recorder_dump_packet_payload(entry);
		} else {
			fprintf(stderr, "STRING|%s\n", entry[i].data);
		}
	}

	fflush(stderr);
	fr->count = 0;
}

#define FLIGHT_RECORDER_STRING(fr, event_id, format, ...)			\
{										\
	struct flight_recorder_entry *next =					\
		flight_recorder_init_next_entry((fr), (event_id),		\
						FR_ENTRY_TYPE_STRING);		\
	int actual_len = snprintf((char *)next->data,				\
					FLIGHT_RECORDER_ENTRY_DATA_LEN,		\
			 		format, ##__VA_ARGS__);			\
	int end_of_string = MIN(actual_len, FLIGHT_RECORDER_ENTRY_DATA_LEN - 1);\
	next->data[end_of_string] = 0;						\
	if ((fr)->count + 1 == FLIGHT_RECORDER_ENTRY_COUNT)			\
		flight_recorder_dump((fr));					\
}

#define FLIGHT_RECORDER_PACKET_HDR(fr, event_id, packet_hdr)			\
{										\
	struct flight_recorder_entry *next =					\
		flight_recorder_init_next_entry((fr), (event_id),		\
						FR_ENTRY_TYPE_PACKET_HDR);	\
	memcpy((void *)next->data, (void *) &(packet_hdr),			\
			sizeof(union fi_opx_hfi1_packet_hdr));			\
	if ((fr)->count + 1 == FLIGHT_RECORDER_ENTRY_COUNT)			\
		flight_recorder_dump((fr));					\
}

#define FLIGHT_RECORDER_PACKET(fr, event_id, packet)				\
{										\
	struct flight_recorder_entry *next =					\
		flight_recorder_init_next_entry((fr), (event_id),		\
						FR_ENTRY_TYPE_PACKET);		\
	memcpy((void *)next->data, (void *) &(packet),				\
			sizeof(union fi_opx_hfi1_packet_payload));		\
	if ((fr)->count + 1 == FLIGHT_RECORDER_ENTRY_COUNT)			\
		flight_recorder_dump((fr));					\
}

#define FLIGHT_RECORDER_INIT(fr)						\
{										\
	flight_recorder_init(fr);						\
}

#else /* !FLIGHT_RECORDER_ENABLE */

#define FLIGHT_RECORDER_STRING(fr, event_id, format, ...)
#define FLIGHT_RECORDER_PACKET_HDR(fr, event_id, packet_hdr)
#define FLIGHT_RECORDER_PACKET(fr, event_id, packet)

#endif /* #ifdef FLIGHT_RECORDER_ENABLE */

#endif /* #define PROV_OPX_INCLUDE_FLIGHT_RECORDER_H */
