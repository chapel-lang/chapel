/*
 * Copyright 2004-2018 Cray Inc.
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Internal declarations for the OFI-based Chapel comm layer.
//

#ifndef _comm_ofi_internal_h_
#define _comm_ofi_internal_h_

#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_domain.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_errno.h>


//
// Simplify internal error checking
//
#define CHK_EQ_TYPED(expr, wantVal, type, fmtSpec)                      \
    do {                                                                \
      type _exprVal = (expr);                                           \
      type _wantVal = (wantVal);                                        \
      if (_exprVal != _wantVal) {                                       \
        chpl_internal_error_v("%s == %" fmtSpec ", expected %" fmtSpec, \
                              #expr, _exprVal, _wantVal);               \
      }                                                                 \
    } while (0)


//
// Memory allocation
//

// wish we had typeof() in all target compilers ...

#define CHK_SYS_CALLOC(p, t, n, s)                                      \
    do {                                                                \
      if ((p = (t) sys_calloc((n), (s))) == NULL) {                     \
        chpl_internal_error_v("sys_calloc(%#zx, %#zx): out of memory",  \
                              (size_t) (n), (size_t) (s));              \
      }                                                                 \
    } while (0)


//
// Out-of-band support
//

struct gather_info {
  int node;
  char info[0];
};

void chpl_comm_ofi_oob_init(void);
void chpl_comm_ofi_oob_fini(void);
void chpl_comm_ofi_oob_barrier(void);
void chpl_comm_ofi_oob_allgather(void*, void*, int);


#endif
