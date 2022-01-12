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


#ifndef PROV_BGQ_INCLUDE_FLIGHT_RECORDER_H
#define PROV_BGQ_INCLUDE_FLIGHT_RECORDER_H

#include "rdma/bgq/fi_bgq_spi.h"

#define FLIGHT_RECORDER_ENTRY_STRLEN (256-8-2)
#define FLIGHT_RECORDER_ENTRY_COUNT (1024)
#define FLIGHT_RECORDER_INTERVAL (1024*1024*1024)

#ifdef FLIGHT_RECORDER_ENABLE
struct flight_recorder_entry {
	uint64_t	timebase;
	uint16_t	event;
	//uint16_t	line;
	char		str[FLIGHT_RECORDER_ENTRY_STRLEN];
};

struct flight_recorder {
	uint64_t	last_dump;
	unsigned	count;
	uint32_t	rank;
	int		thread;
	uint8_t		pad[64-8-4-4-4];
	struct flight_recorder_entry	entry[FLIGHT_RECORDER_ENTRY_COUNT];
};

#define FLIGHT_RECORDER(fr, event_id, format, ...)			\
{									\
	unsigned count = (fr)->count;					\
	struct flight_recorder_entry * next = &(fr)->entry[count];	\
	next->timebase  = GetTimeBase();				\
	next->event     = event_id;					\
	snprintf((char *)next->str, FLIGHT_RECORDER_ENTRY_STRLEN, format, __VA_ARGS__);	\
	next->str[FLIGHT_RECORDER_ENTRY_STRLEN-1] = 0x0;		\
	(fr)->count = count+1;						\
	if (count+1 == FLIGHT_RECORDER_ENTRY_COUNT)			\
		flight_recorder_dump((fr));				\
}

static inline
void flight_recorder_init (struct flight_recorder * fr) {
	fr->rank	= Kernel_GetRank();
	fr->thread	= Kernel_ProcessorID();
	fr->count	= 0;
	fr->last_dump	= GetTimeBase();
}

static inline
void flight_recorder_dump (struct flight_recorder * fr) {
	const unsigned count = fr->count;
	if (count == 0) {
		fr->last_dump = GetTimeBase();
		return;
	}

	const unsigned rank = fr->rank;
	const unsigned thread = fr->thread;


	fprintf(stderr, "#FLIGHT_RECORDER |----timebase---| -rank -p- -event \"str...\"\n");

	struct flight_recorder_entry * entry = &fr->entry[0];
	unsigned i;
	for (i=0; i<count; ++i) {
		fprintf(stderr, "FLIGHT_RECORDER  t%016lx r%04u p%02u e%05u \"%s\"\n",
			entry[i].timebase, rank, thread, entry[i].event,
			entry[i].str);
	}

	fflush(stderr);
	fr->count = 0;
	fr->last_dump = GetTimeBase();
}


static inline
void flight_recorder_write (struct flight_recorder * fr, uint16_t event, uint16_t line, const char * str) {
	const unsigned count = fr->count;
	struct flight_recorder_entry * next = &fr->entry[count];

	next->timebase	= GetTimeBase();
	next->event	= event;
	if (str) {
		strncpy(next->str, str, FLIGHT_RECORDER_ENTRY_STRLEN);
		next->str[FLIGHT_RECORDER_ENTRY_STRLEN-1] = 0x0;
	} else {
		next->str[0] = 0x0;
	}

	fr->count = count+1;
	if (count+1 == FLIGHT_RECORDER_ENTRY_COUNT) {
		flight_recorder_dump(fr);
	}
}

static inline
void flight_recorder_poll (struct flight_recorder * fr) {
	if ((GetTimeBase() - fr->last_dump) > FLIGHT_RECORDER_INTERVAL)
		flight_recorder_dump(fr);
}
#else
#define FLIGHT_RECORDER(fr, event_id, format, ...)
#endif

#endif /* PROV_BGQ_INCLUDE_FLIGHT_RECORDER_H */
