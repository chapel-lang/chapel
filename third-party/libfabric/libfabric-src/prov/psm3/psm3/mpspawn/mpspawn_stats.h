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

#ifndef _MPSPAWN_STATS_H
#define _MPSPAWN_STATS_H

#include <math.h>

#define MPSPAWN_STATS_VERSION	1

typedef enum {
	MPSPAWN_STATS_TYPE_DOUBLE = 0x1,
#define MPSPAWN_STATS_TYPE_DOUBLE      0x1
	MPSPAWN_STATS_TYPE_HEADER = 0x2,
#define MPSPAWN_STATS_TYPE_HEADER      0x2
	MPSPAWN_STATS_REDUCTION_MAX = 0x1000,
#define MPSPAWN_STATS_REDUCTION_MAX    0x1000
	MPSPAWN_STATS_REDUCTION_MIN = 0x2000,
#define MPSPAWN_STATS_REDUCTION_MIN    0x2000
	MPSPAWN_STATS_REDUCTION_MEDIAN = 0x4000,
#define MPSPAWN_STATS_REDUCTION_MEDIAN 0x4000
	MPSPAWN_STATS_SKIP_IF_ZERO = 0x8000
#define MPSPAWN_STATS_SKIP_IF_ZERO     0x8000
} mpspawn_stats_flags;

#define MPSPAWN_STATS_REDUCTION_ALL (MPSPAWN_STATS_REDUCTION_MAX | \
	    MPSPAWN_STATS_REDUCTION_MIN | MPSPAWN_STATS_REDUCTION_MEDIAN)

#define MPSPAWN_STATS_DOUBLE_TO_U64(arg) (*((uint64_t *) &(arg)))
#define MPSPAWN_NAN_U64 ((uint64_t) ~0ULL)
#define MPSPAWN_ISNAN_U64(x)    (((uint64_t)(x)) == MPSPAWN_NAN_U64)

#define MPSPAWN_NAN	    ((uint64_t) ~0ULL)	/* NAN) */
#define MPSPAWN_ISNAN(x)    (isnan(x))

#if 0   // unused code, specific to QLogic MPI

struct mpspawn_stats_add_args;	/* client->mpspawn stats registration */
struct mpspawn_stats_req_args;	/* mpspawn->client fn callback stats request */
struct mpspawn_stats_init_args;	/* mpspawn->client "downcall" to register */

/* Clients implement this function to fill in mpspawn request for stats */
typedef void (*mpspawn_stats_req_fn) (struct mpspawn_stats_req_args *);
/* mpspawn implements this function to allow clients to register new stats */
typedef void (*mpspawn_stats_add_fn) (struct mpspawn_stats_add_args *);
/* mpspawn implements this function to map rank indexes into epaddr structs */
struct psm2_epaddr;
typedef struct psm2_epaddr *(*mpspawn_map_epaddr_fn) (int rank);

typedef struct mpspawn_stats_req_args {
	int version;
	int num;
	uint64_t *stats;
	uint16_t *flags;
	void *context;
} mpspawn_stats_req_args_t;

typedef
struct mpspawn_stats_add_args {
	int version;
	int num;
	char *header;
	char **desc;
	uint16_t *flags;
	mpspawn_stats_req_fn req_fn;
	void *context;
} mpspawn_stats_add_args_t;

typedef
struct mpspawn_stats_init_args {
	int version;
	psm2_mq_t mq;		/* initialized mq endpoint */
	int num_epaddr;		/* number of endpoints in job */
	mpspawn_stats_add_fn add_fn;	/* function for client to add stats */
	mpspawn_map_epaddr_fn epaddr_map_fn;
	const char *stats_types;	/* stats type string mpirun -M */
} mpspawn_stats_init_args_t;

/* Function in psm exposed to register stats */
void *psmi_stats_register(struct mpspawn_stats_init_args *args);

#endif
#endif
