/*
 * Copyright (c) 2015-2016 Los Alamos National Security, LLC.
 *                         All rights reserved.
 * Copyright (c) 2016 Cray Inc.  All rights reserved.
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

#include "gnix_vector.h"

static gnix_vector_ops_t __gnix_vec_lockless_ops;
static gnix_vector_ops_t __gnix_vec_locked_ops;

/*******************************************************************************
 * INTERNAL HELPER FNS
 ******************************************************************************/
static inline uint32_t __gnix_vec_get_new_size(gnix_vector_t *vec, uint32_t index)
{
	uint32_t new_size = vec->attr.cur_size;

	if (vec->attr.vec_increase_type == GNIX_VEC_INCREASE_ADD) {
		do {
			new_size += vec->attr.vec_increase_step;
		} while (index >= new_size);
	} else {
		if (new_size)
			new_size *= vec->attr.vec_increase_step;
		else
			new_size = (new_size + 1) * vec->attr.vec_increase_step;

		while (index >= new_size)
			new_size *= vec->attr.vec_increase_step;

	}

	if (new_size > vec->attr.vec_maximum_size) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Maximum vector size of %lu "
			  "reached in __gnix_vec_new_size\n", vec->attr.vec_maximum_size);
		new_size = vec->attr.vec_maximum_size;
	}

	return new_size;
}

static inline void __gnix_vec_close_entries(gnix_vector_t *vec)
{
	memset(vec->vector, 0, (sizeof(gnix_vec_entry_t) * vec->attr.cur_size));
}

/*******************************************************************************
 * INTERNAL WORKER FNS
 ******************************************************************************/
static inline int __gnix_vec_resize(gnix_vector_t *vec, uint32_t new_size)
{
	void *tmp;

	if (new_size <= vec->attr.cur_size) {
		GNIX_WARN(FI_LOG_EP_DATA, "In __gnix_vec_resize, the new vector"
			  "size is less than or equal to the current size.\n");
	}

	tmp = realloc(vec->vector, new_size * sizeof(gnix_vec_entry_t));

	if (!tmp) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Insufficient memory in "
			  "__gnix_vec_resize\n");
		return -FI_ENOMEM;
	}

	vec->vector = tmp;

	if (new_size > vec->attr.cur_size) {
		memset(vec->vector + vec->attr.cur_size, 0,
		       (sizeof(gnix_vec_entry_t) * (new_size - vec->attr.cur_size)));
	}

	vec->attr.cur_size = new_size;

	return FI_SUCCESS;
}

static inline int __gnix_vec_create(gnix_vector_t *vec, gnix_vec_attr_t *attr)
{
	if (OFI_UNLIKELY(vec->state == GNIX_VEC_STATE_READY)) {
		GNIX_DEBUG(FI_LOG_EP_DATA, "The vector (%p) is already ready.\n",
			   vec);
		return -FI_EINVAL;
	}

	vec->vector = calloc(attr->vec_initial_size, sizeof(gnix_vec_entry_t));

	if (OFI_UNLIKELY(!vec->vector)) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Insufficient memory in "
			  "_gnix_vec_init.\n");
		return -FI_ENOMEM;
	} else {
		attr->cur_size = attr->vec_initial_size;
	}

	memcpy(&vec->attr, attr, sizeof(gnix_vec_attr_t));

	return FI_SUCCESS;
}

static inline int __gnix_vec_close(gnix_vector_t *vec)
{
	if (OFI_UNLIKELY(vec->state == GNIX_VEC_STATE_DEAD)) {
		GNIX_DEBUG(FI_LOG_EP_DATA, "The vector (%p) is already dead.\n",
			   vec);
		return -FI_EINVAL;
	}

	free(vec->vector);
	vec->ops = NULL;
	vec->attr.cur_size = 0;
	vec->state = GNIX_VEC_STATE_DEAD;

	return FI_SUCCESS;
}

static inline int __gnix_vec_insert_at(gnix_vector_t *vec,
				       gnix_vec_entry_t *entry,
				       gnix_vec_index_t index)
{
	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (OFI_UNLIKELY(index >= vec->attr.vec_maximum_size)) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Invalid parameter to "
			  "__gnix_vec_insert_at\n");
		return -FI_EINVAL;
	}

	if (OFI_UNLIKELY(vec->state == GNIX_VEC_STATE_DEAD)) {
		GNIX_FATAL(FI_LOG_EP_CTRL, "gnix_vector_t is in state "
			   "GNIX_VEC_STATE_DEAD in __gnix_vec_insert_at.\n");
	}

	if (index >= vec->attr.cur_size) {
		uint32_t new_size = __gnix_vec_get_new_size(vec, index);
		int ret = __gnix_vec_resize(vec, new_size);

		if (OFI_UNLIKELY(ret))
			return ret;
	}

	if (vec->vector[index]) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Existing element found in "
			  "__gnix_vec_insert_at\n");
		return -FI_ECANCELED;
	} else {
		vec->vector[index] = entry;
		return FI_SUCCESS;
	}
}

static inline int __gnix_vec_remove_at(gnix_vector_t *vec,
				       gnix_vec_index_t index)
{
	if (OFI_UNLIKELY(vec->state == GNIX_VEC_STATE_DEAD)) {
		GNIX_FATAL(FI_LOG_EP_CTRL, "gnix_vector_t is in state "
			   "GNIX_VEC_STATE_DEAD in __gnix_vec_remove_at.\n");
	} else if (index >= vec->attr.cur_size) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Index (%lu) too large in "
			  "__gnix_vec_remove_at\n", index);
		return -FI_EINVAL;
	} else {
		if (!vec->vector[index]) {
			GNIX_WARN(FI_LOG_EP_CTRL, "No entry exists in "
				  "__gnix_vec_remove_at\n");
			return -FI_ECANCELED;
		} else {
			vec->vector[index] = NULL;
		}
	}
	return FI_SUCCESS;
}

static inline int __gnix_vec_at(gnix_vector_t *vec, void **element,
				gnix_vec_index_t index)
{
	if (OFI_UNLIKELY(vec->state == GNIX_VEC_STATE_DEAD)) {
		GNIX_FATAL(FI_LOG_EP_CTRL, "gnix_vector_t is in state "
			   "GNIX_VEC_STATE_DEAD in __gnix_vec_at.\n");
	} else if (index >= vec->attr.cur_size) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Index (%lu) too large in "
			  "__gnix_vec_at\n", index);
		return -FI_EINVAL;
	} else {
		if (OFI_LIKELY((uint64_t) vec->vector[index])) {
			*element = vec->vector[index];
		} else {
			GNIX_DEBUG(FI_LOG_EP_CTRL, "There is no element at index "
				   "(%lu) in __gnix_vec_at\n", index);
			return -FI_ECANCELED;
		}
	}
	return FI_SUCCESS;
}

/*******************************************************************************
 * LOCKLESS FNS
 ******************************************************************************/
static int __gnix_vec_lf_init(gnix_vector_t *vec, gnix_vec_attr_t *attr)
{
	int ret;

	ret = __gnix_vec_create(vec, attr);
	vec->ops = &__gnix_vec_lockless_ops;
	vec->state = GNIX_VEC_STATE_READY;

	return ret;
}

static int __gnix_vec_lf_close(gnix_vector_t *vec)
{
	int ret;

	__gnix_vec_close_entries(vec);
	ret = __gnix_vec_close(vec);

	return ret;
}

static int __gnix_vec_lf_resize(gnix_vector_t *vec, uint32_t size)
{
	return __gnix_vec_resize(vec, size);
}

static int __gnix_vec_lf_insert_last(gnix_vector_t *vec,
			      gnix_vec_entry_t *entry)
{
	return __gnix_vec_insert_at(vec, entry, vec->attr.cur_size - 1);
}

static int __gnix_vec_lf_insert_at(gnix_vector_t *vec,
				   gnix_vec_entry_t *entry,
				   gnix_vec_index_t index)
{
	return __gnix_vec_insert_at(vec, entry, index);
}

static int __gnix_vec_lf_remove_last(gnix_vector_t *vec)
{
	return __gnix_vec_remove_at(vec, vec->attr.cur_size - 1);
}

static int __gnix_vec_lf_remove_at(gnix_vector_t *vec,
				   gnix_vec_index_t index)
{
	return __gnix_vec_remove_at(vec, index);
}

static int __gnix_vec_lf_last(gnix_vector_t *vec, void **element)
{
	return __gnix_vec_at(vec, element, vec->attr.cur_size - 1);
}

static int __gnix_vec_lf_at(gnix_vector_t *vec, void **element, gnix_vec_index_t index)
{
	return __gnix_vec_at(vec, element, index);
}

gnix_vec_entry_t *__gnix_vec_lf_iter_next(struct gnix_vector_iter *iter)
{
	uint32_t i;

	for (i = iter->cur_idx; i < iter->vec->attr.cur_size; i++) {
		if (iter->vec->vector[i]) {
			iter->cur_idx = i + 1;
			return iter->vec->vector[i];
		}
	}

	iter->cur_idx = iter->vec->attr.cur_size;
	return NULL;
}

/*******************************************************************************
 * LOCKED FNS
 ******************************************************************************/
static int __gnix_vec_lk_init(gnix_vector_t *vec, gnix_vec_attr_t *attr)
{
	int ret;

	rwlock_init(&vec->lock);
	ret = __gnix_vec_create(vec, attr);
	vec->ops = &__gnix_vec_locked_ops;
	vec->state = GNIX_VEC_STATE_READY;

	return ret;
}

static int __gnix_vec_lk_close(gnix_vector_t *vec)
{
	int ret;

	rwlock_wrlock(&vec->lock);

	__gnix_vec_close_entries(vec);
	ret = __gnix_vec_close(vec);

	rwlock_unlock(&vec->lock);

	rwlock_destroy(&vec->lock);

	return ret;
}

static int __gnix_vec_lk_resize(gnix_vector_t *vec, uint32_t size)
{
	int ret;

	rwlock_wrlock(&vec->lock);

	ret = __gnix_vec_resize(vec, size);

	rwlock_unlock(&vec->lock);

	return ret;
}

static int __gnix_vec_lk_insert_last(gnix_vector_t *vec,
			      gnix_vec_entry_t *entry)
{
	int ret;

	rwlock_wrlock(&vec->lock);

	ret = __gnix_vec_insert_at(vec, entry, vec->attr.cur_size - 1);

	rwlock_unlock(&vec->lock);

	return ret;
}

static int __gnix_vec_lk_insert_at(gnix_vector_t *vec,
				   gnix_vec_entry_t *entry,
				   gnix_vec_index_t index)
{
	int ret;

	rwlock_wrlock(&vec->lock);

	ret = __gnix_vec_insert_at(vec, entry, index);

	rwlock_unlock(&vec->lock);

	return ret;
}

static int __gnix_vec_lk_remove_last(gnix_vector_t *vec)
{
	int ret;

	rwlock_wrlock(&vec->lock);

	ret = __gnix_vec_remove_at(vec, vec->attr.cur_size - 1);

	rwlock_unlock(&vec->lock);

	return ret;
}

static int __gnix_vec_lk_remove_at(gnix_vector_t *vec,
				   gnix_vec_index_t index)
{
	int ret;

	rwlock_wrlock(&vec->lock);

	ret = __gnix_vec_remove_at(vec, index);

	rwlock_unlock(&vec->lock);

	return ret;
}

static int __gnix_vec_lk_last(gnix_vector_t *vec, void **element)
{
	int ret;

	rwlock_rdlock(&vec->lock);

	ret = __gnix_vec_at(vec, element, vec->attr.cur_size - 1);

	rwlock_unlock(&vec->lock);

	return ret;
}

static int __gnix_vec_lk_at(gnix_vector_t *vec, void **element, gnix_vec_index_t index)
{
	int ret;

	rwlock_rdlock(&vec->lock);

	ret = __gnix_vec_at(vec, element, index);

	rwlock_unlock(&vec->lock);

	return ret;
}

gnix_vec_entry_t *__gnix_vec_lk_iter_next(struct gnix_vector_iter *iter)
{
	uint32_t i;
	gnix_vec_entry_t *entry;

	rwlock_rdlock(&iter->vec->lock);

	for (i = iter->cur_idx; i < iter->vec->attr.cur_size; i++) {
		if (iter->vec->vector[i]) {
			iter->cur_idx = i + 1;
			entry = iter->vec->vector[i];
			rwlock_unlock(&iter->vec->lock);

			return entry;
		}
	}

	iter->cur_idx = iter->vec->attr.cur_size;

	rwlock_unlock(&iter->vec->lock);

	return NULL;
}

/*******************************************************************************
 * API FNS
 ******************************************************************************/
/**
 * Create the initial vector.  The user is responsible for initializing the
 * "attr" parameter prior to calling this function.
 *
 * @param[in] vec	the vector to initialize
 * @param[in] attr	the vector attributes
 *
 * @return FI_SUCCESS	Upon successfully creating the vector
 * @return -FI_EINVAL	Upon receiving an invalid parameter
 * @return -FI_ENOMEM	Upon insufficient memory to create the vector
 */
int _gnix_vec_init(struct gnix_vector *vec, gnix_vec_attr_t *attr)
{
	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (OFI_UNLIKELY(!vec || !attr ||
		     attr->vec_initial_size > attr->vec_maximum_size)) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Invalid parameter to _gnix_vec_init."
			  "\n");
		return -FI_EINVAL;
	}

	if (attr->vec_internal_locking == GNIX_VEC_LOCKED) {
		return __gnix_vec_lk_init(vec, attr);
	} else {
		return __gnix_vec_lf_init(vec, attr);
	}
}

/**
 * Close the vector elements and then the vector.
 *
 * @param[in] vec       the vector to close
 *
 * @return FI_SUCCESS	Upon successfully closing the vector
 * @return -FI_EINVAL	Upon a uninitialized or dead vector
 */
int _gnix_vec_close(gnix_vector_t *vec)
{
	GNIX_TRACE(FI_LOG_EP_CTRL, "\n");

	if (OFI_UNLIKELY(!vec)) {
		GNIX_WARN(FI_LOG_EP_CTRL, "Invalid parameter to _gnix_vec_close."
			  "\n");
		return -FI_EINVAL;
	} else {
		if (vec->attr.vec_internal_locking == GNIX_VEC_LOCKED) {
			return __gnix_vec_lk_close(vec);
		} else {
			return __gnix_vec_lf_close(vec);
		}
	}
}

static gnix_vector_ops_t __gnix_vec_lockless_ops = {
	.resize = __gnix_vec_lf_resize,

	.insert_last = __gnix_vec_lf_insert_last,
	.insert_at = __gnix_vec_lf_insert_at,

	.remove_last = __gnix_vec_lf_remove_last,
	.remove_at = __gnix_vec_lf_remove_at,

	.last = __gnix_vec_lf_last,
	.at = __gnix_vec_lf_at,

	.iter_next = __gnix_vec_lf_iter_next,
};

static gnix_vector_ops_t __gnix_vec_locked_ops = {
	.resize = __gnix_vec_lk_resize,

	.insert_last = __gnix_vec_lk_insert_last,
	.insert_at = __gnix_vec_lk_insert_at,

	.remove_last = __gnix_vec_lk_remove_last,
	.remove_at = __gnix_vec_lk_remove_at,

	.last = __gnix_vec_lk_last,
	.at = __gnix_vec_lk_at,

	.iter_next = __gnix_vec_lk_iter_next,
};
