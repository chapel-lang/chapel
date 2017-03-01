//
// Copyright 2012, 2014 Cray Inc.  All Rights Reserved.
//
// U.S. GOVERNMENT RESTRICTED RIGHTS NOTICE
//
// The Computer Software is delivered as "Commercial Computer Software"
// as defined in DFARS 48 CFR 252.227-7014.
//
// All Computer Software and Computer Software Documentation acquired by
// or for the U.S. Government is provided with Restricted Rights.  Use,
// duplication or disclosure by the U.S. Government is subject to the
// restrictions described in FAR 48 CFR 52.227-14 or DFARS 48 CFR
// 252.227-7014, as applicable.
//
// Technical Data acquired by or for the U.S. Government, if any, is
// provided with Limited Rights.  Use, duplication or disclosure by the
// U.S. Government is subject to the restrictions described in FAR 48
// CFR 52.227-14 or DFARS 48 CFR 252.227-7013, as applicable.
//

//
// Tasking-related macros for the Chapel uGNI communication layer.
//

#ifndef _COMM_TASK_DECLS_H_
#define _COMM_TASK_DECLS_H_

#include <stddef.h>
#include <stdint.h>

#include "chpltypes.h"

// The type of task private data.
#include "chpl-cache-task-decls.h"
#define HAS_CHPL_CACHE_FNS

typedef struct {
  chpl_cache_taskPrvData_t cache_data;
} chpl_comm_taskPrvData_t;

//
// Comm layer private area within executeOn argument bundles
// (bundle.comm)
typedef struct {
  chpl_fn_int_t fid;
  int caller;
  void* done; // where to indicate completion on caller
} chpl_comm_bundleData_t;

//
// Nonblocking GET support.  Handle is a unique handle for the GET.
// This value is initially returned by chpl_com_get_nb(), and can then
// be passed to chpl_comm_test_get_nb() while polling for the GET to
// complete.  Once chpl_comm_test_get_nb() returns true, however, the
// handle is expired and must not be passsed to it again.
//
// Code external to the comm layer must not assume anything about or
// change any of the contents of a nonblocking GET handle.  The only
// supported interface is via the functions described below.
//
// chpl_comm_get_nb()
//   Get 'size' bytes of remote data at 'raddr' on locale 'locale' to
//   local data at 'addr', nonblocking.
//
// chpl_comm_test_get_nb()
//   Return nonzero if the GET associated with the given handle has
//   completed.
//
typedef void* chpl_comm_nb_handle_t;

chpl_comm_nb_handle_t chpl_comm_get_nb(void* addr, int32_t locale, void* raddr,
                                       size_t size, int32_t typeIndex,
                                       int ln, int32_t fn);
chpl_bool chpl_comm_test_get_nb(chpl_comm_nb_handle_t handle,
                                int ln, int32_t fn);

#endif
