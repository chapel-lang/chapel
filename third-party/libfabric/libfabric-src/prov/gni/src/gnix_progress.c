/*
 * Copyright (c) 2017 Cray Inc. All rights reserved.
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

/*
 * Progress common code
 */

#include <stdlib.h>

#include "gnix_progress.h"

struct gnix_prog_obj {
	struct dlist_entry list;
	int ref_cnt;
	void *obj;
	int (*prog_fn)(void *data);
};


int _gnix_prog_progress(struct gnix_prog_set *set)
{
	struct gnix_prog_obj *pobj, *tmp;
	int rc;

	COND_READ_ACQUIRE(set->requires_lock, &set->lock);

	dlist_for_each_safe(&set->prog_objs, pobj, tmp, list) {
		rc = pobj->prog_fn(pobj->obj);
		if (rc) {
			GNIX_WARN(FI_LOG_EP_CTRL,
				  "Obj(%p) prog function failed: %d\n",
				  pobj, rc);
		}
	}

	COND_RW_RELEASE(set->requires_lock, &set->lock);

	return FI_SUCCESS;
}

int _gnix_prog_obj_add(struct gnix_prog_set *set, void *obj,
		       int (*prog_fn)(void *data))
{
	struct gnix_prog_obj *pobj, *tmp;

	COND_WRITE_ACQUIRE(set->requires_lock, &set->lock);

	dlist_for_each_safe(&set->prog_objs, pobj, tmp, list) {
		if (obj == pobj->obj && prog_fn == pobj->prog_fn) {
			pobj->ref_cnt++;
			COND_RW_RELEASE(set->requires_lock, &set->lock);
			return FI_SUCCESS;
		}
	}

	pobj = malloc(sizeof(struct gnix_prog_obj));
	if (!pobj) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Failed to add OBJ to prog set.\n");
		COND_RW_RELEASE(set->requires_lock, &set->lock);
		return -FI_ENOMEM;
	}

	pobj->obj = obj;
	pobj->prog_fn = prog_fn;
	pobj->ref_cnt = 1;
	dlist_init(&pobj->list);
	dlist_insert_tail(&pobj->list, &set->prog_objs);

	COND_RW_RELEASE(set->requires_lock, &set->lock);

	GNIX_INFO(FI_LOG_EP_CTRL, "Added obj(%p) to set(%p)\n",
		  obj, set);

	return FI_SUCCESS;
}

int _gnix_prog_obj_rem(struct gnix_prog_set *set, void *obj,
		       int (*prog_fn)(void *data))
{
	struct gnix_prog_obj *pobj, *tmp;

	COND_WRITE_ACQUIRE(set->requires_lock, &set->lock);

	dlist_for_each_safe(&set->prog_objs, pobj, tmp, list) {
		if (obj == pobj->obj && prog_fn == pobj->prog_fn) {
			if (!--pobj->ref_cnt) {
				dlist_remove(&pobj->list);
				free(pobj);
				GNIX_INFO(FI_LOG_EP_CTRL,
					  "Removed obj(%p) from set(%p)\n",
					  obj, set);
			}
			COND_RW_RELEASE(set->requires_lock, &set->lock);
			return FI_SUCCESS;
		}
	}

	COND_RW_RELEASE(set->requires_lock, &set->lock);

	GNIX_WARN(FI_LOG_EP_CTRL, "Object not found on prog set.\n");
	return -FI_EINVAL;
}

int _gnix_prog_init(struct gnix_prog_set *set)
{
	dlist_init(&set->prog_objs);
	rwlock_init(&set->lock);
	set->requires_lock = 1;

	return FI_SUCCESS;
}

int _gnix_prog_fini(struct gnix_prog_set *set)
{
	struct gnix_prog_obj *pobj, *tmp;

	COND_WRITE_ACQUIRE(set->requires_lock, &set->lock);

	dlist_for_each_safe(&set->prog_objs, pobj, tmp, list) {
		dlist_remove(&pobj->list);
		free(pobj);
	}

	COND_RW_RELEASE(set->requires_lock, &set->lock);

	rwlock_destroy(&set->lock);

	return FI_SUCCESS;
}

