/*
 * perf_util.h - helper functions for perf_events
 *
 * Copyright (c) 2009 Google, Inc
 * Contributed by Stephane Eranian <eranian@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef __PERF_UTIL_H__
#define __PERF_UTIL_H__

#include <sys/types.h>
#include <inttypes.h>
#include <perfmon/pfmlib_perf_event.h>

typedef struct {
	struct perf_event_attr hw;
	uint64_t value, prev_value;
	char *name;
	uint64_t id; /* event id kernel */
	void *buf;
	size_t pgmsk;
	int fd;
} perf_event_desc_t;

/* handy shortcut */
#define PERF_FORMAT_SCALE (PERF_FORMAT_TOTAL_TIME_ENABLED|PERF_FORMAT_TOTAL_TIME_RUNNING)

extern int perf_setup_argv_events(char **argv, perf_event_desc_t **fd);
extern int perf_setup_list_events(const char *events, perf_event_desc_t **fd);
extern int perf_read_buffer(struct perf_event_mmap_page *hdr, size_t pgmsk, void *buf, size_t sz);
extern void perf_skip_buffer(struct perf_event_mmap_page *hdr, size_t sz);

static inline int
perf_read_buffer_32(void *hdr, size_t pgmsk, void *buf)
{
	return perf_read_buffer(hdr, pgmsk, buf, sizeof(uint32_t));
}

static inline int
perf_read_buffer_64(void *hdr, size_t pgmsk, void *buf)
{
	return perf_read_buffer(hdr, pgmsk, buf, sizeof(uint64_t));
}

/*
 * values[0] = raw count
 * values[1] = TIME_ENABLED
 * values[2] = TIME_RUNNING
 */
static inline uint64_t
perf_scale(uint64_t *values)
{
	uint64_t res = 0;

	if (values[2])
		res = (uint64_t)((double)values[0] * values[1]/values[2]);
	return res;
}

/*
 * TIME_RUNNING/TIME_ENABLED
 */
static inline double
perf_scale_ratio(uint64_t *values)
{
	if (!values[1])
		return 0.0;

	return values[2]*1.0/values[1];
}

static inline int
perf_fd2event(perf_event_desc_t *fds, int num_events, int fd)
{
	int i;

	for(i=0; i < num_events; i++)
		if (fds[i].fd == fd)
			return i;
	return -1;
}

/*
 * id = PERF_FORMAT_ID
 */
static inline int
perf_id2event(perf_event_desc_t *fds, int num_events, uint64_t id)
{
	int j;
	for(j=0; j < num_events; j++)
		if (fds[j].id == id)
			return j;
	return -1;
}
#endif
