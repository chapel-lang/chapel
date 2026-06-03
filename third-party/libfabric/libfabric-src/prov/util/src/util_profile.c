/*
 * Copyright (c) 2023, Intel Corporation. All rights reserved.
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
 *
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <ofi.h>
#include <ofi_profile.h>

#define PROF_LIST_SIZE	64

#define OFI_PROF_DESC_SET(dest, src)		 \
	do {					     \
		(dest)->id = (src)->id;		      \
		(dest)->datatype_sel = (src)->datatype_sel;  \
		(dest)->datatype = (src)->datatype;	  \
		(dest)->flags = (src)->flags;		\
		(dest)->size = (src)->size;		  \
		(dest)->name = (src)->name;		  \
		(dest)->desc = (src)->desc;		  \
	} while (0)

enum fi_prof_var_scope {
	FI_PROV_VAR,
	FI_SYS_VAR,
};

struct fi_profile_desc  ofi_common_vars[] = {
	{
	 .id = FI_VAR_UNEXP_MSG_CNT,
	 .datatype_sel = fi_primitive_type,
	 .datatype.primitive = FI_UINT64,
	 .flags = 0,
	 .size = 8,
	 .name = "pvar_unexp_msg_cnt",
	 .desc = "Unexpected Receive Message Count"
	},
	{
	 .id = FI_VAR_UNEXP_MSG_QUEUE,
	 .datatype_sel = fi_defined_type,
	 .datatype.defined = FI_TYPE_CQ_ERR_ENTRY,
	 .flags = 0,
	 .size = sizeof(struct fi_cq_err_entry),
	 .name = "pvar_unexp_msg_queue",
	 .desc = "Unexpected Receive Message Queue"
	},
	{
	 .id = FI_VAR_MSG_QUEUE_CNT,
	 .datatype_sel = fi_primitive_type,
	 .datatype.primitive = FI_UINT64,
	 .flags = 0,
	 .size = 8,
	 .name = "pvar_msg_queue_cnt",
	 .desc = "Number of message queues created"
	},
	{
	 .id = FI_VAR_CONNECTION_CNT,
	 .datatype_sel = fi_primitive_type,
	 .datatype.primitive = FI_UINT64,
	 .flags = 0,
	 .size = 8,
	 .name = "pvar_connection_cnt",
	 .desc = "Number of connection established"
	},
	{
	 .id = FI_VAR_CONN_REQUEST,
	 .datatype_sel = fi_primitive_type,
	 .datatype.primitive = FI_UINT64,
	 .flags = 0,
	 .size = 8,
	 .name = "pvar_conn_request",
	 .desc = "Number of connection requests issued"
	},
	{
	 .id = FI_VAR_CONN_ACCEPT,
	 .datatype_sel = fi_primitive_type,
	 .datatype.primitive = FI_UINT64,
	 .flags = 0,
	 .size = 8,
	 .name = "pvar_conn_accept",
	 .desc = "Number of connection requests accepted"
	},
	{
	 .id = FI_VAR_CONN_REJECT,
	 .datatype_sel = fi_primitive_type,
	 .datatype.primitive = FI_UINT64,
	 .flags = 0,
	 .size = 8,
	 .name = "pvar_conn_reject",
	 .desc = "Number of connection requests rejected"
	},
	{
	 .id = FI_VAR_OFI_MEM,
	 .datatype_sel = fi_defined_type,
	 .datatype.defined = FI_TYPE_ATOMIC_TYPE,
	 .flags = FI_SYS_VAR,
	 .size = 8,
	 .name = "pvar_ofi_mem_alloc(MB)",
	 .desc = "Memory pools allocated by OFI"
	},
};

struct fi_profile_desc  ofi_common_events[] = {
	{
	 .id = FI_EVENT_UNEXP_MSG_RECVD,
	 .datatype_sel = fi_defined_type,
	 .datatype.defined = FI_TYPE_CQ_ERR_ENTRY,
	 .flags = 0,
	 .size = sizeof(struct fi_cq_err_entry),
	 .name = "pevent_unexp_msg_recd",
	 .desc = "Unexpected Message Received"
	},
	{
	 .id = FI_EVENT_UNEXP_MSG_MATCHED,
	 .datatype_sel = fi_defined_type,
	 .datatype.defined = FI_TYPE_CQ_ERR_ENTRY,
	 .flags = 0,
	 .size = sizeof(struct fi_cq_err_entry),
	 .name = "pevent_unexp_msg_matched",
	 .desc = "Unexpected Message Matched"
	},
};

size_t ofi_common_var_count = ARRAY_SIZE(ofi_common_vars);
size_t ofi_common_event_count = ARRAY_SIZE(ofi_common_events);

enum {
	SYS_VAR_MEM = 0,
};

static ofi_atomic64_t  ofi_sys_vars[1];
size_t ofi_sys_var_count = ARRAY_SIZE(ofi_sys_vars);

static bool ofi_sys_var_enabled = false;

static inline int
ofi_prof_var2_sys_idx(uint32_t var_id)
{
	if (ofi_sys_var_enabled) {
		switch (var_id) {
		case FI_VAR_OFI_MEM:
			return SYS_VAR_MEM;
		default:
			break;
		}
	}

	return -1;
}

struct fi_profile_desc *
ofi_prof_var2_desc(struct util_profile *prof, uint32_t var_id)
{
        int idx;

        idx = ofi_prof_id2_idx(var_id, ofi_common_var_count);
	if (!prof) { // common var, not specified on an profile instance
		if (idx < ofi_common_var_count)
			return &(ofi_common_vars[var_id]);
		else
			return NULL;
	}

        if ((idx < prof->varlist_size) &&
            (OFI_VAR_ENABLED(&(prof->varlist[idx])))) {
                return &(prof->varlist[idx]);
        } else {
                return NULL;
        }
}

struct fi_profile_desc *
ofi_prof_event2_desc(struct util_profile *prof, uint32_t event_id)
{
        int idx = ofi_prof_id2_idx(event_id, ofi_common_event_count);
        if ((idx < prof->eventlist_size) &&
            (OFI_EVENT_ENABLED(&(prof->eventlist[idx])))) {
                return &(prof->eventlist[idx]);
        } else {
                return NULL;
        }
}

void ofi_prof_sys_init()
{
	for (int i = 0; i < ofi_sys_var_count; i++)
                ofi_atomic_initialize64(&ofi_sys_vars[i], 0);

	ofi_sys_var_enabled = true;
}

void ofi_prof_inc_sys_var(uint32_t var_id, int64_t val)
{
	int idx = ofi_prof_var2_sys_idx(var_id);
	if (idx >= 0)
		ofi_atomic_add64(&(ofi_sys_vars[idx]), val);
}

uint64_t ofi_prof_read_sys_var(uint32_t var_id)
{
	int idx = ofi_prof_var2_sys_idx(var_id);

	if (idx >= 0)
		return (uint64_t)ofi_atomic_get64(&(ofi_sys_vars[idx]));

	return 0;
}

int ofi_prof_pcb_noop(struct fid_profile *prof_fid,
		      struct fi_profile_desc *event,
		      void *param, size_t size, void *context)
{
	OFI_UNUSED(prof_fid);
	OFI_UNUSED(event);
	OFI_UNUSED(param);
	OFI_UNUSED(size);
	OFI_UNUSED(context);
	return -FI_ENOSYS;
}

int ofi_prof_init(struct util_profile *prof, struct fid *fid,
		  uint64_t flags, void *context, struct fi_profile_ops *ops,
		  int prov_vars_size, int prov_events_size)
{
	prof->prof_fid.fid.fclass = FI_CLASS_PROFILE;
	prof->prof_fid.fid.context = context;
	prof->prof_fid.ops = ops;
	prof->fid = fid;
	prof->flags = flags;

	prof->var_count = 0;
	prof->event_count = 0;

	prof->data_cached = false;
	prof->varlist_size = ofi_common_var_count + prov_vars_size;
	prof->vars = (void **)calloc(prof->varlist_size, sizeof(void *));
	prof->data = (void *)calloc(prof->varlist_size, 
				    sizeof(struct util_prof_data));
	prof->varlist = 
		(struct fi_profile_desc *)calloc(prof->varlist_size,
						 sizeof(struct fi_profile_desc));
	if (!prof->varlist || !prof->vars || !prof->data)
		goto errend;

	prof->eventlist_size = ofi_common_event_count + prov_events_size;
	prof->eventlist =
	    (struct fi_profile_desc *)calloc(prof->eventlist_size,
					     sizeof(struct fi_profile_desc));
	prof->pcb = (struct util_pcb *)calloc(prof->eventlist_size,
					      sizeof(struct util_pcb));
	if (!prof->eventlist || !prof->pcb)
		goto errend;

	for (int i = 0; i < prof->eventlist_size; i++) 
		prof->pcb[i].cb = ofi_prof_pcb_noop;

	return 0;

errend:
	free(prof->varlist);
	free(prof->vars);
	free(prof->eventlist);
	free(prof->pcb);

	FI_WARN(prof->prov, FI_LOG_CORE, "connot allocate memory.\n");
	return -FI_ENOMEM;
}

void ofi_prof_reset(struct util_profile *prof, uint64_t flags)
{
	prof->flags = flags;
}

ssize_t ofi_prof_query_vars(struct util_profile *prof,
			    struct fi_profile_desc *varlist, size_t *count)
{
	size_t idx = 0;
	size_t cnt;

	if (!varlist) {
		*count = prof->var_count;
		return 0;
	}

	cnt = (prof->var_count > *count) ? *count : prof->var_count;

	for (size_t i = 0;
	     (i < prof->varlist_size) && (idx < cnt); i++) {
		if (OFI_VAR_ENABLED(&prof->varlist[i])) {
			OFI_PROF_DESC_SET(&varlist[idx], &(prof->varlist[i]));
			idx++;
		}
	}
	*count = prof->var_count;

	return cnt;
}
		
ssize_t ofi_prof_query_events(struct util_profile *prof,
			      struct fi_profile_desc *eventlist, size_t *count)
{
	size_t idx = 0;
	size_t cnt;

	if (!eventlist) {
		*count = prof->event_count;
		return 0;
	}

	cnt = (prof->event_count > *count) ? *count : prof->event_count;
	
	for (size_t i = 0;
	     (i < prof->eventlist_size) && (idx < cnt); i++) {
		if (OFI_EVENT_ENABLED(&prof->eventlist[i])) {
			OFI_PROF_DESC_SET(&eventlist[idx], &prof->eventlist[i]);
			idx++;
		}
	}
	*count = prof->event_count;

	return cnt;
}

void ofi_prof_add_common_vars(struct util_profile *prof)
{
	int i;

	for (i = 0; i < ofi_common_var_count; i++) 
		OFI_PROF_DESC_SET(&(prof->varlist[i]), &(ofi_common_vars[i]));

	prof->var_count += ofi_common_var_count;
	
}

void ofi_prof_add_common_events(struct util_profile *prof)
{
	int i;

	for (i = 0; i < ofi_common_event_count; i++) 
		OFI_PROF_DESC_SET(&(prof->eventlist[i]), &(ofi_common_events[i]));

	prof->event_count += ofi_common_event_count;
}

int ofi_prof_add_var(struct util_profile *prof, uint32_t var_id,
		     struct fi_profile_desc *desc, void *var)
{
	int sys_idx;
	int idx = ofi_prof_id2_idx(var_id, ofi_common_var_count);

	if (!desc) {
		if  (idx < ofi_common_var_count) {
			desc = &ofi_common_vars[idx];
		} else if ((idx > prof->varlist_size) ||
			   !OFI_VAR_ENABLED(&prof->varlist[idx])) {
			FI_WARN(prof->prov, FI_LOG_CORE,
				"No descripton for provider-specific "
				"variable %u\n", var_id);
			return -FI_EINVAL;
		}
	}

	while (idx >=  prof->varlist_size) {
		prof->varlist_size += PROF_LIST_SIZE;
		prof->varlist = realloc(prof->varlist, 
					prof->varlist_size * 
					sizeof(struct fi_profile_desc));
		prof->vars = realloc(prof->vars, 
				     prof->varlist_size * sizeof(void *));
		prof->data = realloc(prof->data, 
				     prof->varlist_size * 
				     sizeof(struct util_prof_data));

		if (!prof->varlist || !prof->vars || !prof->data) {
			FI_WARN(prof->prov, FI_LOG_CORE,
				"connot re-allocate memory.\n");
			return -FI_ENOMEM;
		}
	}
	if (!OFI_VAR_ENABLED(&prof->varlist[idx])) {
		OFI_PROF_DESC_SET(&prof->varlist[idx], desc);
		prof->var_count++;
	}

	if ((desc->flags & 0x01) && !var ) {
		sys_idx = (desc->flags >> 32);
		prof->vars[idx] = (void *)&(ofi_sys_vars[sys_idx]);
	} else  {
		prof->vars[idx] = var;
	}

	return 0;
}

int ofi_prof_add_event(struct util_profile *prof, uint32_t event_id,
		       struct fi_profile_desc *desc)
{
	int idx = ofi_prof_id2_idx(event_id, ofi_common_event_count);

	if (!desc) {
		if (idx < ofi_common_event_count) {
			desc = &ofi_common_events[idx];
		} else {
			FI_WARN(prof->prov, FI_LOG_CORE,
				"No descripton for provider-specific event %u\n",
				event_id);
			return -FI_EINVAL;
		}
	}

	while (idx >= prof->eventlist_size) {
		prof->eventlist_size += PROF_LIST_SIZE;
		prof->eventlist = realloc(prof->eventlist, 
					  prof->eventlist_size * 
					  sizeof(struct fi_profile_desc));
		prof->pcb = realloc(prof->pcb,
				    prof->eventlist_size * sizeof(void *));

		if (!prof->eventlist || !prof->pcb) {
			FI_WARN(prof->prov, FI_LOG_CORE,
				"connot re-allocate memory.\n");
			return -FI_ENOMEM;
		}
	}
	OFI_PROF_DESC_SET(&prof->eventlist[idx], desc);
	prof->event_count++;

	return 0;
}

int ofi_prof_reg_callback(struct util_profile *prof, uint32_t event,
			  ofi_prof_callback_t cb, void *context)
{
	int idx = ofi_prof_id2_idx(event, ofi_common_event_count);

	assert(idx >= 0);

	prof->pcb[idx].cb = cb;
	prof->pcb[idx].context = context;

	return 0;
}

