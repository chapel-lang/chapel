---
layout: page
title: fi_cm(3)
tagline: Libfabric Programmer's Manual
---
{% include JB/setup %}

# NAME

fi_cm - Connection management operations

fi_connect / fi_listen / fi_accept / fi_reject / fi_shutdown
: Manage endpoint connection state.

fi_setname / fi_getname / fi_getpeer
: Set local, or return local or peer endpoint address.

fi_join / fi_close / fi_mc_addr
: Join, leave, or retrieve a multicast address.

# SYNOPSIS

```c
#include <rdma/fi_cm.h>

int fi_connect(struct fid_ep *ep, const void *addr,
    const void *param, size_t paramlen);

int fi_listen(struct fid_pep *pep);

int fi_accept(struct fid_ep *ep, const void *param, size_t paramlen);

int fi_reject(struct fid_pep *pep, fid_t handle,
    const void *param, size_t paramlen);

int fi_shutdown(struct fid_ep *ep, uint64_t flags);

int fi_setname(fid_t fid, void *addr, size_t addrlen);

int fi_getname(fid_t fid, void *addr, size_t *addrlen);

int fi_getpeer(struct fid_ep *ep, void *addr, size_t *addrlen);

int fi_join(struct fid_ep *ep, const void *addr, uint64_t flags,
	struct fid_mc **mc, void *context);

int fi_close(struct fid *mc);

fi_addr_t fi_mc_addr(struct fid_mc *mc);
```

# ARGUMENTS

*ep / pep*
: Fabric endpoint on which to change connection state.

*fid*
Active or passive endpoint to get/set address.

*addr*
: Buffer to address.  On a set call, the endpoint will be assigned the
  specified address.  On a get, the local address will be copied into the
  buffer, up to the space provided.  For connect, this parameter indicates
  the peer address to connect to.  The address must be in the same format as
  that specified using fi_info: addr_format when the endpoint was created.

*addrlen*
: On input, specifies size of addr buffer.  On output, stores number
  of bytes written to addr buffer.

*param*
: User-specified data exchanged as part of the connection exchange.

*paramlen*
: Size of param buffer.

*info*
: Fabric information associated with a connection request.

*mc*
: Multicast group associated with an endpoint.

*flags*
: Additional flags for controlling connection operation.

*context*
: User context associated with the request.

# DESCRIPTION

Connection management functions are used to connect an 
connection-oriented endpoint to a peer endpoint.

## fi_listen

The fi_listen call indicates that the specified endpoint should be
transitioned into a passive connection state, allowing it to accept
incoming connection requests.  Connection requests against a listening
endpoint are reported asynchronously to the user through a bound CM
event queue using the FI_CONNREQ event type.  The number of outstanding
connection requests that can be queued at an endpoint is limited by the
listening endpoint's backlog parameter.  The backlog is initialized
based on administrative configuration values, but may be adjusted
through the fi_control call.

## fi_connect

The fi_connect call initiates a connection request on a
connection-oriented endpoint to the destination address.  fi_connect
may only be called on an endpoint once in its lifetime.

## fi_accept / fi_reject

The fi_accept and fi_reject calls are used on the passive (listening)
side of a connection to accept or reject a connection request,
respectively.  To accept a connection, the listening application first
waits for a connection request event (FI_CONNREQ).
After receiving such an event, the application
allocates a new endpoint to accept the connection.  This endpoint must
be allocated using an fi_info structure referencing the handle from this
FI_CONNREQ event.  fi_accept is then invoked
with the newly allocated endpoint.  If
the listening application wishes to reject a connection request, it calls
fi_reject with the listening endpoint and
a reference to the connection request.

A successfully accepted connection request will result in the active
(connecting) endpoint seeing an FI_CONNECTED event on its associated
event queue.  A rejected or failed connection request will generate an
error event.  The error entry will provide additional details describing
the reason for the failed attempt.

An FI_CONNECTED event will also be generated on the passive side for the
accepting endpoint once the connection has been properly established.
The fid of the FI_CONNECTED event will be that of the endpoint passed to
fi_accept as opposed to the listening passive endpoint.
Outbound data transfers cannot be initiated on a connection-oriented
endpoint until an FI_CONNECTED event has been generated.  However, receive
buffers may be associated with an endpoint anytime.

## fi_shutdown

The fi_shutdown call is used to gracefully disconnect an endpoint from
its peer.  The flags parameter is reserved and must be 0.

Outstanding operations posted to the endpoint when fi_shutdown is
called will be canceled or discarded.  Notification of canceled operations
will be reported by the provider to the corresponding completion
queue(s).  Discarded operations will silently be dropped, with no
completions generated.  The choice of canceling, versus discarding
operations, is provider dependent.  However, all canceled completions
will be written before fi_shutdown returns.

When called, fi_shutdown does not affect completions already written to a
completion queue.  Any queued completions associated with asynchronous
operations posted to the endpoint may still be retrieved from the
corresponding completion queue(s) after an endpoint has been shutdown.

An FI_SHUTDOWN event will be generated for an endpoint when the remote
peer issues a disconnect using fi_shutdown or abruptly closes the endpoint.
Note that in the abrupt close case, an FI_SHUTDOWN event will only be
generated if the peer system is reachable and a service or kernel agent
on the peer system is able to notify the local endpoint that the connection
has been aborted.

## fi_close

Fi_close is used to disassociate an endpoint from a multicast group and
close all resources associated with the group.  Fi_close must be called on
all multicast groups that an endpoint joins.

## fi_setname

The fi_setname call may be used to modify or assign the address of the
local endpoint.  It is conceptually similar to the socket bind operation.
An endpoint may be assigned an address on its creation,
through the fi_info structure.  The fi_setname call allows an endpoint to
be created without being associated with a specific service (e.g., port
number) and/or node (e.g., network) address, with the addressing assigned
dynamically.  The format of the specified addressing data must match that
specified through the fi_info structure when the endpoint was created.

If no service address is specified and a service address
has not yet been assigned to the endpoint, then the provider will allocate
a service address and assign it to the endpoint.  If a node or service
address is specified, then, upon successful completion of fi_setname,
the endpoint will be assigned the given addressing.  If an address cannot
be assigned, or the endpoint address cannot be modified, an appropriate
fabric error number is returned.

## fi_getname / fi_getpeer

The fi_getname and fi_getpeer calls may be used to retrieve the local or
peer endpoint address, respectively.  On input, the addrlen parameter should
indicate the size of the addr buffer.  If the actual address is larger than
what can fit into the buffer, it will be truncated and -FI_ETOOSMALL will
be returned.  On output, addrlen is set to the size of the buffer needed
to store the address, which may be larger than the input value.  

fi_getname is not guaranteed to return a valid source address until after the
specified endpoint has been enabled or has had an address assigned.  An
endpoint may be enabled explicitly through fi_enable, or implicitly, such as
through fi_connect or fi_listen.  An address may be assigned using fi_setname.
fi_getpeer is not guaranteed to return a valid peer address until an endpoint
has been completely connected -- an FI_CONNECTED event has been generated.

## fi_join

This call attaches an endpoint to a multicast group.  By default, the endpoint
will join the group based on the data transfer capabilities of the endpoint.
For example, if the endpoint has been configured to both send and receive data,
then the endpoint will be able to initiate and receive transfers to and from
the multicast group.  The fi_join flags may be used to restrict access to the
multicast group, subject to endpoint capability limitations.

Multicast join operations complete asynchronously.  An endpoint must be bound
to an event queue prior to calling fi_join.  The result of the join operation
will be reported to the EQ as an FI_JOIN_COMPLETE event.  Applications cannot
issue multicast transfers until receiving notification that the join
operation has completed.  Note that an endpoint may begin receiving
messages from the multicast group as soon as the join completes, which can
occur prior to the FI_JOIN_COMPLETE event being generated.

Applications must call fi_close on the multicast group to disconnect the
endpoint from the group.  After a join operation has completed, the
fi_mc_addr call may be used to retrieve the address associated with the
multicast group.

## fi_mc_addr

Returns the fi_addr_t address associated with a multicast group.  This address
must be used when transmitting data to a multicast group and paired with
the FI_MULTICAST operation flag.

# FLAGS

Except in functions noted below, flags are reserved and must be 0.

*FI_SEND*
: Applies to fi_join.  This flag indicates that the endpoint should join the
  multicast group as a send only member.  The endpoint must be configured for
  transmit operations to use this flag, or an error will occur.

*FI_RECV*
: Applies to fi_join.  This flag indicates that the endpoint should join the
  multicast group with receive permissions only.  The endpoint must be
  configured for receive operations to use this flag, or an error will occur.

# RETURN VALUE

Returns 0 on success. On error, a negative value corresponding to fabric
errno is returned. Fabric errno values are defined in
`rdma/fi_errno.h`.

# ERRORS


# NOTES

For connection-oriented endpoints, the buffer referenced by param
will be sent as part of the connection request or response, subject
to the constraints of the underlying connection protocol.
Applications may use fi_getopt with the FI_OPT_CM_DATA_SIZE endpoint
option to determine the size of application data that may be exchanged as
part of a connection request or response.  The fi_connect, fi_accept, and
fi_reject calls will silently truncate any application data which cannot
fit into underlying protocol messages.  User data exchanged as part of
the connection process is available as part of the fi_eq_cm_entry
structure, for FI_CONNREQ and FI_CONNECTED events, or as additional
err_data to fi_eq_err_entry, in the case of a rejected connection.

# SEE ALSO

[`fi_getinfo`(3)](fi_getinfo.3.html),
[`fi_endpoint`(3)](fi_endpoint.3.html),
[`fi_domain`(3)](fi_domain.3.html),
[`fi_eq`(3)](fi_eq.3.html)
