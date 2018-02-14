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

#ifndef _comm_ofi_internal_h_
#define _comm_ofi_internal_h_

#include "rdma/fabric.h"
#include "rdma/fi_domain.h"
#include "rdma/fi_endpoint.h"
#include "rdma/fi_cm.h"
#include "rdma/fi_errno.h"

#define CALL_CHECK(fncall, errcode)                                     \
    do {                                                                \
      if ((fncall) != errcode) {                                        \
        chpl_internal_error(#fncall);                                   \
      }                                                                 \
    } while (0)

#define CALL_CHECK_ZERO(fncall) CALL_CHECK(fncall, 0)

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

//
// libfabric stuff
//

struct ofi_stuff {
  struct fid_fabric* fabric;
  struct fid_domain* domain;
  struct fid_av* av;
  fi_addr_t* fi_addrs;
  fi_addr_t** rx_addrs;
  struct fid_mr* mr;

  struct fid_ep* ep; /* scalable endpoint */
  int rx_ctx_bits;

  /* For async puts/gets */
  int num_tx_ctx;
  struct fid_ep** tx_ep;
  struct fid_cq** tx_cq;
  /* FI_CQ_FORMAT_CONTEXT?, FI_WAIT_UNSPEC, signaling_vector? */
  int num_rx_ctx;
  struct fid_ep** rx_ep;
  struct fid_cq** rx_cq;

  /* For active messages */
  int num_am_ctx;
  struct fid_ep** am_tx_ep;
  struct fid_cq** am_tx_cq;
  struct fid_ep** am_rx_ep;
  struct fid_cq** am_rx_cq;

};

#define OFICHKRET(fncall, err) do {              \
    int retval;                                  \
    if ((retval = (fncall)) != err) {             \
      chpl_internal_error(fi_strerror(-retval));  \
    }                                            \
  } while (0)

#define OFICHKERR(fncall) OFICHKRET(fncall, FI_SUCCESS)

//
// GET and PUT
//

void chpl_comm_ofi_put_get_init(struct ofi_stuff*);

//
// Active Messages (executeOn)
//

struct ofi_am_info {
  c_nodeid_t node;
  c_sublocid_t subloc;
  chpl_bool serial_state; // To prevent creation of new tasks
  chpl_bool fast;
  chpl_bool blocking;
  chpl_fn_int_t fid;
  void* ack;
};

void chpl_comm_ofi_am_init(struct ofi_stuff*);
void chpl_comm_ofi_am_handler(struct fi_cq_data_entry*);

//
// Comm diagnostics
//

/* Dupe of the version in chpl-comm.h except with atomics */
struct commDiags_atomic {
  atomic_uint_least64_t get;
  atomic_uint_least64_t get_nb;
  atomic_uint_least64_t put;
  atomic_uint_least64_t put_nb;
  atomic_uint_least64_t test_nb;
  atomic_uint_least64_t wait_nb;
  atomic_uint_least64_t try_nb;
  atomic_uint_least64_t execute_on;
  atomic_uint_least64_t execute_on_fast;
  atomic_uint_least64_t execute_on_nb;
};

struct commDiags_atomic *chpl_comm_ofi_getCommDiags(void);
void chpl_comm_ofi_commDiagsInc(atomic_uint_least64_t *val);

#define CHPL_COMM_DIAGS_INC(comm_type)                                  \
    chpl_comm_ofi_commDiagsInc(&(chpl_comm_ofi_getCommDiags()->comm_type))

#endif
