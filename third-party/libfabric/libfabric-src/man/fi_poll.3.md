---
layout: page
title: fi_poll(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_poll \- Polling and wait set operations

fi_poll_open / fi_close
: Open/close a polling set

fi_poll_add / fi_poll_del
: Add/remove a completion queue or counter to/from a poll set.

fi_poll
: Poll for progress and events across multiple completion queues
  and counters.

fi_wait_open / fi_close
: Open/close a wait set

fi_wait
: Waits for one or more wait objects in a set to be signaled.

fi_trywait
: Indicate when it is safe to block on wait objects using native OS calls.

fi_control
: Control wait set operation or attributes.

# SYNOPSIS

```c
#include <rdma/fi_domain.h>

int fi_poll_open(struct fid_domain *domain, struct fi_poll_attr *attr,
    struct fid_poll **pollset);

int fi_close(struct fid *pollset);

int fi_poll_add(struct fid_poll *pollset, struct fid *event_fid,
    uint64_t flags);

int fi_poll_del(struct fid_poll *pollset, struct fid *event_fid,
    uint64_t flags);

int fi_poll(struct fid_poll *pollset, void **context, int count);

int fi_wait_open(struct fid_fabric *fabric, struct fi_wait_attr *attr,
    struct fid_wait **waitset);

int fi_close(struct fid *waitset);

int fi_wait(struct fid_wait *waitset, int timeout);

int fi_trywait(struct fid_fabric *fabric, struct fid **fids, size_t count);

int fi_control(struct fid *waitset, int command, void *arg);
```

# ARGUMENTS

*fabric*
: Fabric provider

*domain*
: Resource domain

*pollset*
: Event poll set

*waitset*
: Wait object set

*attr*
: Poll or wait set attributes

*context*
: On success, an array of user context values associated with
  completion queues or counters.

*fids*
: An array of fabric descriptors, each one associated with a native
  wait object.

*count*
: Number of entries in context or fids array.

*timeout*
: Time to wait for a signal, in milliseconds.

*command*
: Command of control operation to perform on the wait set.

*arg*
: Optional control argument.

# DESCRIPTION


## fi_poll_open

fi_poll_open creates a new polling set.  A poll set enables an
optimized method for progressing asynchronous operations across
multiple completion queues and counters and checking for their completions.

A poll set is defined with the following attributes.

```c
struct fi_poll_attr {
	uint64_t             flags;     /* operation flags */
};
```

*flags*
: Flags that set the default operation of the poll set.  The use of
  this field is reserved and must be set to 0 by the caller.

## fi_close

The fi_close call releases all resources associated with a poll set.
The poll set must not be associated with any other resources prior to
being closed, otherwise the call will return -FI_EBUSY.

## fi_poll_add

Associates a completion queue or counter with a poll set.

## fi_poll_del

Removes a completion queue or counter from a poll set.

## fi_poll

Progresses all completion queues and counters associated with a poll set
and checks for events.  If events might have occurred, contexts associated
with the completion queues and/or counters are returned.  Completion
queues will return their context if they are not empty.  The context
associated with a counter will be returned if the counter's success
value or error value have changed since the last time fi_poll, fi_cntr_set,
or fi_cntr_add were called.  The number of contexts is limited to the
size of the context array, indicated by the count parameter.

Note that fi_poll only indicates that events might be available.  In some
cases, providers may consume such events internally, to drive progress, for
example.  This can result in fi_poll returning false positives.  Applications
should drive their progress based on the results of reading events from a
completion queue or reading counter values.  The fi_poll function will always
return all completion queues and counters that do have new events.

## fi_wait_open

fi_wait_open allocates a new wait set.  A wait set enables an
optimized method of waiting for events across multiple completion queues
and counters.  Where possible, a wait set uses a single underlying
wait object that is signaled when a specified condition occurs on an
associated completion queue or counter.

The properties and behavior of a wait set are defined by struct
fi_wait_attr.

```c
struct fi_wait_attr {
	enum fi_wait_obj     wait_obj;  /* requested wait object */
	uint64_t             flags;     /* operation flags */
};
```

*wait_obj*
: Wait sets are associated with specific wait object(s).  Wait objects
  allow applications to block until the wait object is signaled,
  indicating that an event is available to be read.  The following
  values may be used to specify the type of wait object associated
  with a wait set: FI_WAIT_UNSPEC, FI_WAIT_FD, FI_WAIT_MUTEX_COND,
  and FI_WAIT_YIELD.

- *FI_WAIT_UNSPEC*
: Specifies that the user will only wait on the wait set using
  fabric interface calls, such as fi_wait.  In this case, the
  underlying provider may select the most appropriate or highest
  performing wait object available, including custom wait mechanisms.
  Applications that select FI_WAIT_UNSPEC are not guaranteed to
  retrieve the underlying wait object.

- *FI_WAIT_FD*
: Indicates that the wait set should use a single file descriptor as
  its wait mechanism, as exposed to the application.  Internally, this
  may require the use of epoll in order to support waiting on a single
  file descriptor.  File descriptor wait objects must be usable in the
  POSIX select(2) and poll(2), and Linux epoll(7) routines (if
  available).  Provider signal an FD wait object by marking it as
  readable or with an error.

- *FI_WAIT_MUTEX_COND*
: Specifies that the wait set should use a pthread mutex and cond
  variable as a wait object.

- *FI_WAIT_POLLFD*
: This option is similar to FI_WAIT_FD, but allows the wait mechanism to use
  multiple file descriptors as its wait mechanism, as viewed by the
  application.  The use of FI_WAIT_POLLFD can eliminate the need to use
  epoll to abstract away needing to check multiple file descriptors when
  waiting for events.  The file descriptors must be usable in the POSIX
  select(2) and poll(2) routines, and match directly to being used with
  poll.  See the NOTES section below for details on using pollfd.

- *FI_WAIT_YIELD*
: Indicates that the wait set will wait without a wait object but instead
  yield on every wait.

*flags*
: Flags that set the default operation of the wait set.  The use of
  this field is reserved and must be set to 0 by the caller.

## fi_close

The fi_close call releases all resources associated with a wait set.
The wait set must not be bound to any other opened resources prior to
being closed, otherwise the call will return -FI_EBUSY.

## fi_wait

Waits on a wait set until one or more of its underlying wait objects
is signaled.

## fi_trywait

The fi_trywait call was introduced in libfabric version 1.3.  The behavior
of using native wait objects without the use of fi_trywait is provider
specific and should be considered non-deterministic.

The fi_trywait() call is used in conjunction with native operating
system calls to block on wait objects, such as file descriptors.  The
application must call fi_trywait and obtain a return value of
FI_SUCCESS prior to blocking on a native wait object.  Failure to
do so may result in the wait object not being signaled, and the
application not observing the desired events.  The following
pseudo-code demonstrates the use of fi_trywait in conjunction with
the OS select(2) call.

```c
fi_control(&cq->fid, FI_GETWAIT, (void *) &fd);
FD_ZERO(&fds);
FD_SET(fd, &fds);

while (1) {
	if (fi_trywait(&cq, 1) == FI_SUCCESS)
		select(fd + 1, &fds, NULL, &fds, &timeout);

	do {
		ret = fi_cq_read(cq, &comp, 1);
	} while (ret > 0);
}
```

fi_trywait() will return FI_SUCCESS if it is safe to block on the wait object(s)
corresponding to the fabric descriptor(s), or -FI_EAGAIN if there are
events queued on the fabric descriptor or if blocking could hang the
application.

The call takes an array of fabric descriptors.  For each wait object
that will be passed to the native wait routine, the corresponding
fabric descriptor should first be passed to fi_trywait.  All fabric
descriptors passed into a single fi_trywait call must make use of the
same underlying wait object type.

The following types of fabric descriptors may be passed into fi_trywait:
event queues, completion queues, counters, and wait sets.  Applications
that wish to use native wait calls should select specific wait objects
when allocating such resources.  For example, by setting the item's
creation attribute wait_obj value to FI_WAIT_FD.

In the case the wait object to check belongs to a wait set, only
the wait set itself needs to be passed into fi_trywait.  The fabric
resources associated with the wait set do not.

On receiving a return value of -FI_EAGAIN from fi_trywait, an application
should read all queued completions and events, and call fi_trywait again
before attempting to block.  Applications can make use of a fabric
poll set to identify completion queues and counters that may require
processing.

## fi_control

The fi_control call is used to access provider or implementation specific
details of a fids that support blocking calls, such as wait sets, completion
queues, counters, and event queues.  Access to the wait set or fid should be
serialized across all calls when fi_control is invoked, as it may redirect
the implementation of wait set operations. The following control commands
are usable with a wait set or fid.

*FI_GETWAIT (void \*\*)*
: This command allows the user to retrieve the low-level wait object
  associated with a wait set or fid. The format of the wait set is specified
  during wait set creation, through the wait set attributes. The fi_control
  arg parameter should be an address where a pointer to the returned wait
  object will be written. This should be an 'int *' for FI_WAIT_FD,
  'struct fi_mutex_cond' for FI_WAIT_MUTEX_COND, or 'struct fi_wait_pollfd'
  for FI_WAIT_POLLFD. Support for FI_GETWAIT is provider specific.

*FI_GETWAITOBJ (enum fi_wait_obj \*)*
: This command returns the type of wait object associated with a wait set
  or fid.

# RETURN VALUES

Returns FI_SUCCESS on success.  On error, a negative value corresponding to
fabric errno is returned.

Fabric errno values are defined in
`rdma/fi_errno.h`.

fi_poll
: On success, if events are available, returns the number of entries
  written to the context array.

# NOTES

In many situations, blocking calls may need to wait on signals sent
to a number of file descriptors.  For example, this is the case for
socket based providers, such as tcp and udp, as well as utility providers
such as multi-rail.  For simplicity, when epoll is available, it can
be used to limit the number of file descriptors that an application
must monitor.  The use of epoll may also be required in order
to support FI_WAIT_FD.

However, in order to support waiting on multiple file descriptors on systems
where epoll support is not available, or where epoll performance may
negatively impact performance, FI_WAIT_POLLFD provides this mechanism.
A significant different between using POLLFD versus FD wait objects
is that with FI_WAIT_POLLFD, the file descriptors may change dynamically.
As an example, the file descriptors associated with a completion queues'
wait set may change as endpoint associations with the CQ are added and
removed.

Struct fi_wait_pollfd is used to retrieve all file descriptors for fids
using FI_WAIT_POLLFD to support blocking calls.

```c
struct fi_wait_pollfd {
    uint64_t      change_index;
    size_t        nfds;
    struct pollfd *fd;
};
```

*change_index*
: The change_index may be used to determine if there have been any changes
  to the file descriptor list.  Anytime a file descriptor is added, removed,
  or its events are updated, this field is incremented by the provider.
  Applications wishing to wait on file descriptors directly should cache
  the change_index value.  Before blocking on file descriptor events, the
  app should use fi_control() to retrieve the current change_index and
  compare that against its cached value.  If the values differ, then the
  app should update its file descriptor list prior to blocking.

*nfds*
: On input to fi_control(), this indicates the number of entries in the
  struct pollfd * array.  On output, this will be set to the number of
  entries needed to store the current number of file descriptors.  If
  the input value is smaller than the output value, fi_control() will
  return the error -FI_ETOOSMALL.  Note that setting nfds = 0 allows
  an efficient way of checking the change_index.

*fd*
: This points to an array of struct pollfd entries.  The number of entries
  is specified through the nfds field.  If the number of needed entries
  is less than or equal to the number of entries available, the struct
  pollfd array will be filled out with a list of file descriptors and
  corresponding events that can be used in the select(2) and poll(2)
  calls.

The change_index is updated only when the file descriptors associated with
the pollfd file set has changed.  Checking the change_index is an additional
step needed when working with FI_WAIT_POLLFD wait objects directly.  The use
of the fi_trywait() function is still required if accessing wait objects
directly.

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_cntr`(3)](fi_cntr.3.html),
[`fi_eq`(3)](fi_eq.3.html)
