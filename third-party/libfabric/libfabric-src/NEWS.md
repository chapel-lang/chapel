Libfabric release notes
=======================

This file contains the main features as well as overviews of specific
bug fixes (and other actions) for each version of Libfabric since
version 1.0.  New major releases include all fixes from minor
releases with earlier release dates.

v2.3.1, Mon October 20, 2025
============================

## Core ##

- include/windows: Add definition for realpath()

## EFA ##

- Tune recvwindow and peer reorder buffer pool sizes
- Allocate peer reorder buffers from a pre-allocated pool
- Allocate peer map entry pool during the rdm ep create
- Create shm info on demand for efa_rdm_ep_open
- Check whether to enable shm inside efa_shm_info_create
- Ring doorbell when reaching the max batch wqe cnt
- Fix the queued ope progress in ep close
- Logging improvements and unit test fix
- Grab srx lock when releasing rx entries in AV close
- Remove duplicate macro for rdma sge limit
- Fix memory leak from efa_srx_unexp_pkt unit test
- Fix unit test build error for rdma-core without HAVE_CAPS_UNSOLICITED_WRITE_RECV
- Check rdma iov limit in data path direct
- Add packet entry flag tracking for double linked list management
- Remove the extra dlist remove for peers in ep close
- Add traces for completions bypassing util-cq
- Update shared domain caps and modes
- Move domain check inside the fabric lock
- Take domain lock to protect concurrent access to domain fields
- Return matching fabric/domain in fi_getinfo
- Remove duplicated efa_cq_construct_cq_entry
- Fix the memleak for efa_rdm_peer
- Introduce error status for ope
- Cleanup the queue flags after dlist_remove
- Allocate cq err_buf on demand
- Bypass util cq in the data path of efa-direct
- Fix the unsolicited write recv support in unit-test
- Add unit-tests for lock types
- Shift the src_addr correctly in sreadfrom
- Acquire the same lock for qp lifecycle
- Ignore stale cqes from destroyed qps
- Use IBV_QUERY_QP_DATA_IN_ORDER_DEVICE_ONLY flag when available

## OPX ##

- 0 byte write_data Error
- 16B 3-nodes seg fault with openmpi
- Atomics and bounce buffer fixes
- Only open IPC cache if HMEM initialized and IPC enabled
- Fix ROCR IPC build errors
- Use getpid() instead of gettid() for POSIX compliancy.
- Fix compilation error

## RXD ##

- Fix return of EAGAIN when not enough tx entries

## UCX ##

- Use noop callback in ucx_inject_write
- Add a noop callback function for inject send path

## UDP ##

- Cleanup CQ parameters
- Fix CQ overflow case

## Util ##

- Remove unmatched rx entry from unspec unexpected queue
- Add status enum to util rx entry
- Extract helper function to lookup existing fabric/domain
- Use hints name for fabric/domain lookup

## Verbs ##

- Return PCI bus information with fi_getinfo()
- Fix compiler warning on the bound of 'strncpy' call

## Fabtests ##

- efa: Run FI_MORE test with more iterations
- efa: Add implicit AV test
- Split OOB address exchange function
- efa: Fix the av operation
- efa: Improve rnr_read_cq_error test


v2.3.0, Mon September 15, 2025
==============================

## Core ##

- configure: Improve the restricted-dl help text
- ofi_list: Introduce dlist_entry_in_list
- man/fi_peer: Fix `FI_ADDR_NOTAVAIL` typo
- common: Make common runtime parameters working for DL providers
- configure.ac: Move cuda cppflag set before DMABUF check
- Add address format FI_SOCKADDR_IP
- include/fi_peer.h: remove fi_peer_rx_entry dlist fields
- configure: Fix clang checking
- hmem/neuron: Implement put_dmabuf_fd op
- man/fi_endpoint: Clarify rx_attr->caps usage

## CXI ##

- Fix debug assert in fi_close of counter
- Fix configure typo
- Add criterion test flag to configure
- Return correct error for HTTP 400
- Fix service allocation
- Fix bug related exp next seqn
- Fix bug related concurrent mcast's
- Fix memory leak
- Improve multithreaded performance of bound counter operations
- Fix RNR counter logic for new counter progress
- Use native curl timeout
- Improve counter performance by spin waiting
- RDMA get logic on a collective root/leaf
- Conditionally progress EPs bound to cntr
- Optimize counter thread locking
- Redo FI_CXI_CNTR_CACHED implementation
- Add support for FI_ORDER_RMA_RAR
- Revert "prov/cxi: Fix RMA/AMO network ordering"
- Fix dlopen using default value for json-c library
- Cache the last cmdq CP to reduce lock acquisition
- Add env vars to disable non-inject IDC for RMA/AMO
- Regularize collectives error logging
- Include wait FD support for RNR protocol
- Fix multi-threaded CQ WAIT_FD implementation
- Update fi_cxi, add cxi_collectives man pages
- Correct RE arming behavior
- Support cuda sync_memops pointer attribute
- Isolate alt_read gets to restricted only cmdq
- Allow RX context to specify command queue on s/w DMA
- Update unit test vm startup to load configfs
- Disable collective logical operators
- Retry root->leaf send after timeout

## EFA ##

- Make implicit AV unbounded by default
- Increase default efa-direct ibvcq size
- Fix logging of GID in efa_rdm_cq_lookup_raw_addr
- Rename efadv_cq_attr db to doorbell
- Only build EFA provider on applicable linux platforms
- Fix the pkt type derivation from ope
- Fix the rx sgl in data path direct
- Store evicted peers in a hashset and ignore future packets
- Implement LRU eviction for implicit AV entries
- Decrement rx_pkts_posted before efa_rdm_pke_release_rx
- Enable direct data path by default
- Bypass rdma-core in blocking cq read path
- Add traces for RX/TX completions
- Fix the unsolicited write recv check
- Refactor efa_base_ep_create_qp
- Add generic function to process queued op entries
- Deduce queued packet list from op entry
- Add generic utility for fetching RDM packet type
- Create abstraction for IBV CQ polling sequence
- Bypass rdma-core in data path.
- Refactor ibv_cq_ex open call
- Fix stale links in docs/overview.md
- Initialize nevents in efa_domain_cq_open_ext
- Fix conflicting types for efa_mock_efa_ibv_cq_wc_read_opcode_return_mock
- Remove duplicate mock function declarations
- Use efa specific cq trywait
- Implement fi_control for efa direct cq
- Support blocking cq read in efa-direct
- Remove cq status prediction
- Fix wait_send procedure
- Restrict GDA domain ops to efa-direct
- Fix efa device selection in test_rdm_cq_create_error_handling
- Fix the hmem flags setting
- Fix segfault in LTTNG tracing when peer is NULL
- Revert "prov/efa: Only do dmabuf reg when FI_MR_DMABUF is set"
- Add implicit AV and logic to move entries to explicit AV
- Add get_mr_lkey to GDA ops
- Move gda operations to FI_EFA_GDA_OPS
- Improve the handshake tx error handling
- Avoid flushing cq during ep close for external cq
- Replace the address members of peer struct with conn
- Add test_av_reinsertion unit test
- Skip call to efa_rdm_ep_get_peer with FI_ADDR_NOTAVAIL
- Replace pke->addr with pke->peer and remove ope->addr
- Rename efa_rdm_msg_alloc_rxe to efa_rdm_msg_alloc_rxe_zcpy
- Add efa_ep_addr_print debug function
- Refactor reverse AV addition and deletion
- Rename efa_conn_rdm_init to efa_conn_rdm_insert_shm_av
- Drain cq after qp destroy
- Fix the locking procedure in ep close.
- Move rxe_map to peer struct
- Clean stale error handling
- Remove illegal ope state

## LNX ##

- Fix missing peer_entry field updates

## OPX ##

- Asynchronous HMEM memcopy for IPC
- Fix infinite loop/crash in SIGUSR2 handler
- CN5000 SR-IOV pbc lid update
- CN5000 alpha SR-IOV loopback support
- Fixing typo in reliability_service_max_outstanding_bytes parameter.
- CYR: Support 256B rcvhdr size
- Fix overwrite of entropy field in 16-byte packet headers
- Add debug dump of endpoint upon receiving SIGUSR2
- GPU/driver configure check only warns
- Adding initialization requirements for when opx is built as a dl
- Rename intranode as needed for clarity
- Use fi_opx_check_info function for library location lookup
- Add IPC cache to OPX
- Initialize SDMA work entry field
- Fix RMA with CQ Data Bug
- Change shm tx connection table from array to RBT
- Fix FI_REMOTE_CQ_DATA write
- Fix bug in setting pbc dlid for 16B sendv_egr
- Resolve IPC HMEM D to H Segfault
- Parameter mismatch fix for opx_ep_complete_receive_operation()
- Link bounce support for context sharing
- CN6000 simulator support

## PSM3 ##

- Support fi_av_insertsvc()

## Util ##

- Configure `control_progress` with the user provided progress mode
- Handle the new address format FI_SOCKADDR_IP
- Add example directory with first example
- Get srx lock in the caller of util_foreach_unspec
- ofi_util.h: Fix bug in fi_cq_readfrom with no FI_SOURCE
- mon_sampler: Disable the sampler together with the monitor provider

## Verbs ##

- Add peer mem support for Broadcom Devices

## Fabtests ##

- Add dmabuf support for Neuron memory
- Add FT_INFO macro
- Fix `make distcheck` error caused by SUBDIRS
- efa: Initialize timespec as 0
- ft_finalize: Serialized sync in ft_finalize.
- pingpong: Post recv before ep finalize
- efa: Add volatile to prevent compiler optimization of CQE flag
- efa: fix missing rdma check in test_rma_bw_sread
- efa: Add FT_COMP_WAIT_FD tests
- efa: Add fi_cq_sread tests
- efa: Allow shared AV in multi_ep_mt test
- efa: Add pytest marker and fixture to GDA fabtest
- Fix type mismatch build warning
- efa: Fix EFA device query
- efa: Add GPU Direct Async test
- multinode: Fix multi_barrier EAGAIN path
- efa: Check device number in efa_device_selection
- efa: Increase the timeout for test_rma_bw_range
- efa: Add multi_ep_mt into pytest suites
- efa: Introduce multi_ep_mt test
- pytest: Fix the additional environment return
- Support customized fi_addr for ft_post_rx_buf
- Add New Benchmark rma_tx_completion


v2.2.0, Mon June 30, 2025
========================

## Core

- log: Fix buffer overrun when accessing the 'log_levels' array
- man/fi_mr: Clarify fi_close behavior
- rdma/fabric.h: Add new FI_RESCAN flag to fi_getinfo()
- hmem/cuda: Add fallback for dmabuf flag with CUDA_ERROR_NOT_SUPPORTED
- hmem/cuda: Add runtime fallback for unsupported dmabuf flag
- hmem/cuda: Add a flag for exporting dmabuf fd on GB200
- man: Clarify fi_close behavior on FI_ENDPOINT
- av: introduce FI_FIREWALL_ADDR flag for insert operations
- common: ofi_ifname_toaddr check ifa->ifa_addr for null
- man/fi_mr: Add note that requested_key may be ignored w/o remote access

## CXI

- Fix alt_read unit test to use rdzv_threshold
- Adjust cxi environment variable defaults
- Fix regression which could cause deadlock
- Support libfabric 2.2 API
- Set cq_data in peer unexpected message
- Fix locking on the SRX path
- Allow for passing opaque 64-bit data in ctrl_msg
- Fix cxi driver paths for CI
- Fix use of alt_read rget restricted TC type
- Fix compile warnings associated with new dlopen of curl/json
- Fix curl CXIP_WARN that included extra parameter
- Decouple existence CXI_MAP_IOVA_ALLOC for build
- New conf opt for binding of json symbols
- New conf opt for binding of curl symbols
- Pad struct to address hash mismatch bug
- Consistency for initialization of cxip_addr structure
- Fix uninitialized padding in cxip_addr structure causing hash mismatches
- Mem reg test with READ/WRITE access
- Support read-only cached MRs
- Remove cached mem reg -FI_EAGAIN comment
- Add access ctrl bits to internal mem reg
- Fix broken tests
- Added collective logical operators to cxip_query_collective()
- Log rc from cxi_eq_get_event() failure
- Add nanosecond timestamp to tracing
- Stuff the sending rank into the reduction packet
- Increase the reduction engine timeout to the max
- Increase RX buffer size for collectives

## EFA

- Update error message for invalid MRs
- Set IBV_ACCESS_LOCAL_WRITE for FI_REMOTE_WRITE
- Update packet printing functions
- Avoid iteration when cuda_api_permitted is false
- Do not add explicit MRs from application to MR cache
- Move peer bufpool to endpoint
- Move RDMA check functions to efa_rdm_ep.h
- Add detailed logging of device enumeration and selection
- Extend domain ops to open CQ with external memory
- Extend domain ops to allow querying of QP and CQ attributes
- Add a domain operation to query address info
- Minimize calls to efa_rdm_ep_get_peer in the CQ read path
- Remove unused function get_first_nic_name
- Post initial rx pkt when qp is enabled.
- Update qp table after qp destroy
- Track cloned rx pkts
- Clear domain level peer lists when closing endpoints
- Allow multiple EPs to bind to a single AV
- Minimize calls to efa_rdm_ep_get_peer in the RMA path
- Minimize efa_rdm_ep_get_peer calls in the atomic path
- Add RNR test with fi_tsend
- Minimize efa_rdm_ep_get_peer calls in the send path
- Remove duplicate filtering on FI_EFA_IFACE
- Update man page
- Clear cur_device when filtering EFA devices with FI_EFA_IFACE
- Move address handle to domain level.
- Make 1:1 relationship between efa domain and pd
- Delete unused efa_conn_release prototype
- Fix unit test build warning
- Use Mutex when removing from g_efa_domain_list
- Add missing lock in list iteration
- Remove ope from ope_longcts_list during release
- Use mutex locks for FI_THREAD_COMPLETE
- Do not try to initialize non-EFA devices
- Only initialize devices that match FI_EFA_IFACE filter
- Rename g_efa_device_list and g_efa_device_cnt
- Remove efa_device->device_idx
- Initialize EFA environment variables before devices
- Add lock around g_efa_domain_list
- Improve debug logging
- Do copy for inline send
- Enforce FI_MR_LOCAL in efa-direct
- Add lock to ensure efa direct cq poll is thread safe
- Require FI_MR_ALLOCATED in domain_attr
- Introduce setopt flag for homogeneous peers
- Change FI_AV_MAP logs to info
- Check if the util_av entry is valid before fetching

## HOOK

- Add monitor hook provider

## LNX

- Dump statistics
- Refactor LNX to follow libfabric semantics

## OPX

- Cornelis Networks OPX provider upstream June 5, 2025
- Cornelis Networks OPX provider upstream May 9, 2025
- Cornelis Networks OPX provider upstream April 19, 2025
- Make room for new RPC field in fid_ep

## PSM3

- Update psm3 to match IEFS 12.1.0.0 release

## RXD

- Add an environment variable to control FI_RESCAN

## RXM

- Fix user id case
- Fix potential completion mismatch for the rndv_write protocol
- Fix potential premature recv completion with rndv_write protocol
- Fix rndv_write state machine issue that may cause premature completion
- Fix the flow control enabling checking
- Add an environment variable to control FI_RESCAN
- fix auto progress
- Add a new error code for firewall

## SHM

- Add shm rename and retry
- Fix srx entry cleanup
- Acquire ep lock when freeing entries
- Don't run smr progress if region isn't initialized
- Update shm man page to be accurate and fix typos/formatting

## TCP

- Enable keepalive during CM exchange
- Add FI_GET_FD support to tcp provider
- Add firewall flags into verify flag set
- Fix memory out of bound access in RDM
- Add a new error code for firewall
- Add firewall support for RDM
- Process EPOLLERR properly
- Handle the error correctly

## UTIL

- Add fi_mon_sampler
- poll: Add `kqueue` support
- Move/Rename fi_thread_level() to header
- ofi_util.h: Make util_prov::info non const
- Only allocate MR key when needed
- bufpool: Add new flag to skip zeroing new memory allocations

## USNIC

- Fix memory leak in usd_ib_get_devlist()

## Verbs

- Avoid holding vrb_info_mutex when reloading interfaces
- Use a separate lock for provider initialization
- Register vrb_info_mutex in util_prov
- Code clean up.
- Optimize init info.
- Enable logging of ibv_async_events
- Fix unprotected access to vrb_util_prov.info
- Fix memory leak of fi_info structure in vrb_domain()
- Fix the flow control enabling checking
- Reload the list of interfaces on each call to fi_getinfo()
- Use single pointer instead of double pointer
- Always protect vrb_util_prov.info accesses with the mutex
- Make vrb_get_verbs_info() static
- Make a copy of the provider info in the fabric object
- Rename vrb_init_mutex to vrb_info_mutex
- Clarify little endian limitation for verbs atomics
- Fix atomic work request fields
- Memory leak for address formats different than FI_ADDR_IB_UD

## Fabtests

- Document limitations of fi_rma_pinpong
- Add additional_env to ClientServerTest
- Fix leak of dmabuf fd
- Define ft_hmem_put_dmabuf_fd
- efa: Make FI_EFA_IFACE tests check the Libfabric build
- Extend efa_info_test to accept fabric argument
- efa: Check rdma capability for both sides
- efa: Add FI_EFA_IFACE tests
- Add FI_EFA_IFACE tests
- efa: Fix get_efa_device_names
- Use yaml safe_load
- Add MR mode and descriptor to resource mgmt test
- Fix rdm_bw_mt unchecked fi_close return
- Support rdm_bw_mt in runfabtests.py
- Add rdm_bw_mt test to runfabtests on windows
- Add threaded_tests support to runfabtests.sh
- Add multi-threaded bandwidth test
- Add pthread_barrier support to windows
- Add pthread_barrier support to osx
- efa: Do not set multinode marker for test_efa_shm_addr


v2.1.0, Sat Mar 15, 2025
========================

## Core

- man/fi_domain: Define resource mgmt unreachable EP
- man/fi_domain: Update connectionless EP disable
- hmem: Fix missing rocr dlopen function assignments
- Fix data race on log_prefix
- hmem: Define ofi_hmem_put_dmabuf_fd and add support for cuda and rocr
- Fix a few minor man page issues

## CXI

- Fix ss_plugin_auth_key_priority test
- Bump internal CXI version to support 2.1
- Fix possible cq_open segfault
- Fix peer CQ support
- Added collectives logical operators
- Fix bug in constrained LE test cases in test.sh and test_sw.sh
- Fix unit test missing pthread initialization
- Add FI_WAIT_YIELD EQ support
- Make string setup of FI_CXI_CURL_LIB_PATH safe
- Add FI_CXI_CURL_LIB_PATH #define from autoconf
- Test CUDA with DMA buf FD recycling
- Test ROCR with DMA buf FD recycling
- Test ROCR with DMA buf offset
- Integrate with ofi_hmem_put_dmabuf_fd
- Test monitor unsubscribe
- Fix fi_cq_strerror
- Cxi EQ do not support wait objects
- Fix CQ wait FD logic
- Disable retry logic for experimental collectives
- Ignore drop count during init
- Remove CXI_MAP_IOVA_ALLOC flag.
- Synchronous fi_close on collective multicast
- Fix deferred work test
- Depreciate FI_CXI_WEAK_FENCE
- Update message and target ordering doc
- Define FI_CXI_MR_TARGET_ORDERING
- Remove FI_CXI_ENABLE_UNRESTRICTED_RO
- Set MR relax order on EP order size
- Fix RMA/AMO network ordering
- Update CXI provider max order size

## EFA

- Do not allocate rdm_peer struct for efa-direct and dgram paths
- Remove efa_av->ep_type in favor of efa_domain->info_type
- Replace domain->rdm_ep with domain->info_type
- Release matched rxe before destroying the srx rx_pool
- Fix the error code from ibv wr API
- Fix the clean up issue for efa_util_prov
- Fix the cntr interface for efa-direct
- Add unit test for efa-direct progress model
- Fix the max_msg_size reporting for efa-direct
- Clean up rxe map during rxe release
- rdm: Do not claim support for FI_PROGRESS_AUTO
- Always return efa_prov in EFA_INI
- Do not write cq error for ope from internal operations
- Remove unused field efa_domain->mr_mode
- Do GDRCopy registrations only in the EFA RDM path
- Reset g_efa_hmem_info after each test
- Fix the unexp_pkt clean up.
- Call efa_fork_support_enable_if_requested earlier
- Check efa_prov_info_set_fabric_name return code
- Clean up efa_prov_info_set_hmem_flags
- Bug fix in the RDM path with FI_MSG_PREFIX mode
- Rework the efa_cq unit tests
- Improve efa_cq's completion report
- Unit test additions and fixes for efa-direct
- Remove incorrect usage of rdm_info->ep_attr->max_msg_size
- Add new efa-direct fi_info objects
- Cleanup efa_user_info
- Add debug log for efa-direct data transfer
- Use cuda_put_dmabuf_fd
- Fix leak of dmabuf fd in cuda p2p probe
- Implement FI_CONTEXT2 in EFA Direct
- Remove x86-64 architecture check for static_assert
- Do infinite rnr retry for base ep by default
- Extend efa_ep interface
- Migrate efa_dgram_ep to efa_ep
- Adjust the logging level for unreleased rxe
- Regulate the usage of optnames
- Move struct efa_ep_addr to efa_base_ep
- Remove util_av_fi_addr from efa_conn
- Make efa_rdm_cq use efa_cq
- Deprecate FI_AV_MAP
- Remove inline write logic for rma inject
- Add missing mock for wc_is_unsolicited in unit test
- Implement the cq progress
- Remove err_msg from efa_rdm_ep
- Move raw addr functions
- Move efa_rdm_cq_wc_is_unsolicited to efa_cq
- Correct the error code for IBV_WC_RECV_RDMA_WITH_IMM
- Add missing locks in efa_msg and efa_rma
- Move fork handler installation to efa_domain_open
- Detect unsolicited write recv support status on both sides
- Add unit tests for efa_rma
- Add tracepoints for efa_msg and efa_rma
- Add unit tests for efa_msg
- Add tracepoint for poll cq ope
- Adjust the error code for flushed receive

## LPP

- Add check for atomics

## OPX

- Adding changes to resolve Coverity Scan Defects
- Link bounce fixes
- SDMA error handling fix
- Fix build with GCC 15
- Move CUDA sync attribute setting to mr registration
- Add HMEM handle for GDRCopy in GET/PUT
- Add newline to trace entry
- Add debug trace messages to RMA functions
- Disable out of order RC if TID is enabled
- Unexpected packet processing modifications
- Use inlined call to process_header for payloadless RZV_DATA (TID) packets
- Run opx-format on upstream opx provider change
- Remove reliability handshake
- Add PR close event to Cornelis Networks internal workflow triggers
- Use cycle timer as long as all set CPUs are same socket
- fi_opx_addr changes as pre-context sharing and pre-CYR
- Replace intranode hashmap with array
- Default RTS/CTS to in-order route control
- Write CQ entry for successful data transfer operation by default
- Resolve OPX fi_writedata() reliability errors
- Remove extraneous warning
- Enable TID by default.
- Fixed OPX trace points
- Set route control based on packet type
- Implement FI_MR_VIRT_ADDR in OPX
- Use reliability timer for link bounce status check
- Link bounce for JKR
- Fix debug print array indexing
- Resolve new Coverity scan defects
- Enhanced simulation and debug support
- Add HFI1 Direct Verbs support
- Making pkey related failures more obvious
- Reformat full OPX provider
- Add .clang-format file for OPX provider
- Identify and resolve new Coverity scan defects
- Changing default pkey to fetch from pkey table index 0
- Fix wrong function name for getting hmem iface.
- Handle Cuda Managed/Unified memory
- Fix OPX hint checking and capability setting
- Implement fi_writedata()
- Set rate control defaults
- Process RZV payload immediately
- CN5000/JKR 16B: 3B Lid changes
- Set entropy to rx/tx pair
- Don't send immediate data in send_rzv when send buffer is not host memory
- Use `page_sizes[OFI_PAGE_SIZE]-1` instead of `PAGE_MASK`

## PSM3

- Update provider to sync with IEFS 12.0.0.0.36

## RXM

- Fix rxm multi recv getopt segfault

## SHM

- Free all pending srx entries on ep close
- Remove prefix from map inserts
- Fix name compare bug

## TCP

- Only disable ep if the failure can not be retried
- Fix data race caused by parallel access to xnet_rdm_fid_ops
- Fix FI_MULTI_RECV not set on error
- Fix race in writing to xnet_ep_fi_ops

## Util

- Change util_av lock to genlock
- Roundup_power_of_two remove unnecessary decrement
- Enchance performace of roundup_power_of_two
- Fix FI_MULTI_RECV not set on FI_ECANCELED
- Fix flag initialization for generic receive of unexpected entry
- Add fabric argument to pingpong test
- Statically set uffd callbacks
- Fix ROCR and memhooks deadlock
- Support mem monitors with per sub ctx
- Separate uffd and import mem monitors
- pingpong: close mr after ep close

## Verbs

- Always return vrb_prov in VERBS_INI
- Fix data race vrb_open_ep function

## Fabtests

- efa: Bind eq to ep in remote exits early test
- Fix some test function prototypes
- efa: Add remote exit early test with post recv
- Do not require FI_TAGGED for fi_av_xfer test
- efa: print err for recv failure
- efa: Add fabtests for efa-direct
- Set the min of tx/rx_mr_size
- efa: Add remote exit early test
- efa: Fix the rnr read cq error test for efa-direct
- multi_ep: Support customized transfer size
- Re-enable psm3 rdm_tagged_peek
- Disable multi_recv
- Run efa tests with efa fabric name
- Add fabric argument to ClientServerTest
- efa: add rdma check for unsolicited write recv
- Add support for FI_CONTEXT2
- Bugfixes for neuron
- Corrected flags argument type in ft_sendmsg/ft_recvmsg functions
- pytest/efa: Avoid duiplicate completion semantic for RMA test
- pytest/efa: merge memory_type and check_rma_bw_memory_type


v2.0.0, Fri Dec 13, 2024
========================

## Core

- hmem/cuda: avoid stub loading at runtime
- Makefile.am: Keep using libfabric.so.1 as the soname
- xpmem: Cleanup xpmem before monitors
- Remove redundant windows.h
- hmem/cuda: Add env variable to enable/disable CUDA DMABUF
- Update ofi_vrb_speed

## CXI

- Add FI_OPT_CUDA_API_PERMITTED tests
- Define FI_CXI_FORCE_DEV_REG_COPY
- Support FI_OPT_CUDA_API_PERMITTED
- Testing FI_RM_ENABLED
- Correct checking of MR test rc
- Update unit test for collectives
- Add test for invalid client RKEY
- Fix broken client key check
- Ignore FLT_OVERFLOW and FLT_INVALID errors
- Update CXI man page.
- Enable dmabuf for ROCR by default.
- Remove disable_dmabuf_cuda and disable_dmabuf_rocr
- Disable use of dmabuf by default for cuda
- Remove use of deprecated FI_ORDER_NONE
- Report RMA order used in debug output
- Remove srx unittests
- Add FI_PEER capability bit
- Support shared receive queues
- Implement shared Completion Queues

## EFA

- Skip rx pkt refill under certain threshold
- Fix efa multi recv setopt segfault
- Add tracepoints for rma operations
- Adjust the location of tracepoint
- Implement the rma interface
- Fix efa_msg flags
- Remove efa_send_wr, send_wr_pool and recv_wr_pool from dgram_ep
- Fix the read_bad_recv_status unit test
- Implement efa_msg interface
- Implement FI_MORE for fi_recv in zero copy recv mode
- Fix the error path of zero copy recv
- Move inject sizes from rdm ep to base ep
- Fix the ep list scan in cq/cntr read
- Fix the error handling for unsolicited recv
- Fall back to zero sl when non-zero sl qp creation failed
- Disable zero copy receive if p2p is not available
- Initialize efa fork support in EFA_INI
- Update efa_hmem and efa_fork_support log to FI_LOG_CORE
- Make efa_hmem_info a global variable
- Set max rma order size correctly

## Hook

- Fix the preprocessor

## LNX

- Initialize flags to 0
- Convert peer table to use buffer pools
- Fix av strncpy
- Fix various issues with initial commit

## PSM2

- Check return value of asprintf

## RXM

- Fix rxm multi recv setopt segfault
- Replace rxm managed srx with util srx, support FI_PEER
- Add rxm support for using a peer CQs and counters
- Add FI_AV_USER_ID support
- Fix definition of the rxm SAR segment enum

## SHM

- Cleanup op flags

## Sockets

- Fixed coverity issue for unchecked return value.

## UCX

- Fix segfault in ucx_send_callback
- Fix incorrect return value checking for fi_param_get()

## Util

- Set srx completion flags and msg_len properly
- fi_pingpong: Fix coverity issue about integer overflow

## Verbs

- Fix coverity issue about overflowed return value
- Enable implicit dmabuf mr reg for more HMEM ifaces

## Fabtests

- Add opts.min_multi_recv_size to set opt before enable
- Add FI_MORE pytest for fi_recv in zcpy recv mode
- Allow tests with FI_MORE flag by using fi_recvmsg
- New fabtest fi_flood to test over subscription of resources
- test_configs/ofi_rxm/tcp.test: remove cntr RMA testing
- Fix compiler warning about unitialized variable


v2.0.0 beta, Fri Oct 25, 2024
==============================

## Core

- xpmem: Fix compilation warning
- Change the xpmem log level to info
- Clarify FI_HMEM support of inject calls
- Introduce Sub-MR
- Define capbility for directed receive without wildcard src_addr
- Define capability for tagged message only directed recv
- Define capability bit for tagged multi receive
- Define flag for single use MR
- Move flags only used for memory registration calls to fi_domain.h
- windows/osd.h: fix and refactor logical operations on complex numbers
- man/fi_peer: update peer fid initialization language
- Remove CURRENT_SYMVER() macro
- 1.8 ABI compat

## CXI

- Update provider man page
- Update version to 2.0
- Remove setting total_buffered_recv
- Update CXI provider

## EFA

- Remove unused fields from various data structures
- Update efa shm implementation to allocate fi_peer_srx_context
- Avoid gdr_pin/gdr_map for dmabuf mrs
- Only do dmabuf reg when FI_MR_DMABUF is set
- Report correct inject_msg_size for zcpy rx
- Add setopt/getopt support for remaining EP sizes
- Split RDM EP inject size field into MSG,RMA variants
- Use tclass to prioritize the messages from an ep
- Remove tx_size and rx_size from efa_rdm_ep
- Remove tx_iov_limit and rx_iov_limit from efa_rdm_ep
- Remove DC NACK packet from rxe map after recv completed
- Correctly handle fallback longcts-rtw send completion
- Differentiate unresponsive receiver errors following rdma-core
- Make NACK protocol fall back to DC longCTS when DC is requested
- Update help message for inter_min_read_write_size
- Adjust log level for setopt/getopt
- Add dependency header file in fi_ext_efa.h
- Test: Disable shm via fi_setopt
- Rename p2p_available to mr_p2p_available
- Always use p2p for system memory
- Test: Use correct qp num in the mock
- Shrink the size of extra_info array
- Improve the zero-copy recv error message.
- Update read nack protocol docs
- Receiver send NACK if p2p is unavailable
- Sender switch to emulated long CTS write if p2p unavailable
- Adjust log level for shm disabling.
- Check p2p support to use rdma read
- Add device to host copy for inject rdma write
- Copy user buffer for fi_sendmsg with FI_INJECT
- Respect FI_MR_LOCAL in transport path

## HOOK

- Trace: Add trace log for domain_attr.

## LNX

- Initial addition

## OPX

- Use page_sizes[OFI_PAGE_SIZE] instead of PAGE_SIZE
- Set immediate ACK requested bit when sending last packet of RMA PUT
- Add debug check for zero-byte length data packets
- Conditionally set FI_REMOTE_CQ_DATA on receive
- Include less immediate data in RTS packet to improve rendezvous performance
- Investigate and address indeterminate behavior or segfault resulting from ignored context creation error
- fi_info -e fix for FI_OPX_UUID env var
- Fix last_bytes field for replay over sdma
- Fix eager and mp eager
- Fix payload copy
- Add FI_OPX_TID_MIN_PAYLOAD_BYTES param
- Fix incorrect calculation of immediate block offset in send rendezvous
- Initialize nic info in fi_info
- Simplify fi_opx_check_rma() function.
- added OPX Tracer points to RMA code paths
- Fix credit return
- Remove polling call from internal rma write
- Support 16B SDMA CTS work
- Fix uepkt 16B headers
- 16B SDMA header support
- Man: Document OPX max ping envvars
- Link bounce support for OPX WFR
- Scb/hdr changes
- Updated configure.m4 for ROCR
- Capitalized env var used for production override, also added opx to the front.
- Remove FI_CONTEXT2 requirement
- Only posting one completion for rzv truncation receives.
- Fixing bug for credit check in inject code path.
- Resolve coverity scan defects uncovered after upstream
- Replace fi_opx_context_slist with slist
- Remove assert from find pkt by tag
- Add OPX Tracer EP lock and Recv entries
- CN5000/JKR: Changes needed to get RMA working in 16B
- Added GDRCopy logging and failure path
- Initial 16B header support
- Fix wrong function used when copying from HMEM/rocr.
- Create GPU-specific SDMA/RZV thresholds
- Don't try to get HMEM iface for NULL pointers
- Limit the number of reliability pings on credit-constrained flows
- Remove function table entries for reliability types other than ONLOAD

## PSM3

- Fix logical atomic function calls
- Check atomic op error code
- Disable complex comparison combinations

## SHM

- Add unmap_region function
- Use owner-allocated srx
- Fix incorrect capability set
- Make progress errors ints instead of unit64
- Remove unused err path from progress_iov
- Refactor initialization process
- Put smr_map memory into av

## TCP

- Fix incorrect usage of av insert apis when multiplexing
- Initialize addr_size when duplicating an av

## Util

- Change uffd stop routine to use pipe
- Integrate kdreg2 into libfabric
- mr_cache: Support compile default monitor
- Handle page faults in uffd monitor
- Allow providers to update cache MR IOV
- Log AV insert with AV's specified address format
- Add uffd user mode flag for kernels

## Fabtests

- Fix compilation error about CMPLX with C99
- Added -E/env option to multinode test script
- Change xfer-method variable to xfer_method in runmultinode.sh
- Fix complex fill cast
- efa: Remove rnr cq error message check
- efa: Loose assertion for read request counters
- runfabtests.cmd: add atomic tests to windows testing
- runfabtests.sh: add rdm_atomic validation tests
- rdm_atomic: add data validation
- Change ZE memset to use uint8
- Change sync message to be 0 bytes instead of 1 byte
- Fix atomic buffer
- Add hmem support to common atomic validation
- Move ubertest atomic validation code to common
- Use new synapse api
- Update fi_multinode test
- Update runmultinode.py with args
- Added inband sync to ft_init_fabric_cm
- lpp: remove deprecated FI_MR_BASIC
- Add option for conditionally building lpp
- Make building efa conditional
- Call provider specific configure
- efa: Skip inter_min_write_write_size test when rdma write is on
- efa: Add efa_rdma_checker
- lpp: remove invalid condition in fi_tsenddata
- Support no prepost RX pingpong test
- Split out ft_sync logic
- Define common run pingpong function
- Move pingpong logic into pre-posted func
- lpp: update version and protocol in fi_getinfo
- lpp: fix compile warnings
- Remove multi_ep from tcp exclude
- runfabtests.sh: add more multi_ep tests
- Add common threading option
- multi_ep: use common long ops, switch shared-av and cq opts
- multi_ep: add closing and reopening of MRs
- multi_ep: add RMA validation
- Create common raw key functions
- multi_ep: separate MR resources per EP
- efa: Skip memory registration that hit device limit
- efa: Avoid testing duplicate mixed memory type workload
- lpp: Fix compiler warning about unused variables
- Remove deprecated MR modes
- Remove fi_poll and fi_dgram_waitset tests (deprecated feature)


v2.0.0 alpha, Fri Aug 30, 2024
==============================

## Core

- hmem/ze: Fix mistmatched library name in an error message
- Add FI_PEER as a capability
- Add missing FI_AV_USER_ID to cap tostr
- Update and clarify peer SRX API flow
- Prefix public xpmem symbols with ofi
- Add rbmap foreach node utility function
- ofi_mem: Add release bufpool validity check
- hmem/rocr: Don't attempt to get device info when pointer type is unknown.
- hmem: Added handle field to close_handle
- Introduce new atomic datatypes and operation
- Define new tag formats
- Add new peer group feature
- Add fi_fabric2() API
- Deprecate old MR modes
- Deprecate FI_WAIT_MUTEX_COND
- Deprecate wait set and poll set
- Require using libfabric APIs to allocate fi_info structures
- Cleanup FI_ORDER flags
- Deprecate support for async memory registration
- Remove total_buffered_recv
- Deprecate comp_order attribute
- Simplify progress definition
- Simplify threading models
- Move FI_BUFFERED_RECV to internal flag
- Simplify the AV API
- Remove internally used definitions from public headers
- hmem/cuda: Modify the logging for nvml dlopen
- hmem/rocr: Fix dmabuf for amd gpu implementation

## CXI

- FI_PATH_MAX is removed in 2.0 API

## EFA

- Zero the cq entry array in dgram ep progress
- Remove unit test for libfabric 1.1 API
- Replace deprecated MR modes
- Remove deprecated FI_ORDER flag
- Update EP's `inject_size` in zero-copy mode
- Add support for `FI_OPT_INJECT_RMA_SIZE`
- Query for shm's FI_PEER capability
- Require FI_MR_LOCAL for zero-copy receive
- Correctly handle fallback longcts-rtm send completion
- Adjust the logging for pke exhaustion
- Fix a memory leak in local read
- Use dlist_foreach_container_safe to iterate progressed ep list
- refactor hmem interface initialization
- Fix a memory leak in efa_rdm_ep_post_handshake
- disable zero-copy receive if p2p is not supported
- Update data types for various IOV operations
- Require shm to be disabled for using zero-copy recv
- Register user recv buffer for zero-copy receive mode
- Make fi_cancel return EOPNOTSUPP for zero copy receive mode.
- Handle receive window overflow
- Introduce FI_EFA_IFACE to restrict visible NICs
- Allow disabling unsolicited write recv via env

## LPP

- Initial addition

## PSM2

- Fix incorrect unlock function

## PSM3

- Fix incorrect unlock function

## SHM

- Add FI_PEER capability
- Refactor ze ipc path to use pidfd

## TCP

- Introduce sub-domains to support FI_THREAD_COMPLETION

## UCX

- Support FI_OPT_CUDA_API_PERMITTED in fi_setopt()
- Fix error code for fi_setopt()/fi_getopt()

## Util

- Initialize ROCR name in memory monitor struct
- Support specific placement of addr into the av

## Verbs

- Fix resource leak in error handling path
- Replace __BITS_PER_LONG with LONG_WIDTH
- Fix issue while displaying addresses with fi_info -a <addr_format>

## Fabtests

- Add LPP specific fabtests
- Add `inject_size` to `ft_opts`
- Add pytests for FI_MORE Test fi_rma_bw and fi_rdm_tagged_bw with flag FI_MORE.
- Use fi_writemsg to test rma write/writedata with FI_MORE
- Use fi_sendmsg to test rdm_tagged_bw with FI_MORE
- Add option for running tests with FI_MORE
- synapse: Remove dependency of scal
- Pass `memory_type` to client server test


v1.22.0, Fri Jul 26, 2024
=========================

## Coll

- Fix Coverity issues

## Core

- General bug fixes
- hmem: change neuron get_dmabuf_fd error code
- Fix an error in the error handling path of fi_param_define()
- Makefile.am: Add Windows build files to distribution tarball
- hmem: disable ZE IPC
- Add profile variables for connections and memory allocated
- hmem: Fix `cuDeviceCanAccessPeer()` error reporting
- man: Update text for `len` parameter
- Add page size MR attr field
- man: Extend fi_mr_refresh support
- man: Improve FI_MR_ALLOCATED documentation
- man: Support optional MR desc
- man: Improve FI_MR_HMEM documentation
- Added ofi_get_realtime interfaces
- Add endpoint options for max message size and inject size
- Add Windows definition for `EREMOTEIO`

## EFA

- General improvement and bug fixes
- Handle recv cancel for zero copy recv
- Avoid iterating EP list in CQ read
- Add RDMA core errno for remote unknown peer
- Map EFA errnos to Libfabric codes
- Improve the zero-copy receive feature
- Improve the handshake enforcement procedure
- Support unsolicited rdma-write recv
- Support FI_MORE for eager send and rdma-write
- Improve the EFA_IO_COMP error code and explanation
- Improve the unit test for LL128 protocol
- Distinguish max RMA size from msg size

## Hooks

- dmabuf: Fix incompatible pointer warning

## OPX

- Add missing file needed for fabric direct build to release package
- Fix performance issue caused by not setting ACK bit in the single
  SDMA packet case
- TID cache debug improvements
- Detection of driver lack of support for TID
- Multi-CTS support for TID
- Removal of statement that TID is not supported
- OPX Tracer improvements
- Improvements to OPX shared memory cleanup
- H to H performance improvements for build that supports HMEM
- Bug fix for a threshold check
- Bug fix for FI_SELECTIVE_COMPLETION
- CN5000 fixes
- Parameterization of various thresholds
- Further enhancements to support NVIDIA GPUs, included CUDA-allocated
  bounce buffers and in-provider support for GDRCopy
- Enhancements to enable support for CN5000 hardware
- Better checking for TID support
- General TID enhancements
- Pkey error handling
- Send work queue splitting
- Support for OPX tracer for profiling purposes
- Coverity scan fixes
- Fixes and enhancements to logging and debug messages
- Intranode RMA read fixes
- Fix compile issues
- Fix shared memory segment index creation bug

## PSM3

- Update provider to sync with IEFS 11.7.0.0.110
- Improved auto-tuning features for PSM3, including dynamic Credit Flows
  and detecting the presence of the rv kernel module
- Improved PSM3 intra-node performance for large message sizes

## SHM

- Added support for write() method to submit DSA work
- Touch all buffer pages after DSA page fault
- Add return and more descriptive error message
- Fix coverity about incorrect sign
- Fix memory leaks for srx
- Fix atomic read

## Sockets

- Fix Coverity issues

## USNIC

- Fix a few Coverity issues

## Util

- Discard outstanding operations in util_srx_close
- Enable profile on the size of bufpool allocated.
- Add more predefined profile variables.
- Fix issue while displaying addresses with fi_info -a <addr_format>
- fi_pingpong: Fix out of scope memory leak
- Add source address to fi_pingpong

## Verbs

- Flush CQ for SQ on no SQ credit
- Optimize search for device max inline size
- Enable profiling

## Fabtests

- pytest/shm: reduce the msg size in test_unexpected_msg
- Fix synapseai fabtests build
- Add pytests for EFA zero-copy receive
- Add benchmark option for `FI_OPT_MAX_MSG_SIZE`
- benchmarks: Add synapseai support
- Disable fi_rdm_tagged_peek test for ucx and psm3
- Add manual init sync to fi_rdm_multiclient and fi_rdm
- Refactor ft_sock_sync to take in a socket
- Add fi_rdm_bw test
- Skip rma_pingpong write tests
- Init rx_buf before sending data
- Add rma_pingpong tests to makefile
- pytest: use different message sizes for rma pingpong
- Fix missing fixture memory_type in test_rma_pingpong_range_no_inject
- pytest: account for process startup overhead in client-server tests
- pytest: save client process output to a file
- Support testing inject with cq data
- multinode: update arguments
- multi_ep: Fix memory leak
- rdm_tagged_peek: Align rx's msg_order with tx's
- Add backlog > 0 to listen call


v1.21.1, Fri July 26, 2024
==========================

## Core

- Fix integer overflow in ofi_get_mem_size
- Fix overflow issue in ofi_rbinit
- Disable ZE IPC due to possible memory corruption
- Fix an error in the error handling path of fi_param_define()

## EFA

- Add tracepoints for rx pkt processing events
- Destroy rx_atomrsp_pool during ep close.
- Free user_info during ep close.
- Use srx lock from domain directly
- Fix error handling in efa_rdm_cq_poll_ibv_cq
- Move efa_rdm_cq_poll_ibv_cq to efa_rdm_cq.h
- Remove unused cq_attr
- Remove unnecessary app_info check
- Remove unnecessary ope check
- Make the inflight read msg per domain

## SHM

- Added support for write() method to submit DSA work
- Touch all buffer pages after DSA page fault
- add return and more descriptive error message
- fix coverity about incorrect sign
- Fix memory leaks for srx
- fix atomic read

## Verbs

- Flush CQ for SQ on no SQ credit

## Fabtests

- efa: reset error completion entry for each fi_cq_readerr call
- pytest: Skip rma_pingpong write tests
- Init rx_buf before sending data
- Add rma_pingpong tests to makefile
- pytest: use different message sizes for rma pingpong
- Fix missing fixture memory_type in test_rma_pingpong_range_no_inject
- pytest: account for process startup overhead in client-server tests
- pytest: save client process output to a file
- Fix memory leaks for efa_exhaust_mr_reg test
- Fix memory leak in multi_ep test
- Fix memory leak in efa_info_test


v1.21.0, Fri Mar 22, 2024
========================

## Core

- Various update and fixed in man pages
- Fix xpmem memory corruption
- Extend FI_PROVIDER_PATH to allow setting preferred DL provider
- Add a SECURITY.md file
- Document preferred threading model for scalable endpoints
- Move FI_PRIORITY to internal flag
- Remove FI_PROV_SPECIFIC
- Remove unimplemented or unused features
- Support cntr byte counting
- configure: Do not check for xpmem if disabled
- Add FI_PROGRESS_CONTROL_UNIFIED
- hmem/cuda: Get multiple attributes at once in cuda_is_addr_valid
- configure: Add -pipe by default to CFLAGS
- Selectively generate warnings on failed loading of DL providers
- hmem: introduce ofi_dev_reg_copy_*_iov ops
- Print provider path on fabric creation
- Introduce FI_OPT_SHARED_MEMORY_PERMITTED
- README.md: Add badge for openssf scorecard
- man: Regulate the fi_setopt call sequence.
- man: Clarify the usage of FI_RMOTE_CQ_DATA flag
- man: Add ucx provider to the fi_provider man page
- configure.ac: add extra check for 128 bit atomic support
- include/osd: align atomic complex definitions
- hmem/synapseai: Refine the error handling and warning
- Specify C11 standard for Visual Studio builds

## BGQ

Removed.

## CXI

New provider supporting Cray's Slingshot network.

## EFA

- General improvement and bug fixes
- Propagate errnos from core functions untouched
- Create 1:1 relationship between libfabric CQs and IBV CQs
- Do not progress ep inside transmission call when hitting EAGAIN
- Remove unnecessary check in rdma write.
- Handle rx pkts error without ope
- Add a new rx pkt counter
- Enable runting for neuron with a different runt size
- Distinguish unresponsive receiver errors
- Remove unnecessary handshake in send path
- Don't fail the whole domain init if cudamalloc failed
- Introduce efa specific domain operations
- Implement FI_OPT_SHARED_MEMORY_PERMITTED
- Do not memset rxe to 0 on init
- Reduce # of error cases in happy path
- Add FI_EFA_USE_HUGE_PAGE to efa man page.
- Don't do handshake for local fi_write
- Add pingpong test after exhausting MRs
- Introduce utilities to exhaust MRs on EFA device
- Test EFA with a 1GiB message
- Do not abort on all deprecated env vars
- Onboard fi_mr_dmabuf API in mem reg ops.
- Try registering cuda memory via dmabuf when checking p2p
- Introduce HAVE_EFA_DMABUF_MR macro in configure
- Use long CTS protocol if long read and runting read protocols fail
  because of memory registration limits

## GNI

Removed.

## Hooks

- dmabuf_peer_mem: initialize fd to supress compiler warning

## NETDIR

Removed. The functionality is intergrated into the verbs provider.

## OPX

- Fix compiler warnings and coverity issues
- General improvement and bug fixes
- Add GPU support to expected TID
- RZV RTS packet exclude empty immediate data
- Add more efficient check for cuda-resident user buffer
- Improve default HFI selection logic in multi rail environments
- Flush dead list opportunistically
- Add RISC-V support
- Make update HDRQ register frequency configurable at build time
- Removed all references to the reliability nack threshold env var
- Added missing tuneables, rearraged to match fi_info -e output
- Use BAR load/store macros
- Check HFI driver version to allow GPU-enabled build/run
- Added kernel and driver version check to allow/disallow expected receive TID
- Fix max SHM connections to allow up to 16 HFIs
- Use FI_HMEM_SYSTEM for Cuda-Managed (Unified) memory
- Handle FI_OPT_CUDA_API_PERMITTED
- Use contiguous send when only one iov present
- Always replay TID packets over SDMA
- Add Virtual Lane and Partition pkey (FI_OPX_SL and FI_OPX_PKEY)
- Forced AV type to be AV Map when requested AV is unsupported
- Reduce size of opx_shm_tx
- Add GPU support for RMA Atomic operations
- Add GPU support for RMA reads and writes
- Add HMEM debug counters
- Print debug counters upon receiving SIGUSR1
- Fix multi-receive to work with contiguous rzv payload
- Initial support for GPU / FI_HMEM
- Limit multipacket eager implementation to tagged sends
- Read, verify and store some hfi chip attributes

## PSM3

- Update provider to sync with IEFS 11.6.0.0.231
- Fix some conditional build errors

## RSTREAM

Removed.

## RXM

- Add option to auto detect hmem iface of user buffers

## SHM

- Manually align 8 byte fields in memory region
- Close device_fds for connected peers when the EP is closed
- Print shm name and error code when failed to open
- Mark send as completed when a message is discarded
- Don't close dmabuf-fd when a request is done
- Revert the smr_region fields adjustment
- Fix various coverity issues
- Add ep to cq ep list once in cq bind
- Add ofi_buf_alloc error handling

## SOCKETS

- fix compiler warnings and coverity issues

## UCX

- Fix incorrect enum value in FI_DBG() and FI_WARN()

## USNIC

- Turn off compiler warnings of possible string truncation

## Util

- Make ep_list_lock noop for FI_PROGRESS_CONTROL_UNIFIED
- Save control progress model to util_domain
- Set import monitor state to idle upon close
- Add name field to memory monitors
- memhooks: Fix a bug when calculating mprotect region
- Modify domain_attr based on FI_AV_AUTH_KEY

## Verbs

- Non-blocking EP creation
- Address cm_id resource leak in rdma_reject path
- Redirected error handle logic for dmabuf failure in verbs
- Added rocr dmabuf support under verbs
- Windows: Check error code from GetPrivateData
- Add missing lock to protect SRX
- Fix compiler warnings about out of boundary access

## Fabtests

- Fix various coverity issues
- General improvement and bug fixes
- Add multi_ep test
- Serialize the run of fi_cq_test
- Utilize `junitparser` module directly
- Add progress models to SHM/EFA fabtests
- Add option to change progress model
- efa/rnr_cq_read_err: poll cq when hitting EAGAIN
- Allow testing multi_ep with shared/non-shared cq and av
- Print warning for HMEM iface init failure
- efa: Add small tx_rx size test
- pytest: Make ssh connection error pattern less stringent
- Add new exclude file for io_uring tests
- Add rma_pingpong benchmark
- efa: Make 1G tests run faster
- pytests: add command line argument for dmabuf reg
- Bump Libfabric API version.
- Add option to support dmabuf MR
- Add dmabuf ops for cuda.


v1.20.2, Fri March 29, 2024
========================

## Core

- configure: Do not check for xpmem if disabled
- man page fixes

## EFA

- Remove unnecessary check in rdma write.
- Enable runting for neuron with a different runt size
- Handle rx pkts error without ope
- Distinguish unresponsive receiver errors
- Add `efa_show_help()`
- Refactor error code definitions
- Remove error message assertions from CQ unit tests
- Refactor `efa_strerror()`
- Doxyfile: Configure tabs to 8 spaces
- Rename Doxyfile

## SHM

- Revert the smr_region fields adjustment
- Don't close dmabuf-fd when a request is done
- Mark send as completed when a message is discarded
- Print shm name and error code when failed to open
- Close device_fds for connected peers when the EP is closed

## Fabtests

- Replace strtok with strtok_r
- Add new exclude file for io_uring tests


v1.20.1, Mon Jan 22, 2024
=========================

## Core

- hmem/ze: Change the library name passed to dlopen
- hmem/ze: map device id to physical device
- hmem/ze: skip duplicate initialization
- hmem/ze: dynamically allocate device resources based on number of devices
- hmem/ze: fix hmem_ze_copy_engine variable look up
- hmem/ze: Increase ZE_MAX_DEVICES to 32
- man: Fix typo in fi_getinfo man page
- Fix compiler warning when compiling with ICX
- man: Fix fi_rxm.7 and fi_collective.3 man pages
- man: Update EFA docs for FI_EFA_INTER_MIN_READ_WRITE_SIZE

## EFA

- efa_rdm_ep_record_tx_op_submitted() rm peer lookup
- Remove peer lookup from efa_rdm_pke_sendv()
- Make handshake response use txe
- test: Only close SHM if SHM peer is Created
- Handshake code allocs txe via efa util
- Initialize txe.rma_iov_count to 0
- Switch fi_addr to efa_rdm_peer in trigger_handshake
- Downgrade EFA Endpoint Creation WARN to INFO
- Init srx_ctx before use
- Clean up generic_send path
- Pass in efa_rdm_ep to efa_rdm_msg_generic_recv()
- Make recv path slightly more efficient
- re-org rma write to avoid duplicate checks
- Add missing sync_memops call to writedata
- use peer pointer from txe in read, write and send
- Pass in peer pointer to txe
- Get rid of noop instruction from empty #define
- Remove noop memset
- Fix the ibv cq error handling.
- Don't do handshake for local read
- Fix a typo in configure.m4
- Make runt_size aligned

## NetDir

- Add missing unlock in error path of nd_send_ack()

## OPX

- Initialize cq error data size

## RXM

- Fix data error with FI_OFI_RXM_USE_RNDV_WRITE=1

## SHM

- Fix coverity issue about resource leak
- Adjust the order of smr_region fields.
- Allocate peer device fds dynamically

## Util

- Fix coverity issue about missing lock
- Implement timeout in util_wait_yield_run()
- Fix bug in util_cq startup error case
- util_mem_hooks: add missing parantheses

## Verbs

- Windows: Resolve regression in user data retrieval

## Fabtests

- efa: Close ibv device after use
- efa: Get device MR limit from ibv_query_device
- efa: Add simple unexpected test to MR exhaustion test
- pytest: add a new ssh connection error pattern


v1.19.1, Mon Jan 22, 2024
=========================

## Core

- hmem/ze: Change the library name passed to dlopen
- hmem/ze: map device id to physical device
- hmem/ze: skip duplicate initialization
- hmem/ze: dynamically allocate device resources based on number of devices
- hmem/ze: fix hmem_ze_copy_engine variable look up
- hmem/ze: Increase ZE_MAX_DEVICES to 32
- man: Fix typo in fi_getinfo man page
- Fix compiler warning when compiling with ICX
- man: Fix fi_rxm.7 and fi_collective.3 man pages
- man: Fix the fi_provider.7 man page for the man page converter
- hmem/synapseai: Refine the error handling and warning
- configure.ac Fix `--with-lttng` causing `yes/` to populate {CPP,LD}FLAGS
- hmem: Only initalize synapseai if device exists
- hmem/ze: fix incorrect device id in copy function
- configure.ac: Fix `with_synaposeai` typo

## EFA

- Fix the ibv cq error handling.
- Don't do handshake for local read
- Don't do handshake for local fi_write
- Make runt_size aligned
- Add pingpong test after exhausting MRs
- Introduce utilities to exhaust MRs on EFA device
- Add read nack protocol docs
- Receiver send NACK if runt read fails with ENOMR
- Sender switch to long CTS protocol if runt read fails with ENOMR
- Receiver send NACK if long read fails with ENOMR
- Update efa_rdm_rxe_map_remove to accept msg_id and addr
- Sender switch to long CTS protocol if long read fails with ENOMR
- Introduce new READ_NACK feature
- Do not abort on all deprecated env vars
- Allocate pke_vec, recv_wr_vec, sge_vec from heap
- Close shm resource when it is disabled in ep
- Disable RUNTING for Neuron
- Move cuda-sync-memops from MR to EP
- Do not insert shm av inside efa progress engine
- Fix coverity warning in efa_mr_reg_impl
- Fix typos in packet macros
- Adjust posted receive size to pkt_size
- RDMA write with immediate data completion bugfix
- Do not create SHM peer when SHM is disabled
- Use correct threading model for shm
- Restrict RDMA read to compatible EFA devices
- Add EFA device version to handshake
- Cleanup/fix some unit test code
- Touch up RDM protocol header, doc
- Fix efa device name matching
- Add missing locks in efa_cntr_wait.
- Fix the efa_env_initialize() call sequence.
- Fix a compilation warning
- Handle RNRs from RDMA writedata
- Add writedata RNR fabtest
- Correct typo in RMA context type

## NetDir

- Add missing unlock in error path of nd_send_ack()

## RXM

- Fix data error with FI_OFI_RXM_USE_RNDV_WRITE=1

## SHM

- Fix coverity issue about resource leak
- Allocate peer device fds dynamically
- Add memory barrier before updating resp for atomic
- Use peer cntr inc ops in smr_progress_cmd
- Only increment tx cntr when inject rma succeeded.

## TCP

- Pass through rdm_ep flags to msg eps.
- Derive cq flags from op and msg flags
- Set FI_MULTI_RECV for last completed RX slice

## UCX

- Initialize ep_flush to 1

## Util

- Fix coverity issue about missing lock
- Implement timeout in util_wait_yield_run()
- memhooks: Fix a bug when calculating mprotect region

## Verbs

- Windows: Resolve regression in user data retrieval
- Windows: Check error code from GetPrivateData
- Bug fix for matching domain name with device name

## Fabtests

- efa: Close ibv device after use
- efa: Get device MR limit from ibv_query_device
- efa: Add simple unexpected test to MR exhaustion test
- pytest: Add a new ssh connection error pattern
- Make ft_force_progress non-static
- memcopy-xe: Fix data verification error for device buffer
- dmabuf: Increase the number of NICs that can be tested
- cq_data: Relax CQ data validation to cq_data_size
- dmabuf: Handle partial read scenario for fi_xe_rdmabw test
- pytest/efa: Add cuda memory marker


v1.18.3, Mon Jan 22, 2024
=========================

## Core

- hmem/ze: Change the library name passed to dlopen
- hmem/ze: map device id to physical device
- hmem/ze: skip duplicate initialization
- hmem/ze: dynamically allocate device resources based on number of devices
- hmem/ze: fix hmem_ze_copy_engine variable look up
- hmem/ze: Increase ZE_MAX_DEVICES to 32
- man: Fix typo in fi_getinfo man page
- man: Fix fi_rxm.7 and fi_collective.3 man pages
- man: Fix the fi_provider.7 man page for the man page converter
- configure.ac Fix `--with-lttng` causing `yes/` to populate {CPP,LD}FLAGS
- hmem/ze: fix incorrect device id in copy function
- configure.ac: Fix `with_synaposeai` typo

## EFA

- Fix efa device name matching
- Add writedata RNR fabtest
- Handle RNRs from RDMA writedata

## NetDir

- Add missing unlock in error path of nd_send_ack()
- Release lock prior to returning from nd_send_ack

## RXM

- Fix data error with FI_OFI_RXM_USE_RNDV_WRITE=1

## SHM

- Fix coverity issue about resource leak
- Allocate peer device fds dynamically

## TCP

- Pass through rdm_ep flags to msg eps.
- Derive cq flags from op and msg flags
- Set FI_MULTI_RECV for last completed RX slice

## UCX

- Initialize ep_flush to 1

## Util

- Fix coverity issue about missing lock
- Implement timeout in util_wait_yield_run()
- memhooks: Fix a bug when calculating mprotect region

## Verbs

- Windows: Resolve regression in user data retrieval
- Windows: Check error code from GetPrivateData
- Bug fix for matching domain name with device name

## Fabtests

- rdm_tagged_peek: Fix race condition synchronization
- Make rdm_tagged_peek test more general
- Split cq_read and cq_readerr in ft_spin_for_comp
- sock_test: Do not use epoll if not available
- Use dummy ft_pin_core on macOS
- Avoid using memset function name
- Fix some header includes
- memcopy-xe: Fix data verification error for device buffer
- dmabuf: Increase the number of NICs that can be tested
- dmabuf: Handle partial read scenario for fi_xe_rdmabw test
- pytest/efa: add cuda memory marker


v1.20.0, Fri Nov 17, 2023
=========================

## Core

- General bug fixes and code clean-up
- configure.ac: add extra check for 128 bit atomic support
- hmem/synapseai: Refine the error handling and warning
- Introduce FI_ENOMR
- hmem/cuda: fix a bug when calculating aligned size.
- Handle dmabuf for ofi_mr_cache* functions.
- Handle dmabuf flag in ofi_mr_attr_update
- Handle dmabuf for mr_map insert.
- man: Fix the description of virtual address when FI_MR_DMABUF is set
- man: Clarify the defition of FI_OPT_MIN_MULTI_RECV
- hmem/cuda: Add dmabuf fd ops functions
- include/ofi_atomic_queue: Properly align atomic values
- Define fi_av_set_user_id
- Support multiple auth keys per EP
- Simplify restricted-dl feature
- hmem: Only initalize synapseai if device exists
- Add "--enable-profile" option
- windows: Updated config.h
- Add environment variable for selective HMEM initialization
- Add restricted dlopen flag to configure options
- hmem: generalize the use of OFI_HMEM_DATA to non-cuda iface
- hmem: fail cuda_dev_register if gdrcopy is not enabled
- Add 1.7 ABI compat
- Define fi_domain_attr::max_ep_auth_key
- hmem: Add new op to hmem_ops for getting dmabuf fd
- hmem/cuda: Update cuda_gdrcopy_dev_register's signature
- mr_cache: Define ofi_mr_info::flags
- Add ABI compat for fi_cq_err_entry::src_addr
- Define fi_cq_err_entry::src_addr
- Add base_addr to fi_mr_dmabuf
- hmem: Set FI_HMEM_HOST_ALLOC for ze addr valid
- hmem: Support dev reg with FI_HMEM_ZE
- tostr: Added fi_tostr() for data type struct fi_cq_err_entry.
- hmem_ze: fix incorrect device id in copy function
- Introduce new profiling interface for low-level statistics
- hmem: Support dev reg with FI_HMEM_CUDA
- hmem: Support dev reg with FI_HMEM_ROCR
- hmem: Support dev reg with FI_HMEM_SYSTEM
- hmem: Define optimized HMEM memcpy APIs
- Implement memhooks atfork child handler
- hmem: Support ofi_hmem_get_base_addr with sys mem
- hmem: Add length field to ofi_hmem_get_base_addr
- mr_cache: Improve cache hit rate
- mr_cache: Purge dead regions in find
- mr_cache: Update find to remove invalid MR entries
- mr_cache: Update find with MM valid check
- Add direct support for dma-buf memory registration
- man/fi_tagged: Remove the peek for data ability
- indexer: Add byte idx abstraction
- Add missing FI_REMOTE_CQ_DATA for fi_inject_writedata
- Add configure flags for more sanitizers
- Fix fi_peer man page inconsistency
- include/fi_peer: Add cq_data to rx_entry, allow peer to modify on unexp
- Add XPMEM support

## EFA

- General bug fix and code clean-up
- Do not abort on all deprecated env vars
- Onboard fi_mr_dmabuf API in mem reg ops.
- Try registering cuda memory via dmabuf when checking p2p
- Introduce HAVE_EFA_DMABUF_MR macro in configure
- Add read nack protocol docs
- Receiver send NACK if runt read fails with ENOMR
- Sender switch to long CTS protocol if runt read fails with ENOMR
- Receiver send NACK if long read fails with ENOMR
- Update efa_rdm_rxe_map_remove to accept msg_id and addr
- Sender switch to long CTS protocol if long read fails with ENOMR
- Introduce new READ_NACK feature
- Use SHM's full inject size
- Add testing for small messages without inject
- Enable inject rdma write
- Use bounce buffer for 0 byte writes
- Onboard ofi_hmem_dev_register API
- Update cuda_gdrcopy_dev_register's signature
- Allocate pke_vec, recv_wr_vec, sge_vec from heap
- Close shm resource when it is disabled in ep
- Disable RUNTING for Neuron
- Move cuda-sync-memops from MR to EP
- Do not insert shm av inside efa progress engine
- Enable shm when FI_HMEM and FI_ATOMIC are requested
- Adjust posted receive size to pkt_size
- Do not create SHM peer when SHM is disabled
- Use correct threading model for shm
- Restrict RDMA read to compatible EFA devices
- Add EFA device version to handshake
- Add missing locks in efa_cntr_wait.
- Add writedata RNR fabtest
- Handle RNRs from RDMA writedata
- Check opt_len in efa_rdm_ep_getopt
- Use correct tx/rx op_flags for shm

## Hooks

- dmabuf: Initialize fd to supress compiler warning
- trace: Add log on FI_VAR_UNEXP_MSG_CNT when enabled.
- trace: Fixed trace log format on some attributes.

## OPX

- Fix compiler warnings

## PSM3

- Fix compiler warnings
- Update provider to sync with IEFS 11.5.1.1.1

## RXM

- Remove unused function
- Use gdrcopy in rma when emulating injection
- Use gdrcopy in eager send/recv
- Add hmem gdrcopy functions
- Remove unused dynamic rbuf support

## SHM

- General bug fixes and cleanup
- Add ofi_buf_alloc error handling
- Only copy header + msg on unexpected path
- Add FI_HMEM atomic support
- Add memory barrier before updating resp for atomic
- Add more error output
- Reduce atomic locking with ofi_mr_map_verify
- Only increment tx cntr when inject rma succeeded.
- Use peer cntr inc ops in smr_progress_cmd
- Allow for inject protocol to buffer more unexpected messages
- Change pending fs to bufpool to allow it to grow
- Add unexpected SAR buffering
- Use generic acronym for shm cap
- Move CMA to use the p2p infrastructure
- Add p2p abstraction
- Load DSA dependency dynamically
- Replace tx_lock with ep_lock
- Calculate comp vars when writing completion
- Move progress_sar above progress_cmd
- Rename SAR status enum to be more clear
- Make SAR protocol handle 0 byte transfer.
- Move selection logic to smr_select_proto()

## Sockets

- Fix compiler warnings
- Fix provider name and api version in returned fi_info struct

## TCP

- Add profiling interface support
- Pass through rdm_ep flags to msg eps
- Derive cq flags from op and msg flags
- Do not progress ep that is disconnected
- Set FI_MULTI_RECV for last completed RX slice
- Return an error if invalid sequence number received
- xnet_progress_rx() must only be called when connected
- Reset ep->rx_avail to 0 after RX queue is flushed
- Disable the EP if an error is detected for zero-copy
- Add debug tracking of transfer entries
- Negotiate support for rendezvous
- Add rendezvous protocol option
- Generalize xnet_send_ack
- Flatten protocol header definitions
- Remove unused dynamic rbuf support
- Define tcp specific protocol ops
- Remove unneeded and incorrect rx_entry init code

## UCX

- Add FI_HMEM support
- Initialize ep_flush to 1

## Util

- General bug fixes
- memhooks: Fix a bug when calculating mprotect region
- Check the return value of ofi_genlock_init()
- Update checks for FI_AV_AUTH_KEY
- Define domain primary and secondary caps
- Add profiling util functions
- Update util_cq to support err_data
- Update ofi_cq_readerr to use new memcpy
- Update ofi_cq_err_memcpy to handle err_data
- Zero util cancel err entry
- Move FI_REMOTE/LOCAL_COMM to secondary caps
- Alter domain max_ep_auth_key
- Add domain checks for max_ep_auth_key
- Revert util_cntr->ep_list_lock to ofi_mutex
- Add NIC FID functions to ofi.h
- Add EP and domain auth key checking
- Add bounds checks to ibuf get
- Define dlist_first_entry_or_null
- Update util_getinfo to dup auth_key
- Revert util_av, util_cq and util_cntr to mutex
- Add missing calls to (de)initialize monitor's mutexes
- Avoid attempting to cleanup an uninitialized MR cache
- Rename ofi_mr_info fields
- Add rv64g support to memory hooks

## Verbs

- Windows: Check error code from GetPrivateData
- Add missing lock to protect SRX
- Add synapseai dmabuf mr support
- Bug fix for matching domain name with device name
- Windows: Fetch rejected connection data
- Add support for DMA-buf memory registration
- Windows: Fix use-after-free in case of failure in fi_listen
- Windows: Map ND request type to ibverbs opcode
- Fix memory leak when creating EQ with unsupported wait object
- Track ep state to prevent duplicate shutdown events

## Fabtests

- Update man page
- pytests/efa: onboard dmabuf argument for test_mr
- pytest: make do_dmabuf_reg_for_hmem an cmdline argument
- Bump Libfabric API version.
- mr_test: Add dmabuf support
- Introduce ft_get_dmabuf_from_iov
- unexpected_msg: Use ft_reg_mr to register memory
- pytest: Allow registering mr with dmabuf
- Add dmabuf support to ft_reg_mr
- Add dmabuf ops for cuda.
- Test max inject size
- Add FI_HMEM support to fi_rdm_rma_event and fi_rdm tests
- memcopy-xe: Fix data verification error for device buffer
- dmabuf-rdma: Increase the number of NICs that can be tested
- dmabuf-rdma: Remove redundant libze_ops definition
- fi-mr-reg-xe: Skip native dmabuf reg test for system memory
- Check if fi_info is returned correctly in case of FI_CONNREQ
- cq_data: relax CQ data validation to cq_data_size
- Add ZE host alloc function
- Use common device host buffer for check_buf
- hmem_ze: allocate one cq and cl on init
- fi-mr-reg-xe: Add testing for dmabuf registration
- scripts: use yaml safe_load
- macos: Fix build error with clang
- multinode: Use FI_DELIVERY_COMPLETE for 'barrier'
- Handle partial read scenario for fi_xe_rdmabw test For cross node tests
- pytest/efa: add cuda memory marker
- pytest/efa: Skip some configuration for unexp msg test on neuron.
- runfabtests.py: ignore error due to no tests are collected.
- pytest/efa: extend unexpected msg test range
- pytest/shm: extend unexpected msg test range
- pytest: Allow running shm fabtests in parallel
- unexpected_msg.c: Allow running the test with FI_DELIVERY_COMPLETE
- runfabtests.sh: run fi_unexpected_msg with data validation
- pytest/shm: Extend test_unexpected_message
- unexpected_msg: Make tx/rx_size large enough
- pytest/shm: Extend shm's rma bw test
- Update shm.exclude


v1.19.0, Fri Sep 1, 2023
========================

## Core

- General code cleanup and restructuring
- Add ofi_hmem_any_ipc_enabled()
- ofi_consume_iov allows 0-byte consume
- ofi_consume_iov consistency
- ofi_indexer: return error code when iterating
- getinfo: Add post filters for domain and fabric names
- Filter loopback device if iface is specified
- bsock: Fix error checking for -EAGAIN
- windows/osd: Remove unneeded check to silence coverity
- windows/osd: Move variable declaration to silence coverity
- Introduce gdrcopy awareness to hmem copy
- mr/cache: Fix fi_mr_info initialization
- hmem_cuda: remove gdrcopy from cuda hmem copy path
- iouring: Fix wrong indent in ofi_sockapi_accept_uring()
- Implement ofi_sockctx_uring_poll_add()
- hmem: introduce gdrcopy from/to cuda iov functions
- hmem: Deprecate `FI_HMEM_CUDA_ENABLE_XFER`
- hmem_cuda: Restrict CUDA IPC based on peer accessibility
- hmem_cuda: Log number of CUDA devices detected
- hmem_cuda: Refactor global variables
- tostr: Remove the extra dir "shared/" from "include/" and "src/" .
- hmem_ze: fix ZE is valid check
- hmem_rocr: fix offset calculation
- hmem_rocr: use ofi spinlock functions
- hmem_rocr: minor fixes
- hmem_neuron: convert warn to info for nrt_get_dmabuf_fd not found
- hmem_neuron: check existance of neuron devices during initialization
- tostr: Moved Windows functions in shared/ofi_str.c to windows/osd.h
- tostr: Add helper functions ofi_tostr_size() and ofi_tostr_count().

## EFA

- Onboard Peer API, use shm provider as a peer provider
- Uses util SRX framework in shared receive procedures.
- Register shm MR with hmem_data, allow shm to use gdrcopy for cuda data movement
- Finish the refactor for rxr squash.
- Use rdma-core WR API for send requests
- Check optlen in getopt call
- Fix the rdma-read support check in RMA and MSG operations
- Optimize ep lock usage
- Use an internal fi_mr_attr for memory registration

## Hooks

- Init field in mr_attr to silence coverity
- Add profiling hook provider
- Rename cq hooking functions' names
- Added trace for resource creation operations

## OPX

- Initialize ofi_mr_info
- Fix dput credit check
- Only allocate replay buffer if psn is valid
- Support SHM Intra-node communication between single server HFI devices
- Fix incorrect packet size in packet header when sending CTS packet
- Added check to address Coverity scan defect
- Add multi-entry caching to TID rendezvous
- Fall back to default domain name for TID fabric
- Properly handle multiple IOVs in fi_opx_tsendmsg
- Fix OPX Rzv RTS receive operation SHM error (DAOS-related)
- Fix non-tagged sends may incorrectly set FI_TAGGED in send completions
- Add more info to reliability IOV buffer validation check
- Move dput packet build functions to new inline include
- Use fi_mr_attr in fi_opx_mr
- Disable Pre-NAKing by default, throttle until all outstanding replays ACK'd
- Fix reliability bug when NAKing the last PSN
- Update HeaderQ Register more frequently
- No rbuf_wrap needed for expected receive (TID)
- Fixes for Coverity scan issues
- Enhanced tag matching
- Tune expected recv for unaligned buffers
- Observability: Add finer logging granularity
- Reduce RTS immediate data and fix packet estimate for odd TID lengths
- Add additional sources for FI_OPX_UUID

## Peer

- Add cq_data to rx_entry, allow peer to modify on unexp
- Introduce peer cntr API
- Add foreach_unspec_addr API
- Add size as an input of the get_tag op

## PSM3

- Sync with IEFS 11.5.0.0.172

## SHM

- Only poll IPC list when ROCR IPC is enabled
- Allow for SAR and inject protocol to buffer more unexpected messages
- Remove unused sar fields
- Make SAR protocol handle 0 byte transfer
- Load DSA dependency dynamically
- Change recv entry freestack into bufpool
- Remove shm signal
- Use util peer cntr implementation
- Make SHM default to domain level threading level
- Replace internal shared receive implementation with util_srx
- Lock entire progress loop
- Fix ROCR data coherency
- Add FI_LOCAL_COMM to shm attrs
- Handle empty freestack
- Fix bug in configure.m4 in atomics_happy assignment happy
- Add memory barrier before update resp->status for SAR
- Do not use inline/inject for read op
- Allow shm to use gdrcopy
- Refactor protocol selection code
- Init map fi addrs to FI_ADDR_NOTAVAIL

## TCP

- General code cleanups
- Restrict which EPs can be opened per domain
- Increase CM error debug output
- Avoid calling close() on an invalid socket after accept error
- Mark the EP as disconnected before flushing the queues
- Add assertion failures for xnet_{monitor,halt}_sock
- Disable ofi_dynpoll_wait() for non-blocking progress
- Move PEP pollin operations to io_uring
- Move EP poll operations to io_uring
- Early exit if ofi_bsock_flush() has operation in progress
- Implement pollin sockctx in bsock
- Add missing call to xnet_submit_uring()
- Add return error to xnet_update_pollflag()
- Remove the cancel sockctx from the EP structure
- Move io_uring cqe from the stack to progress struct
- Reduce stack size for epoll event array
- handle NULL av in xnet_freeall_conns()

## UCX

- Publish FI_LOCAL_COMM and FI_REMOTE_COMM capabilities
- Fix configure error with newer MOFED
- Fix segfault in unsignalled completions

## Util

- Add FI_PEER support to util counter
- Refactor the usage of cntrs
- Change util_ep to be a genlock
- Add util shared receive implementation
- Update log message for invalid AV type message
- Fix fi_mr_info initialization
- Add peer ID to MR cache
- Store hmem_data in ofi_mr_map
- Split the cq progress and reading entries in ofi_cq_readfrom

## Verbs

- Add event lock to EQ to serialize closing ep
- Remove saved_wc_list and use CQ directly
- Consolidate peer_mem and dmabuf support check
- Fix vrb_add_credits signature
- Introduce new progress engine structure
- Simplify (and correct) locking around progress operations
- General code restructuring

## Fabtests

- Fix reading addressing options
- Allow to change only the OOB address
- Allow to use FI_ADDR_STR with -F
- Fix bw buffer utilization
- Separate RX and RMA counters
- Fix tx counter with RMA
- Add FI_CONTEXT mode to rdm_cntr_pingpong
- Add HMEM support to fi_unexpected_msg test
- Fix array OOB during fabtest list parsing
- Enable shm tagged_peek test
- Fix windows build warnings
- Make tx_buf and rx_buf aligned to 64 bytes by default
- Fix windows build warnings for sscanf
- Use dummy ft_pin_core on macOS
- Fix some header includes
- sock_test: Do not use epoll if not available
- recv_cancel: initialize error entry
- Fix wrong size used to allocate tx_msg_buf
- unexpected: change defaults to support tcp
- unexpected: add unknown unexpected peer test
- Enable a list of arbitrary message sizes
- Enabled data validation for rma read & write
- bw_rma operates on distinct buffer offsets
- ft_post_rma issues reads from remote's tx_buf
- General code cleanup and restructuring
- rdm_tagged_peek: fix race condition synchronization
- Add FI_LOCAL_COMM/FI_REMOTE_COMM presence check to fi_getinfo_test
- Correct ft_exchange_keys in prefix-mode
- Make rdm_tagged_peek test more general
- Add unit test for fi_setopt

v1.18.2, Fri Sep 1, 2023
========================

## Core

- Check for CUDA devices with nvmlDeviceGetCount_v2() first
- Try libnvidia-ml.so.1 if .so symlink missing
- Fix ssize_t format specifiers

## EFA

- Remove rxr_rm_tx/rx_cq_check()
- Report cntr completion for shm inject write

## SHM

- Change recv entry freestack into bufpool
- Load DSA dependency dynamically

## TCP

- Fix missing iov truncation on saved message path
- Add locking to trywait path for potential data race
- Fix incorrect locking around MR operations

## UCX

- Updated ucx.exclude and Makefile.am

## Verbs

- Add additional checks to vrb_shutdown_qp_in_err
- Prevent duplicate FI_SHUTDOWN events
- Fix memory leak when creating EQ with unsupported wait object

## Fabtests

- Extend the test_unexpected_msg
- Rename dmabuf-rdma tests to prefix with xe

v1.18.1, Fri Jun 30, 2023
=========================

## Core

- Fix build warning for ofi_dynpoll_get_fd

## EFA

- Handle 0-byte writes
- Apply byte_in_order_128_byte for all memory type
- Increase default shm_av_size to 256
- Force handshake before selecting rtm for non-system ifaces.
- Only select readbase_rtm when both sides support rdma-read
- Bugfix for initializing SHM offload
- Correct CPPFLAGS during configure
- Make setopt support sendrecv aligned 128 bytes
- Make data size to be 128 byte multiples for in-order aligned send/recv
- prepare local read pkt entry for in-order aligned send/recv.
- Disable gdrcopy and cudamemcpy for in-order aligned recv.
- Increase the pad size in rxr_pkt_entry
- Make readcopy pkt pool 128 byte aligned
- Introduce alignment to support in order aligned ops
- Fix a bug when calling ibv_query_qp_data_in_order
- RMA operations will ensure FI_ATOMIC cap
- RMA operations will ensure FI_RMA cap
- Unittest atomics without FI_ATOMIC cap.
- Unittest RMA without FI_RMA cap.
- Refactor pkt_entry assignment in poll_ibv loop
- Fixes for RDMA Write and Writedata

## RXM

- Revert rxm util peer CQ support
- Fix credit size parameter for flow ctrl

## SHM

- Fix DSA enable
- Assert read op and inject proto are mutually exclusive
- Fix ROCR data coherency
- Add FI_LOCAL_COMM to shm attrs
- Signal peer when peer is out of resources
- Handle empty freestack
- Fix bug in configure.m4 in atomics_happy assignment happy
- Add memory barrier before update resp->status for SAR
- Fix resource leak reported by coverity
- Switch cmd_ctx pool from freestack to bufpool
- Add iface parameter to smr_select_proto

## TCP

- Fix spinning on fi_trywait()
- Handle truncation of active message
- Handle prefetched data after reporting ETRUNC error
- Progress all ep's on unexp_msg_list when posting recv
- Removed unused saved_msg::ep field to fix assert
- Continue receiving after truncation error
- Create function to allocate internal msg buffer
- Add runtime setting for max saved message size
- Increase default max_saved value
- Dynamically allocate large saved Rx buffers
- Separate the max inject and recv buf size
- Remove 1-line xnet_cq_add_progress function
- Changed default wait object to epoll
- Handle case where epoll isn't natively supported
- Hold domain lock while deregistering memory
- Rename DL package from libnet to libtcp

## UCX

- Align the provider version with the libfabric version

## Verbs

- Delay device initialization to when fi_getinfo is called
- Consolidate peer_mem and dmabuf support check
- verbs_nd: Init len to 0 for WCSGetProviderPath call
- verbs_nd: Verify CQs are valid in rdma_create_qp
- verbs_nd: Initialize ibv_wc fields
- verbs_nd: Release lock in network direct error paths
- Fix vrb_add_credits signature
- Fix credit size parameter for flow ctrl
- Recover RXM connection from verbs QP in error state

## Fabtests

- Add ze-dlopen functions to component tests
- Call cudaSetDevice() for selected device
- pytest/efa: Adjust get_efa_devices()
- pytest/common: Support parallel neuron test
- pytest/common: Use different cuda device for parallel cuda set
- efa: Test_flood_peer.py increase timeout
- pytest/efa: Test to flood peer during startup
- fi-rdmabw-xe: Add option to set maximum message size
- fi-rdmabw-xe: Add option to set batch size

v1.18.0, Fri Apr 7, 2023
========================

## Core

- rocr: fix offset calculation
- rocr: use ofi spinlock functions
- rocr: minor fixes
- neuron: convert warn to info for nrt_get_dmabuf_fd not found
- neuron: check existance of neuron devices during initialization
- neuron: Add support for neuron dma-buf
- ze: update ZE to support new driver index specification
- List variables read from config file
- Add switch to prefer system-config over environment
- Add basic system-config support for setting library variables
- Move peer provider defines into new header
- rocr: Support asynchronous memory copies
- rocr: Add support for ROCR IPC
- rocr: rename rocr data-structures
- synpaseai: return 0 for host_register and host_deregister
- fabric: Improve log level of provider mismatch
- cuda: Allow CUDA IPC when P2P disabled
- ze: add ZE command list pool to reuse command lists
- cuda: implement cuda_get_xfer_setting for non cuda build
- cuda: adjust FI_HMEM_CUDA_ENABLE_XFER behavior
- cuda.c: Add const to param to remove warning
- Add IFF_RUNNING check to indicate iface is up and running
- io_uring support enhancements

## EFA

- Implement CUDA support on instance types that do not support GPUDirect RDMA
- Implement fi_write using device's RDMA write capability
- Enrich error messages with debug and connection info
- Implement support for FI_OPT_EFA_USE_DEVICE_RDMA in fi_setopt
- Implement support for FI_OPT_CUDA_API_PERMITTED in fi_setopt
- Add support for neuron dma-buf
- Use gdrcopy to improve the intra-node CUDA communication performance for small messages
- Use shm provider's FI_AV_USER_ID support
- Fix bugs in efa provider’s shm info initialization procedure

## Hooks

- dmabuf_peer_mem: Handle IPC handle caching in L0
- trace: Add trace log for CM operation APIs
- trace: Change tag in trace log to hex format
- trace: Enhance trace log for data transfer API calls
- trace: Add trace log for API fi_cq_readerr()
- trace: Add trace log for CQ operation APIs
- Add tracing hook provider

## Net

- Net provider optimizations have been integrated into the tcp provider.
- Net provider has been removed as a reported provider.

## OPX

- Fixes for Coverity scan issues
- Enhanced tag matching
- Tune expected recv for unaligned buffers
- Add finer logging granularity
- Reduce RTS immediate data and fix packet estimate for odd TID lengths
- Add additional sources for FI_OPX_UUID
- Exclude opx from build if missing needed defines
- Move some logs to optimized builds
- Fix build warnings for unused return code from posix_memalign
- Add reliability sanity check to detect when send buffer is illegally altered
- SDMA Completion workaround for driver cache invalidation race condition
- Fix replay payload pointer increment
- Handle completion counter across multiple writes in SDMA
- Cleanup pointers after free()
- Modify domain creation to handle soft cache errors
- Two biband performance improvements
- Fixes based on Coverity Scan related to auto progress patch
- Changed poll many argument to rx_caps instead of caps
- Resync with server configured for Multi-Engines (DAOS CART Self Tests)
- Remove import_monitor as ENOSYS case
- Address memory leaks reported on OFIWG issues page
- General code cleanup
- Add replays over SDMA
- Implement basic TID Cache
- Revert work_pending check change
- Fix use_immediate_blocks
- Restore state after replay packet is NULL
- Fix memory leak from early arrival packets
- Fix segfault in SHM operations from uninitialized value in atomic path
- Prevent SDMA work entries from being reused with outstanding replays
- Set runtime as default for OPX_AV
- Fix RTS replay immediate data
- Fix errors caught by the upstream libfabric Coverity Scan
- fi_getInfo - Support multiple HFI devices
- Support OFI_PORT and Contiguous endpoint addresses for CART & Mercury
- Add fi_opx_tid.h to Makefile.include
- Fix progress checks and default domain
- Revert is_intranode simplification.
- Don't inline handle_ud_ping function
- Allow atomic fetch ops to use SDMA for sufficiently large counts
- Cleaned up FI_LOG_LEVEL=warn output
- Cleaned up unused macros for FI_REMOTE_COMM and FI_LOCAL_COMM
- Reset default progress to FI_PROGRESS_MANUAL
- Fixed GCC 10 build error with Auto Progress
- Add support for FI_PROGRESS_AUTO
- Use max allowed packet size in SDMA path when expected TID is off
- Expected receive (TID) rendezvous
- RMA Read/Write operations over SDMA
- Remove origin_rs from cts and dput packet header
- Fix for hang in DAOS CART tests
- Use single IOV for bounce buffer in SDMA requests.
- Check for FI_MULTI_RECV with bitwise OR instead of AND
- Fix for intermittent intra-node deadlock hang (DAOS CART tests)
- Fix to RPC transport error failure (DAOS CART tests)
- Fix for context->buf set to NULL
- Fix bad asserts
- Ensure atomicity of atomic ops
- fi_opx_cq_poll_inline count and head check fix
- Fix intermittent intra-node hang causing RPC timeouts (DAOS CART tests)

## PSM3

- Update provider to sync with IEFS 11.4.1.1.2
- Fix warnings from build
- Add oneapi ZE support to OFI configure

## RXD

- Ignore error path in av_close return

## RXM

- Handle NULL av in rxm_freeall_conns()
- Implement the FI_OPT_CUDA_API_PERMITTED option
- Write "len" field for remote write
- Ignore error path domain_close return
- Free coll_pool on ep close
- Update rxm to use util_cq FI_PEER support functions
- Fix incorrect CQ completion field
- Rename srx to msg_srx
- Disable FI_SOURCE if not requested
- Memory leaks removed
- Set offload_coll_mask based on actual configuration
- Report on coll offload capabilities with OFI_OFFLOAD_PROV_ONLY
- Fabric setups collective offload fabric
- Create eq for collective offload provider
- Close collective providers ep when rxm_ep is closed
- Fix incorrect use of OFI_UNUSED()
- Rework collective support to use collective provider(s)

## SHM

- Fix potential deadlock in smr_generic_rma()
- smr_generic_rma() wwrite error completion with positive errno
- Update SHM to use ROCR
- Fix incorrect discard call when cleaning up unexpected queues
- Separate smr_generic_msg into msg and tagged recv
- Fix start_msg call
- Implement the FI_OPT_CUDA_API_PERMITTED option
- Assert not valid atomic op
- Fix a bug in smr_av_insert
- Optimize locking on the SAR path
- Remove unneeded sar_cnt
- Optimize locking
- Enable multiple GPU/interface support
- Remove HMEM specific calls from atomic path
- Use util_cq FI_PEER support
- Import shm as device host memory
- Add HMEM flag to smr region
- Fix user_id support
- Write tx err comp to correct cq
- Fix index when setting FI_ADDR_USER_ID

## TCP

- Provider source has been replaced by net provider source
- Removed incorrect reporting of support for FI_ATOMIC
- Do not save unmatched messages until we have the peer's fi_addr
- Use internal flag for FI_CLAIM messages, versus a reserved tag bit
- Fix updating error counter when discarding saved messages
- Allow saved messages to be received after the underlying ep has been closed
- Enhanced debug logging in connection path
- Force CM progress on unconnected ep's when posting data transfers
- Support connect and accept calls with io_uring
- Fix segfault accessing an invalid fi_addr
- Add io_uring support for CM message exchange
- Move CM progress from fabric to EQ to improve multi-threaded performance
- Fix small memory leak destroying an EQ
- Fix race where same rx entry could be freed twice
- Handle NULL av in rdm ep cleanup
- Reduce stack use for epoll event array

## UCX

- New provider targeting Nvidia fabrics that layers over libucp

## Util

- Fix the behavior of cq_read for FI_PEER
- rocr: Fix compilation issue
- cuda: Use correct debug string calls
- Free cq->peer_cq on close
- Remove extra new line from av insert log
- Check for count = 0 in ofi_ip_av_insert
- rocr: Add support for ROCR IPC
- Add FI_PEER support to util_cq
- Disable FI_SOURCE if not requested
- Remove FID events from the EQ when closing endpoint
- Rework collective support to be a peer collective provider(s)
- Allow FI_PEER to pass CQ, EQ and AV attr checking
- Remove annoying WARNING message for FI_AFFINITY
- Add utility collective provider

## Verbs

- Implement the FI_OPT_CUDA_API_PERMITTED option
- Add support for ROCR IPC

## Fabtests

- Add fi_setopt_test unit test
- Update ze device registration calls
- fi-rdmabw-xe: Always use host buffer for synchronization
- Fix bug in posting RMA operation
- fi_cq_data: Extend test to fi_writedata
- fi_cq_data: Extend validation of completion data
- Rename fi_msg_inject tests to fi_inject_test to reflect its use
- fi_rdm_stress: Add count option to json key/pair options
- Add and fix OOB option handling in several tests
- fi_eq_test: Fix incorrect return value
- fi_rdm_multi_client: Increase the size of ep name buffer
- Add FI_MR_RAW to default mr_mode
- Support larger control messages needed by newer providers
- fi-rdmabw-xe: Update to work with the ucx provider
- fi_ubertest: Cleanup allocations in failure cases
- Change ft_reg_mr to not assume hmem iface & device
- fi_multinode: Bugfix multinode test for ze + verbs
- fi_multinode: Remove unused validation print
- fi_multinode: Skip tests for unsupported collective operations
- fi_ubertest: Fix data validation with device memory
- fi_peek_tagged: Restructure and expand test

v1.17.1, Fri Mar 3, 2023
========================

## Core

- Fix spinlocks for macOS
- hmem_cuda Add const to param to remove warning
- Fix typos in fi_ext.h
- ofi_epoll: Remove unused hot_index struct member

## EFA

- Print local/peer addresses for RX write errors
- Unit test to verify no copy with shm for small host message
- Avoid unnecessary copy when sending data from shm
- Compare pci bus id in hints
- Fix double free in rxr endpoint init
- Initialize efawin library before EFA device on Windows

## Hooks

- dmabuf_peer_mem: Handle IPC handle caching in L0

## OPX

- Exclude from build if missing needed defines
- Move some logs to optimized builds
- Fix build warnings for unused return code from posix_memalign
- Add reliability sanity check to detect when send buffer is illegally altered
- SDMA Completion workaround for driver cache invalidation race condition
- Fix replay payload pointer increment
- Handle completion counter across multiple writes in SDMA
- Cleanup pointers after free()
- Modify domain creation to handle soft cache errors
- Two biband performance improvements
- Fixes based on Coverity Scan related to auto progress patch
- Changed poll many argument to rx_caps instead of caps
- Resynch with server configured for Multi-Engines (DAOS CART Self Tests)
- Remove import_monitor as ENOSYS case
- Address memory leaks reported on OFIWG issues page
- Remove unused fields
- Fix unwanted print statement case
- Add replays over SDMA
- Implement basic TID Cache
- Revert work_pending check change
- Fix use_immediate_blocks
- Restore state after replay packet is NULL
- Fix memory leak from early arrival packets.
- Fix segfault in SHM operations from uninitialized value in atomic path.
- Prevent SDMA work entries from being reused with outstanding
  replays pointing to bounce buf.
- Set runtime as default for OPX_AV
- Fix RTS replay immediate data
- Fix errors caught by the upstream libfabric Coverity Scan
- Support multiple HFI devices
- Support OFI_PORT and Contiguous endpoint addresses
- Update man pages

## Util

- util_cq: Remove annoying WARNING message for FI_AFFINITY

v1.17.0, Fri Dec 16, 2022
=========================

## Core

- Add IFF_RUNNING check to indicate iface is up and running
- General code cleanups
- Add abstraction for common io_uring operations
- Support ROCR get_base_addr
- Add a 'flags' parameter to fi_barrier()
- Introduce new calls for opening domain and endpoint with flags
- Add ability to re-sort the fi_info list
- Allowing layering of rxm over net provider
- General cleanup of provider filtering functions
- Add io_uring operations to be used by sockapi
- Modify internal handling of async socket operations
- Sockets operations are moved to a common sockapi abstraction
- Add support for Ze host register/unregister
- Add new offload provider type
- Rename fi_prov_context and simplify its use
- Convert interface prefix string checks to exact checks

## EFA

- Code cleanups and various bug fixes
- Improved debug logging and warnings and assertions
- Do not ignore hints->domain_attr->name
- Fix the calculation of REQ header size for a packet entry
- Fix default value for host memory's max_medium_msg_size
- Add tracepoints to send/recv/read ops
- Simplified emulated read protocol
- Set use_device_rdma according to efa device id
- Fix shm initialization path on error
- Fix Implementation of FI_EFA_INTER_MIN_READ_MESSAGE_SIZE
- Do not enable rdma_read if rxr_env.use_device_rdma is false
- Remove de-allocated CUDA memory region during registration
- Fix the error handling path of efa_mr_reg_impl()
- Fix rxr_ep unit tests involving ibv_cq_ex
- Add check of rdma-read capability for synapseai
- Report correct default for runt_size parameter
- Toggle cuda sync memops via environment variable.

## Net

- Continued fork of tcp provider, will eventually merge changes back
- Fix inject support
- Fix memory leak in peek/claim path
- General code cleanups and bug fixes from initial fork
- Allow looking ahead in tcp stream to handle out-of-order messages
- Add message tracing ability
- Fetch correct ep when posting to a loopback connection
- Release lock in case of error in rdm_close
- Fix error path in xnet_enable_rdm
- Add missing progress lock in srx cleanup
- Code restructuring and enhancements with longer term goal of supporting io_uring
- Disable the progress thread in most situations
- Rename DL from libxnet-fi to libnet-fi
- Add missing initialization calls for DL provider
- Add support for FI_PEEK, FI_CLAIM, and FI_DISCARD
- Include source address with CQ entry
- Fix support for FI_MULTI_RECV

## OPX

- Bug fixes and general code cleanup
- Fix progress checks and default domain
- Allow atomic fetch ops to use SDMA for sufficiently large counts
- Cleaned up FI_LOG_LEVEL=warn output
- Reset default progress to FI_PROGRESS_MANUAL
- Fixed GCC 10 build error with Auto Progress
- Add support for FI_PROGRESS_AUTO
- Use max allowed packet size in SDMA path when expected TID is turned off
- Expected receive (TID) rendezvous
- RMA Read/Write operations over SDMA
- Remove origin_rs from cts and dput packet header.
- Fix for hang - unable to match inbound packets with receive
  context->src_addr (DAOS CART tests)
- Use single IOV for bounce buffer in SDMA requests.
- Check for FI_MULTI_RECV with bitwise OR instead of AND
- Fix for intermittent intra-node deadlock hang (DAOS CART tests)
- Fix to RPC transport error failure (DAOS CART tests)
- Fix for context->buf set to NULL
- Fix bad asserts
- Ensure atomicity of atomic ops
- fi_opx_cq_poll_inline count and head check fix
- Fix intermittent intra-node hang causing RPC timeouts (DAOS CART tests)
- Temporarily reduce SDMA queue ring size for possible driver bug workaround
- Fix alignment issue and asserts
- Enable more parallel SDMA operations

## PSM3

- Synced to IEFS 11.4.0.0.198
- Tech Preview Ubuntu 22.04 Support
- Tech Preview Intel DSA Support
- Improved Intel GPU Support
- Various performance improvements
- Various bug fixes

## RxM

- Always use rendezvous protocol for ZE device memory send
- Code cleanup
- Add option to free resources on AV removal

## SHM

- Fix user_id support
- Write tx err comp to correct cq
- Fix index when setting FI_ADDR_USER_ID
- Remove extraneous ofi_cirque_next() call
- Add support for FI_AV_USER_ID
- Fix multi_recv messaging
- General code restructuring for maintainability
- Implement shared completion queues
- Decouple error processing from cq completion path to avoid switch
- Fix incorrect op passed into recv cancel operation
- Enhanced SHM implementation with DSA offload
- Use multiple SAR buffers per copy operation
- Fix ZE IPC race condition on startup

## TCP

- Minor updates in preparation for io_uring support (via net provider)

## Util

- Add option to free resources on AV removal
- Add 'flags' parameter to new fi_barrier2() call
- Add debugging in ofi_mr_map_verify
- Rename internal bitmask struct to include ofi prefix

## Verbs

- Add option to disable dmabuf support
- FI_SOCKADDR includes support of FI_SOCKADDR_IB

## Fabtests

- shared: Expand hmem support
- fi_loopback: Add support for tagged messages
- fi_mr_test: add support of hmem
- fi_rdm_atomic: Fix hmem support
- fi_rdm_tagged_peek: Read messages in order, code cleanup and fixes
- fi_multinode: Add performance and runtime control options, cleanups
- benchmarks: Add data verification to some bw tests
- fi_multi_recv: Fix possible crash in cleanup

v1.16.1, Fri Oct 7, 2022
========================

## EFA

- Flush MR cache when fork() is called

## RxM

- Disable 128-bit atomics

## SHM

- Add safeguards around peer mapping initialization
- Fix Ze IPC race condition on startup

## Verbs

- Add missing header file to release package

## Fabtests

- Add net provider test config files to release package

v1.16.0, Fri Sep 30, 2022
=========================

## Core
- Added HMEM IPC cache
- Use exact string comparison checks for network interfaces
- Restructuring of poll/epoll abstraction
- Add ability to disable locks completely in debug builds
- Serialize access to modifying the logging calls
- Minor fixes to fi_tostr text formatting
- Fix Windows build warnings
- Add hmem interface checks to memory registration

## EFA
- Added support of Synapse AI memory.
- Introduced Runting read message protocol for CUDA memory and Neuron memory
- Mix use of both local read and gdrcopy for copying of CUDA memory
- Use SHM provider's CUDA IPC support to implement intra-node
  communication for CUDA memory
- Improved error message

## Net

- Temporarily forked, optimized version of tcp provider
- Focused on improved performance and scalability over tcp sockets
- Fork ensures tcp provider stability while net provider is developed
- Shares the tcp provider protocol and base implementation for msg endpoints
- Integrates direct support for rdm endpoints, using a derivative from rxm
- Implements own protocol for rdm endpoints, separate from rxm;tcp

## OPX

- Added initial support for SDMA
- General performance enhancements
- Performance improvements to reliability protocol
- Improved deferred work pending complete
- Added support for OPX_AV=runtime
- Support iov memory registration ops
- Added DAOS RPC support
- Atomic ops enhancements
- Improved documentation
- Debug build enhancements
- Fixed compiler warnings
- Reduced time to compile prov/opx code
- General bug fixes
- Fixed PSN wrapping scaling
- Added intranode fence
- Addressed bugs discovered by coverity scan

## PSM2

- Fix sending CQ data in some instances of fi_tsendmsg

## PSM3

- Updated to match Intel Ethernet Fabric Suite (IEFS) 11.3 release

## RxM

- Update to read multiple completions at once from msg provider
- Move RxM AV implementation to util code to share with net provider
- Minor code cleanups

## SHM

- Implement and use ipc_cache
- Add log messages for debugging and error tracking
- Fix check for FI_MR_HMEM mr_mode
- Move shm signal handlers initialization to EP
- Added log messages for errors detected

## TCP

- Fix incorrect signaling of the CQ
- Increase max number of poll events to retrieve
- Acquire ep lock prior to flushing socket in shutdown
- Verify ep state prior to progressing socket data
- Read cm error data when receiving connreq response
- Log error on connect failure
- Fix assertion failure in CQ progress function

## Util

- Fix text in log of UFFD ioctl failure
- Introduce cuda ipc monitor
- Fix CQ memory leak handling overflow
- Fix MR mode bit check for ver 1.5 and greater
- Add max_array_size to track/check array overflow
- Always progress transfers when reading from a CQ
- Handle NULL address insertion
- Try IPv4 before IPv6 addresses when starting name server
- Fix IP util av default address length
- Fix util IP getinfo path to read hints->addr_format
- Fix debug print mismatch
- Fix return code when memory allocation fails.
- Fix build sign warning in ofi_bufpool_region_alloc
- Minor code cleanups
- Print warning if an addr is inserted into an AV again

## Verbs

- Fix support of FI_SOCKADDR_IB when requested by the application
- Ensure all posted receives are flushed to the application
- Update ofi_mr_cache_search API for hmem IPC support
- Reduce logging verbosity for "no active ports"
- Fix incorrect length used in memory registration
- Various minor bug fixes for test failures
- Fix a memory leak getting IB address
- Implement verbs provider on Windows over NetworkDirect API
- Set and check address format correctly
- Only close qp if it was initialized
- Portable detection of loopback device

## Fabtests

- multi_ep: Separate EP resources and fix MR registration
- multi_recv: Fix possible crash and check for valid buffer
- unexpected_msg: Fix printf compiler warning
- dgram_pingpong.c: Use out-of-band sync
- multinode: Make multinode tests platform agnostic, fix formatting
- ubertest: Fix string comparison to include length, fix writedata completion check
- av_test: add support for -e <ep_type>

### New tests

- dmabuf-rdma: Component level test for dma-buf RDMA
- sock_test: Component level performance test of poll, epoll, and select
- rdm_stress: Multi-threaded, multi-process stress test for RDM endpoints
- sighandler_test: Regression test for signal handler restoration

### Common

- Pass in correct remote_fi_addr instead of 0 on fi_recv
- Ensure that first option is processed in getopt
- Save and restore errno in log messages
- Windows: Free hints memory in module that allocated it,
  allow building verbs tests on windows

### pytest/efa

- Run fi_getinfo_test with GID as address
- Add function efa_retrieve_gid()
- Skip runt_read test for single node
- Increase number of sends for read_rnr_cq_entry
- Verify the prov error message in rnr_read_cq_error()
- Add test case for runt read protocol
- Return None if HW counter does not exist
- Extend rma_bw test to multiple memory types
- Test multi_recv with 8k message size
- Increase timeout limit for cuda tests when testing all msg sizes.
- Adjust dgram test

### pytest/common

- Introduce shm test suite
- Skip cuda tests if provider does not support hmem hints
- Add pyyaml to requirements.txt
- Fix a bug in processing return code
- Add warmup_iteration_type to ClientServerTest
- Adjust default behavior of junit_xml
- Increase ssh ConnectTimeout

### HMEM testing options

- ZE: Increase the number of supported ZE devices
- CUDA: Use device allocated host buffer to fill device buffer
- CUDA: Ensure data consistency, add cuda_memory market in pytest
- Run check_hmem correctly
- Fix issues in check_hmem.c

### EFA provider specific tests

- Add more message range tests
- Add fork-related test
- Fix the command in efa_retrieve_hw_counter_value()
- Do not run efa rnr test with strict mode
- Add efa fabric id test check

### Scripts

- runfabtests.py: Support an argument to specify junit report verbosity
- runfabtests.py  remove unnecessary good_address argument
- runfabtests.cmd: Rewrite to be more like runfabtests.sh
- fabtests/scripts: Add runmultinode.sh
- runfabtests.sh: Print timestamp of each test, fix -e option

v1.15.2, Mon Aug 22, 2022
=========================

## Core

- Fix incorrect cleanup on gdrcopy initialization success
- Change the neuron library file name
- Use neuron's memcpy API to copy from host to device.
- Fix signaling race in pollfds abstraction
- Check correct number of events in pollfds abstraction
- Fix locking in pollfds reading event contexts
- Reserve CXI provider constants to avoid future conflicts
- Initialize genlock lock_type to fix always picking mutex
- Prioritize psm2 over opx provider

## EFA

- Release tx_entry on error in rxr_atomic_generic_efa()
- Avoid iteration of iov array to address coverity report
- Add locks around MR map
- Fix RNR error reporting and handling

## Hooks

- Close dmabuf fd when no longer in use

## RxM

- Read multiple completions at once to limit progress looping
- Fix windows compiler errors
- Use sparse logging for common EQ errors
- Fix a memory leak in AV remove path
- Reject simultaneous connections with correct error code

## SHM

- Fix incorrect use of peer_id with id in SAR path

## TCP

- Verify endpoint state prior to progressing socket data
- Acquire ep lock prior to flushing socket in shutdown
- Fix incorrect signaling of the CQ for threads waiting in sread

## OPX

- Disable OPX provider if not supported by platform

## PSM3

- Add missing reference to neuron_init
- Remove bashisms from configure script

## Util

- Fix non-error CQ auxillary queue entry memory leak
- Check for duplicate address insertion in util AV code
- Fix locking around removing an address from an AV

## Verbs

- Fix incorrect length for Ze HMEM memory registration
- Fix memory leak when closing a device

v1.15.1, Fri May 13, 2022
=========================

## Core

- Fix windows implementation to remove fd from poll set

## PSM3

- Add missing files to release tarball

## Util

- Handle NULL address insertion to fi_av_insert

v1.15.0, Fri Apr 29, 2022
=========================

## Core

- Fix fi_info indentation error in fi_tostr
- hmem_ze: Add runtime option to choose specific copy engine
- Cleanup of configure HMEM checks
- Fixed stringop-truncation in ofi_ifaddr_get_speed
- Add utility provider log suffix to make logs easier to read
- Fix truncation of ipv6 addressing
- hmem: add support for AWS Trainium devices
- Fix potential sscanf overflows
- hmem: pass through device and flags when querying memory interface
- Rework locking in several areas to convert spinlocks to mutexes
- Add new locking abstractions to select lock types at runtime
- Add new FI_PROTO_RXM_TCP for optimized rxm over tcp path

## EFA

- Added windows support through efawin (https://github.com/aws/efawin)
- Added support of AWS neuron.
- Added support of using gdrcopy to copy data from host to device.
- Fixed a bug that cause 0 byte read to fail.
- Fixed a memory corruption issue that can caused forked process to crash.
- Extended testing coverage through new pytest based testing framework.

## HOOKS

- Add new hooking provider dmabuf_peer_mem
- Enable DL build of hooking providers
- Add HMEM memory registration hook

## OPX

- New provider supporting Cornelis Networks Omni-path hardware

## PSM3

- Updated psm3 to match IEFS 11.2.0.0 release
- Added support for sockets (TCP/UDP) via a runtime selectable Hardware
  Abstraction Layer (HAL)
- Added support for IPv6 addressing in RoCE and sockets
- Added various NIC selection filtering options (wildcarded NIC name,
  address format, wildcarded IP subnet, link speed)
- Performance tuning in conjunction with OneAPI and OneCCL
- Tested with aws-nccl-plugin using NVIDIA NCCL over OFI over PSM3
- Improved PSM3_IDENTIFY output
- Rename most internal symbols to psm3_
- Corrected vulnerabilities found during Coverity scans
- configure options refined and help text improved
- PSM3_MULTI_EP has been deprecated (recommend always enabled, default
  is enabled [same default as previous releases])
- Various bug fixes

## RxM

- Add check that atomic size is valid
- Add support to passthru calls to tcp provider in specific cases

## TCP

- Add assert to verify RMA source/target msg sizes match
- Wake-up threads blocked on CQ to update their poll events
- Fix use of incorrect events in progress handler
- Fixes for various compile warnings, mostly on Windows
- Add support for FI_RMA_EVENT capability
- Add support for completion counters
- Fix check for CQ data in tagged messages
- Add cancel support to shared rx context
- Add src_addr receive buffer matching
- Add provider control to assign a src_addr with an ep
- Handle trecv with FI_PEEK flag
- Allow binding a CQ with an SRX
- Restructuring of code in source files
- Handle EWOULDBLOCK returned by send call
- Add hot (active) pollfd list

## SHM

- Properly chain the original signal handlers
- Avoid uninitialized variable with invalid atomic parameters
- Fix 0 byte SAR read
- Initialize len parameter to accept
- Refactor and simplify protocol code
- Remove broken support for 128-bit atomics
- Fix FI_INJECT flag support
- Add assert to verify RMA source/target msg sizes match
- Set domain threading to thread safe
- Fix possible use of uninitiated var in av_insert

## Util

- Fix sign warning in ofi_bufpool_region_alloc
- Remove unused variable from ofi_bufpool_destroy
- Fix check for valid datatype in ofi_atomic_valid
- Return with error if util_coll_sched_copy fails
- Fix use of uninitialized variable in ofi_ep_allreduce
- Fix memory access in ip_av_insertsym
- Track ep per collective operation not with multicast
- Restructure collective av set creation/destruction
- Change most locks from spin locks to mutexes
- Allow selection of spinlocks for CQ and domain objects
- Fix AV default addrlen
- Update fi_getinfo checks to include hints->addr_format

## Verbs

- Initial changes for compiling on Windows (via NetworkDirect)
- Add a failover path to dma-buf based memory registration
- Replace use of spin locks with mutexes
- Check for valid qp prior to cleanup
- Set and check for address format correct in fi_getinfo

## Fabtests

- hmem_cuda: used device allocated host buff to fill device buf
- Add python scripts to control test execution
- test_configs: include util provider in core config file
- Add option "--pin-core"
- Only call nrt_init once
- Fix a bug in ft_neuron_cleanup
- Correct help for unit test programs
- Remove duplicate help prints from fi_mcast
- configure.ac: fix --enable-debug=no not properly detected
- msg_inject: handle the case ft_tsendmsg return -FI_EAGAIN
- Add AWS Trainium device support
- fi_inj_complete: Add FI_INJECT to fabtests
- inj_complete.c: Make arguments align with the other tests
- dgram_pingpong: handle the error return of fi_recv
- recv_cancel: Remove requirement for unexpected msg handling
- poll: Fix crash if unable to allocate pollset
- ubertest: Add GPU testing and validation support
- Add HMEM options parsing support
- Update and re-enable fi_multi_ep test

v1.14.1, Fri Apr 15, 2022
=========================

## Core

- Use non-shared memory allocations to use MADV_DONTFORK safely
- Various fixes for compiler warnings
- Fix incorrect use of gdr_copy_from_mapping
- Ensure proper timeout time for pollfds to avoid early exit

## EFA

- Use non-shared buffer pool allocations to use MADV_DONTFORK safely
- Handle read completion properly for multi_recv
- Use shm's inject write when possible
- Support 0 byte read

## RxD

- Verify valid atomic size

## RxM

- Ensure signaling the CQ fd after writing completion
- Fix inject path for sending tagged messages with cq data
- Negotiate credit based flow control support over CM
- Add PID to CM messages to detect stale vs duplicate connections
- Fix race handling unexpected messages from unknown peers
- Fix possible leak of stack data in cm_accept
- Restrict reported caps based on core provider
- Delay starting listen until endpoint fully initialized
- Verify valid atomic size

## Sockets

- Fix coverity reports on uninitialized data
- Check for NULL pointers passed to memcpy
- Minor cleanups
- Add missing error return code from sock_ep_enable

## TCP

- Fix performance regression resulting from sparse pollfd sets
- Fix assertion failure in CQ progress function
- Do not generate error completions for inject msgs
- Fix use of incorrect event names in progress handler
- Fix check for CQ data in tagged messages
- Make start_op array a static to reduce memory
- Wake-up threads blocked on CQ to update their poll events

## Verbs

- Generate error completions for all failed transmits
- Set all fields in the fi_fabric_attr for FI_CONNREQ events
- Set proper completion flags for all failed transfer
- Minor updates to silence coverity warnings on NULL pointers
- Ensure that all attributes are provided when opening an endpoint
- Fix error handling in vrb_eq_read
- Fix memory leak in error case in vrb_get_sib
- Work-around bug in verbs HW not reported correct send opcodes
- Only call ibv_reg_dmabuf_mr when kernel support exists
- Add a failover path to dma-buf based memory registration
- Negotiate credit based flow control support over CM
- Add OS portable detection of loopback devices

## Fabtests

- Disable inject when FI_HMEM is enabled
- Increase the number of supported ZE devices
- Change cq format if remote cq data is received
- Fix ubertest config exclude file check
- Fix ubertest checks for expected completions

v1.14.0, Fri Nov 19, 2021
=========================

## Core

- Add time stamps to log messages
- Fix gdrcopy calculation of memory region size when aligned
- Allow user to disable use of p2p transfers
- Update fi_tostr print FI_SHARED_CONTEXT text instead of value
- Update fi_tostr to output field names matching header file names
- Fix narrow race condition in ofi_init
- Minor optimization to pollfds to handle timeout of 0
- Add new fi_log_sparse API to rate limit repeated log output
- Define memory registration for buffers used for collective operations

## EFA
- Provide better support for long lived applications utilizing the RDM
  endpoint, that may reuse an EFA queue pair after an application restarts.
- Fixes for RNR support (enabled in v1.13.1), to allow Libfabric to manage
  backoff when a receiver's queue is exhausted. A setopt parameter was added to
  allow applications to set the number of re-transmissions done by the device
  before a packet is queued by Libfabric, or if Libfabric is configured to not
  handle resource errors, write an error entry to the application.
- Potentially reduce memory utilization by waiting until first CQ read to
  allocate pools
- Deprecate the FI_EFA_SHM_MAX_MEDIUM_SIZE environment variable
- Fix a bug in the send path which caused a performance regression for large
  messages
- Fix issue in MR registration path when cache is used with CUDA buffers
- Print a clearer warning message when the reorder buffer is too small
- Various bugfixes in send path causing unneeded copies
- Various bugfixes caught by inspection and coverity
- Add documentation describing version 4 of the RDM protocol

## SHM

- Separate HMEM caps and disable FI_ATOMIC when requested
- Fix casting ints to pointers of different sizes
- Add error checking in smr_setname
- Distinguish between max shm name and max path name
- Move allocation of sar_msg into smr_format_sar()

## TCP

- Use IP_BIND_ADDRESS_NO_PORT socket option to improve scaling
- Fix situation where we can leave socket in blocking mode
- Add specific fi_info output to fi_getinfo for srx case
- Code restructuring and renames to improve maintenance
- Initial implementation to support tagged messages at tcp layer
- Optimize RMA handling at receiver
- Remove non-defined CQ flags when reporting completions

## RXM

- Reset connection state if we receive a new connection request
- Increase and update debug log messages to be more consistent
- Force CM progress if msg ep's are actively connecting
- Optimize handling for cm_progress_interval = 0

## Util

- Fix fi_getinfo check if provider requires the use of shared contexts
- Replace deprecated pthread_yield with sched_yield
- Fix compiler warning mixing u64 with size_t fields
- Fix memory leak in util_av_set_close
- Fix ofi_av_set to use passed in start_addr and end_addr values
- Add logic to detect if another library is intercepting memory calls
- Update 128-bit atomic support
- Fix possible deadlock if multiple memory monitors are enabled for the
  same memory type

## Verbs

- Fix setting MR access to handle read-only buffers
- Expand debug output
- Fail FI_HMEM support if p2p is disabled
- Handle FI_HMEM_HOST_ALLOC flag for FI_HMEM_ZE

## Fabtests

- Fix rdm_rma_trigger support for hmem
- Add key exchanges to common code to support device memory
- Remove need for OOB address exchange when hmem is enabled
- Always use command line provided inject size when given
- Add ability to test tagged messages over msg ep's
- Add support for shared rx contexts to common code
- Update scripts to allow provider specific fabtests
- Add an EFA RDM RNR fabtest

v1.13.2, Fri Oct 15, 2021
========================

## Core

- Provide work-around for segfault in Ze destructor using DL provider
- Minor code fixes supporting Ze
- Use copy only engine when accessing GPUs through Ze
- Sort DL providers to ensure consistent load ordering
- Update hooking providers to handle fi_open_ops calls to avoid crashes
- Replace cassert with assert.h to avoid C++ headers in C code
- Enhance serialization for memory monitors to handle external monitors

## EFA

- Limit memcpy in packet processing to only copy valid data
- Removed maximum wait time sending packet to avoid silent drops
- Fix unconditionally growing buffer pools that should not grow
- Handle possible large backlog of unexpected messages via SHM
- Update Tx counter for inject operations
- Allow in flight sends to finish when closing endpoint
- Fix handing of prefix size when receiving data
- Removed unnecessary data copy

## SHM

- Fix possible sigbus error
- Handle errors if peer is not yet initialized

## TCP

- Fix reporting RMA write CQ data
- Fix RMA read request error completion handling
- Avoid possible use after free in reject path
- Remove restriction where EQs and CQs may not share wait sets
- Increase max supported rx size
- Fix possible memory leak of CM context structure in error cases
- Set source address for active EPs to ensure correct address is used
- Fix memory leak of dest address in CM requests

## RxM

- Improve connection handling responsiveness to fix application stalls
- Add missing locks around AV data structures
- Add missing hmem initialization for DL builds
- Do not ignore user specified rx/tx sizes
- Fix source address reported to peer
- Fix possible use of uninitialized memory handling CQ errors
- Fix address comparison to remove duplicate connections
- Reworked CM code to fix several possible crash scenarios
- Fix setting the conn_id when generated 'fake' tagged headers

## Util

- Fix AV set to use non-zero starting address
- Fix setting of CQ completion flags

## Verbs

- Work-around compilation error with Intel compiler 2018.3.222
- Avoid possible user after free issue accessing rdma cm id in error cases

## Fabtests

- Add missing prints to fi_av_xfer to report failures
- Fix memory leak in fi_multinode test
- Add device validation for hmem tests
- Update fi_info hints mode field based on user options
- Fix use of incorrect message prefix sized in fi_pingpong test

v1.13.1, Tue Aug 24, 2021
=========================

## Core

- Fix ZE check in configure
- Enable loading ZE library with dlopen()
- Add IPv6 support to fi_pingpong
- Fix the call to fi_recv in fi_pingpong

## EFA

- Split ep->rx_entry_queued_list into two lists
- Split ep->tx_entry_queued_list into two lists
- Only set FI_HMEM hint for SHM getinfo when requested
- Include qkey in smr name
- Do not ignore send completion for a local read operation
- Convert pkt_entry->state to pkt_entry->flags
- Detect recvwin overflow and print an error message
- Add function ofi_recvwin_id_processed()
- Let efa_av_remove() remove peer with resources
- Ignore received packets from a remove address
- Check for and handle empty util_av->ep_list in efa_av
- Invalidate peer's outstanding TX packets' address when removing peer
- Extend the scope of deep cleaning resources in rxr_ep_free_res()
- Eefactor error handling functions for x_entry
- Only write RNR error completion for send operation
- Ignore TX completion to a removed peer.
- Release peer's tx_entry and rx_entry when removing peer
- Make efa_conn->ep_addr a pointer and use it to identify removed peer
- Mix the using of released packet in rxr_cq_handler_error()
- Refactor tx ops counter updating
- Make rxr_release_tx_entry() release queued pkts
- Rename rxr_pkt_entry->type to rxr_pkt_entry->alloc_type
- Initialize rxr_pkt_entry->x_entry to NULL
- Fix ep->pkt_sendv_pool size
- Add rnr_backoff prefix to variables related to RNR backoff
- Refactor rxr_cq_queue_pkt()
- Eliminate rnr_timeout_exp in rdm_peer
- Eliminate the flag RXR_PEER_BACKED_OFF
- Adjust unexpected packet pool chunk size
- Defer memory allocation to 1st call to progress engine
- Enable RNR support
- Remove peer from backoff peer list in efa_rdm_peer_reset()
- Make rxr_pkt_req_max_header_size use RXR_REQ_OPT_RAW_ADDR_HDR_SIZE
- Use ibv_is_fork_initialized in EFA fork support

## PSM3

- Update Versions
- Clean ref's to split cuda hostbufs when no longer needed
- Fix issue when running gpudirect on gpu with small bar size
- Fix issues with debug statistics
- Fix issue with unreleased MR in cache

## SHM

- Fix unsigned comparison introduced in #6948
- Use hmem iov copies in mmap progression
- Correct return values in smr_progress.c
- Fix smr_progress_ipc error handling

## Util

- Do not override default monitor if already set
- Do not set impmon.impfid to NULL on monitor init
- Initialize the import monitor
- Add memory monitor for ZE

## Fabtests

- Use dlopen to load ZE library
- Bug fixes related to IPv6 address format
- Do not immediately kill server process

v1.13.0, Thu Jul 1, 2021
========================

## Core

- Fix behavior of fi_param_get parsing an invalid boolean value
- Add new APIs to open, export, and import specialized fid's
- Define ability to import a monitor into the registration cache
- Add API support for INT128/UINT128 atomics
- Fix incorrect check for provider name in getinfo filtering path
- Allow core providers to return default attributes which are lower then
  maximum supported attributes in getinfo call
- Add option prefer external providers (in order discovered) over internal
  providers, regardless of provider version
- Separate Ze (level-0) and DRM dependencies
- Always maintain a list of all discovered providers
- Fix incorrect CUDA warnings
- Fix bug in cuda init/cleanup checking for gdrcopy support
- Shift order providers are called from in fi_getinfo, move psm2 ahead of
  psm3 and efa ahead of psmX

## EFA

- Minor code optimizations and bug fixed
- Add support for fi_inject for RDM messages
- Improve handling of RNR NACKs from NIC
- Improve handling of zero copy receive case, especially when sender does not
  post receive buffer
- Numerous RMA read bug fixes
- Add unexpected receive queue for each peer
- Fixed issue releasing rx entries
- Decrease the initial size of the out-of-order packeet pool allocation size
  to reduce the common-case memory footprint
- Handle FI_ADDR_NOTAVAIL in rxr_ep_get_peer
- Identify and handle QP reuse
- Use the memory monitor specified by the user
- Replace provider code with common code in select places
- Update efa_av_lookup to return correct address
- Update rdm endpoint directly poll cq from ibv_cq
- Avoid possible duplicate completions
- Add reference counting for peer tracking
- Fix EFA usage of util AV causing incorrect refcounting
- Do not allow endpoints to share address vectors
- Improve fork support; users can set the FI_EFA_FORK_SAFE environment variable
  for applications which call fork()
- Adjust the timing of clearing deferred memory registration list
- Do not use eager protocol for cuda message and local peer
- Fixes for shm support
- Enable MR cache for CUDA
- Disable shm when application requests FI_HMEM

## PSM3

- Added CUDA Support, GPU Direct through RV kernel module
- Changed PSM3 Provider Version to match IEFS version
- Expanded Multi-Rail support
- Enhanced debug logging
- Removed internal copy of libuuid, added as linked lib
- Various Bug Fixes

## RxD

- Fix peer connection and address cleanup
- Maintain peer connection after AV removal to send ACKs

## RxM

- Fix rx buffer leak in error case
- Dynamically allocate buffer space for large unexpected messages
- Separate the eager protocol size from allocated receive buffers
  to reduce memory footprint
- Make eager limit a per ep value, rather than global for all peers
- Separate definitions and use of buffer, eager, and packet sizes
- Fix calling fi_getinfo to the msg provider with FI_SOURCE set but
  null parameters
- General code cleanups, simplifications, and optimizations
- Fix retrieving tag from dynamic receive buffer path
- Enable dynamic receive buffer path over tcp by default
- Use correct check to select between tagged and untagged rx queues
- Repost rx buffers immediately to fix situation where applications can hang
- Update help text for several environment variables
- Fix use_srx check to enable srx by default layering over tcp provider
- Reduce default tx/rx sizes to shrink memory footprint
- Fix leaving stale peer entries in the AV
- Handle error completions from the msg provider properly, and avoid passing
  internal transfers up to the application
- Reduce memory footprint by combining inject packets into one
- Reduce inject copy overhead by using memcpy instead of hmem copy routines
- Restrict the number of outstanding user transfers to prevent memory
  overflow
- Enable direct send feature by default for the tcp provider
- Fix initialization of atomic headers
- Only ignore interrupts in wait calls (e.g. poll) in debug builds, otherwise
  return control to the caller
- Combine and simplify internal buffer pools to reduce memory footprint
- Remove request for huge pages for internal buffer pools
- Add optimized tagged message path over tcp provider, removing need for
  rxm header overhead
- Several optimizations around supporting rxm over tcp provider

## SHM

- Use signal to reduce lock contention between processes
- Fix communication with a peer that was restarted
- Code cleanup to handle issues reported by coverity
- Add check that IPC protocol is accessing device only memory
- Fix interface selection used for IPC transfers
- Change address to use a global ep index to support apps that open
  multiple fabrics
- Add environment variable to disable CMA transfers, to handle environments
  where CMA checks may succeed, but CMA may not be usable
- Add missing lock in ofi_av_insert_addr
- Add support for GPU memory in inject operations.

## Sockets

- Fix possible ring buffer overflow calculating atomic lengths
- Use correct address length (IPv6 vs 4) walking through address array

## TCP

- Add send side coalescing buffer to improve small message handling
- Add receive side prefetch buffer to reduce kernel transitions
- Fix initializing the mr_iov_limit domain attribute
- Add support for zero copy transfers, with configurable threshold settings.
  Disable zero copy by default due to negative impact on overall performance
- Add environment variable overrides for default tx/rx sizes
- Simplify and optimize handling of protocol headers
- Add a priority transmit queue for internally generated messages (e.g. ACKs)
- Check that the endpoint state is valid before attempting to drive progress
  on the underlying socket
- Limit the number of outstanding transmit and receive operations that a
  user may post to an ep
- Remove limitations on allocating internally generated messages to prevent
  application hangs
- Combine multiple internal buffer pools to one to reduce memory footprint
- Optimize socket progress based on signaled events
- Optimize pollfd abstraction to replace linear searches with direct indexing
- Update both rx and tx cq's socket poll list to prevent application hangs
- Optimize reading in extra headers to reduce loop overhead
- Continue progressing transmit data until socket is full to reduce progress
  overhead
- Add msg id field to protocol headers (debug only) for protocol debugging
- Drive rx progress when there's an unmatched 0-byte received message to
  avoid application hangs
- Avoid kernel transitions that are likely to do not work (return EAGAIN)
- Fail try_wait call if there's data already queued in user space prefetch
  buffers to avoid possible hangs
- Fix possible access to freed tx entry
- Optimize socket receive calls in progress function to skip progress loop
  and immediately handle a received header.  This also fixes an application
  hang handling 0-byte messages
- Broad code cleanups, rework, and simplifications aimed at reducing
  overhead and improving code stability
- Improve handling of socket disconnect or fatal protocol errors
- Fix reporting failures of internal messages to the user
- Disable endpoints on fatal protocol errors
- Validate response messages are what is expected
- Simplify and align transmit, receive, and response handling to improve code
  maintainability and simplify related data structures
- Copy small messages through a coalescing buffer to avoid passing SGL to
  the kernel
- Fix race handling a disconnected event during the CM handshake
- Report default attributes that are lower than the supported maximums
- Remove use of huge pages, which aren't needed by tcp, to reserve them for
  the user
- Increase default inject size to be larger than the rxm header
- Add tagged message protocol header for sending tagged messages using the
  tcp headers only
- Separate definition of maximum header size from maximum inject size

## Util

- Added lock validation checks to debug builds
- Fix MR cache flush LRU behavior
- Always remove dead memory regions from the MR cache immediately
- Update buffer pools to handle an alignment of 0
- Fail memory registration calls for HMEM if the interface isn't available
- Pass through failures when a requested memory monitor fails to start
- Always process deferred work list from pollfd wait abstraction

## Verbs

- Fixed checks setting CQ signaling vector
- Internal code cleanups and clarifications
- Fixed XRC MOFED 5.2 incompatibility
- Add dmabuf MR support for GPU P2P transfers

v1.12.1, Thu Apr 1, 2021
========================

## Core

- Fix initialization checks for CUDA HMEM support
- Fail if a memory monitor is requested but not available
- Adjust priority of psm3 provider to prefer HW specific providers,
  such as efa and psm2

## EFA
- Adjust timing clearing the deferred MR list to fix memory leak
- Repost handshake packets on EAGAIN failure
- Enable mr cache for CUDA memory
- Support FI_HMEM and FI_LOCAL_COMM when used together
- Skip using shm provider when FI_HMEM is requested

## PSM3
- Fix AVX2 configure check
- Fix conflict with with-psm2-src build option to prevent duplicate
  symbols
- Fix checksum generation to support different builddir
- Remove dependency on librdmacm header files
- Use AR variable instead of calling ar directly in automake tools
- Add missing PACK_SUFFIX to header

v1.12.0, Mon Mar 8, 2021
=========================

## Core

- Added re-entrant version of fi_tostr
- Added fi_control commands for accessing fid-specific attributes
- Added Ze (level-0) HMEM API support
- Fixed RoCR memory checks
- Minor code cleanups, restructuring, and fixes
- Fix possible stack buffer overflow with address string conversion
- Handle macOS socket API size limitations
- Verify and improve support for CUDA devices
- Update internal string functions to protect against buffer overflow
- Support gdrcopy in addition to cudaMemcpy to avoid deadlocks
- Properly mark if addresses support only local communication
- Prevent providers from layering over each other non-optimally
- Fix pollfds abstraction to fix possible use after free

## EFA
- Added support for FI_DELIVERY_COMPLETE via an acknowledgment packet in the
  provider. Applications that request FI_DELIVERY_COMPLETE will see a
  performance impact from this release onward. The default delivery semantic
  for EFA is still FI_TRANSMIT_COMPLETE and acknowledgment packets will not be
  sent in this mode.
- Added ability for the provider to notify device that it can correctly handle
  receiver not ready (RNR) errors. There are still known issues so this is
  currently turned off by default; the device is still configured to retry
  indefinitely.
- Disable FI_HMEM when FI_LOCAL_COMM is requested due to problems in the
  provider with loopback support for FI_HMEM buffers.
- Use a loopback read to copy from host memory to FI_HMEM buffers in the
  receive path. This has a performance impact, but using the native copy API
  for CUDA can cause a deadlock when the EFA provider is used with NCCL.
- Only allow fork support when the cache is disabled, i.e. the application
  handles registrations (FI_MR_LOCAL) to prevent potential data corruption.
  General fork support will be addressed in a future release.
- Moved EFA fork handler check to only trigger when an EFA device is present
  and EFA is selected by an application.
- Changed default memory registration cache monitor back to userfaultfd due to
  a conflict with the memory hooks installed by Open MPI.
- Fixed an issue where packets were incorrectly queued which caused message
  ordering issues for messages the EFA provider sent via SHM provider.
- Fixed a bug where bounce buffers were used instead of application provided
  memory registration descriptors.
- Various fixes for AV and FI_HMEM capability checks in the getinfo path.
- Fix bug in the GPUDirect support detection path.
- Various fixes and refactoring to the protocol implementation to resolve some
  memory leaks and hangs.

## PSM3

- New core provider for psm3.x protocol over verbs UD interfaces, with
  additional features over Intel E810 RoCEv2 capable NICs
- See fi_psm3.7 man page for more details

## RxD

- Added missing cleanup to free peer endpoint data with AV
- Add support for FI_SYNC_ERR flag

## RxM

- Cleanup atomic buffer pool lock resources
- Fix unexpected message handling when using multi-recv buffers
- Handle SAR and rendezvous messages received into multi-recv buffers
- Give application entire size of eager buffer region
- Minor code cleanups based on static code analysis
- Simplify rendezvous message code paths
- Avoid passing internal errors handling progress directly to applications
- Limit fi_cancel to canceling at most 1 receive operation
- Remove incorrect handling if errors occur writing to a CQ
- Only write 1 CQ entry if a SAR message fails
- Continue processing if the receive buffer pool is full and reposting delayed
- Add support for dynamic receive buffering when layering over tcp
- Add support for direct send to avoid send bounce buffers in certain cases
- Prioritize credit messages to avoid deadlock
- Fix conversion to message provider's mr access flags
- Reduce inject size by the minimum packet header needed by rxm
- Fix checks to enable shared rx when creating an endpoint
- Minor code restructuring
- Fix trying to access freed memory in error handling case
- Use optimized inject limits to avoid bounce buffer copies
- Fix possible invalid pointer access handling rx errors
- Add support for HMEM if supported by msg provider
- Add missing locks around progress to silence thread-sanitizer
- Support re-connecting to peers if peer disconnects (client-server model)
- Cleanup rendezvous protocol handling
- Add support for RMA write rendezvous protocol

## SHM

- Add support for Ze IPC protocol
- Only perform IPC protocol related cleanup when using IPC
- Disable cross-memory attach protocol when HMEM is enabled
- Fix cross-memory attach support when running in containers
- Always call SAR protocol's progress function
- Enable cross-memory attach protocol when sending to self
- Minor code cleanups and restructuring for maintenance

## Sockets

- Verify CM data size is less than supported value
- Handle FI_SYNC_ERR flag on AV insert
- Improve destination IP address checks
- Minor coding cleanups based on static code analysis
- Fix possible use after free access in Rx progress handling

## TCP

- Fix hangs on windows during connection setup
- Relax CQ checks when enabling EP to handle send/recv only EPs
- Fix possible use of unset return value in EP enable
- Minor coding cleanups based on static code analysis
- Handle EAGAIN during CM message exchanges
- Set sockets to nonblocking on creation to avoid possible hangs at scale
- Improve CM state tracking and optimize CM message flows
- Make passive endpoints nonblocking to avoid hangs
- Allow reading buffered data from disconnected endpoints
- Implement fi_cancel for receive queues
- Flush outstanding operations to user when an EP is disabled
- Support dynamic receive buffering - removes need for bounce buffers
- Add direct send feature - removes need for bounce buffers
- Minor code cleanups and restructuring to improve maintenance
- Add support for fo_domain_bind

## Util

- Improve checks that EPs are bound to necessary CQs
- Fix mistaking the AV's total size with current count to size properly
- Fix CQ buffer overrun protection mechanisms to avoid lost events

## Verbs

- Add SW credit flow control to improve performance over Ethernet
- Skip verbs devices that report faulty information
- Limit inline messages to iov = 1 to support more devices
- Minor code improvements and restructuring to improve maintenance
- Enable caching of device memory (RoCR, CUDA, Ze) registrations
- Add HMEM support, including proprietary verbs support for P2P
- Add support for registering device memory
- Support GIDs at any GID index, not just 0
- Fix macro definitions to cleanup build warnings
- Support GID based connection establishment, removes ipoib requirement
- Reduce per peer memory footprint for large scale fabrics

v1.11.2, Tue Dec 15, 2020
=========================

## Core

- Handle data transfers > 4GB on OS X over tcp sockets
- Fixed spelling and syntax in man pages
- Fix pmem instruction checks

## EFA

- Use memory registration for emulated read protocol
- Update send paths to use app memory descriptor if available
- Remove unneeded check for local memory registration
- Do not install fork handler if EFA is not used
- Fix medium message RTM protocol
- Fix memory registration leak in error path
- Fix posting of REQ packets when using shm provider

## RxM

- Fix provider initialization when built as a dynamic library

## SHM

- Reverts SAR buffer locking patch
- Include correct header file for process_vm_readv/writev syscalls
- Skip atomic fetch processing for non-fetch operations

## TCP

- Fix swapping of address and CQ data in RMA inject path

## Util

- Fix error code returned for invalid AV flags
- Fix a bug finding the end of a page when the address is aligned

## Verbs

- Fix build warning in XRC CM log messages
- Fix build warnings in debug macros

v1.11.1, Fri Oct 9, 2021
========================

## Core

- Remove calls to cuInit to prevent indirect call to fork
- Ignore case when comparing provider names
- Prevent layering util providers over EFA
- Fix segfault if passed a NULL address to print
- Fail build if CUDA is requested but not available

## EFA

- Switch to memhooks monitor
- Avoid potential deadlock copying data to GPU buffers
- Allow creating packet pools with non-huge pages
- Check return value when processing data packets
- Minor code restructuring and bug fixes
- Check if outstanding TX limit has been reached prior to sending
- Move RDMA read registration to post time
- Do not overwrite a packet's associated MR when copying packets
- Pass in correct packet when determining the header size
- Do not release rx_entry in EAGAIN case
- Disable MR cache if fork support is requested
- Turn off MR cache if user supports FI_MR_LOCAL
- Add FI_REMOTE_READ to shm registrations
- Remove use_cnt assert closing domain to allow driver cleanup
- Fix off by 1 returned AV address when using AV map
- Ensure setting FI_HMEM capability is backwards compatible

## RxD

- Fix bug that prevents sending timely ACKs for segmented messages
- Remove calls that recursively try to acquire the EP lock

## RxM

- Allow re-connecting to peers

## SHM

- Create duplicate fi_info's when reporting FI_HMEM support
- Handle transfers larger than 2GB
- Register for signal using SA_ONSTACK
- Fix segfault if peer has not been inserted into local AV
- Fix command/buffer tracking for sending connection requests
- Return proper errno on AV lookup failures
- Remove duplicate call to ofi_hmem_init
- Fix using incorrect peer id for mid-sized message transfers
- Fix addressing race conditions
- Fix mixing of shm AV index values with fi_addr_t values
- Fix initialization synchronization
- Ensure progress is invoked for mid-sized message transfers
- Always use CMA when sending data to self
- Fix hang using SAR protocol

## Sockets

- Retry address lookup for messages received during CM setup

## TCP

- Fix possible deadlock during EP shutdown due lock inversion
- Rework CM state machine to fix lock inversion handling disconnect

## Util

- Correctly mark if addresses support local/remote communication
- Check madvise memhook advice
- Update mmap intercept hook function
- Replace memhooks implementation to intercept syscalls
- Fix shmat intercept hook handling
- Fix error handling obtaining page sizes
- Fix incorrect locking in MR cache
- Fix memory leak in rbtree cleanup

## Verbs

- Fix XRC transport shared INI QP locking
- Account for off-by-one flow control credit issue
- Fix disabling of receive queue flow control
- Reduce overall memory footprint on fully connected apps
- Skip reporting native IB addresses when network interface is requested

v1.11.0, Fri Aug 14, 2020
=========================

## Core

- Add generalized hmem_ops interface for device ops
- Add FI_HMEM_CUDA, FI_HMEM_ROCR, and FI_HMEM_ZE interfaces and device support
- Add CUDA and ROCR memory monitors and support for multiple monitors
- Add fi_tostr for FI_HMEM_* interfaces
- Add utility interface and device support
- Add documentation for hmem override ops
- Save mr_map mem_desc as ofi_mr
- Rework and reorganize memory monitor code
- Add mr_cache argument flush_lru to ofi_mr_cache_flush
- Fix 1.1 ABI domain, EP, and tx attributes
- Add loading of DL providers by name
- Add CMA wrappers and define CMA for OSX
- Fix util getinfo: use base fi_info caps, altering mr_mode properly,
  FI_MR_HMEM support, NULL hints, set CQ FI_MSG flag, query FI_COLLECTIVE,
  list FI_MATCH_COMPLETE, select and request specific core provider
- Add rbmap interface to get root node
- Add support of AF_IB to addr manipulation functions
- Windows: Map strtok_r() to strtok_s()
- Define OFI_IB_IP_{PORT,PS}_MASK
- Make fi_addr_format() public
- Remove mr_cache entry subscribed field
- Update memhooks brk and implement sbrk intercepts
- Fix vrb_speed units
- Fix possible null dereference in ofi_create_filter
- Add ofi_idx_ordered_remove
- Add functions ofi_generate_seed() and ofi_xorshift_random_r()
- Call correct close fd call in util_wait_fd_close
- Set a libfabric default universe size
- Add compatibility with SUSE packaging
- Windows: Handle socket API size limitations
- Fix UBSAN warnings
- Save and restore the errno in FI_LOG
- Ensure that access to atomic handlers are in range
- Ensure ifa_name is null terminated in ofi_get_list_of_addr
- Buffer pools fallback to normal allocations when hugepage allocations fail

## EFA

- Add support to use user posted receive buffers with RDM EP when requested
- Various fixes to FI_HMEM support
- Added fork handler and abort if rdma-core is incorrectly configured
- Fix bandwidth regression due to increased structure size
- Reuse verbs protection domain when in same process address space
- Periodically flush MR cache to reduce MR usage
- Properly handle setting/unsetting RDMAV_HUGEPAGES_SAFE
- Fix provider_version reported by EFA
- Populate additional fields in fid_nic
- Fix various bugs in the completion, info, and domain paths
- Fix various memory leaks

## PSM2

- Treat dynamic connection errors as fatal
- Add missing return status checking for PSM2 AM calls

## RxD

- updated AV design to be dynamically extensible using indexer and index map.
- updated static allocation of peers with runtime allocation during rts.
- added wrapper to fetch pointer to a peer from the peers data structure.
- Updated to show correct msg_ordering.
- Check datatype size when handling atomic ops.
- Verify atomic opcode in range for fixing Klocwork issue.
- Corrected use of addr in rxd_atomic_inject for retrieving rxd_addr.

## RxM

- Align reporting of FI_COLLECTIVE with man pages
- Show correct ordering of atomic operations
- Fix error handling inserting IP addresses into an AV
- Minor code cleanups and bug fixes
- Select different optimizations based on running over tcp vs verbs
- Use SRX by default when using tcp to improve scaling
- Correct CQ size calculation when using SRX
- Fix MR registration error path when handling iov's
- Allow selecting tcp wait objects separate from verbs
- Only repost Rx buffers if necessary

## SHM

- Fix a CMA check bug
- Fix shm provider signal handler calling the original handler
- Add initial framework for IPC device copies
- Add FI_HMEM support and integrate hmem_ops
- Fix error handling path in smr_create
- Fix AV insertion error handling
- Verify atomic op value
- Redefine shm addrlen to not use NAME_MAX
- Fix snprintf to exclude byte for null terminator
- Mark smr_region as volatile
- Fix memory leaks

## Sockets

- Fix backwards compatibility accessing struct fi_mr_attr
- Fix use after free error in CM threads
- Free unclaimed messages during endpoint cleanup to avoid memory leaks
- Improve handling of socket disconnection
- Limit time spent in progress when expected list is long
- Avoid thread starvation by converting spinlocks to mutex

## TCP

- Minor bug fixes
- Verify received opcode values are valid
- Avoid possible receive buffer overflow from malformed packets
- Fix fi_cq_sread failing with ECANCELED
- Optimize receive progress handling
- Do not alter pseudo random sequence numbers
- Increase default listen backlog size to improve scaling
- Handle processing of NACK packets during connection setup
- Fix wrong error handling during passive endpoint creation
- Add logging messages during shutdown handling
- Improve logging and error handling
- Fix possible use after free issues during CM setup
- Minor code restructuring

## Util

- Use internal flags in place of epoll flags for portability
- Support HMEM with the mr_cache
- Verify application requested FI_HMEM prior to accessing fi_mr_attr fields
- Fix memory leak when using POLLFD wait sets
- Ensure AV data is aligned even if address length is not
- Fix handling of mr mode bits for API < 1.5
- Allow user to force use of userfaultfd memory monitor

## Verbs

- Add support for AF_IB and native IB addressing
- Minor code cleanups
- Avoid possible string overrun parsing interface names
- Fix memory leak handling duplication interface names
- Add XRC shared Rx CQ credit reservation
- Fix possible segfault when closing an XRC SRQ
- Fix verbs speed units to MBps
- Add flow control support to avoid RQ overruns
- Fix memory leak of address data when creating endpoints

v1.10.1, Fri May 8, 2020
========================

## Core

- Fixed library version

## EFA

- Allow endpoint to choose shm usage
- Fix handling of REQ packets
- Fix logic writing a Tx completion entry
- Use correct Tx operation flags for msg sends

## Fabtests

- Use pax tar format when creating source packages

## RxD

- Use correct peer address for atomic_inject calls

## SHM

- Fix BSD build failure

## TCP

- Add locking around signaling a wait fd

v1.10.0, Fri Apr 24, 2020
=========================

## Core

- Added new pollfd wait object to API
- Added ability to query for a fid's wait object type
- Updated most providers to a new provider versioning format
- Support using multiple fds for blocking calls, in place of epoll
- Fix memory leak when destroying rbtrees
- Record interface names and network names for IP addressable providers
- Improved performance of timing calculations
- Improvements to MR caching mechanism

## EFA

- Replaces custom admin commands with native use of rdma-core APIs
- Added support for FI_RMA using RDMA Reads
- Added rendezvous protocol for long messages using RDMA Reads
- Added support for CUDA buffers (FI_HMEM)
- Added medium-message protocol
- Added support for atomic operations
- Added randomized Queue Key assignment to endpoints
- Improved support for client-server applications
- Disables use of shared-memory if FI_LOCAL_COMM is not required
- Updated protocol to v4
- Refactor packet handling functions and headers for better extensibility
- Added handshake protocol to negotiate protocol features with peers
- Refactor send/recv paths for improved memory descriptor handling
- Use inlined device sends for FI_INJECT
- Removes fork() to detect CMA support from the init path
- Better reuse of MRs keys across EFA and SHM control path
- Squashes the MR functions in the RxR and EFA layers
- Squashes the AV functions in the RxR and EFA layers
- Use 0-based offset if FI_MR_VIRT_ADDR not set
- Retries memory registration in MR cache error paths
- Fixes to addr_format handling in the RDM endpoint
- Fixes memory leaks
- Fixes AV error handling paths
- Fixes shm error handling paths
- Fixes compiler warnings

## PSM2

- Improve source address translation for scalable endpoints

## RxM

- Add support for pollfd wait objects
- Fix double free in error path
- Report CQ errors for failed RMA transfers
- Fixing locking in tagged receive path
- Remove incorrect rx_attr capability bits
- Handle unexpected messages when posting multi-recv buffers
- Repost multi-recv buffers to the receive queue head
- Fix unexpected message handling
- Fix stall in collective progress caused by lost receive buffers
- Add support for collection operations

## RxD

- Replace rxd_ep_wait_fd_add with direct call to ofi_wait_fd_add
- Reorganize attr caps
- Add rxd to fi_provider man page

## SHM

- Fix pointer ofi_cq_init progress pointer
- Add CQ wait object support with new FI_WAIT_YIELD wait type
- Include string terminator in addrlen
- Fix av_insert address cast
- Fix unexpected messaging processing on empty receive queue
- Fix unexpected messaging locking
- Progress unexpected queue for non-tagged receives
- Move ep_name_list initialization/cleanup and fix signal handling
- Reorganize attr caps
- Warn once on peer mapping failures
- Add FI_DELIVERY_COMPLETE support
- Fix FI_MULTI_RECV reporting and allow writing to overflow CQ for unexpected MULTI_RECV
- Refactor and simplify msg processing, formating, and recv posting
- Rename ep_entry to rx_entry and add tx_entry for pending outgoing messages
- Properly align cmd data
- Return correct addrlen on av lookup
- Fix id passed into rma fast path
- Fix typo
- Fix potential data ordering issue in atomic fetch path
- Add proper RMA read protocol without CMA
- Add runtime CMA check during mapping
- Add mmap-based fallback protocol for large messages without CMA
- Add large message segmentation fallback protocol for large messages without CMA and
  add FI_SHM_SAR_THRESHOLD to control switching between segmentation and mmap
- Define macros for address translation
- Allow building of shm provider on older kernels with x86 arch
- Rename peer_addr to peer_data
- Change locking when progressing response entries
- Fix cmd_cnt increment on RMA ops
- Add error handling when inserting more than SMR_MAX_PEERS
- Add shm size space check
- Fix locking when processing response from self
- Add locking around the ep_name_list

## TCP

- Fix incorrect signaling of fd waking up thread in fi_cq_sread
- Switch to using pollfd wait object instead of epoll as default
- Add missing ep lock to fix possible ep list corruption
- Remove incorrectly reported CQ events posted to EQ
- Update domain name to IP network name
- Improved socket processing to improve scalability and performance
- Remove incorrect implementation of FI_MULTI_RECV support
- Report error completions even if successful completion is suppressed
- Report correct EQ event for aborted connections

## Verbs

- Fix XRC request identification
- Fix small memory leak for XRC connections
- Add retry logic for XRC connections
- Fix mapping of domains to NICs when multiple NICs are present
- Allow filtering of device names via environment variable
- Fix compilation with -fno-common option
- Code restructuring to improve maintenance

v1.9.1, Fri Mar 6, 2020
=======================

## Core

- Fix gcc 9.2 warnings
- Fix thread hangs in MR cache when using userfaultfd monitor
- Add missing header for FreeBSD build
- Allow a core provider to discover and use filtered providers

## EFA

- Change MR cache count and size limits
- Fixes to 32-bit msg_id wraparound handling
- Adds address map to look up EFA address from shm address
- Remove unnecessary EFA device name check
- Detect availability of CMA directly from EFA provider
- Use OFI_GETINFO_HIDDEN flag when querying for shm
- Allow use of EFA when shm is unavailable
- Fixes info and domain capabilities for RDM endpoint
- Fixes to dest_addr returned with info objects
- Fixes segfault in efa_mr_cache_entry_dereg()
- Fixes compilation warning in DSO build of the provider
- Fixes compilation errors with -fno-common
- Fixes to send-side control path

## PSM2

- Clean up of AV entries that have been removed

## RxM

- Fix multi-recv buffer handling to use entire buffer
- Consume entire multi-recv buffer before using buffer
- Continue execution after handling transfer errors
- Properly cleanup CM progress thread
- Minor code cleanups and restructuring

## SHM

- Properly restore captured signals
- Track ptrace_scope globally, and allow disabling
- Properly initialize endpoint name list
- Fix potential deadlock resulting from missed handling of unexpected messages
- Fix multi-threading issue accessing unexpected messages
- Handle multiple addresses passed to fi_av_insert
- NULL terminate address strings
- Pass correct pointer to ofi_cq_init

## TCP

- Removed incorrect implementation for multi-recv buffer support
- Always report error completions
- Report correct EQ event for aborted connection requests
- Improve connection data corner cases

## Verbs

- Fix segfault handling error completions
- Avoid null dereference handling EQ events
- Remove possible deadlock in XRC error path
- Enable credit tracking to avoid SQ, RQ, and CQ overruns
- Verify that CQ space is available for bound EPs
- Minor code cleanups and restructuring


v1.9.0, Fri Nov 22, 2019
========================

## Core

- Add generic implementation for collective operations
- Add support for traffic class selection
- Fixes and enhancements to memory registration cache
- Add support for older kernels to the MR cache (hook malloc related calls)
- Fix setting loopback address byte ordering
- Fix MR cache locking from spinlock to a mutex to avoid starvation
- Add API enhancements for heterogeneous memory (e.g. GPUs)
- Limit default size of MR cache to avoid out of memory errors
- Fix g++ compile error
- Enhanced the hooking provider infrastructure
- Enhanced windows support for IPv6 and NIC selection
- Fix timeout calculation in wait operations
- Add simple spell checker for FI_PROVIDER
- Fix red-black tree possible use after free issue
- Fix segfault running libfabric within a linux container
- Minor cleanups and bug fixes
- Work-around possible long delay in getaddrinfo()

## EFA

- Introduce support for shared-memory communication using shm provider
- Enable Memory Registration caching by default
- Refactor TX and CQ handling functions to reduce branching
- Use application-provided MR descriptors when available
- Optimize progress engine polling loop for shm and EFA completions
- Enable inline registration for emulated RMA reads
- Inherit FI_UNIVERSE_SIZE for AV sizing
- Increase default min AV size to 16K
- Fix uninitialized objects with DSO build of the provider
- Fix handling of FI_AV_UNSPEC
- Fix crash and resource leak with fi_cancel() implementation
- Fix issues with EFA's registration cache under efa;ofi_rxd
- Fix MR allocation handlers to use correct pointer and size
- Fix error handling in multi-recv completion code
- Fix compilation errors when built with valgrind annotations
- Fix compilation errors when packet poisoning was enabled
- Fix incorrect parameter definitions
- Fix leaks of internal resources
- Miscellaneous cleanups and bug fixes

## MRail

- Renamed address control environment variable
- Implement large message striping using rendezvous
- Properly set tx/rx op flags

## PSM2

- Fix memory leaks
- Add fi_nic support
- Report correct value for max_order_raw_size
- Report max_msg_size as a page aligned value
- Fix potential multi-threaded race condition
- Avoid potential deadlock in disconnect protocol

## RxD

- Fix default AV count
- Minor cleanups and optimizations
- Handle errors unpacking packets
- Report all failures when inserting addresses into AV
- Remove unneeded posted buffer tracking

## RxM

- Fix inject completion semantics
- Fix MR key handling when mismatched with core provider
- Add basic support for some collective operations
- Fix senddata desc parameter mismatch
- Serialize EQ processing to avoid use after free issue
- Minor cleanup and optimizations
- Remove atomic buffer limitations
- Provide mechanism to force auto-progress for poorly designed apps
- Fix high memory usage when using RMA
- Fix segfault handling memory deregistration
- Discard canceled receive buffers when closing msg ep
- Fix memory leaks in connection management

## SHM

- Cleanup tmpfs after unclean shutdown
- Increase the size of endpoint names
- Align endpoint count attribute with maximum supported peer count
- Add user ID to shared memory name
- Only support small transfers if ptrace is restricted
- Fix incorrect reporting of completion buffer
- Return correct addrlen on fi_getname
- Round tx/rx sizes up in case sizes are not already a power of two
- Skip utility providers for shm provider

## TCP

- Report aborted requests as canceled
- Fixed support for 0-length transfers
- Return positive error code for CQ entries
- Bind ports using SO_REUSEADDR
- Properly check for correct recv completion length
- Fix potential deadlock due to lock ordering issue

## Verbs

- Enable on-demand paging memory registration option
- Enable send queue overflow optimization for mlx devices
- Cleanup EQ when closing an associated endpoint
- Minor optimizations and code restructuring
- Avoid potential deadlock accessing EQ and EP
- Speedup XRC connection setup
- Handle IPv6 link local address scope id
- Updates to support new versions of rdma-core libraries
- XRC connection optimizations, cleanups, and error handling improvements
- Fix possible segfault in error handling path
- Remove support for vendor specific and experimental verbs
- Handle 0-length memory registrations
- Fix EQ trywait behavior to check for software events


v1.8.1, Mon Sep 30, 2019
========================

## Core

- Limit default size of memory registration cache
- Verify that correct entry is removed from MR cache

## EFA

- Fixes to fi_cancel() when used with multi-recv buffers
- Fixes to registered memory handling after a fork()
- Fixes to the long message flow-control protocol
- Use FI_AV_TABLE as the preferred AV type
- Fixes to the bufpool allocation handlers
- Fixes to RTS handler
- Fix to use correct arch detection preprocessor macro
- Expose fid_nic information
- Fix memory leaks

## PSM2

- Fix incorrect value of max_order_raw_size
- Report page aligned max_msg_size
- Always enable the lock accessed by the disconnection thread
- Fix race condition with progress thread and FI_THREAD_DOMAIN
- Avoid a potential deadlock in disconnection protocol

## RxD
- Fix default AV count with environment variable FI_OFI_RXD_MAX_PEERS

## RxM

- Fix connection handle shutdown/CQ processing race
- Fix RMA ordering bits for FI_ATOMIC

## SHM
- Add correct reporting of FI_MR_BASIC
- Add correct reporting and proper support of FI_DIRECTED_RECV

## Verbs

- Allow zero length memory registrations
- Improve connection scale up by removing synchronous calls in fi_getinfo
- Fix missing serialization to event channel during CM ID migration
- Protect XRC EQ processing from EP API connect/accept calls
- Fix XRC connection tag to EP return value in error case
- return EAGAIN to user if an unhandled rdmacm event is received
- handle IPv6 link local addresses correctly


v1.8.0, Fri Jun 28, 2019
========================

## Core

- Reworked memory registration cache to use userfaultfd
- Allow disabling atomic support as build option
- Updated default provider priority
- Define new data ordering bits to separate atomic from RMA ordering
- Improved support for huge page allocations
- Convert all python scripts to version 3
- Add logging to report atomic implementation in use
- Fix timeout calculation in util wait abstraction.
- Fix hang when multiple threads wait on util counter.

## EFA

- New core provider for Amazon EC2 Elastic Fabric Adapter (EFA)

## GNI

- Fix handling of incorrect fi_addr_t value
- Fix several problems when using FI_ADDR_STR format
- Fix several problems when using multi-receive buffers
- Fix problem with possible receive truncation
- Fix possible overrunning of receive buffers
- Implement fi_getopt/fi_setopt for scalable endpoints
- Only generate FI_EADDRNOTAVAIL if FI_SOURCE_ERR enabled

## MRAIL

- Several performance enhancements and optimizations

## PSM2

- Disable some optional psm2 features due to instabilities
- Work around possibly long delays in getaddrinfo()

## RxD

- Ensure that peers are initialized before sending data packets
- Maintain a full posting of receive buffers to the DGRAM EP
- Minor bug fixes and cleanups
- Verify protocol versions and reject incompatible versions
- Update and simplify protocol headers
- Optimize packet initialization
- Various improvements handling protocol messages
- Remove pending_cnt tracking (moved to verbs provider)
- Fix setting of mr_mode on getinfo
- Handle error unpacking packet headers properly

## RxM

- Improved responsiveness to CM events with manual progress mode
- Add proper versioning to CM data exchange protocol
- Refactor how CQ and EQ errors are handled and reported
- Minor code cleanups and bug fixes
- Support atomic operations in auto progress mode
- Fix high memory usage when a large number of RMA ops are posted without
  FI_COMPLETION

## SHM

- Fix possible segfault
- Fix smr_freestack_pop to properly remove entry from stack

## TCP

- Enable multi-recv support
- Allow restricting tcp to specific port range for firewall purposes

## Verbs

- Add NIC info (fi_nic) in fi_info
- Improved support for UD QPs over RoCE
- Added CQ resource tracking to avoid CQ overrun on HFI1
- Enable memory registration cache by default
- Reduce unnecessary log message noise
- Bug fixes for EQ trywait, QP creation attributes.

## Fabtests

- Add new test to test memory registration caching correctness
- Fix segfault with fi_pingpong
- Allow specifying flags as command line arguments
- Add threading level option to ubertest
- Enable support for all endpoint types with multi_recv test
- Add new AV insertion unit test
- Allow for out-of-band address exchange with in-band test synchronization
- Check for and use exclusion files by default if found
- Use regular expressions for test exclusion files
- Have test scripts use full test names
- Replace socket provider with tcp and udp for default testing

v1.7.2, Fri Jun 14, 2019
========================

## Core

- Rename variables that shadow global symbols
- Set slist tail to NULL to handle iterators correctly
- Add new locking to AV EP list to avoid potential deadlock
- Add threadsafe AV implementation

## GNI

- Fix possible overrunning of receive buffers
- Fix compile issue on CLE 7.0.UP01
- Implement fi_getopt/fi_setopt for scalable endpoints
- Only generate FI_EADDRNOTAVAIL if FI_SOURCE_ERR enabled

## RxD

- Align packet type declarations with debug prints
- Track current unexpected messages per peer, rather than globally
- Remove unneeded RXD_CANCELED flag
- Remove unnecessary check of unexpected list
- Support FI_CLAIM, FI_PEEK, and FI_DISCARD flags
- Avoid double free on CQ error destruction path
- Fix message windowing
- Limit number of transfer entries that can be active
- Use utility CQ calls to handle CQ overflow
- Set correct opcode when completing read completions
- Preset and fix tx and rx transfer flags
- Fix segfault

## RxM

- Add missing serialization for RMA and atomics
- Reject connection requests in shutdown state
- Rework cmap and ep lock synchronization
- Add CM events to improve debugging
- Remove incorrect assertion
- Progress EQ events from app thread to drive progress
- Handle message segment ordering when buffering receives
- Generate completions for claimed buffered messages
- Minor other fixes and cleanups

## TCP

- Support FI_SELECTIVE_COMPLETION correctly
- Fix transmit and delivery complete semantics
- Verify fi_info when creating passive EP

## Verbs

- Remove XRC target QP from RDMA CM control
- Fix XRC QP allocation failure return code
- Fix EQ readerr locking
- Add FI_ADDR_IB_UD to known address print format
- Fix addressing return on fi_getifo for native IB addresses
- Serialize access to EQ when destroying connections
- Add serialization to XRC EQ/CM handling
- Fix serialization between AV and cmap
- Fix XRC connection tags
- Remove racy fork support from provider
- Minor other fixes

## Fabtests

- Exclude tests from OS X that require epoll support

v1.7.1, Mon Apr 8, 2019
========================

## Core

- Support layered provider names with FI_PROVIDER filter
- Minor cleanups to man pages
- Add missing header for FreeBSD support
- Fix built-in atomic tests
- Do not overwrite CFLAGS during build

## Fabtests

- Fix memory leaks in fi_getinfo_test
- Fix memory leak in fi_av_xfer error paths
- Fix memory leaks in fi_cm_data
- Fix memory leak in fi_poll
- Add test configuration and exclude files for shm

## GNI

- Fix setting supported tag mask
- Fall back to normal allocations when huge page support is unavailable

## PSM2

- Inline address translation function for performance
- Create tagged ops specialization for FI_AV_MAP
- Bring back true FI_AV_MAP support under certain conditions
- Use psm2_epaddr_to_epid() for epaddr to epid conversion
- Add runtime parameter for connection timeout

## MXM

- Disable provider by default as it is not actively maintained

## RxD

- Fix completion generation
- Use correct flags for *msg APIs
- Return correct error code if Tx is unable to be accepted
- Add missing EP attributes
- Fix use after free error opening an endpoint

## RxM

- Avoid unnecessary fi_getinfo calls to avoid possible failures
- Discard Rx buffers for closed EPs to avoid segfaults
- Repost receive buffers to avoid possible fabric deadlock
- Fix crash accessing invalid Tx CQ
- Use correct flags for *msg APIs
- Report available inject size, rather than reducing value based on user hints
- Fix endpoint configuration checks when enabling endpoint
- Keep eager protocol limit separate from inject size
- Increment correct atomic counter
- Discard CQ entries generated by canceled receives
- Initialize maximum atomic payload size
- Add connection event progress
- Fix SAR protocol truncation error
- Fix setting FI_MSG and fi_RMA caps

## TCP

- Fix synchronization verifying MRs
- Remove duplicate Rx queue removal
- Return internal buffers to correct buffer pools to avoid data corruption
- Free internal buffer pools and fix memory leaks
- Handle peer socket disconnects properly
- Pass signals through to application threads
- Cleanup pending events when closing endpoints
- Only adjust endianness when peers' endianness mismatches
- Optimize header sizes based on message types to improve performance

## Sockets

- Fix acquiring the same lock twice
- Fix accessing an uninitialized pointer handling CM events

## SHM

- Add support for selective completions
- Fix addressing
- Set MR key size
- Fix memory corruption and memory leak

## Verbs

- Cleanup from use of memory registration cache
- Do not update minimum RNR timer for XRC initiator QPs
- Fix possible CQ overrun issues with hfi1 and qib devices
- Fix synchronization issue accessing MR cache from multiple threads
- Fix double free in XRC accept path
- Add missing include file
- Make fi_getinfo call thread safe
- Fix CQ busy issue in MPI finalize when using XRC
- Return correct attribute values when XRC is enabled
- Fix sending atomic response protocol message
- Report flushed receive operations (reverts to behavior in 1.4 and earlier)
- Fix segfault reading CQ error entries
- Fix double free of XRC connection request data
- Set FI_RX_CQ_DATA mode bit correctly

v1.7.0, Mon Jan 7, 2019
=======================

The 1.7 release provides a few enhancements to the libfabric API.
Notably, it extends the fi_info structure in order to report NIC
attributes for domains that have a direct association with network
hardware.  The NIC attributes include details about the device, the
system bus it's attached to, and link state.  NIC attributes are
automatically reported by the fi_info utility application.  See the
fi_nic.3 man page for additional details.

An experimental capability bit is added to optimize receive side
processing.  This is known as variable messages, and targets applications
that do now know what size message a peer with send prior to the
message arriving.  Variable messages can be used to avoid receive
side data copies and eliminate the need for applications to implement
their own rendezvous protocol.  See the fi_msg.3 man page for details on
variable messages and it's sister, buffered messages.

Specific details on changes since the 1.6.2 release are outlined below.

## Core

- Add ability to report NIC details with fi_info data
- Improve MR cache notification mechanisms
- Set sockaddr address format correctly
- Avoid possible null dereference in eq_read
- Handle FI_PEEK in CQ/EQ readerr
- Add debug messages to name server
- Feature and performance enhancements added to internal buffer pool
- Add support for huge pages
- Decrease memory use for idle buffer pools
- Refactor utility AV functionality
- Generic counter support enhancements
- Optimize EP and CQ locking based on application threading level
- Enhance common support for EQ error handling
- Add free/alloc memory notification hooks for MR cache support
- Fix memory monitor unsubscribe handling
- Add CQ fd wait support
- Add CQ overflow protection
- Enhance IPv6 addressing support for AVs
- Enhancements to support for AV address lookup
- Fixes for emulated epoll support
- Allow layering of multiple utility providers
- Minor bug fixes and optimization

## Hook

- Improved hooking infrastructure
- Add support for installing multiple hooks
- Support hooks provided by external libraries.

## GNI

- Fix CQ readfrom overwriting src_addr in case of multiple events
- Signal wait set if error entry is added to CQ
- Fix state data issue with SMSG buffers
- Enhance and fix possible misuse of default authorization key
- Add cancel support for SEP
- Rework SEP setup
- Suppress huge page counting for ARM
- Fix incorrect check of FI_SYNC_ERR flag

## MRAIL

- Initial release of mrail provider. The current status is experimental: not all
  features are supported and performance is not guaranteed.
- Enables increased bandwidth for an underlying provider by utilizing multiple
  network ports (rails).

## NetDir

- Fix crash in initialization code
- Update references to NetworkDirect header packaged

## PSM2

- Requires PSM2 library version 10.2.260 or later
- Clean up connection state in fi_av_remove
- Use psm2_info_query to read HFI device info
- Clean up CQ/counter poll list when endpoint is closed
- Support shared address vector
- Optimize CQ event conversion with psm2_mq_ipeek_dequeue_multi
- Lock optimization for FI_THREAD_DOMAIN
- Use new PSM2 fast path isend/irecv functions for large size RMA
- Support building with latest PSM2 source code (version 11.2.68)
- Support fabric direct

## RxD

- Initial release of RxD provider
- Provides reliable datagram semantics over unreliable datagram EPs
- Target is to improve scalability for very large clusters relative to RxM

## RxM

- Decrease memory use needed to maintain large number of connections
- Set correct op_context and flags on CQ error completions
- Fix file descriptor memory leaks
- Introduce new protocol optimized for medium message transfers
- Improve Rx software performance path
- Use shared receive contexts if required by underlying provider
- Handle addresses inserted multiple times into AV (for AV map)
- Performance optimizations for single-thread applications
- Rework deferred transmit processing
- Separate and optimize eager and rendezvous protocol processing.
- Fix passing incorrect addresses for AV insert/remove
- Fix CM address handling
- Fix race condition accessing connection handles
- Simplify small RMA code path
- Increment correct counter when processing FI_READ events
- Dynamically grow the number of connections that can be supported
- Fix padding in wire protocol structures
- Report correct fi_addr when FI_SOURCE is requested
- Fix truncating rendezvous messages
- Fix use after free error in Rx buffer processing
- Add support for manual progress
- Make Tx/Rx queue sizes independent of MSG EP sizes
- Decrease time needed to repost buffers to the MSG EP Rx queue.
- Miscellaneous bug fixes

## Sockets

- Enable MSG EPs when user calls fi_accept
- Fix fabric names to be underlying IP address
- Add connection timeout environment variable.
- Use size of addresses, not structures
- Add debug messages to display selected addresses
- Use loopback address in place of localhost
- Simplify listen paths
- Add support for IPv6
- Code restructuring
- Avoid unneeded address to string to address translations
- Check length of iovec entries prior to access buffers
- Fix segfault
- Avoid acquiring nested spinlocks resulting in hangs
- Fix use after free error in triggered op handling
- New connection manager for MSG EPs to reduce number of threads
- Avoid retrying recv operations if connection has been broken
- Fixes for Windows socket support

## TCP

- Initial release of optimized socket based tcp provider
- Supports MSG EPs, to be used in conjunction with RxM provider
- Targets eventual replacement of sockets provider

## Verbs

- Remove RDM EP support.  Use RxM and RxD for RDM EPs.
- Improve address handling and report in fi_getinfo
- Handle FI_PEER when calling CQ/EQ readerr functions
- Add support for XRC QPs.
- Ignore destination address when allocating a PEP
- Add workaround for i40iw incorrect return values when posting sends
- Fix completion handling for FI_SELECTIVE_COMPLETION EP setting
- Change format of fabric name to use hex instead of decimal values
- Fix handling of err_data with EQ readerr
- Report correct size of max_err_data
- Fast path performance improvements
- Improve progress under high system load
- Optimize completion processing when handling hidden completions
- Optimize RMA and MSG transfers by pre-formatting work requests
- Remove locks based on application threading model
- Add overflow support for CQ error events
- Minor cleanups and bug fixes

v1.6.2, Fri Sep 28, 2018
========================

## Core

- Cleanup of debug messages

## GNI

- Fix problems with Scalable Endpoint creation
- Fix interoperability problem with HPC toolkit
- Improve configuration check for kdreg

## PSM

- Enforce FI_RMA_EVENT checking when updating counters
- Fix race condition in fi_cq_readerr()
- Always try to make progress when fi_cntr_read is called

## PSM2

- Revert "Avoid long delay in psm2_ep_close"
- Fix memory corruption related to sendv
- Performance tweak for bi-directional send/recv on KNL
- Fix CPU detection
- Enforce FI_RMA_EVENT checking when updating counters
- Remove stale info from address vector when disconnecting
- Fix race condition in fi_cq_readerr()
- Adjust reported context numbers for special cases
- Always try to make progress when fi_cntr_read is called
- Support control functions related to MR mode
- Unblock fi_cntr_wait on errors
- Properly update error counters
- Fix irregular performance drop for aggregated RMA operations
- Reset Tx/Rx context counter when fabric is initialized
- Fix incorrect completion event for iov send

## RXM

- Fix incorrect increments of error counters for small messages
- Increment write completion counter for small transfers
- Use FI_UNIVERSE_SIZE when defining MSG provider CQ size
- Make TX, RX queue sizes independent of MSG provider
- Make deferred requests opt-in
- Fill missing rxm_conn in rx_buf when shared context is not used
- Fix an issue where MSG endpoint recv queue got empty resulting
  in a hang
- Set FI_ORDER_NONE for tx and rx completion ordering
- Serialize access to repost_ready_list
- Reprocess unexpected messages on av update
- Fix a bug in matching directed receives
- Fix desc field when postponing RMA ops
- Fix incorrect reporting of mem_tag format
- Don't include FI_DIRECTED_RECV, FI_SOURCE caps if they're not needed
- Fix matching for RMA I/O vectors

## Sockets

- Increase maximum messages size as MPICH bug work-around

## Verbs

- Detect string format of wildcard address in node argument
- Don't report unusable fi_info (no source IP address)
- Don't assert when a verbs device exposes unsupported MTU types
- Report correct rma_iov_limit
- Add new variable - FI_VERBS_MR_CACHE_MERGE_REGIONS
- eq->err.err must return a positive error code

v1.6.1, Wed May 8, 2018
===========================

## Core

- Fix compile issues with older compilers
- Check that all debug compiler flags are supported by compiler

## PSM2

- Fix occasional assertion failure in psm2_ep_close
- Avoid long delay in psm2_ep_close
- Fix potential duplication of iov send completion
- Replace some parameter checking with assertions
- Check iov limit in sendmsg
- Avoid adding FI_TRIGGER caps automatically
- Avoid unnecessary calls to psmx2_am_progress()

## RXM

- Fix reading pointer after freeing it.
- Avoid reading invalid AV entry
- Handle deleting the same address multiple times
- Fix crash in fi_av_remove if FI_SOURCE wasn't enabled

## Sockets

- Fix use after free error handling triggered ops.


v1.6.0, Wed Mar 14, 2018
========================

## Core

- Introduces support for performing RMA operations to persistent memory
  See FI_RMA_PMEM capability in fi_getinfo.3
- Define additional errno values
- General code cleanups and restructuring
- Force provider ordering when using dynamically loaded providers
- Add const to fi_getinfo() hints parameter
- Improve use of epoll for better scalability
- Fixes to generic name service

## GNI

- Fix a problem with the GNI waitset implementation
- Enable use of XPMEM for intra node data transfers
- Fix a problem with usage of Crays UDREG registration cache
- Fix a problem with an assert statement
- Fix several memory leaks

## PSM

- Move environment variable reading out from fi_getinfo()
- Shortcut obviously unsuccessful fi_getinfo() calls
- Remove excessive name sever implementation
- Enable ordering of RMA operations

## PSM2

- Requires psm2 library version 10.2.235 or later
- Skip inactive units in round-robin context allocation
- Allow contexts be shared by Tx-only and Rx-only endpoints
- Use utility functions to check provider attributes
- Turn on FI_THREAD_SAFE support
- Make address vector operations thread-safe
- Move environment variable reading out from fi_getinfo()
- Reduce noise when optimizing tagged message functions
- Shortcut obviously unsuccessful fi_getinfo() calls
- Improve how Tx/Rx context limits are handled
- Support auto selection from two different tag layout schemes
- Add provider build options to debug output
- Support remote CQ data for tagged messages, add specialization.
- Support opening multiple domains
- Put trigger implementation into a separate file
- Update makefile and configure script
- Replace allocated context with reserved space in psm2_mq_req
- Limit exported symbols for DSO provider
- Reduce HW context usage for certain TX only endpoints
- Remove unnecessary dependencies from the configure script
- Refactor the handling of op context type
- Optimize the conversion between 96-bit and 64-bit tags
- Code refactoring for completion generation
- Remove obsolete feature checking code
- Report correct source address for scalable endpoints
- Allow binding any number of endpoints to a CQ/counter
- Add shared Tx context support
- Add alternative implementation for completion polling
- Change the default value of FI_PSM2_DELAY to 0
- Add an environment variable for automatic connection cleanup
- Abstract the completion polling mechanism
- Use the new psm2_am_register_handlers_2 function when available
- Allow specialization when FI_COMPLETION op_flag is set.
- Put Tx/Rx context related functions into a separate file
- Enable PSM2 multi-ep feature by default
- Add option to build with PSM2 source included
- Simplify the code for checking endpoint capabilities
- Simplify the handling of self-targeted RMA operations
- Allow all free contexts be used for scalable endpoints
- Enable ordering of RMA operations
- Enable multiple endpoints over PSM2 multi-ep support
- Support multiple Tx/Rx contexts in address vector
- Remove the virtual lane mechanism
- Less code duplication in tagged, add more specialization.
- Allow PSM2 epid be reused within the same session
- Turn on user adjustable inject size for all operations
- Use pre-allocated memory pool for RMA requests
- Add support for lazy connection
- Various bug fixes

## RXM

- Add support for completion counters
- Fix MR mode handling
- Add support for FI_MULTI_RECV
- Considerable performance optimizations
- Report correct MR key size based on core provider's size
- Fixes to endpoint address reporting to avoid wildcard addresses
- Ensure progress after core provider returns EAGAIN on transfers
- Fix crash when running of sockets provider
- Bug fixes handling large message transfers
- Report data ordering and limits based on core provider's
- Set mode bits and capabilities correctly
- General code restructuring and cleanups
- Various additional bug fixes
- Handle different API versions correctly
- Expand support for tagged message transfers
- Add support for auto progress on data transfers

## SHM

- Initial release of shared memory provider
- See the fi_shm.7 man page for details on available features and limitations

## Sockets

- Scalability enhancements
- Fix issue associating a connection with an AV entry that could result in
   application hangs
- Add support for new persistent memory capabilities
- Fix fi_cq_signal to unblock threads waiting on cq sread calls
- Fix epoll_wait loop handling to avoid out of memory errors
- Add support for TCP keepalives, controllable via environment variables
- Reduce the number of threads allocated for handling connections
- Several code cleanups in response to static code analysis reports
- Fix reporting multiple completion events for the same request in error cases

## usNIC

- Minor adjustments to match new core MR mode bits functionality
- Several code cleanups in response to static code analysis reports

## Verbs

- Code cleanups and simplifications
- General code optimizations to improve performance
- Fix handling of wildcard addresses
- Check for fatal errors during connection establishment
- Support larger inject sizes
- Fix double locking issue
- Add support for memory registration caching (disabled by default)
- Enable setting thread affinity for CM threads
- Fix hangs in MPI closing RDM endpoints
- Add support for different CQ formats
- Fix RMA read operations over iWarp devices
- Optimize CM progress handling
- Several bug fixes

v1.5.3, Wed Dec 20, 2017
========================

## Core

- Handle malloc failures
- Ensure global lock is initialized on Windows
- Fix spelling and formatting errors in man pages

## GNI

- Fix segfault when using FI_MULTI_RECV
- Fix rcache issue handling overlapping memory regions

## NetDir

- Fix fi_getname
- Remove FI_LOCAL_MR mode bit, being reported erronously
- Avoid crashing in fi_join

## PSM

- Fix print format mismatches
- Remove 15 second startup delay when no hardware is installed
- Preserve FI_MR_SCALABLE mode bit for backwards compatability

## PSM2

- Fix print format mismatches
- Allow all to all communication between scalable endpoints
- Preserve FI_MR_SCALABLE mode bit for backwards compatability
- Fix reference counting issue with opened domains
- Fix segfault for RMA/atomic operations to local scalable endpoints
- Fix resource counting related issues for Tx/Rx contexts
- Allow completion suppression when fi_context is non-NULL
- Use correct queue for triggered operations with scalable endpoints

## RXM

- Fix out of bounds access to receive IOVs
- Serialize access to connection map
- Fix CQ error handling
- Fix issue being unable to associate an fi_addr with a connection
- Fix bug matching unexpected tagged messages
- Indicate that FI_RMA is supported
- Return correct r/w ordering size limits

## Sockets

- Fix check for invalid connection handle
- Fix crash in fi_av_remove

## Util

- Fix number of bits used for connection index

## Verbs

- Fix incorrect CQ entry data for MSG endpoints
- Properly check for errors from getifaddrs
- Retry getifaddr on failure because of busy netlink sockets
- Ack CM events on error paths

v1.5.2, Wed Nov 8, 2017
=======================

## Core

- Fix Power PC 32-bit build

## RXM

- Remove dependency on shared receive contexts
- Switch to automatic data progress
- Fix removing addresses from AV

## Sockets

- Fix incorrect reporting of counter attributes

## Verbs

- Fix reporting attributes based on device limits
- Fix incorrect CQ size reported for iWarp NICs
- Update man page with known issues for specific NICs
- Fix FI_RX_CQ_DATA mode check
- Disable on-demand paging by default (can cause data corruption)
- Disable loopback (localhost) addressing (causing failures in MPI)

v1.5.1, Wed Oct 4, 2017
=======================

## Core

- Fix initialization used by DL providers to avoid crash
- Add checks for null hints and improperly terminated strings
- Check for invalid core names passed to fabric open
- Provide consistent provider ordering when using DL providers
- Fix OFI_LIKELY definitions when GNUC is not present

## GNI

- Add ability to detect local PE rank
- Fix compiler/config problems
- Fix CQ read error corruption
- Remove tests of deprecated interfaces

## PSM

- Fix CQ corruption reporting errors
- Always generate a completion on error

## PSM2

- Fix CQ corruption reporting errors
- Always generate a completion on error
- Add checks to handle out of memory errors
- Add NULL check for iov in atomic readv/writev calls
- Fix FI_PEEK src address matching
- Fix bug in scalable endpoint address resolution
- Fix segfault bug in RMA completion generation

## Sockets

- Fix missing FI_CLAIM src address data on completion
- Fix CQ corruption reporting errors
- Fix serialization issue wrt out of order CPU writes to Tx ring buffer

## Verbs

- Allow modifying rnr retry timout to improve performance
- Add checks to handle out of memory errors
- Fix crash using atomic operations for MSG EPs

v1.5.0, Wed Aug 9, 2017
============================

The 1.5 release includes updates to the libfabric API and ABI.  As a
result, the ABI bumps from 1.0 to 1.1.  All changes are backwards
compatible with previous versions of the interface.  The following
features were added to the libfabric API.  (Note that individual
providers may not support all new features).  For full details
see the man pages.

- Authorization keys
  Authorization keys, commonly referred to as job keys, are used to
  isolate processes from communicating with other processes for security
  purposes.
- Multicast support
  Datagram endpoints can now support multicast communication.
- (Experimental) socket-like endpoint types
  New FI_SOCK_STREAM and FI_SOCK_DGRAM endpoint types are introduced.
  These endpoint types target support of cloud and enterprise based
  middleware and applications.
- Tagged atomic support
  Atomic operations can now target tagged receive buffers, in
  addition to RMA buffers.
- (Experimental) deferred work queues
  Deferred work queues are enhanced triggerred operations.  They
  target support for collective-based operations.
- New mode bits: FI_RESTRICTED_COMP and FI_NOTIFY_FLAGS_ONLY
  These mode bits support optimized completion processing to
  minimize software overhead.
- Multi-threaded error reporting
  Reading CQ and EQ errors now allow the application to provide the
  error buffer, eliminating the need for the application to synchronize
  between multiple threads when handling errors.
- FI_SOURCE_ERR capability
  This feature allows the provider to validate and report the source
  address for any received messages.
- FI_ADDR_STR string based addressing
  Applications can now request and use addresses provided using a
  standardized string format.  This makes it easier to pass full
  addressing data through a command line, or handle address exchange
  through text files.
- Communication scope capabilities: FI_LOCAL_COMM and FI_REMOTE_COMM
  Used to indicate if an application requires communication with
  peers on the same node and/or remote nodes.
- New memory registration modes
  The FI_BASIC_MR and FI_SCALABLE_MR memory registration modes have
  been replaced by more refined registration mode bits.  This allows
  applications to make better use of provider hardware capabilities
  when dealing with registered memory regions.
- New mode bit: FI_CONTEXT2
  Some providers need more than the size provided by the FI_CONTEXT
  mode bit setting.  To accomodate such providers, an FI_CONTEXT2
  mode bit was added.  This mode bit doubles the amount of context
  space that an application allocates on behalf of the provider.

## BGQ provider notes

- The OFI 1.5 BGQ provider officially supports the Open Fabrics Interfaces
  utilized by the MPICH implementation of the MPI-3 standard.  In addition
  to the MPICH test suite it has been tested by several scientific applications
  running MPICH on BGQ at scale, and several bugs in the provider and MPICH
  have been identified and fixed.  At least one INCITE project is attempting
  to use it for production science.  Support of this provider is a high-
  priority for ALCF, and MPICH users on BGQ are encouraged to utilize it to
  compare function and performance against the PAMI-based default toolchain.
  Any discovered bugs will be quickly addressed with high priority.  Results
  so far have shown significant point-to-point and RMA latency improvements
  over PAMI as well as RMA functional improvements at scale.  The only
  potential drawback is collective performance degradation against the PAMI
  optimizations, but at certain message and partition sizes, performance has
  been observed to be comparable or even better.

## MLX provider notes

- New provider to replace the deprecated mxm provider.
- Targets Mellanox InfiniBand fabrics, through the UCX library.
- Supports RDM endpoints with the tagged interfaces.
- Requires FI_CONTEXT mode support.
- See fi_mlx.7 man page for more details.

## NetDir provider nodes

- New provider for Windows that runs over the NetworkDirect API.
- Supports FI_EP_MSG endpoints, with FI_MSG and FI_RMA interfaces.
- Supports shared receive contexts
- Supports FI_SOCKADDR, FI_SOCKADDR_IN, and FI_SOCKADDR_IN6 addressing
- Asynchronous operations make forward progress automatically

## PSM provider notes

- Improve the name server functionality and move to the utility code
- Handle updated mr_mode definitions
- Add support of 32 and 64 bit atomic values

## PSM2 provider notes

- Add option to adjust the locking level
- Improve the name server functionality and move to the utility code
- Add support for string address format
- Add an environment vaiable for message inject size
- Handle FI_DISCARD in tagged receive functions
- Handle updated mr_mode definitions
- Add support for scalable endpoint
- Add support of 32 and 64 bit atomic values
- Add FI_SOURCE_ERR to the supported caps
- Improve the method of checking device existence

## Sockets provider notes

- Updated and enhanced atomic operation support.
- Add support for experimental deferred work queue operations.
- Fixed counter signaling when used with wait sets.
- Improved support on Windows.
- Cleaned up event reporting for destroyed endpoints.
- Fixed several possible crash scenarios.
- Fixed handling socket disconnect events which could hang the provider.

## RxM provider notes

- Add OFI RxM provider. It is an utility provider that supports RDM
  endpoints emulated over a base provider that supports only MSG end-
  points.
- The provider was earlier experimental. It's functional from this
  release onwards.
- Please refer to the man page of the provider for more info.

## UDP provider notes

- Add support for multicast data transfers

## usNIC provider notes

- Only requires libibverbs when necessary
- Updated to handle 1.5 interface changes.

## Verbs provider notes

- Fix an issue where if the user requests higher values for tx, rx
  context sizes than default it wasn't honored.
- Introduce env variables for setting default tx, rx context sizes
  and iov limits.
- Report correct completion ordering supported by MSG endpoints.


v1.4.2, Fri May 12, 2017
========================

## Core

- Fix for OS X clock_gettime() portability issue.

## PSM provider notes

- Updated default counter wait object for improved performance
- Fix multi-threaded RMA progress stalls

## PSM2 provider notes

- Updated default counter wait object for improved performance
- Fix multi-threaded RMA progress stalls

## Sockets provider notes

- Fix error in fi_cq_sreadfrom aborting before timeout expires
- Set atomic iov count correct correctly inside fi_atomicv

## Verbs provider notes

- Fix handling of apps that call fork.  Move ibv_fork_init() before
  calling any other verbs call.
- Fix crash in fi_write when connection is not yet established and
  write data size is below inline threshold.
- Fix issues not handling multiple ipoib interfaces
- Reduce lock contention on buffer pools in send/completion handling
  code.
- To see verbs provider in fi_info output, configure the corresponding
  IPoIB interface with an IP address. This is a change in behavior from
  previous versions. Please refer fi_verbs man page for more info.

v1.4.1, Fri Feb  3, 2017
========================

## PSM provider notes

- Defer initialization of the PSM library to allow runtime selection from
  different versions of the same provider before fi_getinfo is called.

## PSM2 provider notes

- Defer initialization of the PSM2 library to allow runtime selection from
  different versions of the same provider before fi_getinfo is called.
- General bug fixes.

## UDP provider notes

- Fix setting address format in fi_getinfo call.

## usNIC provider notes

- Fixed compilation issues with newer versions of libibverbs.

v1.4.0, Fri Oct 28, 2016
========================

- Add new options, `-f` and `-d`, to fi_info that can be used to
  specify hints about the fabric and domain name. Change port to `-P`
  and provider to `-p` to be more in line with fi_pingpong.

## GNI provider notes

- General bug fixes, plugged memory leaks, performance improvements,
  improved error handling and warning messages, etc.
- Additional API support:
  - FI_THREAD_COMPLETION
  - FI_RMA_EVENT
  - iov length up to 8 for messaging data transfers
- Provider-specific API support:
  - Aries native AXOR atomic operation
  - Memory registation cache flush operation
- Memory registration cache improvements:
  - IOMMU notifier support
  - Alternatives to the internal cache
  - Additional tuning knobs
- On-node optimization for rendezvous message communication via XPMEM
- Internal fixes to support accelerators and KNL processors
- Better support for running in CCM mode (in support for fabtests)

## MXM provider

- The mxm provider has been deprecated and will be replaced in a
  future release.

## PSM provider notes

- General bug fixes
- Use utility provider for EQ, wait object, and poll set
- Allow multi-recv to post buffer larger than message size limit

## PSM2 provider notes

- General bug fixes
- Add support for multi-iov RMA read and aromic operations
- Allow multi-recv to post buffer larger than message size limit

## Sockets provider notes

- General code cleanup and bug fixes
- Set tx/rx op_flags correctly to be consistent with manpage
- Restructure struct sock_ep to support alias ep
- Refactor CQ/Cntr bindings, CQ completion generation, and counter
  increments
- Copy compare data to internal buffer when FI_INJECT is set in
  fi_compare_atomic
- Correctly handle triggered operation when FI_INJECT is set or
  triggered op is enqueued or counter is incremented. Initialize
  counter threshold to INT_MAX
- Refactor and cleanup connection management code, add locks to avoid
  race between main thread and progress thread, add logic to correctly
  handle FI_SHUTDOWN and FI_REJECT
- Set fabric name as network address in the format of a.b.c.d/e and
  domain name as network interface name
- Remove sock_compare_addr and add two utility functions ofi_equals_ipaddr
  and ofi_equals_sockaddr in fi.h
- Refactor fi_getinfo to handle corner cases and add logic if a given
  src_addr matches to any local interface addr
- Restructure acquiring/releasing the list_lock in progress thread so
  that it is only acquired once per iteration
- Refactor connection management of MSG ep so that it uses TCP instead
  of UDP for connection management msg and new port for every MSG
  endpoint
- Add sock_cq_sanitize_flags function to make sure only flags returned in
  CQ events are the ones that are listed on the manpage
- Update fi_poll semantics for counters so that it returns success if the
  counter value is different from the last-read-value
- Allow multiple threads to wait on one counter
- Update code to use ofi_util_mr - the new MR structure added to util code
- Fix fi_av_insert not to report error when the number of inserted addr
  exceeds the count attribute in fi_av_attr
- Add garbage collection of AV indices after fi_av_remove, add ep list
  in AV and cleanup conn map during fi_av_remove
- Use correct fi_tx_attr/fi_rx_attr for scalable ep

## UDP provider notes

- Enhance parameter checks for several function calls.
- Fix memory leak freeing CQ structure.
- Bind to a source address when enabling endpoint.
- Reduce reported resource limits (domain attributes).

## usNIC provider notes

- Fix handling of EP_MSG peers on different IP subnets [PR #1988]
- Fix handling of CM data. Fixes a bug where data received would
  overwrite parts of the connection management structure [PR #1991]
- Fix bug in CM connect/accept handling that would cause a seg fault
  if data was sent as part of a connection request [PR #1991]
- Fix invalid completion lengths in the MSG and RDM endpoint
  implementations of fi_recvv and fi_recvmsg [PR #2026]
- Implement the FI_CM_DATA_SIZE option for fi_getopt on passive
  endpoints [PR #2033]
- Add fi_reject implementation that supports data exchange [PR #2038]
- Fix fi_av_straddr bug that reported port in network order [PR #2244]
- Report -FI_EOPBADSTATE if the size left functions are used on an
  endpoint which has not been enabled [PR #2266]
- Change the domain/fabric naming. The fabric is now represented as
  the network address in the form of a.b.c.d/e and the domain name is
  the usNIC device name. For more information see fi_usnic(7) [PR
  #2287]
- Fix the domain name matching in fi_getinfo/fi_domain [PR #2298]
- Fix issue with AV where it is fully closed before pending
  asynchronous inserts can finish leading to invalid data accesses [PR
  #2397]
- Free all data associated with AV when fi_av_close is called [PR
  #2397]
- Fail with -FI_EINVAL if a value of FI_ADDR_NOTAVAIL is given to
  fi_av_lookup.  [PR #2397]
- Verify AV attributes and return an error if anything that is
  unsupported is requested (FI_AV_TABLE, named AVs, FI_READ, etc.) [PR
  #2397]

## Verbs provider notes

- Add fork support. It is enabled by default and can be turned off by
  setting the FI_FORK_UNSAFE variable to "yes". This can improve
  performance of memory registrations but also makes fork unsafe. The
  following are the limitations of fork support:
  - Fabric resources like endpoint, CQ, EQ, etc. should not be used in
    the forked process.
  - The memory registered using fi_mr_reg has to be page aligned since
    ibv_reg_mr marks the entire page that a memory region belongs to
    as not to be re-mapped when the process is forked (MADV_DONTFORK).
- Fix a bug where source address info was not being returned in
  fi_info when destination node is specified.

- verbs/MSG
  - Add fi_getopt for passive endpoints.
  - Add support for shared RX contexts.
- verbs/RDM
  - General bug fixes
  - Add FI_MSG capability
  - Add FI_PEEK and FI_CLAIM flags support
  - Add completion flags support
  - Add selective completion support
  - Add fi_cq_readerr support
  - Add possibility to set IPoIB network interface via FI_VERBS_IFACE
    environment variable
  - Add large data transfer support (> 1 GB)
  - Add FI_AV_TABLE support
  - Add fi_cntr support
  - Add environment variables for the provider tuning:
    FI_VERBS_RDM_BUFFER_NUM, FI_VERBS_RDM_BUFFER_SIZE,
    FI_VERBS_RDM_RNDV_SEG_SIZE, FI_VERBS_RDM_CQREAD_BUNCH_SIZE,
    FI_VERBS_RDM_THREAD_TIMEOUT, FI_VERBS_RDM_EAGER_SEND_OPCODE
  - Add iWarp support

v1.3.0, Mon Apr 11, 2016
========================

## General notes

* [See a list of provider features for this
  release](https://github.com/ofiwg/libfabric/wiki/Provider-Feature-Matrix-v1.3.0)

## GNI provider notes

- CLE 5.2UP04 required for building GNI provider
- General bug fixes, plugged memory leaks, etc.
- Improved error handling, warning messages, etc.
- Added support for the following APIs:
  - fi_endpoint: fi_getopt, fi_setopt, fi_rx_size_left, fi_tx_size_left, fi_stx_context
  - fi_cq: fi_sread, fi_sreadfrom
  - fi_msg: FI_MULTI_RECV (flag)
  - fi_domain: FI_PROGRESS_AUTO (flag)
  - fi_direct: FI_DIRECT
- Added support for FI_EP_DGRAM (datagram endpoint):
  - fi_msg, fi_tagged, fi_rma
- Memory registration improvements:
  - Improved performance
  - Additional domain open_ops
- Initial support for Cray Cluster Compatibility Mode (CCM)
- Implemented strict API checking
- Added hash list implementation for tag matching (available by domain open_ops)

Note: The current version of fabtests does not work with the GNI
provider due to the job launch mechanism on Cray XC systems.  Please
see the [GNI provider
wiki](https://github.com/ofi-cray/libfabric-cray/wiki) for
alternatives to validating your installation.

## MXM provider notes

- Initial release

## PSM provider notes

- Remove PSM2 related code.

## PSM2 provider notes

- Add support for multi-iov send, tagged send, and RMA write.
- Use utility provider for EQ, wait object, and poll set.

## Sockets provider notes

- General code cleanup
- Enable FABRIC_DIRECT
- Enable sockets-provider to run on FreeBSD
- Add support for fi_trywait
- Add support for map_addr in shared-av creation
- Add shared-av support on OSX
- Allow FI_AV_UNSPEC type during av_open
- Use loop-back address as source address if gethostname fails
- Disable control-msg ack for inject operations that do not expect completions
- Increase max_atomic_msg_size to 4096 bytes
- Remove check for cq_size availability while calculating tx/rx_size_left
- Use util-buffer pool for overflow entries in progress engine.
- Synchronize accesses to memory-registration operations
- Fix an issue that caused out-of-order arrival of messages
- Fix a bug in processing RMA access error
- Fix a bug that caused starvation in processing receive operations
- Add reference counting for pollset
- Fix a bug in connection port assignment

## UDP provider notes

- Initial release

## usNIC provider notes

- Implement fi_recvv and fi_recvmsg for FI_EP_RDM. [PR #1594]
- Add support for FI_INJECT flag in the FI_EP_RDM implementation of fi_sendv.
  [PR #1594]
- Fix crashes that occur in the FI_EP_RDM and the FI_EP_MSG implementations
  when messages are posted with the maximum IOV count.  [PR #1784]
- Fix crashes that occur in the FI_EP_RDM and the FI_EP_MSG implementations
  when posting messages with IOVs of varying lengths.  [PR #1784]
- Handle FI_PEEK flag in fi_eq_sread. [PR #1758]
- Return -FI_ENOSYS if a named AV is requested. [PR #1749]
- The ethernet header does not count against the MTU. Update reported
  max_msg_size when using FI_EP_DGRAM to reflect this. [PR #1738]
- Set the DF (do not fragment) bit in the IP header. [PR #1665]
- Fix crashes that may occur from improper handling of receive state tracking
  [PR #1809]
- Fortify the receive side of libnl communication [PR #1655]
- Fix handling of fi_info with passive endpoints. Connections opened on a
  passive endpoint now inherit the properties of the fi_info struct used to
  open the passive endpoint. [PR #1806]
- Implement pollsets. [PR #1835]
- Add version 2 of the usnic getinfo extension [PR #1866]
- Implement waitsets [PR #1893]
- Implement fi_trywait [PR #1893]
- Fix progress thread deadlock [PR #1893]
- Implement FD based CQ sread [PR #1893]

## Verbs provider notes

- Add support for fi_trywait
- Support building on OSes which have older versions of librdmacm (v1.0.16 or
  lesser). The functionality of the provider when the user passes AF_IB
  addresses is not guaranteed though.
- Added a workaround to support posting more than 'verbs send work queue length'
  number of fi_inject calls at a time.
- Make CQ reads thread safe.
- Support the case where the user creates only a send or recv queue for the
  endpoint.
- Fix an issue where RMA reads were not working on iWARP cards.
- verbs/RDM
  - Add support for RMA operations.
  - Add support for fi_cq_sread and fi_cq_sreadfrom
  - Rework connection management to make it work with fabtests and also allow
    connection to self.
  - Other bug fixes and performance improvements.

v1.2.0, Thu Jan 7, 2016
=======================

## General notes

- Added GNI provider
- Added PSM2 provider

## GNI provider notes
- Initial release

## PSM provider notes
- General bug fixes
- Support auto progress mode
- Support more threading modes
- Only set FI_CONTEXT mode if FI_TAGGED or FI_MSG is used
- Support Intel Omni-Path Fabric via the psm2-compat library

## PSM2 provider notes
- Initial addition

## Sockets provider notes

- General bug fixes and code cleanup
- Update memory registration to support 32-bit builds and fix build warnings
- Initiate conn-msg on the same tx_ctx as the tx operation for scalable ep
- Fix av mask calculation for scalable ep
- Mask out context-id during connection lookup for scalable ep
- Increase buffered receive limit
- Ignore FI_INJECT flag for atomic read operation
- Return -FI_EINVAL instead of -FI_ENODATA for fi_endpoint for invalid attributes
- Set default tag format to FI_TAG_GENERIC
- Set src/dest iov len correctly for readv operations
- Fix random crashes while closing shared contexts
- Fix an out of bound access when large multi-recv limit is specified by user
- Reset tag field in CQ entry for send completion
- Do not set prov_name in fabric_attr
- Validate flags in CQ/Cntr bind operations
- Scalability enhancements
- Increase mr_key size to 64 bit
- Use red-black tree for mr_key lookup

## usNIC provider notes
- The usNIC provider does not yet support asynchronous memory registration.
  Return -FI_EOPNOTSUPP if an event queue is bound to a domain with FI_REG_MR.
- Set fi_usnic_info::ui_version correctly in calls to
  fi_usnic_ops_fabric::getinfo().
- Improve fi_cq_sread performance.
- Return -FI_EINVAL from av_open when given invalid paramters.
- Fix bug in fi_av_remove that could lead to a seg fault.
- Implement fi_av_insertsvc.
- Report FI_PROTO_RUDP as protocol for EP_RDM.

## Verbs provider notes

- Add support for RDM EPs. Currently only FI_TAGGED capability is supported.
  RDM and MSG EPs would be reported in seperate domains since they don't share
  CQs. The RDM enpoint feature is currently experimental and no guarantees are
  given with regard to its functionality.
- Refactor the code into several files to enable adding RDM support.
- Consolidate send code paths to improve maintainability.
- Fix a bug in fi_getinfo where wild card address was not used when service
  argument is given.
- Fix fi_getinfo to always return -FI_ENODATA in case of failure.
- Add support for fi_eq_write.
- Other misc bug fixes.

v1.1.1, Fri Oct 2, 2015
=======================

## General notes

## PSM provider notes

- General bug fixes
- Proper termination of the name server thread
- Add UUID and PSM epid to debug output
- Add environment variable to control psm_ep_close timeout
- Code refactoring of AM-based messaging
- Check more fields of the hints passed to fi_getinfo
- Generate error CQ entries for empty result of recv with FI_SEEK flag
- Correctly handle overlapped local buffers in atomics
- Handle duplicated addresses in fi_av_insert
- Fix the return value of fi_cq_readerr
- Call AM progress function only when AM is used
- Detect MPI runs and turns off name server thread automatically

## Sockets provider notes

- General clean-up and restructuring
- Add fallback mechanism for getting source address
- Fix fi_getinfo to use user provided capabilities from hints
- Fix hostname and port number and added checks in sock_av_insertsym
- Add retry for connection timeout
- Release av resources in the error path
- Remove separate read/write CQ to be consistent with the man page
- Increase default connection map size and added environment variable to specify
  AV, CQ, EQ and connection map size to run large scale tests
- Fix FI_PEEK operation to be consistent with the man page
- Fix remote write event not to generate CQ event
- Fix CSWAP operation to return initial value
- Use size_t for min_multi_recv and buffered_len
- Set address size correctly in fi_getname/fi_getpeer

## usNIC provider notes

- Fix EP_RDM reassembly issue for large messages
- Return correct number of read completions on error
- Fix EP_RDM and EP_MSG data corruption issue when packets are actually
  corrupted on the wire
- Fix EP_RDM and EP_MSG fi_tx_size_left/fi_rx_size_left functions

## Verbs provider notes

- Add more logging for errors
- Bug fixes

v1.1.0, Wed Aug 5, 2015
=======================

## General notes

- Added fi_info utility tool
- Added unified global environment variable support
- Fixed configure issues with the clang/llvm compiler suite

## PSM provider notes

- General bug fixes
- Move processing of triggered ops outside of AM handlers
- Generate CQ entries for cancelled operations
- Remove environment variable FI_PSM_VERSION_CHECK
- Fix multi-recv completion generation
- Environment variable tweaks

## Sockets provider notes

- General bug fixes and code cleanup
- Add triggered operation suppport
- Generate error completion event for successful fi_cancel
- Support fi_cancel for tx operations
- Enable option for setting affinity to progress thread
- Improve error handling during connection management
- Avoid reverse lookup for every received message
- Avoid polling all connections while checking for incoming message
- Use fast_lock for progress engine's list_lock
- Handle disconnected sockets
- Add rx entry pool
- Mark tx entry as completed only if data is sent out to wire
- Add rx control context for every tx context for progressing control messages
- Set source address when addressing information is not passed by the application
- Reset return value after polling CQ ring buffer
- Reset FI_TRIGGER flag while triggering triggered operations
- Ensure progress of control context

## usNIC provider notes

- General bug fixes
- Add support for fi_getname/fi_setname, fi_cq_sread
- Change FI_PREFIX behavior per fi_getinfo(3)
- Fix to report correct lengths in all completions
- Support fi_inject() with FI_PREFIX
- Properly support iov_limit
- Support FI_MORE
- Fixed fi_tx_size_left() and fi_rx_size_left() usage
- Fixed obscure error when posting cq_size operations without reading
  a completion

## Verbs provider notes

- AF_IB addreses can now be passed as node argument to fi_getinfo
- Added support for fi_setname and migrating passive EP to active EP
- Detect and report multiple verbs devices if present
- Bug fixes

v1.0.0, Sun May 3, 2015
=======================

Initial public release, including the following providers:

- PSM
- Sockets
- usNIC
- Verbs
