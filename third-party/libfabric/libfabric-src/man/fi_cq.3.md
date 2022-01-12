---
layout: page
title: fi_cq(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_cq \- Completion queue operations

fi_cq_open / fi_close
: Open/close a completion queue

fi_control
: Control CQ operation or attributes.

fi_cq_read / fi_cq_readfrom / fi_cq_readerr
: Read a completion from a completion queue

fi_cq_sread / fi_cq_sreadfrom
: A synchronous (blocking) read that waits until a specified condition
  has been met before reading a completion from a completion queue.

fi_cq_signal
: Unblock any thread waiting in fi_cq_sread or fi_cq_sreadfrom.

fi_cq_strerror
: Converts provider specific error information into a printable string

# SYNOPSIS

```c
#include <rdma/fi_domain.h>

int fi_cq_open(struct fid_domain *domain, struct fi_cq_attr *attr,
    struct fid_cq **cq, void *context);

int fi_close(struct fid *cq);

int fi_control(struct fid *cq, int command, void *arg);

ssize_t fi_cq_read(struct fid_cq *cq, void *buf, size_t count);

ssize_t fi_cq_readfrom(struct fid_cq *cq, void *buf, size_t count,
    fi_addr_t *src_addr);

ssize_t fi_cq_readerr(struct fid_cq *cq, struct fi_cq_err_entry *buf,
    uint64_t flags);

ssize_t fi_cq_sread(struct fid_cq *cq, void *buf, size_t count,
    const void *cond, int timeout);

ssize_t fi_cq_sreadfrom(struct fid_cq *cq, void *buf, size_t count,
    fi_addr_t *src_addr, const void *cond, int timeout);

int fi_cq_signal(struct fid_cq *cq);

const char * fi_cq_strerror(struct fid_cq *cq, int prov_errno,
      const void *err_data, char *buf, size_t len);
```

# ARGUMENTS

*domain*
: Open resource domain

*cq*
: Completion queue

*attr*
: Completion queue attributes

*context*
: User specified context associated with the completion queue.

*buf*
: For read calls, the data buffer to write completions into.  For
  write calls, a completion to insert into the completion queue.  For
  fi_cq_strerror, an optional buffer that receives printable error
  information.

*count*
: Number of CQ entries.

*len*
: Length of data buffer

*src_addr*
: Source address of a completed receive operation

*flags*
: Additional flags to apply to the operation

*command*
: Command of control operation to perform on CQ.

*arg*
: Optional control argument

*cond*
: Condition that must be met before a completion is generated

*timeout*
: Time in milliseconds to wait.  A negative value indicates infinite
  timeout.

*prov_errno*
: Provider specific error value

*err_data*
: Provider specific error data related to a completion

# DESCRIPTION

Completion queues are used to report events associated with data
transfers.  They are associated with message sends and receives, RMA,
atomic, tagged messages, and triggered events.  Reported events are
usually associated with a fabric endpoint, but may also refer to
memory regions used as the target of an RMA or atomic operation.

## fi_cq_open

fi_cq_open allocates a new completion queue.  Unlike event queues,
completion queues are associated with a resource domain and may be
offloaded entirely in provider hardware.

The properties and behavior of a completion queue are defined by
`struct fi_cq_attr`.

```c
struct fi_cq_attr {
	size_t               size;      /* # entries for CQ */
	uint64_t             flags;     /* operation flags */
	enum fi_cq_format    format;    /* completion format */
	enum fi_wait_obj     wait_obj;  /* requested wait object */
	int                  signaling_vector; /* interrupt affinity */
	enum fi_cq_wait_cond wait_cond; /* wait condition format */
	struct fid_wait     *wait_set;  /* optional wait set */
};
```

*size*
: Specifies the minimum size of a completion queue. A value of 0 indicates that
  the provider may choose a default value.

*flags*
: Flags that control the configuration of the CQ.

- *FI_AFFINITY*
: Indicates that the signaling_vector field (see below) is valid.

*format*
: Completion queues allow the application to select the amount of
  detail that it must store and report.  The format attribute allows
  the application to select one of several completion formats,
  indicating the structure of the data that the completion queue
  should return when read.  Supported formats and the structures that
  correspond to each are listed below.  The meaning of the CQ entry
  fields are defined in the _Completion Fields_ section.

- *FI_CQ_FORMAT_UNSPEC*
: If an unspecified format is requested, then the CQ will use a
  provider selected default format.

- *FI_CQ_FORMAT_CONTEXT*
: Provides only user specified context that was associated with the
  completion.

```c
struct fi_cq_entry {
	void     *op_context; /* operation context */
};
```

- *FI_CQ_FORMAT_MSG*
: Provides minimal data for processing completions, with expanded
  support for reporting information about received messages.

```c
struct fi_cq_msg_entry {
	void     *op_context; /* operation context */
	uint64_t flags;       /* completion flags */
	size_t   len;         /* size of received data */
};
```

- *FI_CQ_FORMAT_DATA*
: Provides data associated with a completion.  Includes support for
  received message length, remote CQ data, and multi-receive buffers.

```c
struct fi_cq_data_entry {
	void     *op_context; /* operation context */
	uint64_t flags;       /* completion flags */
	size_t   len;         /* size of received data */
	void     *buf;        /* receive data buffer */
	uint64_t data;        /* completion data */
};
```

- *FI_CQ_FORMAT_TAGGED*
: Expands completion data to include support for the tagged message
  interfaces.

```c
struct fi_cq_tagged_entry {
	void     *op_context; /* operation context */
	uint64_t flags;       /* completion flags */
	size_t   len;         /* size of received data */
	void     *buf;        /* receive data buffer */
	uint64_t data;        /* completion data */
	uint64_t tag;         /* received tag */
};
```

*wait_obj*
: CQ's may be associated with a specific wait object.  Wait objects
  allow applications to block until the wait object is signaled,
  indicating that a completion is available to be read.  Users may use
  fi_control to retrieve the underlying wait object associated with a
  CQ, in order to use it in other system calls.  The following values
  may be used to specify the type of wait object associated with a
  CQ: FI_WAIT_NONE, FI_WAIT_UNSPEC, FI_WAIT_SET, FI_WAIT_FD,
  FI_WAIT_MUTEX_COND, and FI_WAIT_YIELD.  The default is FI_WAIT_NONE.

- *FI_WAIT_NONE*
: Used to indicate that the user will not block (wait) for completions
  on the CQ.  When FI_WAIT_NONE is specified, the application may not
  call fi_cq_sread or fi_cq_sreadfrom.

- *FI_WAIT_UNSPEC*
: Specifies that the user will only wait on the CQ using fabric
  interface calls, such as fi_cq_sread or fi_cq_sreadfrom.  In this
  case, the underlying provider may select the most appropriate or
  highest performing wait object available, including custom wait
  mechanisms.  Applications that select FI_WAIT_UNSPEC are not
  guaranteed to retrieve the underlying wait object.

- *FI_WAIT_SET*
: Indicates that the completion queue should use a wait set object to
  wait for completions.  If specified, the wait_set field must
  reference an existing wait set object.

- *FI_WAIT_FD*
: Indicates that the CQ should use a file descriptor as its wait
  mechanism.  A file descriptor wait object must be usable in select,
  poll, and epoll routines.  However, a provider may signal an FD wait
  object by marking it as readable, writable, or with an error.

- *FI_WAIT_MUTEX_COND*
: Specifies that the CQ should use a pthread mutex and cond variable
  as a wait object.

- *FI_WAIT_YIELD*
: Indicates that the CQ will wait without a wait object but instead
  yield on every wait. Allows usage of fi_cq_sread and fi_cq_sreadfrom
  through a spin.

*signaling_vector*
: If the FI_AFFINITY flag is set, this indicates the logical cpu number
  (0..max cpu - 1) that interrupts associated with the CQ should target.
  This field should be treated as a hint to the provider and may be
  ignored if the provider does not support interrupt affinity.

*wait_cond*
: By default, when a completion is inserted into a CQ that supports
  blocking reads (fi_cq_sread/fi_cq_sreadfrom), the corresponding wait
  object is signaled.  Users may specify a condition that must first
  be met before the wait is satisfied.  This field indicates how the
  provider should interpret the cond field, which describes the
  condition needed to signal the wait object.

  A wait condition should be treated as an optimization.  Providers
  are not required to meet the requirements of the condition before
  signaling the wait object.  Applications should not rely on the
  condition necessarily being true when a blocking read call returns.

  If wait_cond is set to FI_CQ_COND_NONE, then no additional
  conditions are applied to the signaling of the CQ wait object, and
  the insertion of any new entry will trigger the wait condition.  If
  wait_cond is set to FI_CQ_COND_THRESHOLD, then the cond field is
  interpreted as a size_t threshold value.  The threshold indicates
  the number of entries that are to be queued before at the CQ before
  the wait is satisfied.

  This field is ignored if wait_obj is set to FI_WAIT_NONE.

*wait_set*
: If wait_obj is FI_WAIT_SET, this field references a wait object to
  which the completion queue should attach.  When an event is inserted
  into the completion queue, the corresponding wait set will be
  signaled if all necessary conditions are met.  The use of a wait_set
  enables an optimized method of waiting for events across multiple
  event and completion queues.  This field is ignored if wait_obj is
  not FI_WAIT_SET.

## fi_close

The fi_close call releases all resources associated with a completion
queue. Any completions which remain on the CQ when it is closed are
lost.

When closing the CQ, there must be no opened endpoints, transmit contexts, or
receive contexts associated with the CQ.  If resources are still associated
with the CQ when attempting to close, the call will return -FI_EBUSY.

## fi_control

The fi_control call is used to access provider or implementation
specific details of the completion queue.  Access to the CQ should be
serialized across all calls when fi_control is invoked, as it may
redirect the implementation of CQ operations.  The following control
commands are usable with a CQ.

*FI_GETWAIT (void \*\*)*
: This command allows the user to retrieve the low-level wait object
  associated with the CQ.  The format of the wait-object is specified
  during CQ creation, through the CQ attributes.  The fi_control arg
  parameter should be an address where a pointer to the returned wait
  object will be written.  See fi_eq.3 for addition details using
  fi_control with FI_GETWAIT.

## fi_cq_read

The fi_cq_read operation performs a non-blocking
read of completion data from the CQ.  The format of the completion
event is determined using the fi_cq_format option that was specified
when the CQ was opened.  Multiple completions may be retrieved from a
CQ in a single call.  The maximum number of entries to return is
limited to the specified count parameter, with the number of entries
successfully read from the CQ returned by the call.  (See return
values section below.)

CQs are optimized to report operations which have completed
successfully.  Operations which fail are reported 'out of band'.  Such
operations are retrieved using the fi_cq_readerr function.  When an
operation that has completed with an unexpected error is encountered,
it is placed into a temporary error queue.  Attempting to read
from a CQ while an item is in the error queue results in fi_cq_read
failing with a return code of -FI_EAVAIL.  Applications may use this
return code to determine when to call fi_cq_readerr.

## fi_cq_readfrom

The fi_cq_readfrom call behaves identical to fi_cq_read, with the
exception that it allows the CQ to return source address
information to the user for any received data.  Source address data is
only available for those endpoints configured with FI_SOURCE
capability.  If fi_cq_readfrom is called on an endpoint for which
source addressing data is not available, the source address will be
set to FI_ADDR_NOTAVAIL.  The number of input src_addr entries must
be the same as the count parameter.

Returned source addressing data is converted from the native address
used by the underlying fabric into an fi_addr_t, which may be used in
transmit operations.  Under most circumstances, returning fi_addr_t
requires that the source address already have been inserted into the
address vector associated with the receiving endpoint.  This is true for
address vectors of type FI_AV_TABLE.  In select providers when FI_AV_MAP is
used, source addresses may be converted algorithmically into a
usable fi_addr_t, even though the source address has not been inserted
into the address vector.  This is permitted by the API, as it allows
the provider to avoid address look-up as part of receive message processing.
In no case do providers insert addresses into an AV separate from an
application calling fi_av_insert or similar call.

For endpoints allocated using the FI_SOURCE_ERR capability, if the
source address cannot be converted into a valid fi_addr_t value,
fi_cq_readfrom will return -FI_EAVAIL, even if the data were received
successfully.  The completion will then be reported through fi_cq_readerr
with error code -FI_EADDRNOTAVAIL.  See fi_cq_readerr for details.

If FI_SOURCE is specified without FI_SOURCE_ERR, source addresses
which cannot be mapped to a usable fi_addr_t will be reported as
FI_ADDR_NOTAVAIL.

## fi_cq_sread / fi_cq_sreadfrom

The fi_cq_sread and fi_cq_sreadfrom calls are the blocking equivalent
operations to fi_cq_read and fi_cq_readfrom.  Their behavior is
similar to the non-blocking calls, with the exception that the calls
will not return until either a completion has been read from the CQ or
an error or timeout occurs.

Threads blocking in this function will return to the caller if
they are signaled by some external source.  This is true even if
the timeout has not occurred or was specified as infinite.

It is invalid for applications to call these functions if the CQ
has been configured with a wait object of FI_WAIT_NONE or FI_WAIT_SET.

## fi_cq_readerr

The read error function, fi_cq_readerr, retrieves information
regarding any asynchronous operation which has completed with an
unexpected error.  fi_cq_readerr is a non-blocking call, returning
immediately whether an error completion was found or not.

Error information is reported to the user through `struct
fi_cq_err_entry`.  The format of this structure is defined below.

```c
struct fi_cq_err_entry {
	void     *op_context; /* operation context */
	uint64_t flags;       /* completion flags */
	size_t   len;         /* size of received data */
	void     *buf;        /* receive data buffer */
	uint64_t data;        /* completion data */
	uint64_t tag;         /* message tag */
	size_t   olen;        /* overflow length */
	int      err;         /* positive error code */
	int      prov_errno;  /* provider error code */
	void    *err_data;    /*  error data */
	size_t   err_data_size; /* size of err_data */
};
```

The general reason for the error is provided through the err field.
Provider specific error information may also be available through the
prov_errno and err_data fields.  Users may call fi_cq_strerror to
convert provider specific error information into a printable string
for debugging purposes.  See field details below for more information
on the use of err_data and err_data_size.

Note that error completions are generated for all operations, including
those for which a completion was not requested (e.g. an endpoint
is configured with FI_SELECTIVE_COMPLETION, but the request did not have
the FI_COMPLETION flag set).  In such cases, providers will return as
much information as made available by the underlying software and
hardware about the failure, other fields will be set to NULL or 0.  This
includes the op_context value, which may not have been provided or was
ignored on input as part of the transfer.

Notable completion error codes are given below.

*FI_EADDRNOTAVAIL*
: This error code is used by CQs configured with FI_SOURCE_ERR to report
  completions for which a usable fi_addr_t source address could not
  be found.  An error code of FI_EADDRNOTAVAIL indicates that the data
  transfer was successfully received and processed, with the
  fi_cq_err_entry fields containing information about the completion.
  The err_data field will be set to the source address data.  The
  source address will be in the same format as specified through
  the fi_info addr_format field for the opened domain. This may be
  passed directly into an fi_av_insert call to add the source address
  to the address vector.

## fi_cq_signal

The fi_cq_signal call will unblock any thread waiting in fi_cq_sread
or fi_cq_sreadfrom.  This may be used to wake-up a thread
that is blocked waiting to read a completion operation.  The fi_cq_signal
operation is only available if the CQ was configured with a wait object.

# COMPLETION FIELDS

The CQ entry data structures share many of the same fields.  The meanings
of these fields are the same for all CQ entry structure formats.

*op_context*
: The operation context is the application specified context value that
  was provided with an asynchronous operation.  The op_context field is
  valid for all completions that are associated with an asynchronous
  operation.

  For completion events that are not associated with a posted operation,
  this field will be set to NULL.  This includes completions generated
  at the target in response to RMA write operations that carry CQ data
  (FI_REMOTE_WRITE | FI_REMOTE_CQ_DATA flags set), when the FI_RX_CQ_DATA
  mode bit is not required.

*flags*
: This specifies flags associated with the completed operation.  The
  _Completion Flags_ section below lists valid flag values.  Flags are
  set for all relevant completions.

*len*
: This len field only applies to completed receive operations (e.g. fi_recv,
  fi_trecv, etc.).  It indicates the size of received _message_ data --
  i.e. how many data bytes were placed into the associated receive buffer by
  a corresponding fi_send/fi_tsend/et al call.  If an endpoint has
  been configured with the FI_MSG_PREFIX mode, the len also reflects the size
  of the prefix buffer.

*buf*
: The buf field is only valid for completed receive operations, and only
  applies when the receive buffer was posted with the FI_MULTI_RECV flag.
  In this case, buf points to the starting location where the receive
  data was placed.

*data*
: The data field is only valid if the FI_REMOTE_CQ_DATA completion flag
  is set, and only applies to receive completions.  If FI_REMOTE_CQ_DATA
  is set, this field will contain the completion data provided by the peer
  as part of their transmit request.  The completion data will be given
  in host byte order.

*tag*
: A tag applies only to received messages that occur using the tagged
  interfaces.  This field contains the tag that was included with the
  received message.  The tag will be in host byte order.

*olen*
: The olen field applies to received messages.  It is used to indicate
  that a received message has overrun the available buffer space and
  has been truncated.  The olen specifies the amount of data that did
  not fit into the available receive buffer and was discarded.

*err*
: This err code is a positive fabric errno associated with a completion.
  The err value indicates the general reason for an error, if one occurred.
  See fi_errno.3 for a list of possible error codes.

*prov_errno*
: On an error, prov_errno may contain a provider specific error code.  The
  use of this field and its meaning is provider specific.  It is intended
  to be used as a debugging aid.  See fi_cq_strerror for additional details
  on converting this error value into a human readable string.

*err_data*
: The err_data field is used to return provider specific information, if
  available, about the error.  On input, err_data should reference a data
  buffer of size err_data_size.  On output, the provider will fill in this
  buffer with any provider specific data which may help identify the cause
  of the error.  The contents of the err_data field and its meaning is
  provider specific.  It is intended to be used as a debugging aid.  See
  fi_cq_strerror for additional details on converting this error data into
  a human readable string.  See the compatibility note below on how this
  field is used for older libfabric releases.

*err_data_size*
: On input, err_data_size indicates the size of the err_data buffer in bytes.
  On output, err_data_size will be set to the number of bytes copied to the
  err_data buffer.  The err_data information is typically used with
  fi_cq_strerror to provide details about the type of error that occurred.

  For compatibility purposes, the behavior of the err_data and err_data_size
  fields is may be modified from that listed above.  If err_data_size is 0
  on input, or the fabric was opened with release < 1.5, then any buffer
  referenced by err_data will be ignored on input.  In this situation, on
  output err_data will be set to a data buffer owned by the provider.
  The contents of the buffer will remain valid until a
  subsequent read call against the CQ.  Applications must serialize access
  to the CQ when processing errors to ensure that the buffer referenced by
  err_data does not change.

# COMPLETION FLAGS

Completion flags provide additional details regarding the completed
operation.  The following completion flags are defined.

*FI_SEND*
: Indicates that the completion was for a send operation.  This flag
  may be combined with an FI_MSG or FI_TAGGED flag.

*FI_RECV*
: Indicates that the completion was for a receive operation.  This flag
  may be combined with an FI_MSG or FI_TAGGED flag.

*FI_RMA*
: Indicates that an RMA operation completed.  This flag may be combined
  with an FI_READ, FI_WRITE, FI_REMOTE_READ, or FI_REMOTE_WRITE flag.

*FI_ATOMIC*
: Indicates that an atomic operation completed.  This flag may be combined
  with an FI_READ, FI_WRITE, FI_REMOTE_READ, or FI_REMOTE_WRITE flag.

*FI_MSG*
: Indicates that a message-based operation completed.  This flag may be
  combined with an FI_SEND or FI_RECV flag.

*FI_TAGGED*
: Indicates that a tagged message operation completed.  This flag may be
  combined with an FI_SEND or FI_RECV flag.

*FI_MULTICAST*
: Indicates that a multicast operation completed.  This flag may be combined
  with FI_MSG and relevant flags.  This flag is only guaranteed to be valid
  for received messages if the endpoint has been configured with FI_SOURCE.

*FI_READ*
: Indicates that a locally initiated RMA or atomic read operation has
  completed.  This flag may be combined with an FI_RMA or FI_ATOMIC flag.

*FI_WRITE*
: Indicates that a locally initiated RMA or atomic write operation has
  completed.  This flag may be combined with an FI_RMA or FI_ATOMIC flag.

*FI_REMOTE_READ*
: Indicates that a remotely initiated RMA or atomic read operation has
  completed.  This flag may be combined with an FI_RMA or FI_ATOMIC flag.

*FI_REMOTE_WRITE*
: Indicates that a remotely initiated RMA or atomic write operation has
  completed.  This flag may be combined with an FI_RMA or FI_ATOMIC flag.

*FI_REMOTE_CQ_DATA*
: This indicates that remote CQ data is available as part of the
  completion.

*FI_MULTI_RECV*
: This flag applies to receive buffers that were posted with the
  FI_MULTI_RECV flag set.  This completion flag indicates that the
  original receive buffer referenced by the completion has been
  consumed and was released by the provider.  Providers may set
  this flag on the last message that is received into the multi-
  recv buffer, or may generate a separate completion that indicates
  that the buffer has been released.

  Applications can distinguish between these two cases by examining
  the completion entry flags field.  If additional flags, such as
  FI_RECV, are set, the completion is associated with a received message.  In
  this case, the buf field will reference the location where the received
  message was placed into the multi-recv buffer.  Other fields in the
  completion entry will be determined based on the received message.
  If other flag bits are zero, the provider is reporting that the multi-recv
  buffer has been released, and the completion entry is not associated
  with a received message.

*FI_MORE*
: See the 'Buffered Receives' section in `fi_msg`(3) for more details.
  This flag is associated with receive completions on endpoints that
  have FI_BUFFERED_RECV mode enabled.  When set to one, it indicates that
  the buffer referenced by the completion is limited by the
  FI_OPT_BUFFERED_LIMIT threshold, and additional message data must be
  retrieved by the application using an FI_CLAIM operation.

*FI_CLAIM*
: See the 'Buffered Receives' section in `fi_msg`(3) for more details.
  This flag is set on completions associated with receive operations
  that claim buffered receive data.  Note that this flag only applies
  to endpoints configured with the FI_BUFFERED_RECV mode bit.

# COMPLETION EVENT SEMANTICS

Libfabric defines several completion 'levels', identified using operational
flags.  Each flag indicates the soonest that a completion event may be
generated by a provider, and the assumptions that an application may make
upon processing a completion.  The operational flags are defined below,
along with an example of how a provider might implement the semantic.  Note
that only meeting the semantic is required of the provider and not the
implementation.  Providers may implement stronger completion semantics
than necessary for a given operation, but only the behavior defined by the
completion level is guaranteed.

To help understand the conceptual differences in completion levels, consider
mailing a letter.  Placing the letter into the local mailbox for pick-up is
similar to 'inject complete'.  Having the letter picked up and dropped off
at the destination mailbox is equivalent to 'transmit complete'.  The
'delivery complete' semantic is a stronger guarantee, with a person at the
destination signing for the letter.  However, the person who signed for the
letter is not necessarily the intended recipient.  The 'match complete'
option is similar to delivery complete, but requires the intended recipient
to sign for the letter.

The 'commit complete' level has different semantics than the previously
mentioned levels.  Commit complete would be closer to the letter
arriving at the destination and being placed into a fire proof safe.

The operational flags for the described completion levels are defined below.

*FI_INJECT_COMPLETE*
: Indicates that a completion should be generated when the
  source buffer(s) may be reused.  A completion guarantees that
  the buffers will not be read from again and the application may
  reclaim them.  No other guarantees are made with respect to the
  state of the operation.

  Example: A provider may generate this completion event after copying
  the source buffer into a network buffer, either in host memory or
  on the NIC.  An inject completion does not indicate that the data has
  been transmitted onto the network, and a local error could occur after
  the completion event has been generated that could prevent it from being
  transmitted.

  Inject complete allows for the fastest completion reporting (and, hence,
  buffer reuse), but provides the weakest guarantees against network errors.

  Note: This flag is used to control when a completion entry is inserted
  into a completion queue.  It does not apply to operations that do not
  generate a completion queue entry, such as the fi_inject operation, and
  is not subject to the inject_size message limit restriction.

*FI_TRANSMIT_COMPLETE*
: Indicates that a completion should be generated when the transmit
  operation has completed relative to the local provider.  The exact
  behavior is dependent on the endpoint type.

  For reliable endpoints:

  Indicates that a completion should be generated when the operation has
  been delivered to the peer endpoint.  A completion guarantees that the
  operation is no longer dependent on the fabric or local resources.  The
  state of the operation at the peer endpoint is not defined.

  Example: A provider may generate a transmit complete event upon receiving
  an ack from the peer endpoint.  The state of the message at the peer is
  unknown and may be buffered in the target NIC at the time the ack has been
  generated.

  For unreliable endpoints:

  Indicates that a completion should be generated when the operation has
  been delivered to the fabric.  A completion guarantees that the
  operation is no longer dependent on local resources.  The state of the
  operation within the fabric is not defined.

*FI_DELIVERY_COMPLETE*
: Indicates that a completion should not be generated until an operation
  has been processed by the destination endpoint(s).  A completion
  guarantees that the result of the operation is available; however,
  additional steps may need to be taken at the destination to retrieve the
  results.  For example, an application may need to provide a receive buffers
  in order to retrieve messages that were buffered by the provider.

  Delivery complete indicates that the message has been processed by the peer.
  If an application buffer was ready to receive the results of the message
  when it arrived, then delivery complete indicates that the data was placed
  into the application's buffer.

  This completion mode applies only to reliable endpoints.  For operations
  that return data to the initiator, such as RMA read or atomic-fetch,
  the source endpoint is also considered a destination endpoint.  This is the
  default completion mode for such operations.

*FI_MATCH_COMPLETE*
: Indicates that a completion should be generated only after the operation
  has been matched with an application specified buffer.  Operations using
  this completion semantic are dependent on the application at the target
  claiming the message or results.  As a result, match complete may involve
  additional provider level acknowledgements or lengthy delays.  However, this
  completion model enables peer applications to synchronize their execution.
  Many providers may not support this semantic.

*FI_COMMIT_COMPLETE*
: Indicates that a completion should not be generated (locally or at the
  peer) until the result of an operation have been made persistent.
  A completion guarantees that the result is both available and durable,
  in the case of power failure.

  This completion mode applies only to operations that target persistent
  memory regions over reliable endpoints.  This completion mode is
  experimental.

*FI_FENCE*
: This is not a completion level, but plays a role in the completion
  ordering between operations that would not normally be ordered.  An
  operation that is marked with the FI_FENCE flag and all
  operations posted after the fenced operation are deferred until all
  previous operations targeting the same peer endpoint have completed.
  Additionally, the completion of the fenced operation indicates that
  prior operations have met the same completion level as the fenced
  operation.  For example, if an operation is posted as
  FI_DELIVERY_COMPLETE | FI_FENCE, then its completion indicates prior
  operations have met the semantic required for FI_DELIVERY_COMPLETE.
  This is true even if the prior operation was posted with a lower
  completion level, such as FI_TRANSMIT_COMPLETE or FI_INJECT_COMPLETE.

  Note that a completion generated for an operation posted prior to
  the fenced operation only guarantees that the completion level
  that was originally requested has been met.  It is the completion
  of the fenced operation that guarantees that the additional
  semantics have been met.

The above completion semantics are defined with respect to the initiator
of the operation.  The different semantics are useful for describing
when the initiator may re-use a data buffer, and guarantees what state
a transfer must reach prior to a completion being generated.  This
allows applications to determine appropriate error handling in case
of communication failures.

# TARGET COMPLETION SEMANTICS

The completion semantic at the target is used to determine when data
at the target is visible to the peer application.  Visibility
indicates that a memory read to the same address that was
the target of a data transfer will return the results of the transfer.
The target of a transfer can be identified by the initiator,
as may be the case for RMA and atomic operations, or determined by
the target, for example by providing a matching receive buffer.
Global visibility indicates that the results are available regardless
of where the memory read originates.  For example, the read could come
from a process running on a host CPU, it may be accessed by subsequent
data transfer over the fabric, or read from a peer device such as a GPU.

In terms of completion semantics, visibility usually indicates that the
transfer meets the FI_DELIVERY_COMPLETE requirements from the
perspective of the target.  The target completion semantic may be, but
is not necessarily, linked with the completion semantic specified by the
initiator of the transfer.

Often, target processes do not explicitly state a desired completion
semantic and instead rely on the default semantic.  The
default behavior is based on several factors, including:

- whether a completion even is generated at the target
- the type of transfer involved (e.g. msg vs RMA)
- endpoint data and message ordering guarantees
- properties of the targeted memory buffer
- the initiator's specified completion semantic

Broadly, target completion semantics are grouped
based on whether or not the transfer generates a completion event
at the target.  This includes writing a CQ entry or updating a completion
counter.  In common use cases, transfers that use a message
interface (FI_MSG or FI_TAGGED) typically generate target events, while
transfers involving an RMA interface (FI_RMA or FI_ATOMIC) often do not.
There are exceptions to both these cases, depending on endpoint to CQ
and counter bindings and operational flags.  For example, RMA writes that
carry remote CQ data will generate a completion event at the target,
and are frequently used to convey visibility to the target application.
The general guidelines for target side semantics are described below,
followed by exceptions that modify that behavior.

By default, completions generated at the target indicate that the
transferred data is immediately available to be read from the target buffer.
That is, the target sees FI_DELIVERY_COMPLETE (or better) semantics,
even if the initiator requested lower semantics.
For applications using only data buffers allocated from
host memory, this is often sufficient.

For operations that do not generate a completion event at the target,
the visibility of the data at the target may need to be inferred
based on subsequent operations that do generate target completions.
Absent a target completion, when a completion of an
operation is written at the initiator, the visibility semantic
of the operation at the target aligns with the initiator completion
semantic.  For instance, if an RMA operation completes at the initiator
as either FI_INJECT_COMPLETE or FI_TRANSMIT_COMPLETE, the data visibility
at the target is not guaranteed.

One or more of the following mechanisms can be used by the target process to
guarantee that the results of a data transfer that did not generate a
completion at the target is now visible.  This list is not inclusive of
all options, but defines common uses.  In the descriptions below, the first
transfer does not result in a completion event at the target, but is
eventually followed by a transfer which does.

- If the endpoint guarantees message ordering between two transfers, the
  target completion of a second transfer will indicate that the data from
  the first transfer is available.  For example, if the endpoint supports
  send after write ordering (FI_ORDER_SAW), then a receive completion
  corresponding to the send will indicate that the write data is available.
  This holds independent of the initiator's completion semantic for either
  the write or send.  When ordering is guaranteed, the second transfer
  can be queued with the provider immediately after queuing the first.

- If the endpoint does not guarantee message ordering, the initiator must take
  additional steps to ensure visibility.  If initiator requests
  FI_DELIVERY_COMPLETE semantics for the first operation, the initiator can wait
  for the operation to complete locally.  Once the completion has been
  read, the target completion of a second transfer will indicate that the
  first transfer's data is visible.

- Alternatively, if message ordering is not guaranteed by the endpoint, the
  initiator can use the FI_FENCE and FI_DELIVERY_COMPLETE flags on the second
  data transfer to force the first transfers to meet the
  FI_DELIVERY_COMPLETE semantics.  If the second transfer generates a
  completion at the target, that will indicate that the data is visible.
  Otherwise, a target completion for any transfer after the
  fenced operation will indicate that the data is visible.

The above semantics apply for transfers targeting traditional host memory
buffers.  However, the behavior may differ when device memory and/or
persistent memory is involved (FI_HMEM and FI_PMEM capability bits).  When
heterogenous memory is involved, the concept of memory domains come into
play.  Memory domains identify the physical separation of memory, which
may or may not be accessible through the same virtual address space.  See
the [`fi_mr`(3)](fi_mr.3.html) man page for further details on memory domains.

Completion ordering and data visibility are only well-defined for transfers
that target the same memory domain.  Applications need to be aware of
ordering and visibility differences when transfers target different memory
domains.  Additionally, applications also need to be concerned with the
memory domain that completions themselves are written and if it differs
from the memory domain targeted by a transfer.  In some situations,
either the provider or application may need to call device specific APIs
to synchronize or flush device memory caches in order to achieve the
desired data visibility.

When heterogenous memory is in use, the default target completion semantic
for transfers that generate a completion at the target is still
FI_DELIVERY_COMPLETE, however, applications should be aware that there
may be a negative impact on overall performance for providers to meet
this requirement.

For example, a target process may be using a GPU to accelerate computations.
A memory region mapping to memory on the GPU may be exposed to peers as
either an RMA target or posted locally as a receive buffer.  In this case,
the application is concerned with two memory domains -- system and GPU
memory.  Completions are written to system memory.

Continuing the example, a peer process sends a tagged message.  That message
is matched with the receive buffer located in GPU memory.  The NIC copies
the data from the network into the receive buffer and writes an entry into
the completion queue.  Note that both memory domains were accessed as part
of this transfer.  The message data was directed to the GPU memory, but the
completion went to host memory.   Because separate memory domains may not be
synchronized with each other, it is possible for the host CPU to see and process
the completion entry before the transfer to the GPU memory is visible to either
the host GPU or even software running on the GPU.  From the perspective
of the *provider*, visibility of the completion does not imply visibility of
data written to the GPU's memory domain.

The default completion semantic at the target *application* for message
operations is FI_DELIVERY_COMPLETE.  An anticipated provider implementation
in this situation is for the provider software running on the host CPU to
intercept the CQ entry, detect that the data landed in heterogenous memory,
and perform the necessary device synchronization or flush operation
before reporting the completion up to the application.  This ensures that
the data is visible to CPU _and_ GPU software prior to the application
processing the completion.

In addition to the cost of provider software intercepting completions
and checking if a transfer targeted heterogenous memory, device
synchronization itself may impact performance.  As a result, applications
can request a lower completion semantic when posting receives.  That
indicates to the provider that the application will be responsible for
handling any device specific flush operations that might be needed.
See [`fi_msg`(3)](fi_msg.3.html) FLAGS.

For data transfers that do not generate a completion at the target,
such as RMA or atomics, it is the responsibility of the application
to ensure that all target buffers meet the necessary visibility
requirements of the application.  The previously mentioned bulleted
methods for notifying the target that the data is visible may not
be sufficient, as the provider software at the target could lack
the context needed to ensure visibility.  This implies that the
application may need to call device synchronization/flush APIs
directly.

For example, a peer application could perform several RMA writes
that target GPU memory buffers.  If the provider offloads RMA
operations into the NIC, the provider software at the target will
be unaware that the RMA operations have occurred.  If the peer
sends a message to the target application that indicates that the
RMA operations are done, the application must ensure that the RMA data
is visible to the host CPU or GPU prior to executing code that accesses
the data.  The target completion of having received the sent message
is not sufficient, even if send-after-write ordering is supported.

Most target heterogenous memory completion semantics map to
FI_TRANSMIT_COMPLETE or FI_DELIVERY_COMPLETE.  Persistent memory
(FI_PMEM capability), however, is often used with FI_COMMIT_COMPLETE
semantics.  Heterogenous completion concepts still apply.

For transfers flagged by the initiator with FI_COMMIT_COMPLETE,
a completion at the target indicates that the results are visible
and durable.  For transfers targeting persistent memory, but using
a different completion semantic at the initiator, the visibility
at the target is similar to that described above.  Durability is
only associated with transfers marked with FI_COMMIT_COMPLETE.

For transfers targeting persistent memory that request
FI_DELIVERY_COMPLETE, then a completion, at either the initiator or
target, indicates that the data is visible.  Visibility at the
target can be conveyed using one of the above describe mechanism --
generating a target completion, sending a message from the initiator,
etc.  Similarly, if the initiator requested FI_TRANSMIT_COMPLETE,
then additional steps are needed to ensure visibility at the target.
For example, the transfer can generate a completion at the target,
which would indicate visibility, but not durability.  The initiator
can also follow the transfer with another operation that forces
visibility, such as using FI_FENCE in conjunction with
FI_DELIVERY_COMPLETE.

# NOTES

A completion queue must be bound to at least one enabled endpoint before any
operation such as fi_cq_read, fi_cq_readfrom, fi_cq_sread, fi_cq_sreadfrom etc.
can be called on it.

Completion flags may be suppressed if the FI_NOTIFY_FLAGS_ONLY mode bit
has been set.  When enabled, only the following flags are guaranteed to
be set in completion data when they are valid: FI_REMOTE_READ and
FI_REMOTE_WRITE (when FI_RMA_EVENT capability bit has been set),
FI_REMOTE_CQ_DATA, and FI_MULTI_RECV.

If a completion queue has been overrun, it will be placed into an 'overrun'
state.  Read operations will continue to return any valid, non-corrupted
completions, if available.  After all valid completions have been retrieved,
any attempt to read the CQ will result in it returning an FI_EOVERRUN error
event.  Overrun completion queues are considered fatal and may not be used
to report additional completions once the overrun occurs.

# RETURN VALUES

fi_cq_open / fi_cq_signal
: Returns 0 on success.  On error, a negative value corresponding to
  fabric errno is returned.

fi_cq_read / fi_cq_readfrom / fi_cq_readerr
fi_cq_sread / fi_cq_sreadfrom
: On success, returns the number of completion events retrieved from
  the completion queue.  On error, a negative value corresponding to
  fabric errno is returned.  If no completions are available to
  return from the CQ, -FI_EAGAIN will be returned.

fi_cq_sread / fi_cq_sreadfrom
: On success, returns the number of completion events retrieved from
  the completion queue.  On error, a negative value corresponding to
  fabric errno is returned.  If the timeout expires or the calling
  thread is signaled and no data is available to be read from the
  completion queue, -FI_EAGAIN is returned.

fi_cq_strerror
: Returns a character string interpretation of the provider specific
  error returned with a completion.

Fabric errno values are defined in
`rdma/fi_errno.h`.

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_eq`(3)](fi_eq.3.html),
[`fi_cntr`(3)](fi_cntr.3.html),
[`fi_poll`(3)](fi_poll.3.html)
