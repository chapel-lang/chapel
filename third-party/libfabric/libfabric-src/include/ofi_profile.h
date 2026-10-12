/*
 * Copyright (c) 2023 Intel Corporation. All rights reserved.
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
 *	copyright notice, this list of conditions and the following
 *	disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *	copyright notice, this list of conditions and the following
 *	disclaimer in the documentation and/or other materials
 *	provided with the distribution.
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

#ifndef _OFI_PROFILE_H_
#define _OFI_PROFILE_H_

#include <string.h>
#include <stdint.h>
#include <assert.h>

#include <ofi_str.h>

#include <rdma/fabric.h>
#include <rdma/fi_profile.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * variable and event id is uint32_t.
 * last 16 bits is for an unique number, upper bits is FI_PROV_SPECFIC_XXX
 * (refer fi_ext.h)
 * common pre-defined variables/events will be <idx>
 * provider-defined variables/events will be -FI_PROV_SPECIFIC_XXX + <idx>
 */
#define OFI_PROF_ID_MASK     0xffff       // bit 15-0

extern size_t ofi_common_var_count;
extern size_t ofi_common_event_count;

typedef int (*ofi_prof_callback_t)(struct fid_profile *prof_fid,
				   struct fi_profile_desc *event, void *param,
				   size_t size, void *context);

#define OFI_VAR_DATATYPE_U64(desc)     \
	(((desc)->datatype_sel == fi_primitive_type) && \
	 ((desc)->datatype.primitive <= FI_UINT64))

#define OFI_VAR_ENABLED(desc)		((desc)->name)
#define OFI_EVENT_ENABLED(desc)		((desc)->name)

struct util_pcb {
	ofi_prof_callback_t cb;
	void *context;
};

struct util_prof_data {
	union {
		uint64_t u64;
		void *pt;
	} value;
	size_t size;
};

/*
 * varilist: contains variables
 *    [0, ofi_common_var_count-1]: pre-defined common variables.
 *    [ofi_common_var_count, ...]: provider-specific variables.
 * eventlist: contains events
 *    [0, ofi_common_event_count-1]: pre-defined common events.
 *    [ofi_common_event_count, ...]: provider-specific events.
 */
struct util_profile {
	struct fid_profile prof_fid;
	struct fid *fid;
	struct fi_provider *prov;

	uint64_t flags;

	size_t varlist_size;
	size_t var_count;
	struct fi_profile_desc *varlist;
	void **vars;
	struct util_prof_data *data;
	bool data_cached;

	size_t eventlist_size;
	size_t event_count;
	struct fi_profile_desc *eventlist;
	struct util_pcb *pcb;
};

#define OFI_PROF_DATA_CACHED(prof)      (prof)->data_cached
#define OFI_PROF_START_READS(prof)    \
	do { (prof)->data_cached = true; } while (0)
#define OFI_PROF_END_READS(prof)      \
	do { (prof)->data_cached = false; } while (0)

// for both variable index or event index
static inline int
ofi_prof_id2_idx(uint32_t id, size_t common_desc_count)
{
	int idx = id & OFI_PROF_ID_MASK;

	return (id >> 16) ? (common_desc_count + idx) : idx;
}

static inline void
ofi_prof_event_notify(struct util_profile *prof, uint32_t event,
		      void *param, size_t size)
{
	int idx = ofi_prof_id2_idx(event, ofi_common_event_count);
	struct util_pcb *pcb;

	if ((idx < prof->eventlist_size) &&
	    (OFI_EVENT_ENABLED(&(prof->eventlist[idx])))) {
		pcb = &(prof->pcb[idx]);
		pcb->cb(&(prof->prof_fid), &(prof->eventlist[idx]),
			param, size, pcb->context);
	}
}

static inline uint64_t
ofi_var_data_u64(void *var, enum fi_datatype type)
{
	switch(type) {
	case FI_INT8:
		return (uint64_t)(*(int8_t *)var);
	case FI_UINT8:
		return (uint64_t)(*(uint8_t *)var);
	case FI_INT16:
		return (uint64_t)(*(int16_t *)var);
	case FI_UINT16:
		return (uint64_t)(*(uint16_t *)var);
	case FI_INT32:
		return (uint64_t)(*(int32_t *)var);
	case FI_UINT32:
		return (uint64_t)(*(uint32_t *)var);
	case FI_INT64:
		return (uint64_t)(*(int64_t *)var);
	case FI_UINT64:
		return (uint64_t)(*(uint64_t *)var);
	default:
		break;
	}
	return 0;
}

static inline uint64_t
ofi_var_data_atomic64(void *var)
{
	 return (uint64_t)ofi_atomic_get64((ofi_atomic64_t *)var);
}

static inline ssize_t
ofi_prof_read_u64(struct util_profile *prof, int idx,
		  void *data, size_t *size)
{
	uint64_t *val64 = (uint64_t *)data;

	if (*size < sizeof(uint64_t))
		return -FI_ETOOSMALL;

	if (prof->data_cached) {
		*val64 = prof->data[idx].value.u64;
	} else if (prof->varlist[idx].datatype_sel == fi_primitive_type) {
		*val64 = ofi_var_data_u64(prof->vars[idx],
					  prof->varlist[idx].datatype.primitive);
	} else if (prof->varlist[idx].datatype.defined ==  FI_TYPE_ATOMIC_TYPE) {
		*val64 = ofi_var_data_atomic64(prof->vars[idx]);
	} else {
		*val64 = 0;
	}

	return sizeof(uint64_t);
}

static inline ssize_t
ofi_prof_read_cached_data(struct util_profile *prof, int idx,
			  void *data, size_t *size)
{
	if (*size < prof->data[idx].size)
		return -FI_ETOOSMALL;

	memcpy(data, prof->data[idx].value.pt, prof->data[idx].size);
	return prof->data[idx].size;
}

struct fi_profile_desc *
ofi_prof_event2_desc(struct util_profile *prof, uint32_t event_id);

struct fi_profile_desc *
ofi_prof_var2_desc(struct util_profile *prof, uint32_t var_id);

int ofi_prof_init(struct util_profile *prof, struct fid *fid,
		  uint64_t flags, void *context, struct fi_profile_ops *ops,
		  int prov_vars_size, int prov_events_size);

void ofi_prof_reset(struct util_profile *prof, uint64_t flags);
ssize_t ofi_prof_query_vars(struct util_profile *prof,
			    struct fi_profile_desc *varlist, size_t *count);
ssize_t ofi_prof_query_events(struct util_profile *prof,
			      struct fi_profile_desc *eventlist, size_t *count);
int ofi_prof_reg_callback(struct util_profile *prof, uint32_t event,
			  ofi_prof_callback_t cb, void *context);

void ofi_prof_add_common_vars(struct util_profile *prof);
void ofi_prof_add_common_events(struct util_profile *prof);
int ofi_prof_add_var(struct util_profile *prof, uint32_t var_id,
		     struct fi_profile_desc *desc, void *var);
int ofi_prof_add_event(struct util_profile *prof, uint32_t event_id,
		       struct fi_profile_desc *desc);

int ofi_prof_pcb_noop(struct fid_profile *prof_fid, 
		      struct fi_profile_desc *event, void *param,
		      size_t size, void *context);


//  May be associated to a fabric or a domain.
// for now, use as global, not associated with a profile instance.
void ofi_prof_sys_init();
void ofi_prof_inc_sys_var(uint32_t var_id, int64_t val);
uint64_t ofi_prof_read_sys_var(uint32_t var_id);

#ifdef __cplusplus
}
#endif

#endif /* _OFI_PROFILE_H_ */
