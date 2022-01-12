---
layout: page
title: fi_eq(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_eq \- Event queue operations

fi_eq_open / fi_close
: Open/close an event queue

fi_control
: Control operation of EQ

fi_eq_read / fi_eq_readerr
: Read an event from an event queue

fi_eq_write
: Writes an event to an event queue

fi_eq_sread
: A synchronous (blocking) read of an event queue

fi_eq_strerror
: Converts provider specific error information into a printable string

# SYNOPSIS

```c
#include <rdma/fi_domain.h>

int fi_eq_open(struct fid_fabric *fabric, struct fi_eq_attr *attr,
    struct fid_eq **eq, void *context);

int fi_close(struct fid *eq);

int fi_control(struct fid *eq, int command, void *arg);

ssize_t fi_eq_read(struct fid_eq *eq, uint32_t *event,
    void *buf, size_t len, uint64_t flags);

ssize_t fi_eq_readerr(struct fid_eq *eq, struct fi_eq_err_entry *buf,
    uint64_t flags);

ssize_t fi_eq_write(struct fid_eq *eq, uint32_t event,
    const void *buf, size_t len, uint64_t flags);

ssize_t fi_eq_sread(struct fid_eq *eq, uint32_t *event,
    void *buf, size_t len, int timeout, uint64_t flags);

const char * fi_eq_strerror(struct fid_eq *eq, int prov_errno,
      const void *err_data, char *buf, size_t len);
```

# ARGUMENTS

*fabric*
: Opened fabric descriptor

*eq*
: Event queue

*attr*
: Event queue attributes

*context*
: User specified context associated with the event queue.

*event*
: Reported event

*buf*
: For read calls, the data buffer to write events into.  For write
  calls, an event to insert into the event queue.  For fi_eq_strerror,
  an optional buffer that receives printable error information.

*len*
: Length of data buffer

*flags*
: Additional flags to apply to the operation

*command*
: Command of control operation to perform on EQ.

*arg*
: Optional control argument

*prov_errno*
: Provider specific error value

*err_data*
: Provider specific error data related to a completion

*timeout*
: Timeout specified in milliseconds

# DESCRIPTION

Event queues are used to report events associated with control
operations.  They are associated with memory registration, address
vectors, connection management, and fabric and domain level events.
Reported events are either associated with a requested operation or
affiliated with a call that registers for specific types of events,
such as listening for connection requests.

## fi_eq_open

fi_eq_open allocates a new event queue.

The properties and behavior of an event queue are defined by `struct
fi_eq_attr`.

```c
struct fi_eq_attr {
	size_t               size;      /* # entries for EQ */
	uint64_t             flags;     /* operation flags */
	enum fi_wait_obj     wait_obj;  /* requested wait object */
	int                  signaling_vector; /* interrupt affinity */
	struct fid_wait     *wait_set;  /* optional wait set */
};
```

*size*
: Specifies the minimum size of an event queue.

*flags*
: Flags that control the configuration of the EQ.

- *FI_WRITE*
: Indicates that the application requires support for inserting user
  events into the EQ.  If this flag is set, then the fi_eq_write
  operation must be supported by the provider.  If the FI_WRITE flag
  is not set, then the application may not invoke fi_eq_write.

- *FI_AFFINITY*
: Indicates that the signaling_vector field (see below) is valid.

*wait_obj*
: EQ's may be associated with a specific wait object.  Wait objects
  allow applications to block until the wait object is signaled,
  indicating that an event is available to be read.  Users may use
  fi_control to retrieve the underlying wait object associated with an
  EQ, in order to use it in other system calls.  The following values
  may be used to specify the type of wait object associated with an
  EQ:

- *FI_WAIT_NONE*
: Used to indicate that the user will not block (wait) for events on
  the EQ.  When FI_WAIT_NONE is specified, the application may not
  call fi_eq_sread.  This is the default is no wait object is specified.

- *FI_WAIT_UNSPEC*
: Specifies that the user will only wait on the EQ using fabric
  interface calls, such as fi_eq_sread.  In this case, the underlying
  provider may select the most appropriate or highest performing wait
  object available, including custom wait mechanisms.  Applications
  that select FI_WAIT_UNSPEC are not guaranteed to retrieve the
  underlying wait object.

- *FI_WAIT_SET*
: Indicates that the event queue should use a wait set object to wait
  for events.  If specified, the wait_set field must reference an
  existing wait set object.

- *FI_WAIT_FD*
: Indicates that the EQ should use a file descriptor as its wait
  mechanism.  A file descriptor wait object must be usable in select,
  poll, and epoll routines.  However, a provider may signal an FD wait
  object by marking it as readable or with an error.

- *FI_WAIT_MUTEX_COND*
: Specifies that the EQ should use a pthread mutex and cond variable
  as a wait object.

- *FI_WAIT_YIELD*
: Indicates that the EQ will wait without a wait object but instead
  yield on every wait. Allows usage of fi_eq_sread through a spin.

*signaling_vector*
: If the FI_AFFINITY flag is set, this indicates the logical cpu number
  (0..max cpu - 1) that interrupts associated with the EQ should target.
  This field should be treated as a hint to the provider and may be
  ignored if the provider does not support interrupt affinity.

*wait_set*
: If wait_obj is FI_WAIT_SET, this field references a wait object to
  which the event queue should attach.  When an event is inserted into
  the event queue, the corresponding wait set will be signaled if all
  necessary conditions are met.  The use of a wait_set enables an
  optimized method of waiting for events across multiple event queues.
  This field is ignored if wait_obj is not FI_WAIT_SET.

## fi_close

The fi_close call releases all resources associated with an event queue.  Any
events which remain on the EQ when it is closed are lost.

The EQ must not be bound to any other objects prior to being closed, otherwise
the call will return -FI_EBUSY.

## fi_control

The fi_control call is used to access provider or implementation
specific details of the event queue.  Access to the EQ should be
serialized across all calls when fi_control is invoked, as it may
redirect the implementation of EQ operations.  The following control
commands are usable with an EQ.

*FI_GETWAIT (void \*\*)*
: This command allows the user to retrieve the low-level wait object
  associated with the EQ.  The format of the wait-object is specified
  during EQ creation, through the EQ attributes.  The fi_control arg
  parameter should be an address where a pointer to the returned wait
  object will be written.  This should be an 'int *' for FI_WAIT_FD,
  or 'struct fi_mutex_cond' for FI_WAIT_MUTEX_COND.
  
```c
struct fi_mutex_cond {
	pthread_mutex_t     *mutex;
	pthread_cond_t      *cond;
};
```

## fi_eq_read

The fi_eq_read operations performs a non-blocking read of event data
from the EQ.  The format of the event data is based on the type of
event retrieved from the EQ, with all events starting with a struct
fi_eq_entry header.  At most one event will be returned per EQ read
operation.  The number of bytes successfully read from the EQ is
returned from the read.  The FI_PEEK flag may be used to indicate that
event data should be read from the EQ without being consumed.  A
subsequent read without the FI_PEEK flag would then remove the event
from the EQ.

The following types of events may be reported to an EQ, along with
information regarding the format associated with each event.

*Asynchronous Control Operations*
: Asynchronous control operations are basic requests that simply need
  to generate an event to indicate that they have completed.  These
  include the following types of events: memory registration, address
  vector resolution, and multicast joins.

  Control requests report their completion by inserting a `struct
  fi_eq_entry` into the EQ.  The format of this structure is:

```c
struct fi_eq_entry {
	fid_t            fid;        /* fid associated with request */
	void            *context;    /* operation context */
	uint64_t         data;       /* completion-specific data */
};
```

  For the completion of basic asynchronous control operations, the
  returned event will indicate the operation that has completed, and
  the fid will reference the fabric descriptor associated with
  the event.  For memory registration, this will be an FI_MR_COMPLETE
  event and the fid_mr.  Address resolution will reference an
  FI_AV_COMPLETE event and fid_av.  Multicast joins will report an
  FI_JOIN_COMPLETE and fid_mc.  The context field will be set
  to the context specified as part of the operation, if available,
  otherwise the context will be associated with the fabric descriptor.
  The data field will be set as described in the man page for the
  corresponding object type (e.g., see [`fi_av`(3)](fi_av.3.html) for
  a description of how asynchronous address vector insertions are
  completed).

*Connection Notification*
: Connection notifications are connection management notifications
  used to setup or tear down connections between endpoints.  There are
  three connection notification events: FI_CONNREQ, FI_CONNECTED, and
  FI_SHUTDOWN.  Connection notifications are reported using `struct
  fi_eq_cm_entry`:

```c
struct fi_eq_cm_entry {
	fid_t            fid;        /* fid associated with request */
	struct fi_info  *info;       /* endpoint information */
	uint8_t         data[];     /* app connection data */
};
```

  A connection request (FI_CONNREQ) event indicates that
  a remote endpoint wishes to establish a new connection to a listening,
  or passive, endpoint.  The fid is the passive endpoint.
  Information regarding the requested, active endpoint's
  capabilities and attributes are available from the info field.  The
  application is responsible for freeing this structure by calling
  fi_freeinfo when it is no longer needed.  The fi_info connreq field
  will reference the connection request associated with this event.
  To accept a connection, an endpoint must first be created by passing
  an fi_info structure referencing this connreq field to fi_endpoint().
  This endpoint is then passed to fi_accept() to complete the acceptance
  of the connection attempt.
  Creating the endpoint is most easily accomplished by
  passing the fi_info returned as part of the CM event into
  fi_endpoint().  If the connection is to be rejected, the connreq is
  passed to fi_reject().

  Any application data exchanged as part of the connection request is
  placed beyond the fi_eq_cm_entry structure.  The amount of data
  available is application dependent and limited to the buffer space
  provided by the application when fi_eq_read is called.  The amount
  of returned data may be calculated using the return value to
  fi_eq_read.  Note that the amount of returned data is limited by the
  underlying connection protocol, and the length of any data returned
  may include protocol padding.  As a result, the returned length may
  be larger than that specified by the connecting peer.

  If a connection request has been accepted, an FI_CONNECTED event will
  be generated on both sides of the connection.  The active side -- one
  that called fi_connect() -- may receive user data as part of the
  FI_CONNECTED event.  The user data is passed to the connection
  manager on the passive side through the fi_accept call.  User data is
  not provided with an FI_CONNECTED event on the listening side of the
  connection.

  Notification that a remote peer has disconnected from an active
  endpoint is done through the FI_SHUTDOWN event.  Shutdown
  notification uses struct fi_eq_cm_entry as declared above.  The fid
  field for a shutdown notification refers to the active endpoint's
  fid_ep.

*Asynchronous Error Notification*
: Asynchronous errors are used to report problems with fabric resources.
  Reported errors may be fatal or transient, based on the error, and
  result in the resource becoming disabled.  Disabled resources will fail
  operations submitted against them until they are explicitly re-enabled
  by the application.

  Asynchronous errors may be reported for completion queues and endpoints
  of all types.  CQ errors can result when resource management has been
  disabled, and the provider has detected a queue overrun.  Endpoint
  errors may be result of numerous actions, but are often associated with
  a failed operation.  Operations may fail because of buffer overruns,
  invalid permissions, incorrect memory access keys, network routing
  failures, network reach-ability issues, etc.

  Asynchronous errors are reported using struct fi_eq_err_entry, as defined
  below.  The fabric descriptor (fid) associated with the error is provided
  as part of the error data.  An error code is also available to determine
  the cause of the error.

## fi_eq_sread

The fi_eq_sread call is the blocking (or synchronous) equivalent to
fi_eq_read.  It behaves is similar to the non-blocking call, with the
exception that the calls will not return until either an event has
been read from the EQ or an error or timeout occurs.  Specifying a
negative timeout means an infinite timeout.

Threads blocking in this function will return to the caller if
they are signaled by some external source.  This is true even if
the timeout has not occurred or was specified as infinite.

It is invalid for applications to call this function if the EQ
has been configured with a wait object of FI_WAIT_NONE or FI_WAIT_SET.

## fi_eq_readerr

The read error function, fi_eq_readerr, retrieves information
regarding any asynchronous operation which has completed with an
unexpected error.  fi_eq_readerr is a non-blocking call, returning
immediately whether an error completion was found or not.

EQs are optimized to report operations which have completed
successfully.  Operations which fail are reported 'out of band'.  Such
operations are retrieved using the fi_eq_readerr function.  When an
operation that completes with an unexpected error is inserted into an
EQ, it is placed into a temporary error queue.  Attempting to read
from an EQ while an item is in the error queue results in an FI_EAVAIL
failure.  Applications may use this return code to determine when to
call fi_eq_readerr.

Error information is reported to the user through struct
fi_eq_err_entry.  The format of this structure is defined below.

```c
struct fi_eq_err_entry {
	fid_t            fid;        /* fid associated with error */
	void            *context;    /* operation context */
	uint64_t         data;       /* completion-specific data */
	int              err;        /* positive error code */
	int              prov_errno; /* provider error code */
	void            *err_data;   /* additional error data */
	size_t           err_data_size; /* size of err_data */
};
```

The fid will reference the fabric descriptor associated with the
event.  For memory registration, this will be the fid_mr, address
resolution will reference a fid_av, and CM events will refer to a
fid_ep.  The context field will be set to the context specified as
part of the operation.

The data field will be set as described in the man page for the
corresponding object type (e.g., see [`fi_av`(3)](fi_av.3.html) for a
description of how asynchronous address vector insertions are
completed).

The general reason for the error is provided through the err field.
Provider or operational specific error information may also be available
through the prov_errno and err_data fields.  Users may call fi_eq_strerror to
convert provider specific error information into a printable string
for debugging purposes.

On input, err_data_size indicates the size of the err_data buffer in bytes.
On output, err_data_size will be set to the number of bytes copied to the
err_data buffer.  The err_data information is typically used with
fi_eq_strerror to provide details about the type of error that occurred.

For compatibility purposes, if err_data_size is 0 on input, or the fabric
was opened with release < 1.5, err_data will be set to a data buffer
owned by the provider.  The contents of the buffer will remain valid until a
subsequent read call against the EQ.  Applications must serialize access
to the EQ when processing errors to ensure that the buffer referenced by
err_data does not change.

# EVENT FIELDS

The EQ entry data structures share many of the same fields.  The meanings
are the same or similar for all EQ structure formats, with specific details
described below.

*fid*
: This corresponds to the fabric descriptor associated with the event.  The
  type of fid depends on the event being reported.  For FI_CONNREQ this will
  be the fid of the passive endpoint.  FI_CONNECTED and FI_SHUTDOWN will
  reference the active endpoint.  FI_MR_COMPLETE and FI_AV_COMPLETE will
  refer to the MR or AV fabric descriptor, respectively.  FI_JOIN_COMPLETE
  will point to the multicast descriptor returned as part of the join
  operation.  Applications can use fid->context value to retrieve the
  context associated with the fabric descriptor.

*context*
: The context value is set to the context parameter specified with the
  operation that generated the event.  If no context parameter is
  associated with the operation, this field will be NULL.

*data*
: Data is an operation specific value or set of bytes.  For connection
  events, data is application data exchanged as part of the connection
  protocol.

*err*
: This err code is a positive fabric errno associated with an event.
  The err value indicates the general reason for an error, if one occurred.
  See fi_errno.3 for a list of possible error codes.

*prov_errno*
: On an error, prov_errno may contain a provider specific error code.  The
  use of this field and its meaning is provider specific.  It is intended
  to be used as a debugging aid.  See fi_eq_strerror for additional details
  on converting this error value into a human readable string.

*err_data*
: On an error, err_data may reference a provider specific amount of data
  associated with an error.  The use of this field and its meaning is
  provider specific.  It is intended to be used as a debugging aid.  See
  fi_eq_strerror for additional details on converting this error data into
  a human readable string.

*err_data_size*
: On input, err_data_size indicates the size of the err_data buffer in bytes.
  On output, err_data_size will be set to the number of bytes copied to the
  err_data buffer.  The err_data information is typically used with
  fi_eq_strerror to provide details about the type of error that occurred.

  For compatibility purposes, if err_data_size is 0 on input, or the fabric
  was opened with release < 1.5, err_data will be set to a data buffer
  owned by the provider.  The contents of the buffer will remain valid until a
  subsequent read call against the EQ.  Applications must serialize access
  to the EQ when processing errors to ensure that the buffer referenced by
  err_data does no change.

# NOTES

If an event queue has been overrun, it will be placed into an 'overrun'
state.  Write operations against an overrun EQ will fail with -FI_EOVERRUN.
Read operations will continue to return any valid, non-corrupted events, if
available.  After all valid events have been retrieved, any attempt to read
the EQ will result in it returning an FI_EOVERRUN error event.  Overrun
event queues are considered fatal and may not be used to report additional
events once the overrun occurs.

# RETURN VALUES

fi_eq_open
: Returns 0 on success.  On error, a negative value corresponding to
  fabric errno is returned.

fi_eq_read / fi_eq_readerr
: On success, returns the number of bytes read from the
  event queue.  On error, a negative value corresponding to fabric
  errno is returned.  If no data is available to be read from the
  event queue, -FI_EAGAIN is returned.

fi_eq_sread
: On success, returns the number of bytes read from the
  event queue.  On error, a negative value corresponding to fabric
  errno is returned.  If the timeout expires or the calling
  thread is signaled and no data is available to be read from the
  event queue, -FI_EAGAIN is returned.

fi_eq_write
: On success, returns the number of bytes written to the
  event queue.  On error, a negative value corresponding to fabric
  errno is returned.

fi_eq_strerror
: Returns a character string interpretation of the provider specific
  error returned with a completion.

Fabric errno values are defined in
`rdma/fi_errno.h`.

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_cntr`(3)](fi_cntr.3.html),
[`fi_poll`(3)](fi_poll.3.html)
