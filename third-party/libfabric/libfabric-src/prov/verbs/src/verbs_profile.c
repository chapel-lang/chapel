/*
 * Copyright (c) 2024 Intel Corporation. All rights reserved.
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

#include <rdma/fi_errno.h>
#include <rdma/fabric.h>


#include <ofi_prov.h>
#include "ofi.h"
#include "verbs_ofi.h"

#include <stdio.h>

#ifdef HAVE_FABRIC_PROFILE
#include <ofi_profile.h>


#define VRB_PROF_STR_LEN 64

/* current FI_LOG cannot log data more than 1024 bytes
 * so use fprintf for now.
 */
#define vrb_prof_log(buf)  fprintf(stderr, "%s\n", buf)

#define vrb_prof_get_mem()       ofi_prof_read_sys_var(FI_VAR_OFI_MEM)
#define vrb_prof_mem_var_name()  ((ofi_prof_var2_desc(NULL, FI_VAR_OFI_MEM))->name)

struct vrb_prof_state_entry {
	uint32_t size;
	uint32_t count;
	const char *desc;
	ofi_mutex_t mutex;
	struct vrb_prof_state_time *entry;
};

static struct vrb_prof_state_entry vrb_prof_state_time_table[] = {
	{
	 .size = 0,
	 .count = 0,
	 .desc = "ActiveConnection",
	 .entry = NULL
	},
	{
	 .size = 0,
	 .count = 0,
	 .desc = "PassiveConnection",
	 .entry = NULL
	},
};

struct vrb_prof_counter_entry {
	uint32_t size;
	uint32_t count;
	ofi_mutex_t mutex;
	struct vrb_prof_counter *counter;
};

static struct vrb_prof_counter_entry vrb_prof_counter_table;

#define VRB_FNAME_LEN  128
struct vrb_prof_time_entry {
        uint64_t start;
        uint64_t min_tm;
        uint64_t max_tm;
        uint64_t total;
        uint32_t count;
        char name[VRB_FNAME_LEN];
};

static bool vrb_prof_enabled = true;

static ofi_mutex_t vrb_prof_func_mutex;
static int vrb_prof_num_funcs = 0;
struct vrb_prof_time_entry vrb_prof_func_table[1024];


const char *vrb_state_str[] = {
	OFI_STR(VRB_IDLA),
	OFI_STR(VRB_RESOLVE_ADDR),
	OFI_STR(VRB_RESOLVE_ROUTE),
	OFI_STR(VRB_CONNECTING),
	OFI_STR(VRB_REQ_RCVD),
	OFI_STR(VRB_ACCEPTING),
	OFI_STR(VRB_CONNECTED),
	OFI_STR(VRB_DISCONNECTED),
};

static inline char *vrb_prof_cntr_name(uint32_t cntr)
{
	struct fi_profile_desc *desc;

	desc = ofi_prof_var2_desc(NULL, vrb_prof_cntr2_var(cntr));
	if (desc)
		return (char *)(desc->name);

	return NULL;
} 

static struct vrb_prof_counter *vrb_prof_get_vars()
{
	int ret = 0;
	struct vrb_prof_counter_entry *tab = &vrb_prof_counter_table;
	struct vrb_prof_counter *var = NULL;

	ofi_mutex_lock(&(tab->mutex));
	if (!tab->counter) {
		tab->size = 1024;
		tab->count = 0;
		tab->counter = calloc(tab->size,
					sizeof(struct vrb_prof_counter));
		if (!tab->counter) {
			ret=-ENOMEM;
		}
	}
	if (!ret && (tab->count == tab->size)) {
		tab->size += 1024;
		tab->counter = realloc(tab->counter, 
			sizeof(struct vrb_prof_counter) * tab->size);
		if (!tab->counter)
			ret = -ENOMEM;
	}
	if (!ret) 
		var = &(tab->counter[tab->count++]);
	ofi_mutex_unlock(&(tab->mutex));
	return var;
}

static struct vrb_prof_state_time *vrb_prof_get_state_time(int type)
{
	int ret = 0;
	struct vrb_prof_state_entry *tab = &(vrb_prof_state_time_table[type]);
	struct vrb_prof_state_time *entry = NULL;

	ofi_mutex_lock(&(tab->mutex));
	if (!tab->entry) {
		tab->size = 1024;
		tab->count = 0;
		tab->entry = calloc(tab->size, sizeof(struct vrb_prof_state_time));
		if (!tab->entry)
			ret=-ENOMEM;
	}
	if (!ret && (tab->count == tab->size)) {
		tab->size += 1024;
		tab->entry = realloc(tab->entry,  
			sizeof(struct vrb_prof_state_time) * tab->size);
		if (!tab->entry)
			ret = -ENOMEM;
	}
	if (!ret) 
		entry = &(tab->entry[tab->count++]);
	ofi_mutex_unlock(&(tab->mutex));
	return entry;
}

static int
vrb_prof_init_ep(struct fid *fid, uint64_t flags, void *context,
		 struct fi_profile_ops *ops, struct vrb_profile **vrb_prof)
{
	int ret = 0;
	struct util_profile *prof;
	uint64_t *cntrs;

	ret = vrb_prof_create(vrb_prof);
	if (ret)
		return ret;

	prof = &((*vrb_prof)->util_prof);
	prof->prov = &vrb_prov;
	ret = ofi_prof_init(prof, fid, flags, context, ops, 0, 0);
	if (ret) 
		goto err;

	cntrs = (*vrb_prof)->vars->cntr;
	ret = ofi_prof_add_var(prof, FI_VAR_MSG_QUEUE_CNT, NULL,
			&(cntrs[vrb_prof_var2_cntr(FI_VAR_MSG_QUEUE_CNT)]));
	ret = ofi_prof_add_var(prof, FI_VAR_CONNECTION_CNT, NULL,
			&(cntrs[vrb_prof_var2_cntr(FI_VAR_CONNECTION_CNT)]));
	ret = ofi_prof_add_var(prof, FI_VAR_CONN_REQUEST, NULL,
			&(cntrs[vrb_prof_var2_cntr(FI_VAR_CONN_REQUEST)]));
	ret = ofi_prof_add_var(prof, FI_VAR_CONN_ACCEPT, NULL,
			&(cntrs[vrb_prof_var2_cntr(FI_VAR_CONN_ACCEPT)]));
	ret = ofi_prof_add_var(prof, FI_VAR_CONN_REJECT, NULL,
			&(cntrs[vrb_prof_var2_cntr(FI_VAR_CONN_REJECT)]));

	FI_TRACE(&vrb_prov, FI_LOG_EP_CTRL,
		"vrb_profile_init: flags 0x%lx, "
		"common: vars %zu, events %zu\n",
		flags, ofi_common_var_count, ofi_common_event_count);

	FI_TRACE(&vrb_prov, FI_LOG_EP_CTRL,
		"vrb_profile_init: flags 0x%lx, "
		"total: vars %zu, events %zu\n",
		flags, prof->var_count, prof->event_count);

	return 0;

err:
	if (*vrb_prof)
		free(*vrb_prof);
	return ret;
}

static void vrb_prof_reset(struct fid_profile *prof_fid, uint64_t flags)
{
	struct util_profile *util_prof =
		 container_of(prof_fid, struct util_profile, prof_fid);

	ofi_prof_reset(util_prof, flags);
}

static ssize_t
vrb_prof_query_vars(struct fid_profile *prof_fid,
		    struct fi_profile_desc *varlist, size_t *count)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	return ofi_prof_query_vars(util_prof, varlist, count);
}

static ssize_t
vrb_prof_query_events(struct fid_profile *prof_fid,
		      struct fi_profile_desc *eventlist, size_t *count)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	return ofi_prof_query_events(util_prof, eventlist, count);
}

static int
vrb_prof_reg_cb(struct fid_profile *prof_fid, uint32_t event,
		ofi_prof_callback_t cb, void *context)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	return ofi_prof_reg_callback(util_prof, event, cb, context);
}

static ssize_t
vrb_prof_read_var(struct fid_profile *prof_fid, uint32_t var_id,
		  void *data, size_t *size)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	int idx = ofi_prof_id2_idx(var_id, ofi_common_var_count);

	if ((idx >= util_prof->varlist_size) ||
	    (!OFI_VAR_ENABLED(&util_prof->varlist[idx])))
		return -FI_EINVAL;
	
	if (OFI_VAR_DATATYPE_U64(&(util_prof->varlist[idx])))
		return ofi_prof_read_u64(util_prof, idx, data, size);
	
	if (OFI_PROF_DATA_CACHED(util_prof))
		return ofi_prof_read_cached_data(util_prof, idx, data, size);

	// support only primitive data for now.
	return 0;
}

static void vrb_prof_start_reads(struct fid_profile *prof_fid, uint64_t flags)
{
	int i;
	uint64_t size_u64 = sizeof(uint64_t);
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	// cache primitive data
	OFI_PROF_END_READS(util_prof);
	for (i = 0; i < util_prof->var_count; i++) {
		if (OFI_VAR_DATATYPE_U64(&(util_prof->varlist[i]))) {
			util_prof->data[i].size = 
			       ofi_prof_read_u64(util_prof, i,
						 &(util_prof->data[i].value.u64),
						 &size_u64);
		}
	}
	OFI_PROF_START_READS(util_prof);
}

static void vrb_prof_end_reads(struct fid_profile *prof_fid, uint64_t flags)
{
	struct util_profile *util_prof =
		container_of(prof_fid, struct util_profile, prof_fid);

	OFI_PROF_END_READS(util_prof);
}

static struct fi_profile_ops vrb_prof_ep_ops =  {
	.size = sizeof(struct fi_profile_ops),
	.reset = vrb_prof_reset,
	.query_vars = vrb_prof_query_vars,
	.query_events = vrb_prof_query_events,
	.read_var = vrb_prof_read_var,
	.reg_callback = vrb_prof_reg_cb,
	.start_reads = vrb_prof_start_reads,
	.end_reads = vrb_prof_end_reads,
};

/* 
 * This is for enabling profile API
 */
int vrb_ep_ops_open(struct fid *fid, const char *name,
		     uint64_t flags, void **ops, void *context)
{

	int ret = 0;
	struct vrb_profile *vrb_prof;
	struct vrb_ep *ep;
	struct vrb_domain *dom;

	if (!strcmp(name, "fi_profile_ops") && 
	    ((fid->fclass == FI_CLASS_EP) ||
	     (fid->fclass == FI_CLASS_DOMAIN))) {
		ret = vrb_prof_init_ep(fid, flags, context,
				    &vrb_prof_ep_ops, &vrb_prof);
		if (ret)
			return ret;
		*ops = &(vrb_prof->util_prof.prof_fid.ops);
		if (fid->fclass == FI_CLASS_EP) {
			ep = container_of(fid, struct vrb_ep,
					  util_ep.ep_fid.fid);
			ep->profile = vrb_prof;
		} else {
			dom = container_of(fid, struct vrb_domain, 
					   util_domain.domain_fid.fid);
			dom->profile = vrb_prof;
		}
		return ret;
	}
	FI_WARN(&vrb_prov, FI_LOG_EP_CTRL, "unsupported ep ops <%s>\n", name);
	return -FI_ENOSYS;
}

#define VRB_PROF_STR_FORMAT5 "  %-24s%-16s%-16s%-16s%-16s%-16s\n"
#define VRB_PROF_DATA_FORMAT5 "  %-24s%-16lu%-16lu%-16lu%-16lu%-16lu\n"

static void vrb_prof_report_counter(char *buf, size_t len)
{
	uint64_t total_v[VRB_PROF_VARS];
	struct vrb_prof_counter *cpt = vrb_prof_counter_table.counter;
	int i, j;
	uint64_t mem_size = vrb_prof_get_mem();
	float mb = 1.0 * (1 << 20);

	if (mem_size) {
		ofi_strncatf(buf, len, "%s:\n", "Memory"); 
		ofi_strncatf(buf, len, "  %-24s %.2f\n", 
			vrb_prof_mem_var_name(), (mem_size / mb));
	}

	for (i = 0; i < VRB_PROF_VARS; i++) {
		total_v[i] = 0;
	}

	for (j = 0; j < vrb_prof_counter_table.count; j++) {
		for (i = 0; i < VRB_PROF_VARS; i++) {
			total_v[i] += cpt[j].cntr[i];
		}
	}

        ofi_strncatf(buf, len, "Counter:\n");
	for (i = 0; i < VRB_PROF_VARS; i++) {
		ofi_strncatf(buf, len, "  %-24s %lu\n", 
			vrb_prof_cntr_name(i), total_v[i]);
	}
}


static void vrb_prof_report_state_time(char *buf, size_t len, int type)
{
	uint64_t min_v[VRB_MAX_STATES];
	uint64_t max_v[VRB_MAX_STATES];
	uint64_t total_v[VRB_MAX_STATES];
	uint64_t total_c[VRB_MAX_STATES];
	bool  has_data = false;
	int i;
	int j;
	struct vrb_prof_state_entry *tab = &(vrb_prof_state_time_table[type]);
	struct vrb_prof_state_time *tp;

	if (!tab->count)
		return;

	for (j = 0; j < VRB_MAX_STATES; j++) {
		min_v[j] =  UINT64_MAX;
		max_v[j] = 0;
		total_v[j] = 0;
		total_c[j] = 0;
	}
	
	for (i = 0; i < tab->count; i++) {
		if (!tab->entry[i].start)
			continue;
		tp = &(tab->entry[i]);
		for (j=0; j < VRB_MAX_STATES; j++) {
			if (!tp->time[j]) 
				continue;
			min_v[j] = MIN(tp->time[j], min_v[j]);
			max_v[j] = MAX(tp->time[j], max_v[j]);
                        total_v[j] += tp->time[j];
                        total_c[j]++;
                        has_data = true;
		}
	}

	if (!has_data)
		return;

	ofi_strncatf(buf, len, "%s\n", tab->desc);
	ofi_strncatf(buf, len, VRB_PROF_STR_FORMAT5, 
	       "state", "min(ns)", "max(ns)", "avg(ns)", "total(ns)", "count");
	for (j=0; j < VRB_MAX_STATES; j++) {
		if (!total_c[j])
			continue;
		ofi_strncatf(buf, len, VRB_PROF_DATA_FORMAT5,
			vrb_state_str[j], min_v[j], max_v[j],
			total_v[j]/total_c[j], total_v[j], total_c[j]);
	}
}

#define VRB_PROF_STR_FORMAT4 "  %-24s%-16s%-16s%-16s%-16s\n"
#define VRB_PROF_DATA_FORMAT4 "  %-24s%-16lu%-16lu%-16lu%-16u\n"

static void vrb_prof_report_func_time(char *buf, size_t len)
{
	int i;
	struct vrb_prof_time_entry *entry;

	if (!vrb_prof_num_funcs)
		return;
	ofi_strncatf(buf, len, "Function\n");	
	ofi_strncatf(buf, len, VRB_PROF_STR_FORMAT4,
		"Name", "min(ns)", "max(ns)", "avg(ns)", "count");
	for (i = 0; i < vrb_prof_num_funcs; i++) {
		entry = &(vrb_prof_func_table[i]);
		if (!entry->count) 
			continue;
		ofi_strncatf(buf, len, VRB_PROF_DATA_FORMAT4,
			     entry->name, entry->min_tm, entry->max_tm, 
			     (entry->total/entry->count), entry->count);
	}
}

int vrb_prof_create(struct vrb_profile **prof)
{

	if (!vrb_prof_enabled) {
		*prof = NULL;
		return ENODATA;
	}

	*prof =  calloc(1, sizeof(vrb_profile_t));
	if (!*prof)
		return -ENOMEM;

	(*prof)->vars = vrb_prof_get_vars();
	if (!(*prof)->vars) {
		free(*prof);
		*prof = NULL;
		return -ENOMEM;
	}
	return 0;
}

void vrb_prof_init_state(struct vrb_profile *prof, uint64_t cur_time, int type)
{
	prof->state = vrb_prof_get_state_time(type);
	if (prof->state)
		prof->state->start = cur_time;
}

static inline bool vrb_prof_find_func(const char *fname, bool is_add, int *idx)
{
	int i;
	
	ofi_mutex_lock(&vrb_prof_func_mutex);
	for (i = vrb_prof_num_funcs - 1; i >= 0; i--) {
		if (!strncmp(vrb_prof_func_table[i].name, fname, strlen(fname))) {
			*idx = i;
			ofi_mutex_unlock(&vrb_prof_func_mutex);
			return true;
		}
	}
	if (is_add) {
		memset(&(vrb_prof_func_table[vrb_prof_num_funcs]), 0,
		       sizeof(struct vrb_prof_time_entry));
		strncpy(vrb_prof_func_table[vrb_prof_num_funcs].name, 
			fname, VRB_FNAME_LEN);
		vrb_prof_func_table[vrb_prof_num_funcs].min_tm = UINT64_MAX;
		*idx = vrb_prof_num_funcs++;
	}
	ofi_mutex_unlock(&vrb_prof_func_mutex);
	return false;
}

void vrb_prof_func_start(const char *fname)
{
	int idx;
	struct vrb_prof_time_entry *entry;

	if (!vrb_prof_enabled)
		return;

	vrb_prof_find_func(fname, 1, &idx);
	entry = &(vrb_prof_func_table[idx]);
	entry->start = ofi_gettime_ns();
}

void vrb_prof_func_end(const char *fname)
{
	uint64_t cur_time = ofi_gettime_ns();
	int idx;
	struct vrb_prof_time_entry *entry;
	uint64_t dur;

	if (!vrb_prof_enabled)
		return;

	if (!vrb_prof_find_func(fname, 0, &idx) || 
	    !(vrb_prof_func_table[idx].start))
		return;

	entry = &(vrb_prof_func_table[idx]);
	dur = cur_time - entry->start;
	entry->min_tm = MIN(dur, entry->min_tm);
	entry->max_tm = MAX(dur, entry->max_tm);
	entry->total += dur;
	entry->count++;
}

// This is called in the verbs provider VERBS_INI time.
void vrb_prof_init()
{
	int size = (sizeof(vrb_prof_state_time_table) /
		   sizeof(struct vrb_prof_state_entry));
	int i;
	char *envstr;
	
	envstr = getenv("FI_DISENABLE_PROF");
	if (envstr && strstr(envstr, "verbs")) {
		vrb_prof_enabled = false;
	}
		
	ofi_prof_sys_init();

	for (i = 0; i < size; i++)
		ofi_mutex_init(&(vrb_prof_state_time_table[i].mutex));

	ofi_mutex_init(&vrb_prof_counter_table.mutex);
	ofi_mutex_init(&vrb_prof_func_mutex);
}

void vrb_prof_report(struct vrb_profile *prof)
{
	char *buf;
	char *envstr;
	size_t bufsize = 4096;
	OFI_UNUSED(prof);
	
	if (!vrb_prof_enabled ||
	    !fi_log_enabled(&vrb_prov, FI_LOG_TRACE, FI_LOG_CORE))
		return;

	envstr = getenv("FI_PROF_DATA_SIZE");
	if (envstr)
		bufsize = atoi(envstr);

	buf = calloc(bufsize, sizeof(char));
	if (!buf) {
		FI_WARN(&vrb_prov, FI_LOG_CORE,
			"Unable to allocate report data buf\n");
		return;
	}

	buf[0] = '\n';
	vrb_prof_report_counter(buf, bufsize);
	vrb_prof_report_state_time(buf, bufsize, VRB_ACTIVE_CONN);
	vrb_prof_report_state_time(buf, bufsize, VRB_PASSIVE_CONN);
	vrb_prof_report_func_time(buf, bufsize);
	vrb_prof_log(buf);
	free(buf);
}

#else

int vrb_ep_ops_open(struct fid *fid, const char *name,
		     uint64_t flags, void **ops, void *context)
{
	OFI_UNUSED(fid);
	OFI_UNUSED(name);
	OFI_UNUSED(flags);
	OFI_UNUSED(ops);
	OFI_UNUSED(context);
	return -FI_ENOSYS;
}


int vrb_prof_create(vrb_profile_t **prof)
{
	OFI_UNUSED(prof);
	return -FI_ENOSYS;
}

#endif


