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

#if PSMI_TIMER_STATS
#  define PSMI_TIMER_STATS_ADD_INSERTION(ctrl)	((ctrl)->num_insertions++)
#  define PSMI_TIMER_STATS_ADD_TRAVERSAL(ctrl)	((ctrl)->num_traversals++)
#else
#  define PSMI_TIMER_STATS_ADD_INSERTION(ctrl)
#  define PSMI_TIMER_STATS_ADD_TRAVERSAL(ctrl)
#endif

psm2_error_t psm3_timer_init(struct psmi_timer_ctrl *ctrl)
{
	ctrl->t_cyc_next_expire = PSMI_TIMER_INFINITE;

#if PSMI_TIMER_STATS
	ctrl->num_insertions = 0;
	ctrl->num_traversals = 0;
#endif

	TAILQ_INIT(&ctrl->timerq);
	return PSM2_OK;
}

psm2_error_t psm3_timer_fini(struct psmi_timer_ctrl *ctrl)
{
#if PSMI_TIMER_STATS
	if (ctrl->num_insertions > 0) {
		_HFI_INFO("avg elem traversals/insertion = %3.2f %%\n",
			  100.0 * (double)ctrl->num_traversals /
			  ctrl->num_insertions);
	}
#endif
	return PSM2_OK;
}

void
psm3_timer_request_always(struct psmi_timer_ctrl *ctrl,
			  struct psmi_timer *t_insert, uint64_t t_cyc_expire)
{
	struct psmi_timer *t_cursor;

	psmi_assert(!(t_insert->flags & PSMI_TIMER_FLAG_PENDING));

	t_insert->t_timeout = t_cyc_expire;
	t_insert->flags |= PSMI_TIMER_FLAG_PENDING;

	/*
	 * We keep the list from oldest (head) to newest (tail), with the
	 * assumption that insert and remove occur much more often than search
	 * (when the timer expires).  Newly added timers are more likely to expire
	 * later rather than sooner, which is why the head is older.
	 */
	PSMI_TIMER_STATS_ADD_INSERTION(ctrl);

	if (TAILQ_EMPTY(&ctrl->timerq)) {	/* Common case */
		TAILQ_INSERT_TAIL(&ctrl->timerq, t_insert, timer);
		ctrl->t_cyc_next_expire = t_cyc_expire;
		PSMI_TIMER_STATS_ADD_TRAVERSAL(ctrl);
		return;
	} else if (t_cyc_expire > PSMI_TIMER_PRIO_LAST) {
		TAILQ_FOREACH(t_cursor, &ctrl->timerq, timer) {
			if (t_cursor->t_timeout <= t_cyc_expire) {
				TAILQ_INSERT_BEFORE(t_cursor, t_insert, timer);
				return;
			}
			PSMI_TIMER_STATS_ADD_TRAVERSAL(ctrl);
		}
		/* Got to the end of the list -- We're the next to expire */
		ctrl->t_cyc_next_expire = t_cyc_expire;
		TAILQ_INSERT_TAIL(&ctrl->timerq, t_insert, timer);
		return;
	} else {
		TAILQ_FOREACH_REVERSE(t_cursor, &ctrl->timerq, timerq, timer) {
			if (t_cursor->t_timeout >= t_cyc_expire) {
				TAILQ_INSERT_AFTER(&ctrl->timerq, t_cursor,
						   t_insert, timer);
				ctrl->t_cyc_next_expire =
				    min(t_cyc_expire, ctrl->t_cyc_next_expire);
				return;
			}
			PSMI_TIMER_STATS_ADD_TRAVERSAL(ctrl);
		}
		TAILQ_INSERT_HEAD(&ctrl->timerq, t_insert, timer);
		/* No need to check if we inserted last, given first branch case */
		/* if (TAILQ_LAST(&ctrl->timerq, timerq) == t_insert) */
		/* ctrl->t_cyc_next_expire = t_cyc_expire; */
		return;
	}

	return;
}

psm2_error_t
psm3_timer_process_expired(struct psmi_timer_ctrl *ctrl, uint64_t t_cyc_expire)
{
	psm2_error_t err = PSM2_OK_NO_PROGRESS;
	struct psmi_timer *t_cursor = TAILQ_LAST(&ctrl->timerq, timerq);

	PSM2_LOG_MSG("entering");

	while (t_cursor) {
		if (t_cursor->t_timeout > t_cyc_expire)
			break;

		err = PSM2_OK;
		psmi_assert(t_cursor->flags & PSMI_TIMER_FLAG_PENDING);
		t_cursor->flags &= ~PSMI_TIMER_FLAG_PENDING;
		TAILQ_REMOVE(&ctrl->timerq, t_cursor, timer);
		t_cursor->expire_callback(t_cursor, t_cyc_expire);
		t_cursor = TAILQ_PREV(t_cursor, timerq, timer);
	}

	if (TAILQ_EMPTY(&ctrl->timerq))
		ctrl->t_cyc_next_expire = PSMI_TIMER_INFINITE;
	else
		ctrl->t_cyc_next_expire =
		    TAILQ_LAST(&ctrl->timerq, timerq)->t_timeout;

	PSM2_LOG_MSG("leaving");
	return err;
}

void
psm3_timer_cancel_inner(struct psmi_timer_ctrl *ctrl,
			struct psmi_timer *t_remove)
{

	psmi_assert(t_remove->flags & PSMI_TIMER_FLAG_PENDING);

	t_remove->flags &= ~PSMI_TIMER_FLAG_PENDING;
	TAILQ_REMOVE(&ctrl->timerq, t_remove, timer);

	/*
	 * If we're removing the last entry, we need to reset the
	 * expiration cycle time.
	 */
	if (TAILQ_EMPTY(&ctrl->timerq))
		ctrl->t_cyc_next_expire = PSMI_TIMER_INFINITE;
	else
		ctrl->t_cyc_next_expire =
		    TAILQ_LAST(&ctrl->timerq, timerq)->t_timeout;
	return;
}
