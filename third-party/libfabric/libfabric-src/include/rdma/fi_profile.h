/*
 * Copyright (c) 2023 Intel Corporation, Inc.  All rights reserved.
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

#ifndef FI_PROFILE_H
#define FI_PROFILE_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <rdma/fi_domain.h>

/*
 * pre-defined profiling variables
 */
enum {
	FI_VAR_UNEXP_MSG_CNT,      // datatype: FI_UINT64
	FI_VAR_UNEXP_MSG_QUEUE,    // datatype: FI_TYPE_CQ_ERR_ENTRY
	FI_VAR_MSG_QUEUE_CNT,      // datatype: FI_UNIT64
	FI_VAR_CONNECTION_CNT,	   // datatype: FI_UNIT64
	FI_VAR_CONN_REQUEST,       // datatype: FI_UNIT64
	FI_VAR_CONN_ACCEPT,        // datatype: FI_UNIT64
	FI_VAR_CONN_REJECT,        // datatype: FI_UNIT64
	FI_VAR_OFI_MEM,            // datatype: FI_UINT64
};

/*
 * pre-defined events which can change the profiling variables
 */
enum {
	FI_EVENT_UNEXP_MSG_RECVD,    // var = FI_VAR_UNEXP_MSG_CNT
	FI_EVENT_UNEXP_MSG_MATCHED,  // var = FI_VAR_UNEXP_MSG_CNT
};

enum fi_profile_type {
	fi_primitive_type,
	fi_defined_type,
};

struct fi_profile_desc {
	uint32_t id;
	enum fi_profile_type datatype_sel;	
	union {
		enum fi_datatype primitive;
		enum fi_type defined;
	} datatype;
	uint64_t flags;
	size_t size;
	const char *name;
	const char *desc;
};

struct fid_profile;

struct fi_profile_ops {
	size_t size;
	void (*reset)(struct fid_profile *prof_fid,  uint64_t flags);
	ssize_t (*query_vars)(struct fid_profile *prof_fid,
	                      struct fi_profile_desc *varlist, size_t *count);
	ssize_t (*query_events)(struct fid_profile *prof_fid,
	                       struct fi_profile_desc *eventlist, size_t *count);
	ssize_t (*read_var)(struct fid_profile *prof_fid, uint32_t var_id, 
	                    void *data, size_t *size);
	int (*reg_callback)(struct fid_profile *prof_fid, uint32_t event_id,
	        int (*callback)(struct fid_profile *prof_fid,
	                struct fi_profile_desc *event, void *param, size_t size,
	                void *context),
	        void *context);
	void (*start_reads)(struct fid_profile *prof_fid, uint64_t flags);
	void (*end_reads)(struct fid_profile *prof_fid, uint64_t flags);
};
	
struct fid_profile {
	struct fid  fid;
	struct fi_profile_ops  *ops;
};

static inline void
fi_profile_reset(struct fid_profile *prof_fid,  uint64_t flags)
{
	return prof_fid->ops->reset(prof_fid, flags);
}
	

static inline ssize_t
fi_profile_query_vars(struct fid_profile *prof_fid,
                      struct fi_profile_desc *varlist, size_t *count)
{
	return prof_fid->ops->query_vars(prof_fid, varlist, count);
}

static inline ssize_t
fi_profile_query_events(struct fid_profile *prof_fid,
               struct fi_profile_desc *eventlist, size_t *count)
{
	return prof_fid->ops->query_events(prof_fid, eventlist, count);
}

static inline ssize_t
fi_profile_read_u64(struct fid_profile *prof_fid, uint32_t var_id,
                    uint64_t *data)
{
	size_t size = sizeof(uint64_t);
	ssize_t ret = prof_fid->ops->read_var(prof_fid, var_id, 
	                                      (void *)data, &size);
	return (ret > 0) ? 0 : ret;
}

static inline int
fi_profile_register_callback(struct fid_profile *prof_fid, uint32_t event_id,
        int (*callback)(struct fid_profile *prof_fid,
                        struct fi_profile_desc *event, void *param,
                        size_t size, void *context),
        void *context)
{
	return prof_fid->ops->reg_callback(prof_fid, event_id, callback, context);
}

static inline void
fi_profile_start_reads(struct fid_profile *prof_fid, uint64_t flags)
{
	return prof_fid->ops->start_reads(prof_fid, flags);
}

static inline void
fi_profile_end_reads(struct fid_profile *prof_fid, uint64_t flags)
{
	return prof_fid->ops->end_reads(prof_fid, flags);
}

static inline int
fi_profile_open(struct fid *fid, uint64_t flags,
                    struct fid_profile **prof_fid, void *context)
{
	struct fi_profile_ops *ops;
	int ret = fi_open_ops(fid, "fi_profile_ops", flags,
	                      (void **)&ops, context);
	if (!ret)
		*prof_fid = container_of(ops, struct fid_profile, ops);

	return ret;
}

static inline int
fi_profile_close(struct fid_profile *prof_fid)
{
	return prof_fid->fid.ops->close(&(prof_fid->fid));
}

#endif  /* FI_PROFILE_H */
