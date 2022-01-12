---
layout: page
title: fi_rma(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_rma - Remote memory access operations

fi_read / fi_readv / fi_readmsg
:   Initiates a read from remote memory

fi_write / fi_writev / fi_writemsg
fi_inject_write / fi_writedata
:   Initiate a write to remote memory

# SYNOPSIS

```c
#include <rdma/fi_rma.h>

ssize_t fi_read(struct fid_ep *ep, void *buf, size_t len, void *desc,
	fi_addr_t src_addr, uint64_t addr, uint64_t key, void *context);

ssize_t fi_readv(struct fid_ep *ep, const struct iovec *iov, void **desc,
	size_t count, fi_addr_t src_addr, uint64_t addr, uint64_t key,
	void *context);

ssize_t fi_readmsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
	uint64_t flags);

ssize_t fi_write(struct fid_ep *ep, const void *buf, size_t len,
	void *desc, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
	void *context);

ssize_t fi_writev(struct fid_ep *ep, const struct iovec *iov, void **desc,
	size_t count, fi_addr_t dest_addr, uint64_t addr, uint64_t key,
	void *context);

ssize_t fi_writemsg(struct fid_ep *ep, const struct fi_msg_rma *msg,
	uint64_t flags);

ssize_t fi_inject_write(struct fid_ep *ep, const void *buf, size_t len,
	fi_addr_t dest_addr, uint64_t addr, uint64_t key);

ssize_t fi_writedata(struct fid_ep *ep, const void *buf, size_t len,
	void *desc, uint64_t data, fi_addr_t dest_addr, uint64_t addr,
	uint64_t key, void *context);

ssize_t fi_inject_writedata(struct fid_ep *ep, const void *buf, size_t len,
	uint64_t data, fi_addr_t dest_addr, uint64_t addr, uint64_t key);
```

# ARGUMENTS

*ep*
: Fabric endpoint on which to initiate read or write operation.

*buf*
: Local data buffer to read into (read target) or write from (write
  source)

*len*
: Length of data to read or write, specified in bytes.  Valid
  transfers are from 0 bytes up to the endpoint's max_msg_size.

*iov*
: Vectored data buffer.

*count*
: Count of vectored data entries.

*addr*
: Address of remote memory to access.  This will be the virtual
  address of the remote region in the case of FI_MR_BASIC, or the
  offset from the starting address in the case of FI_MR_SCALABLE.

*key*
: Protection key associated with the remote memory.

*desc*
: Descriptor associated with the local data buffer
  See [`fi_mr`(3)](fi_mr.3.html).

*data*
: Remote CQ data to transfer with the operation.

*dest_addr*
: Destination address for connectionless write transfers.  Ignored
  for connected endpoints.

*src_addr*
: Source address to read from for connectionless transfers.  Ignored
  for connected endpoints.

*msg*
: Message descriptor for read and write operations.

*flags*
: Additional flags to apply for the read or write operation.

*context*
: User specified pointer to associate with the operation.  This parameter is
  ignored if the operation will not generate a successful completion, unless
  an op flag specifies the context parameter be used for required input.

# DESCRIPTION

RMA (remote memory access) operations are used to transfer data
directly between a local data buffer and a remote data buffer.  RMA
transfers occur on a byte level granularity, and no message boundaries
are maintained.

The write functions -- fi_write, fi_writev, fi_writemsg,
fi_inject_write, and fi_writedata -- are used to transmit
data into a remote memory buffer.  The main
difference between write functions are the number and type of
parameters that they accept as input.  Otherwise, they perform the
same general function.

The read functions -- fi_read, fi_readv, and fi_readmsg --
are used to transfer data from a remote memory region into local data
buffer(s).  Similar to the write operations, read operations operate
asynchronously.  Users should not touch the posted data buffer(s)
until the read operation has completed.

Completed RMA operations are reported to the user through one or more
completion queues associated with the endpoint.  Users provide context
which are associated with each operation, and is returned to the user
as part of the completion.  See fi_cq for completion event details.

By default, the remote endpoint does not generate an event or notify
the user when a memory region has been accessed by an RMA read or
write operation.  However, immediate data may be associated with an
RMA write operation.  RMA writes with immediate data will generate a
completion entry at the remote endpoint, so that the immediate data
may be delivered.

## fi_write

The call fi_write transfers the data contained in the user-specified
data buffer to a remote memory region.

## fi_writev

The fi_writev call adds support for a scatter-gather list to fi_write.
The fi_writev transfers the set of data buffers
referenced by the iov parameter to the remote memory region.

## fi_writemsg

The fi_writemsg call supports data transfers over both connected and
connectionless endpoints, with the ability to control the write operation
per call through the use of flags.  The fi_writemsg function takes a
struct fi_msg_rma as input.

```c
struct fi_msg_rma {
	const struct iovec *msg_iov;     /* local scatter-gather array */
	void               **desc;       /* operation descriptor */
	size_t             iov_count;    /* # elements in msg_iov */
	fi_addr_t          addr;        /* optional endpoint address */
	const struct fi_rma_iov *rma_iov;/* remote SGL */
	size_t             rma_iov_count;/* # elements in rma_iov */
	void               *context;     /* user-defined context */
	uint64_t           data;         /* optional immediate data */
};

struct fi_rma_iov {
	uint64_t           addr;         /* target RMA address */
	size_t             len;          /* size of target buffer */
	uint64_t           key;          /* access key */
};
```

## fi_inject_write

The write inject call is an optimized version of fi_write.  It provides
similar completion semantics as fi_inject [`fi_msg`(3)](fi_msg.3.html).

## fi_writedata

The write data call is similar to fi_write, but allows for the sending
of remote CQ data (see FI_REMOTE_CQ_DATA flag) as part of the
transfer.

## fi_inject_writedata

The inject write data call is similar to fi_inject_write, but allows for the sending
of remote CQ data (see FI_REMOTE_CQ_DATA flag) as part of the
transfer.

## fi_read

The fi_read call requests that the remote endpoint transfer data from
the remote memory region into the local data buffer.

## fi_readv

The fi_readv call adds support for a scatter-gather list to fi_read.
The fi_readv transfers data from the remote memory region into
the set of data buffers referenced by the iov parameter.

## fi_readmsg

The fi_readmsg call supports data transfers over both connected and
connectionless endpoints, with the ability to control the read operation
per call through the use of flags.  The fi_readmsg function takes a
struct fi_msg_rma as input.

# FLAGS

The fi_readmsg and fi_writemsg calls allow the user to specify flags
which can change the default data transfer operation.  Flags specified
with fi_readmsg / fi_writemsg override most flags previously
configured with the endpoint, except where noted (see fi_endpoint.3).
The following list of flags are usable with fi_readmsg and/or
fi_writemsg.

*FI_REMOTE_CQ_DATA*
: Applies to fi_writemsg and fi_writedata.  Indicates
  that remote CQ data is available and should be sent as part of the
  request.  See fi_getinfo for additional details on
  FI_REMOTE_CQ_DATA.

*FI_COMPLETION*
: Indicates that a completion entry should be generated for the
  specified operation.  The endpoint must be bound to a completion queue
  with FI_SELECTIVE_COMPLETION that corresponds to the specified operation,
  or this flag is ignored.

*FI_MORE*
: Indicates that the user has additional requests that will
  immediately be posted after the current call returns.  Use of this
  flag may improve performance by enabling the provider to optimize
  its access to the fabric hardware.

*FI_INJECT*
: Applies to fi_writemsg.  Indicates that the outbound data buffer
   should be returned to user immediately after the write call
   returns, even if the operation is handled asynchronously.  This may
   require that the underlying provider implementation copy the data
   into a local buffer and transfer out of that buffer. This flag can only
   be used with messages smaller than inject_size.

*FI_INJECT_COMPLETE*
: Applies to fi_writemsg.  Indicates that a completion should be
  generated when the source buffer(s) may be reused.

*FI_TRANSMIT_COMPLETE*
: Applies to fi_writemsg.  Indicates that a completion should not be
  generated until the operation has been successfully transmitted and
  is no longer being tracked by the provider.

*FI_DELIVERY_COMPLETE*
: Applies to fi_writemsg.  Indicates that a completion should be
  generated when the operation has been processed by the destination.

*FI_COMMIT_COMPLETE*
: Applies to fi_writemsg when targeting persistent memory regions.
  Indicates that a completion should be generated only after the result
  of the operation has been made durable.

*FI_FENCE*
: Applies to transmits.  Indicates that the requested operation, also
  known as the fenced operation, and any operation posted after the
  fenced operation will be deferred until all previous operations
  targeting the same peer endpoint have completed.  Operations posted
  after the fencing will see and/or replace the results of any
  operations initiated prior to the fenced operation.

  The ordering of operations starting at the posting of the fenced
  operation (inclusive) to the posting of a subsequent fenced operation
  (exclusive) is controlled by the endpoint's ordering semantics.

# RETURN VALUE

Returns 0 on success. On error, a negative value corresponding to fabric
errno is returned. Fabric errno values are defined in
`rdma/fi_errno.h`.

# ERRORS

*-FI_EAGAIN*
: See [`fi_msg`(3)](fi_msg.3.html) for a detailed description of handling
  FI_EAGAIN.

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_cq`(3)](fi_cq.3.html)
