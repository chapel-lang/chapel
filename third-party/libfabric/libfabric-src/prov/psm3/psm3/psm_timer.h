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

#ifndef _PSMI_IN_USER_H
#error psm_timer.h not meant to be included directly, include psm_user.h instead
#endif

#ifndef _PSMI_TIMER_H
#define _PSMI_TIMER_H


typedef struct psmi_timer psmi_timer;
/* when invoked by a timer, the return code from callback is ignored.  A
 * return is allowed so a function may be used directly and as a timer callback
 */
typedef psm2_error_t(*psmi_timer_expire_callback_t) (struct psmi_timer *,
						    uint64_t);

struct psmi_timer {
	TAILQ_ENTRY(psmi_timer) timer;	/* opaque */
	uint64_t t_timeout;	/* opaque */
	uint8_t flags;		/* opaque */

	psmi_timer_expire_callback_t expire_callback; /* user -- callback fn */
	void *context;		/* user -- callback param */
};

struct psmi_timer_ctrl {
	uint64_t t_cyc_next_expire;
	 TAILQ_HEAD(timerq, psmi_timer) timerq;

#if PSMI_TIMER_STATS
	uint64_t num_insertions;
	uint64_t num_traversals;
#endif
};

/*
 * Some events need to be unconditionally enqueued at the beginning of the
 * timerq -- they are not timers meant to expire but merely operations that
 * need to be delayed.  For delayed operations, there are 5 levels of
 * priority.
 */
#define PSMI_TIMER_PRIO_0	 0ULL
#define PSMI_TIMER_PRIO_1	 1ULL
#define PSMI_TIMER_PRIO_2	 2ULL
#define PSMI_TIMER_PRIO_3	 3ULL
#define PSMI_TIMER_PRIO_4	 4ULL
#define PSMI_TIMER_PRIO_LAST	 PSMI_TIMER_PRIO_4

#define PSMI_TIMER_INFINITE	 0xFFFFFFFFFFFFFFFFULL
#define PSMI_TIMER_FLAG_PENDING  0x01

/*
 * Timer control initialization and finalization
 */
psm2_error_t psm3_timer_init(struct psmi_timer_ctrl *ctrl);
psm2_error_t psm3_timer_fini(struct psmi_timer_ctrl *ctrl);

/*
 * Timer entry initialization (a timer must be initialized before it can be
 * added to the timer request queue).
 */

PSMI_ALWAYS_INLINE(
void
psmi_timer_entry_init(struct psmi_timer *t_init,
		      psmi_timer_expire_callback_t expire_fn,
		      void *context))
{
	t_init->flags = 0;
	t_init->expire_callback = expire_fn;
	t_init->context = context;
	return;
}

/*
 * Timer requests, conditional (macro) or unconditional
 */
#define psmi_timer_request(ctrl, t_insert, t_cyc)			\
	    if (!((t_insert)->flags & PSMI_TIMER_FLAG_PENDING))		\
		psm3_timer_request_always((ctrl), (t_insert), (t_cyc))

void psm3_timer_request_always(struct psmi_timer_ctrl *ctrl,
			       struct psmi_timer *t_insert,
			       uint64_t t_cyc_expire);

/*
 * Timer cancelations, conditional (macro) only (cancel_inner is internal)
 */
#define psmi_timer_cancel(ctrl, t_remove)		    \
	    if ((t_remove)->flags & PSMI_TIMER_FLAG_PENDING) \
		psm3_timer_cancel_inner(ctrl, t_remove)
void psm3_timer_cancel_inner(struct psmi_timer_ctrl *ctrl,
			     struct psmi_timer *t_remove);

/*
 * Timer processing, conditional or unconditional.
 */
#define psmi_timer_process_if_expired(ctrl, t_cyc_expire)		\
	    (((ctrl)->t_cyc_next_expire <= (t_cyc_expire)) ?		\
	     psm3_timer_process_expired(ctrl, t_cyc_expire) :           \
	     PSM2_OK_NO_PROGRESS)

#define psmi_timer_is_expired(ctrl, t_cyc_expire)			\
	    ((ctrl)->t_cyc_next_expire <= (t_cyc_expire))

psm2_error_t psm3_timer_process_expired(struct psmi_timer_ctrl *ctrl,
				       uint64_t t_cyc_expire);

#endif /* _PSMI_TIMER_H */
