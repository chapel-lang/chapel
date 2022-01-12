---
layout: page
title: fi_verbs(7)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_verbs \- The Verbs Fabric Provider

# OVERVIEW

The verbs provider enables applications using OFI to be run over any verbs
hardware (Infiniband, iWarp, etc). It uses the Linux Verbs API for network
transport and provides a translation of OFI calls to appropriate verbs API calls.
It uses librdmacm for communication management and libibverbs for other control
and data transfer operations.

# REQUIREMENTS

To successfully build and install verbs provider as part of libfabric, it needs
the following packages:
 * libibverbs
 * libibverbs-devel
 * librdmacm
 * librdmacm-devel

You may also want to look into any OS specific instructions for enabling RDMA.
e.g. RHEL has instructions on their documentation for enabling RDMA.

The IPoIB interface should be configured with a valid IP address. This is a
requirement from librdmacm.

# SUPPORTED FEATURES

The verbs provider supports a subset of OFI features.

### Endpoint types
FI_EP_MSG, FI_EP_DGRAM (beta), FI_EP_RDM.

FI_EP_RDM is supported via OFI RxM and RxD utility providers which are layered
on top of verbs. To the app, the provider name string would appear as
"verbs;ofi_rxm" or "verbs;ofi_rxd". Please refer the man pages for RxM (fi_rxm.7)
and RxD (fi_rxd.7) to know about the capabilities and limitations for the
FI_EP_RDM endpoint.

### Endpoint capabilities and features

#### MSG endpoints
FI_MSG, FI_RMA, FI_ATOMIC and shared receive contexts.

#### DGRAM endpoints
FI_MSG

### Modes
Verbs provider requires applications to support the following modes:

#### FI_EP_MSG endpoint type

  * FI_LOCAL_MR / FI_MR_LOCAL mr mode.

  * FI_RX_CQ_DATA for applications that want to use RMA. Applications must
    take responsibility of posting receives for any incoming CQ data.

### Addressing Formats
Supported addressing formats include
  * MSG and RDM (internal - deprecated) EPs support:
    FI_SOCKADDR, FI_SOCKADDR_IN, FI_SOCKADDR_IN6, FI_SOCKADDR_IB
  * DGRAM supports:
    FI_ADDR_IB_UD

### Progress
Verbs provider supports FI_PROGRESS_AUTO: Asynchronous operations make forward
progress automatically.

### Operation flags
Verbs provider supports FI_INJECT, FI_COMPLETION, FI_REMOTE_CQ_DATA,
FI_TRANSMIT_COMPLETE.

### Msg Ordering
Verbs provider support the following message ordering:

  * Read after Read

  * Read after Write

  * Read after Send

  * Write after Write

  * Write after Send

  * Send after Write

  * Send after Send

and the following completion ordering:

  * TX contexts: FI_ORDER_STRICT
  * RX contexts: FI_ORDER_DATA

### Fork
Verbs provider does not provide fork safety by default. Fork safety can be requested
by setting IBV_FORK_SAFE, or RDMAV_FORK_SAFE. If the system configuration supports
the use of huge pages, it is recommended to set RDMAV_HUGEPAGES_SAFE.
See ibv_fork_init(3) for additional details.

### Memory Registration Cache
The verbs provider uses the common memory registration cache functionality that's
part of libfabric utility code. This speeds up memory registration calls from
applications by caching registrations of frequently used memory regions. Please
refer to fi_mr(3): Memory Registration Cache section for more details.

# LIMITATIONS

### Memory Regions
Only FI_MR_BASIC mode is supported. Adding regions via s/g list is supported only
up to a s/g list size of 1. No support for binding memory regions to a counter.

### Wait objects
Only FI_WAIT_FD wait object is supported only for FI_EP_MSG endpoint type.
Wait sets are not supported.

### Resource Management
Application has to make sure CQs are not overrun as this cannot be detected
by the provider.

### Unsupported Features
The following features are not supported in verbs provider:

#### Unsupported Capabilities
FI_NAMED_RX_CTX, FI_DIRECTED_RECV, FI_TRIGGER, FI_RMA_EVENT

#### Other unsupported features
Scalable endpoints, FABRIC_DIRECT

#### Unsupported features specific to MSG endpoints
  * Counters, FI_SOURCE, FI_TAGGED, FI_PEEK, FI_CLAIM, fi_cancel, fi_ep_alias,
    shared TX context, cq_readfrom operations.
  * Completion flags are not reported if a request posted to an endpoint completes
    in error.

### Fork
The support for fork in the provider has the following limitations:

  * Fabric resources like endpoint, CQ, EQ, etc. should not be used in the
    forked process.
  * The memory registered using fi_mr_reg has to be page aligned since ibv_reg_mr
    marks the entire page that a memory region belongs to as not to be re-mapped
    when the process is forked (MADV_DONTFORK).

### XRC Transport
The XRC transport is intended to be used when layered with the RXM provider and
requires the use of shared receive contexts. See [`fi_rxm`(7)](fi_rxm.7.thml).
To enable XRC, the following environment variables must usually be set:
FI_VERBS_PREFER_XRC and FI_OFI_RXM_USE_SRX.

# RUNTIME PARAMETERS

The verbs provider checks for the following environment variables.

### Common variables:

*FI_VERBS_TX_SIZE*
:  Default maximum tx context size (default: 384)

*FI_VERBS_RX_SIZE*
:  Default maximum rx context size (default: 384)

*FI_VERBS_TX_IOV_LIMIT*
: Default maximum tx iov_limit (default: 4). Note: RDM (internal -
  deprecated) EP type supports only 1

*FI_VERBS_RX_IOV_LIMIT*
: Default maximum rx iov_limit (default: 4). Note: RDM (internal -
  deprecated) EP type supports only 1

*FI_VERBS_INLINE_SIZE*
: Default maximum inline size. Actual inject size returned in fi_info
  may be greater (default: 64)

*FI_VERBS_MIN_RNR_TIMER*
: Set min_rnr_timer QP attribute (0 - 31) (default: 12)

*FI_VERBS_CQREAD_BUNCH_SIZE*
: The number of entries to be read from the verbs completion queue
  at a time (default: 8).

*FI_VERBS_PREFER_XRC*
: Prioritize XRC transport fi_info before RC transport fi_info (default:
  0, RC fi_info will be before XRC fi_info)

*FI_VERBS_GID_IDX*
: The GID index to use (default: 0)

*FI_VERBS_DEVICE_NAME*
: Specify a specific verbs device to use by name

### Variables specific to MSG endpoints

*FI_VERBS_IFACE*
: The prefix or the full name of the network interface associated with the verbs
  device (default: ib)

### Variables specific to DGRAM endpoints

*FI_VERBS_DGRAM_USE_NAME_SERVER*
: The option that enables/disables OFI Name Server thread. The NS thread is
  used to resolve IP-addresses to provider specific addresses (default: 1,
  if "OMPI_COMM_WORLD_RANK" and "PMI_RANK" environment variables aren't defined)

*FI_VERBS_NAME_SERVER_PORT*
: The port on which Name Server thread listens incoming connections and
  requests (default: 5678)

### Environment variables notes
The fi_info utility would give the up-to-date information on environment variables:
fi_info -p verbs -e

# Troubleshooting / Known issues

### fi_getinfo returns -FI_ENODATA

- Set FI_LOG_LEVEL=info or FI_LOG_LEVEL=debug (if debug build of libfabric is
  available) and check if there any errors because of incorrect input parameters
  to fi_getinfo.
- Check if "fi_info -p verbs" is successful. If that fails the following checklist
  may help in ensuring that the RDMA verbs stack is functional:
  - If libfabric was compiled, check if verbs provider was built. Building verbs
   provider would be skipped if its dependencies (listed in requirements) aren't
   available on the system.
  - Verify verbs device is functional:
    - Does ibv_rc_pingpong (available in libibverbs) test work?
      - Does ibv_devinfo (available in libibverbs) show the device with PORT_ACTIVE
        status?
        - Check if Subnet Manager (SM) is running on the switch or on one of
          the nodes in the cluster.
        - Is the cable connected?
  - Verify librdmacm is functional:
    - Does ucmatose test (available in librdmacm) work?
    - Is the IPoIB interface (e.g. ib0) up and configured with a valid IP address?

### Other issues

When running an app over verbs provider with Valgrind, there may be reports of
memory leak in functions from dependent libraries (e.g. libibverbs, librdmacm).
These leaks are safe to ignore.

The provider protects CQ overruns that may happen because more TX operations were
posted to endpoints than CQ size. On the receive side, it isn't expected to
overrun the CQ. In case it happens the application developer should take care
not to post excess receives without draining the CQ. CQ overruns can make the
MSG endpoints unusable.

# SEE ALSO

[`fabric`(7)](fabric.7.html),
[`fi_provider`(7)](fi_provider.7.html),
