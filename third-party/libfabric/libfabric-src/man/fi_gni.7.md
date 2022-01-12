---
layout: page
title: fi_gni(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_gni \- The GNI Fabric Provider

# OVERVIEW

The GNI provider runs on Cray XC (TM) systems utilizing the user-space
Generic Network Interface (uGNI) which provides low-level access to
the Aries interconnect.  The Aries interconnect is designed for
low-latency one-sided messaging and also includes direct hardware
support for common atomic operations and optimized collectives.

# REQUIREMENTS

The GNI provider runs on Cray XC systems running CLE 5.2 UP04 or higher
using gcc version 4.9 or higher.

When using the fi_mr_regattr() and fi_mr_regv() functions to register
multiple region, users must register the memory region with 4K
page alignment. Any other page address alignment will result in a return
value of -FI_EINVAL.

When using the scalable memory registration mode, applications must make
registration requests on 4K page alignment boundaries. Any other value
will result in a return value of -FI_EINVAL.

# SUPPORTED FEATURES

The GNI provider supports the following features defined for the
libfabric API:

*Endpoint types*
: The provider supports the *FI_EP_RDM*, *FI_EP_DGRAM*, *FI_EP_MSG* endpoint
  types, including scalable endpoints.

*Address vectors*
: The provider implements both the *FI_AV_MAP* and *FI_AV_TABLE*
  address vector types. FI_EVENT is unsupported.

*Memory registration modes*
: The provider implements basic and scalable memory
  registration modes.

*Data transfer operations*
: The following data transfer interfaces are supported for all
  endpoint types: *FI_ATOMIC*, *FI_MSG*, *FI_RMA*, *FI_TAGGED*.  See
  DATA TRANSFER OPERATIONS below for more details.

*Completion events*
: The GNI provider supports *FI_CQ_FORMAT_CONTEXT*, *FI_CQ_FORMAT_MSG*,
  *FI_CQ_FORMAT_DATA* and *FI_CQ_FORMAT_TAGGED* with wait objects of type
  *FI_WAIT_NONE*, *FI_WAIT_UNSPEC*, *FI_WAIT_SET*.

*Modes*
: The GNI provider does not require any operation modes.

*Progress*
: For both control and data progress, the GNI provider supports both
  *FI_PROGRESS_AUTO* and *FI_PROGRESS_MANUAL*, with a default set to
  *FI_PROGRESS_AUTO*.
  Note that for data progress, progression is only performed
  when data transfers use the rendezvous protocol.

*Wait Objects*
: The GNI provider specifically supports wait object types *FI_WAIT_UNSPEC*,
  and *FI_WAIT_SET*. A wait object must be used when calling fi_cntr_wait,
  fi_cq_sread/from, fi_eq_sread/from, fi_wait.
  The GNI provider spawns an internal wait progress thread that is woken up
  when clients utilize the wait system (e.g., calling fi_wait).

*Additional Features*
: The GNI provider also supports the following capabilities and features:
- *FI_MULTI_RECV*
- *FI_SOURCE*
- *FI_FENCE*
- *FI_RM_ENABLED*
- *FI_RMA_EVENT*
- *FI_REMOTE_CQ_DATA*
- *FABRIC_DIRECT* compilation mode
- *FI_MORE* (For FI_RMA)

# DATA TRANSFER OPERATIONS

## FI_ATOMIC

Currently, the GNI provider only supports atomic operations supported
directly by the Aries NIC.  These include operations on 32- and
64-bit, signed and unsigned integer and floating point values.
Specifically,

### Basic (fi_atomic, etc.)
- *FI_MIN*, *FI_MAX* (no unsigned)
- *FI_SUM* (no 64-bit floating point)
- *FI_BOR*, *FI_BAND*, *FI_BXOR* (no floating point)
- *FI_ATOMIC_WRITE*

### Fetching (fi_fetch_atomic, etc.)
- All of the basic operations as above
- FI_ATOMIC_READ

### Comparison (fi_compare_atomic, etc.)
- FI_CSWAP
- FI_MSWAP

## FI_MSG

All *FI_MSG* operations are supported.

## FI_RMA

All *FI_RMA* operations are supported.

## FI_TAGGED

All *FI_TAGGED* operations are supported except `fi_tinjectdata`.

# GNI EXTENSIONS

The GNI provider exposes low-level tuning parameters via domain, endpoint
and fabric level `fi_open_ops` interfaces.
The domain extensions have been named *FI_GNI_DOMAIN_OPS_1*. The endpoint
extensions have been named *FI_GNI_EP_OPS_1*. The fabric extensions have
been named *FI_GNI_FABRIC_OPS_1* and *FI_GNI_FABRIC_OPS_2*.
The flags parameter is currently ignored.  The fi_open_ops function takes
a `struct fi_gni_ops_domain` or a `struct fi_gni_ops_ep` parameter
respectively and populates it with the following:

```c
struct fi_gni_ops_fab {
	int (*set_val)(struct fid *fid, fab_ops_val_t t, void *val);
	int (*get_val)(struct fid *fid, fab_ops_val_t t, void *val);
};

struct fi_gni_auth_key_ops_fab {
	int (*set_val)(uint8_t *auth_key, size_t auth_keylen, gnix_auth_key_opt_t opt, void *val);
	int (*get_val)(uint8_t *auth_key, size_t auth_keylen, gnix_auth_key_opt_t opt, void *val);
};

struct fi_gni_ops_domain {
	int (*set_val)(struct fid *fid, dom_ops_val_t t, void *val);
	int (*get_val)(struct fid *fid, dom_ops_val_t t, void *val);
	int (*flush_cache)(struct fid *fid);
};

struct fi_gni_ops_ep {
	int (*set_val)(struct fid *fid, dom_ops_val_t t, void *val);
	int (*get_val)(struct fid *fid, dom_ops_val_t t, void *val);
        size_t (*native_amo)(struct fid_ep *ep, const void *buf,
                             size_t count,void *desc,
                             fi_addr_t dest_addr, uint64_t addr,
                             uint64_t key, enum fi_datatype datatype,
                             enum gnix_fab_req_type req_type,
                             void *context);
};
```

The `set_val` function sets the value of a given parameter; the
`get_val` function returns the current value.

For *FI_GNI_FABRIC_OPS_1*, the currently supported values are:

*GNI_WAIT_THREAD_SLEEP*
: Time in seconds for which the progress thread will sleep between
periods of inactivity.

*GNI_DEFAULT_USER_REGISTRATION_LIMIT*
: The number of user registrations that an authorization key is limited
to when using the scalable memory mode, if not specified by
the user during init.

*GNI_DEFAULT_PROV_REGISTRATION_LIMIT*
: The number of provider registration that an authorization key is
limited to when using the scalable memory mode, if not specified
by the user during init.

*GNI_WAIT_SHARED_MEMORY_TIMEOUT*
: The number of seconds that the provider should wait when
attempting to open mmap'd shared memory files for internal
mappings.

For *FI_GNI_FABRIC_OPS_2*, the currently supported values are:

*GNIX_USER_KEY_LIMIT*
: The number of user registrations that an authorization key is limited
to when using the scalable memory mode. This may only be set prior
to the first use of an authorization key in the initialization of a
domain, endpoint, or memory registration.

*GNIX_PROV_KEY_LIMIT*
: The number of provider registrations that an authorization key is
limited to when using the scalable memory mode. This may only be
set prior to the first use of an authorization key in the initialization
of a domain, endpoint, or memory registration.

For *FI_GNI_DOMAIN_OPS_1*, the currently supported values are:

*GNI_MSG_RENDEZVOUS_THRESHOLD*
: Threshold message size at which a rendezvous protocol is used for
  *FI_MSG* data transfers.  The value is of type uint32_t.

*GNI_RMA_RDMA_THRESHOLD*
: Threshold message size at which RDMA is used for *FI_RMA* data
  transfers.  The value is of type uint32_t.

*GNI_CONN_TABLE_INITIAL_SIZE*
: Initial size of the internal table data structure used to manage
  connections.  The value is of type uint32_t.

*GNI_CONN_TABLE_MAX_SIZE*
: Maximum size of the internal table data structure used to manage
  connections.  The value is of type uint32_t.

*GNI_CONN_TABLE_STEP_SIZE*
: Step size for increasing the size of the internal table data
  structure used to manage internal GNI connections.  The value is of
  type uint32_t.

*GNI_VC_ID_TABLE_CAPACITY*
: Size of the virtual channel (VC) table used for managing remote
  connections.  The value is of type uint32_t.

*GNI_MBOX_PAGE_SIZE*
: Page size for GNI SMSG mailbox allocations.  The value is of type
  uint32_t.

*GNI_MBOX_NUM_PER_SLAB*
: Number of GNI SMSG mailboxes per allocation slab.  The value is of
  type uint32_t.

*GNI_MBOX_MAX_CREDIT*
: Maximum number of credits per GNI SMSG mailbox.  The value is of
  type uint32_t.

*GNI_MBOX_MSG_MAX_SIZE*
: Maximum size of GNI SMSG messages.  The value is of type uint32_t.

*GNI_RX_CQ_SIZE*
: Recommended GNI receive CQ size.  The value is of type uint32_t.

*GNI_TX_CQ_SIZE*
: Recommended GNI transmit CQ size.  The value is of type uint32_t.

*GNI_MAX_RETRANSMITS*
: Maximum number of message retransmits before failure.  The value is
  of type uint32_t.

*GNI_MR_CACHE_LAZY_DEREG*
: Enable or disable lazy deregistration of memory.  The value is of
  type int32_t.

*GNI_MR_CACHE*
: Select the type of cache that the domain will use. Valid choices are
  the following: 'internal', 'udreg', or 'none'. 'internal' refers to the GNI
  provider internal registration cache. 'udreg' refers to a user level dreg
  library based cache. Lastly, 'none' refers to device direct registration
  without a provider cache.

*GNI_MR_HARD_REG_LIMIT*
: Maximum number of registrations. Applies only to the GNI provider cache. The value is of type int32_t (-1 for no limit).

*GNI_MR_SOFT_REG_LIMIT*
: Soft cap on the registration limit. Applies only to the GNI provider cache. The value is of type int32_t (-1 for no limit).

*GNI_MR_HARD_STALE_REG_LIMIT*
: Maximum number of stale registrations to be held in cache. This applies to  the GNI provider cache and the udreg cache. The value is of type int32_t (-1 for no limit for the GNI provider cache and udreg cache values must be greater than 0).

*GNI_MR_UDREG_LIMIT*
: Maximum number of registrations. Applies only to the udreg cache. The value is of type int32_t. The value must be greater than 0.

*GNI_XPMEM_ENABLE*
: Enable or disable use of XPMEM for on node messages using the GNI provider internal rendezvous protocol.  The value is of type bool.

*GNI_DGRAM_PROGRESS_TIMEOUT*
: Controls timeout value in milliseconds for the control progress thread.  The value is of type uint32_t.

The `flush_cache` function allows the user to flush any stale registration
cache entries from the cache. This has the effect of removing registrations
from the cache that have been deregistered with the provider, but still
exist in case that they may be reused in the near future. Flushing the stale
registrations forces hardware-level deregistration of the stale memory
registrations and frees any memory related to those stale registrations. Only
the provider-level registration struct is freed, not the user buffer
associated with the registration.
The parameter for `flush_cache` is a struct fid pointer to a fi_domain. The
memory registration cache is tied to the domain, so issuing a `flush_cache` to
the domain will flush the registration cache of the domain.

For *FI_GNI_EP_OPS_1*, the currently supported values are:
*GNI_HASH_TAG_IMPL*
: Use a hashlist for the tag list implementation.  The value is of type uint32_t.

The `native_amo` function allows the user to call GNI native atomics
that are not implemented in the libfabric API.
The parameters for native_amo are the same as the fi_atomic function
but adds the following parameter:

*enum gnix_fab_req_type req_type*
: The req_type's supported with this call are GNIX_FAB_RQ_NAMO_AX
 (AND and XOR), and GNIX_FAB_RQ_NAMO_AX_S (AND and XOR 32 bit),
GNIX_FAB_RQ_NAMO_FAX (Fetch AND and XOR) and GNIX_FAB_RQ_NAMO_FAX_S
 (Fetch AND and XOR 32 bit).

# NOTES

The default address format is FI_ADDR_GNI. This is the only address format
used within the GNI provider for message passing. FI_ADDR_STR is always
parsed and converted to FI_ADDR_GNI for use within the GNI provider.

*FI_ADDR_STR* is formatted as follows:
gni;node;service;GNIX_AV_STR_ADDR_VERSION;device_addr;cdm_id;name_type;cm_nic_cdm_id;cookie;rx_ctx_cnt;key_offset

The GNI provider sets the domain attribute *cntr_cnt* to the CQ limit divided by 2.

The GNI provider sets the domain attribute *cq_cnt* to the CQ limit divided by 2.

The GNI provider sets the domain attribute *ep_cnt* to SIZE_MAX.

Completion queue events may report unknown source address information when
using *FI_SOURCE*. If *FI_SOURCE_ERR* is also specified, the source address
information will be reported in the
err_data member of the struct fi_cq_err_entry populated by fi_cq_readerr. The
err_data member will contain the source address information in the FI_ADDR_GNI
address format. In order to populate the remote peer's address vector
with this mechanism, the application must call fi_cq_readerr to get the
source address followed by fi_av_insert on the populated err_data member.

For FI_MULTI_RECV, the GNI provider generates a separate FI_MULTI_RECV CQ event
once the receive buffer has been consumed.  Also, owing to the out-or-order nature
of the Cray network, the CQ events associated with individual messages arriving in the
receive buffer may be generated out of order with respect to the offset into the buffer
into which the messages were received.

The GNI provider can use a maximum of 4K memory registrations per *node* when using scalable memory registration.
Please consider this limitation when placing multiple processes on each node.

The GNI provider sets the default user registration limit to 192 when using scalable memory registration,
and sets the default provider registration limit to 64. These limits are directly associated
with the authorization key in use when creating the registration. If no authorization key
is used when creating the registration, the registration is automatically bound to the same
authorization key as the domain to which the registration belongs.

When using scalable memory registration, the provider may make registrations which consume some of the
registrations set aside for the provider. This impacts the performance of FI_LOCAL_MR, which
relies on provider-created registrations.

All memory registrations are associated with an authorization key, whether it is the provider
default key(keylen=0) or a user-acquired key (key!=NULL, keylen!=0). Each authorization
key is associated with a unique GNI network key. A GNI network key can only accommodate a single
memory mode, whether it is basic, or scalable memory registration. If a user attempts to open multiple
domains using different memory modes with the same authorization key, the provider will return
-FI_EINVAL.

When using scalable memory registration, the user may request keys beginning at 0, and ending at the
user registration limit for a given authorization key.

When using scalable memory registration and fi_mr_refresh(), only refresh the updated pages, not the entire
registration. If the entire registration is refreshed and some of the pages are not mapped,
then refresh will return -FI_EFAULT.

Registration IDs for scalable memory registration are local to the node. This means that the application is
responsible for handing the coordination of key selection.

The location of the authorization key mapping file can be controlled through two environment variables,
TMPDIR and GNIX_AK_FILENAME. Setting TMPDIR to a non-NULL value with change the directory for the 
authorization key mapping file, and setting GNIX_AK_FILENAME to a non-NULL value will change the filename.
The default path for the authorization key mapping file is '/tmp/gnix_vmdh_info'. The recommendation is that
the user should not change these environment variables unless necessary. 

# KNOWN BUGS

The GNI provider currently treats the fi_shutdown() interface as a strictly
local operation.  That is, fi_shutdown() causes the local endpoint to be shut
down, and a shutdown event to be generated on the local EQ.  However, a
connected remote peer endpoint is not notified of a call to fi_shutdown().

The GNI provider does not currently handle the case when FI_OPT_MULTI_RECV is set to 0
and will return -FI_EINVAL if an application attempts to set this value to zero.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_open_ops`(3)](fi_open_ops.3.html),
[`fi_provider`(7)](fi_provider.7.html),
[`fi_getinfo`(3)](fi_getinfo.3.html)
[`fi_atomic`(3)](fi_atomic.3.html)

For more information on uGNI, see *Using the GNI and DMAPP APIs*
(S-2446-3103, Cray Inc.).  For more information on the GNI provider,
see *An Implementation of OFI libfabric in Support of Multithreaded
PGAS Solutions* (PGAS '15).

