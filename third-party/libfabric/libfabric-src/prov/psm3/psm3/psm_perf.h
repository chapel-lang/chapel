/*

  This file is provided under a dual BSD/GPLv2 license.  When using or
  redistributing this file, you may do so under either license.

  GPL LICENSE SUMMARY

  Copyright(c) 2017 Intel Corporation.

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

  Copyright(c) 2016 Intel Corporation.

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

/* When the perf framework is enabled, GENERIC_PERF_BEGIN/END can be
 * used to mark sections of code to be included in a given "slot number"
 * for performance statistics.
 * The PMU will be used to measure instruction cycles used between the BEGIN/END
 * This permits precise statistics to be gathered for how much CPU is required
 * to execute all the code in a given slot number during a given run.
 * At the end of the run the statistics are output.
 * This capability is only enabled when PSM is built with -DRDPMC_PERF_FRAMEWORK
 */

/* slot numbers for the counters we want */
#define PSM_TX_SPEEDPATH_CTR 0
#define PSM_RX_SPEEDPATH_CTR 1

#ifdef RDPMC_PERF_FRAMEWORK

/* Configuration */

#define RDPMC_PERF_MAX_SLOT_NUMBER (8)	// we only use 2, RX and TX
#define RDPMC_PERF_MAX_SLOT_NAME   (256)

/* RDPMC infrastructure */

extern __thread struct rdpmc_ctx global_rdpmc_ctx;

typedef unsigned long long u64;

extern u64 global_rdpmc_begin[RDPMC_PERF_MAX_SLOT_NUMBER];
extern u64 global_rdpmc_summ[RDPMC_PERF_MAX_SLOT_NUMBER];
extern u64 global_rdpmc_number[RDPMC_PERF_MAX_SLOT_NUMBER];

extern char global_rdpmc_slot_name[RDPMC_PERF_MAX_SLOT_NUMBER][RDPMC_PERF_MAX_SLOT_NAME];

extern unsigned int global_rdpmc_type;
extern unsigned int global_rdpmc_config;

extern unsigned long long psm3_rdpmc_read(struct rdpmc_ctx *ctx);

#define RDPMC_PERF_INIT() \
{                         \
    int i;                \
    for (i = 0; i < RDPMC_PERF_MAX_SLOT_NUMBER; i++) \
    {                                                \
        global_rdpmc_begin[i]  = 0; \
        global_rdpmc_summ[i]   = 0; \
        global_rdpmc_number[i] = 0; \
        global_rdpmc_slot_name[i][0] = '\0'; \
    }                               \
}

/* There is no slot_number max range check */

#define RDPMC_PERF_SET_SLOT_NAME(slot_number, name)  \
{                                                    \
    strncpy(global_rdpmc_slot_name[(slot_number)], (name), RDPMC_PERF_MAX_SLOT_NAME - 1); \
    global_rdpmc_slot_name[(slot_number)][RDPMC_PERF_MAX_SLOT_NAME - 1] = '\0';           \
}

#define RDPMC_PERF_BEGIN(slot_number) \
{                                     \
    global_rdpmc_begin[(slot_number)] = psm3_rdpmc_read(&global_rdpmc_ctx); \
}

#define RDPMC_PERF_END(slot_number) \
{                        \
    global_rdpmc_summ[(slot_number)] += (psm3_rdpmc_read(&global_rdpmc_ctx) - global_rdpmc_begin[(slot_number)]); \
    global_rdpmc_number[(slot_number)]++;                                                                    \
}

#define RDPMC_PERF_DUMP(stream) \
{                         \
    int i;                \
    for (i = 0; i < RDPMC_PERF_MAX_SLOT_NUMBER; i++) \
    {                                                \
        if (global_rdpmc_slot_name[i][0])                  \
        {                                            \
            fprintf((stream), "%s: RDPMC [%s] (%x, %04x) avg = %g (%llu times)\n", \
                    psm3_get_mylabel(), global_rdpmc_slot_name[i], global_rdpmc_type, global_rdpmc_config, \
                    (double)global_rdpmc_summ[i] / global_rdpmc_number[i], global_rdpmc_number[i]); \
            fflush((stream));                                                 \
        } \
    }     \
}

#define GENERIC_PERF_INIT()                           RDPMC_PERF_INIT()
#define GENERIC_PERF_SET_SLOT_NAME(slot_number, name) RDPMC_PERF_SET_SLOT_NAME(slot_number, name)
#define GENERIC_PERF_BEGIN(slot_number)               RDPMC_PERF_BEGIN(slot_number)
#define GENERIC_PERF_END(slot_number)                 RDPMC_PERF_END(slot_number)
#define GENERIC_PERF_DUMP(stream)                     RDPMC_PERF_DUMP(stream)
#else /* RDPMC_PERF_FRAMEWORK */
#define GENERIC_PERF_INIT()
#define GENERIC_PERF_SET_SLOT_NAME(slot_number, name)
#define GENERIC_PERF_BEGIN(slot_number)
#define GENERIC_PERF_END(slot_number)
#define GENERIC_PERF_DUMP(stream)
#endif /* RDPMC_PERF_FRAMEWORK */
