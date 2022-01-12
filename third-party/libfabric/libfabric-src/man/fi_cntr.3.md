---
layout: page
title: fi_cntr(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_cntr \- Completion and event counter operations

fi_cntr_open / fi_close
: Allocate/free a counter

fi_cntr_read
: Read the current value of a counter

fi_cntr_readerr
: Reads the number of operations which have completed in error.

fi_cntr_add
: Increment a counter by a specified value

fi_cntr_set
: Set a counter to a specified value

fi_cntr_wait
: Wait for a counter to be greater or equal to a threshold value

# SYNOPSIS

```c
#include <rdma/fi_domain.h>

int fi_cntr_open(struct fid_domain *domain, struct fi_cntr_attr *attr,
    struct fid_cntr **cntr, void *context);

int fi_close(struct fid *cntr);

uint64_t fi_cntr_read(struct fid_cntr *cntr);

uint64_t fi_cntr_readerr(struct fid_cntr *cntr);

int fi_cntr_add(struct fid_cntr *cntr, uint64_t value);

int fi_cntr_adderr(struct fid_cntr *cntr, uint64_t value);

int fi_cntr_set(struct fid_cntr *cntr, uint64_t value);

int fi_cntr_seterr(struct fid_cntr *cntr, uint64_t value);

int fi_cntr_wait(struct fid_cntr *cntr, uint64_t threshold,
    int timeout);
```

# ARGUMENTS

*domain*
: Fabric domain

*cntr*
: Fabric counter

*attr*
: Counter attributes

*context*
: User specified context associated with the counter

*value*
: Value to increment or set counter

*threshold*
: Value to compare counter against

*timeout*
: Time in milliseconds to wait.  A negative value indicates infinite
  timeout.

# DESCRIPTION

Counters record the number of requested operations that have
completed.  Counters can provide a light-weight completion mechanism
by allowing the suppression of CQ completion entries.  They are
useful for applications that only need to know the number of requests
that have completed, and not details about each request.  For example,
counters may be useful for implementing credit based flow control or
tracking the number of remote processes that have responded to a
request.

Counters typically only count successful completions.  However, if an
operation completes in error, it may increment an associated error
value.  That is, a counter actually stores two distinct values, with
error completions updating an error specific value.

Counters are updated following the completion event semantics defined
in [`fi_cq`(3)](fi_cq.3.html).  The timing of the update is based
on the type of transfer and any specified operation flags.

## fi_cntr_open

fi_cntr_open allocates a new fabric counter.  The properties and
behavior of the counter are defined by `struct fi_cntr_attr`.

```c
struct fi_cntr_attr {
	enum fi_cntr_events  events;    /* type of events to count */
	enum fi_wait_obj     wait_obj;  /* requested wait object */
	struct fid_wait     *wait_set;  /* optional wait set */
	uint64_t             flags;     /* operation flags */
};
```

*events*
: A counter captures different types of events.  The specific type
  which is to counted are one of the following:

- *FI_CNTR_EVENTS_COMP*
: The counter increments for every successful completion that occurs
  on an associated bound endpoint.  The type of completions -- sends
  and/or receives -- which are counted may be restricted using control
  flags when binding the counter and the endpoint.  Counters increment
  on all successful completions, separately from whether the operation
  generates an entry in an event queue.

*wait_obj*
: Counters may be associated with a specific wait object.  Wait
  objects allow applications to block until the wait object is
  signaled, indicating that a counter has reached a specific
  threshold.  Users may use fi_control to retrieve the underlying wait
  object associated with a counter, in order to use it in other system
  calls.  The following values may be used to specify the type of wait
  object associated with a counter: FI_WAIT_NONE, FI_WAIT_UNSPEC,
  FI_WAIT_SET, FI_WAIT_FD, FI_WAIT_MUTEX_COND, and FI_WAIT_YIELD. 
  The default is FI_WAIT_NONE.

- *FI_WAIT_NONE*
: Used to indicate that the user will not block (wait) for events on
  the counter.

- *FI_WAIT_UNSPEC*
: Specifies that the user will only wait on the counter using fabric
  interface calls, such as fi_cntr_wait.  In this case, the
  underlying provider may select the most appropriate or highest
  performing wait object available, including custom wait mechanisms.
  Applications that select FI_WAIT_UNSPEC are not guaranteed to
  retrieve the underlying wait object.

- *FI_WAIT_SET*
: Indicates that the event counter should use a wait set object to
  wait for events.  If specified, the wait_set field must reference an
  existing wait set object.

- *FI_WAIT_FD*
: Indicates that the counter should use a file descriptor as its wait
  mechanism.  A file descriptor wait object must be usable in select,
  poll, and epoll routines.  However, a provider may signal an FD wait
  object by marking it as readable, writable, or with an error.

- *FI_WAIT_MUTEX_COND*
: Specifies that the counter should use a pthread mutex and cond
  variable as a wait object.

- *FI_WAIT_YIELD*
: Indicates that the counter will wait without a wait object but instead
  yield on every wait. Allows usage of fi_cntr_wait through a spin.

*wait_set*
: If wait_obj is FI_WAIT_SET, this field references a wait object to
  which the event counter should attach.  When an event is added to
  the event counter, the corresponding wait set will be signaled if
  all necessary conditions are met.  The use of a wait_set enables an
  optimized method of waiting for events across multiple event
  counters.  This field is ignored if wait_obj is not FI_WAIT_SET.

*flags*
: Flags are reserved for future use, and must be set to 0.

## fi_close

The fi_close call releases all resources associated with a counter.  When
closing the counter, there must be no opened endpoints, transmit contexts,
receive contexts or memory regions associated with the counter.  If resources
are still associated with the counter when attempting to close, the call will
return -FI_EBUSY.

## fi_cntr_control

The fi_cntr_control call is used to access provider or implementation
specific details of the counter.  Access to the counter should be
serialized across all calls when fi_cntr_control is invoked, as it may
redirect the implementation of counter operations.  The following
control commands are usable with a counter:

*FI_GETOPSFLAG (uint64_t \*)*
: Returns the current default operational flags associated with the counter.

*FI_SETOPSFLAG (uint64_t \*)*
: Modifies the current default operational flags associated with the
  counter.

*FI_GETWAIT (void \*\*)*
: This command allows the user to retrieve the low-level wait object
  associated with the counter.  The format of the wait-object is
  specified during counter creation, through the counter attributes.
  See fi_eq.3 for addition details using control with FI_GETWAIT.

## fi_cntr_read

The fi_cntr_read call returns the current value of the counter.

## fi_cntr_readerr

The read error call returns the number of operations that completed in
error and were unable to update the counter.

## fi_cntr_add

This adds the user-specified value to the counter.

## fi_cntr_adderr

This adds the user-specified value to the error value of the counter.

## fi_cntr_set

This sets the counter to the specified value.

## fi_cntr_seterr

This sets the error value of the counter to the specified value.

## fi_cntr_wait

This call may be used to wait until the counter reaches the specified
threshold, or until an error or timeout occurs.  Upon successful
return from this call, the counter will be greater than or equal to
the input threshold value.

If an operation associated with the counter encounters an error, it
will increment the error value associated with the counter.  Any
change in a counter's error value will unblock any thread inside
fi_cntr_wait.

If the call returns due to timeout, -FI_ETIMEDOUT will be returned.
The error value associated with the counter remains unchanged.

It is invalid for applications to call this function if the counter
has been configured with a wait object of FI_WAIT_NONE or FI_WAIT_SET.

# RETURN VALUES

Returns 0 on success.  On error, a negative value corresponding to
fabric errno is returned.

fi_cntr_read /  fi_cntr_readerr
: Returns the current value of the counter.

Fabric errno values are defined in
`rdma/fi_errno.h`.

# NOTES

In order to support a variety of counter implementations, updates made to
counter values (e.g. fi_cntr_set or fi_cntr_add) may not be immediately
visible to counter read operations (i.e. fi_cntr_read or fi_cntr_readerr).
A small, but undefined, delay may occur between the counter changing and
the reported value being updated.  However, a final updated value will
eventually be reflected in the read counter value.

Additionally, applications should ensure that the value of a counter is
stable and not subject to change prior to calling fi_cntr_set
or fi_cntr_seterr.  Otherwise, the resulting value of the counter after
fi_cntr_set / fi_cntr_seterr is undefined, as updates to the counter may
be lost.  A counter value is considered stable if all previous
updates using fi_cntr_set / fi_cntr_seterr and results of related operations
are reflected in the observed value of the counter.

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_eq`(3)](fi_eq.3.html),
[`fi_poll`(3)](fi_poll.3.html)
