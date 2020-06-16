#ifndef HOOK_PROV_H
#define HOOK_PROV_H

#include <ofi.h>
#include "ofi_hook.h"

int hook_bind(struct fid *fid, struct fid *bfid, uint64_t flags);
int hook_control(struct fid *fid, int command, void *arg);
int hook_ops_open(struct fid *fid, const char *name,
			 uint64_t flags, void **ops, void *context);
int hook_close(struct fid *fid);

#if HAVE_PERF
#include "hook_perf.h"
#else
#define perf_msg_ops hook_msg_ops
#define perf_rma_ops hook_rma_ops
#define perf_tagged_ops hook_tagged_ops
#define perf_cntr_ops hook_cntr_ops
#define perf_cq_ops hook_cq_ops

#define hook_perf_create hook_fabric_create
#define hook_perf_destroy hook_fabric_destroy

#endif /* HAVE_PERF */
#endif /* HOOK_PROV_H */
