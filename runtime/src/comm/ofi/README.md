## MCM Conformance in CHPL_COMM=ofi

This document describes how the libfabric-based comm layer helps arrange
for programs to conform to the Chapel Memory Consistency Model (MCM).
The compiler and module code produce an inherently MCM-conforming
program, but the imperfect match between the Chapel and libfabric
communication models and the desire to limit communication overheads and
overlap communication and computation both have the potential to cause
MCM-breaking behavior.

The document introduces some libfabric terms and concepts and describes
how the Chapel communication primitives are implemented in terms of
libfabric operations.  It then quotes the *Program Order* and *Memory
Order* sections of the **Memory Consistency Model** chapter of the
Chapel spec, adding text as needed to describe what the implementation
does to meet the clauses in those sections.

### Background

The comm layer can use three different kinds of libfabric operations for
its various transactions.  It uses the RMA (read/write, or alternatively
GET/PUT) interface for direct transfers between local and remote memory.
It uses the atomics interface for native atomic operations on networks
that support those.  And, it uses the message (send/receive, or
alternatively SEND/RECV)) interface for Active Messages (AMs) for
executeOn transactions for on-statement bodies, memory transfers to or
from unregistered memory, atomic operations which cannot be done
natively, and certain internal operations which don't have MCM
implications.

The comm layer has two libfabric tools it can use to determine when
operations are in some sense "done" and to impose order on them with
respect to other operations:

- _Completion levels_ adjust what effects of an operation must be
  visible before libfabric delivers an event indicating its completion
  to the initiator.

- _Message ordering_ settings specify how operations may or may not be
  reordered with respect to each other.  Note that "message" in this
  context is a general term referring to all kinds of communication
  operations, not just those of the SEND/RECV message interface.

A default completion level can be set for the entire libfabric domain,
that is, the overarching interface to the network, and one can also
specify completion levels individually for network operations.  The
message ordering settings can only be specified for the entire domain.

#### Completion Levels

Libfabric's completion level specifies what state an operation has to
reach before an event is delivered to indicate that it has done so.  (By
"delivered" here we mean either that a completion counter associated
with the transmit endpoint has been updated or that an event has been
placed in the completion queue associated with the endpoint, as
appropriate.)  The various completion level values are discussed in the
libfabric `fi_cq(3)` man page.  The comm layer supports either
`FI_DELIVERY_COMPLETE` (aka *delivery-complete*) or the provider's
default completion level, which in our experience seems to be
`FI_TRANSMIT_COMPLETE` (aka *transmit-complete*) or something closely
related to it.

Delivery-complete says that the operation has arrived at the remote node
and is no longer dependent on initiator-side resources or the network
fabric.  Whether its effects such as memory updates are visible is a
little unclear.  For message (SEND/RECV) traffic for AMs, because the
comm layer always has receive buffer space posted, it seems to mean that
the message data definitely has been placed in the AM request buffer.
This is based on the following sentence in the `fi_cq(3)` man page:

> If an application buffer was ready to receive the results of the
  message when it arrived, then delivery complete indicates that the
  data was placed into the application's buffer.

Note that this does not mean the AM request has been handled.  Knowing
that has occurred requires seeing a 'done' indicator from the AM
handler.

For RMA writes, if the target memory location counts as a "buffer" with
respect to the above sentence in the man page, then similarly that would
seem to indicate that receipt of a delivery-complete completion means
the written data is memory-visible.

But for native atomic operations, the `fi_atomic(3)` man page is quite
clear that remote completions of some sort are needed in order to know
an atomic operation's effect on its target datum is memory-visible.  The
relevant text is this:

> Any updates to data at the target of an atomic operation will be
  visible to agents (CPU processes, NICs, and other devices) on the
  target node prior to one of the following occurring.  If the atomic
  operation generates a completion event or updates a completion counter
  at the target endpoint, the results will be available prior to the
  completion notification.  After processing a completion for the
  atomic, if the initiator submits a transfer between the same endpoints
  that generates a completion at the target, the results will be
  available prior to the subsequent transfer's event.  Or, if a fenced
  data transfer from the initiator follows the atomic request, the
  results will be available prior to a completion at the target for the
  fenced transfer.

The ramifications of this will be discussed more later, but the main
point is that the local completion level, whether delivery-complete or
something else, only describes the relationship between local completion
and the arrival of the result of a fetching atomic, not anything having
to do with the target datum.

For operations that return data to the initiator, such as RMA reads and
native fetching atomics, the source endpoint is considered a destination
as well and libfabric requires providers' default completion level for
such operations to be delivery-complete.  The provider itself determines
the default for other operations.  That may be *transmit-complete*,
which says that the operation has reached the target and is no longer
dependent on the fabric but does not say anything about the visibility
of its effects.  Or, it may even be *inject-complete*, which says only
that the originating buffer can be modified by the initiator without
affecting the operation.

The details will be discussed in greater depth later, but when the comm
layer uses delivery-complete as its default completion level, it doesn't
have to do much more than just that in order to achieve Chapel MCM
conformance.  However, because it requires a full network round trip
before delivering a completion for an operation, delivery-complete is
the slowest completion level.  There is also at least one provider which
advertises it can provide delivery-complete but doesn't actually do so;
see the
[libfabric Bug issue](https://github.com/ofiwg/libfabric/issues/5601)
or more info.
The comm layer only uses delivery-complete as a last resort, therefore.

When it uses some other completion level as its domain default, whether
because the provider doesn't support delivery-complete or because a
different completion level will yield better performance, the comm layer
has to do additional things to ensure MCM conformance and the visibility
of memory effects.

#### Message Ordering

Message ordering limits how operations can be reordered between when
they are initiated and when they are handled on the target node.  The
various message ordering vales are discussed in the libfabric `fi_ep(3)`
man page.  The message ordering settings differentiate between
operations for messages (SEND/RECV traffic), RMA, and native atomics.
The important ones for Chapel's purposes are the following.

- SAS (send-after-send) ordering specifies that messages (AMs, in our
  case) must be transmitted and delivered in the order they are
  submitted relative to other messages.

- SAW (send-after-write) ordering specifies that later messages (AMs)
  must be transmitted and delivered in the order they are submitted
  relative to earlier RMA write and/or non-fetching native atomic
  operations.

- Various kinds of RAW (read-after-write) orderings specify that later
  RMA read and/or fetching native atomic operations must be transmitted
  and delivered in the order they are submitted relative to earlier RMA
  write and/or non-fetching native atomic operations.

Message ordering only applies to operations across a specific pair of
initiating and targeted endpoints or contexts.  Libfabric does not
provide any direct way to control ordering between operations over
differing pairs of transmitting and targeted endpoints.

Because message ordering is confined to endpoint pairs, its usefulness
to the comm layer depends on the tasking implementation and whether or
not a task is using a "bound" endpoint.  If the tasking implementation
has a fixed number of threads, doesn't switch tasks across threads, and
the comm layer can bind a given transmit endpoint to the task's thread
for the duration of the task (or actually in practice, for the duration
of the entire program), then message ordering affects all operations
done throughout the life of a task, against a given target node.  This
happens, for example, when we're using Qthreads-based tasking and the
provider can supply at least as many transmit endpoints as there are
Qthreads worker threads.  (Plus 2 more, but that's a detail discussed in
the code.)  But if tasks can move from thread to thread, or something
else prevents tasks from being bound permanently to endpoints, then
message ordering can only affect the operations done while the task is
holding a specific transmit endpoint, during which it is not allowed to
yield.

**_Note (Implementation):_** For safety, when built for debug the comm
layer can verify an assumed absence of thread-to-thread task switching,
checking each time a task yields that it does indeed come back on the
same thread afterward.

The comm layer always requests send-after-send ordering, to prevent AMs
from being reordered in transit.  This may be overkill, however.  It is
certainly overkill for so-called "blocking" AMs where the sender, after
sending a request, suspends until the recipient sends back a 'done'
indication (via RMA) saying that it has finished handling the request.

**_Note (Implementation):_** We should have a little think-through about
whether we can stop requiring send-after-send ordering.  We don't have
any particular indication that it's slowing things down, but removing
unnecessary ordering constraints surely cannot hurt.

#### Store Operations May "Dangle"

The comm layer always requests either the delivery-complete completion
level or one or more forms of read-after-write message ordering.  With
either of these, however, some effects of libfabric operations may not
be guaranteed to be visible until the comm layer has taken additional
steps to make them so.  These are all remote stores whose visibility
state isn't known.  Here we refer to them as _dangling stores_.

##### AM-Mediated Non-fetching Atomics

The comm layer defines a number of AM types, but there are only MCM
implications for those that implement executeOn (on-statement bodies),
RMA where the remote address is unregistered, and atomic operations when
we're using a provider that can't do those natively.  AMs can either be
blocking, where the initiator waits for the target to send back a "done"
response before continuing, or nonblocking ("fire and forget").  All
effects of a blocking AM are visible before the target sends back the
"done" response.  AMs for unregistered RMA are always blocking.  AMs for
Chapel on-statements are blocking, with a few exceptions for internal
special cases that needn't concern us here.

AMs for fetching atomic operations are blocking, obviously, since we
have to wait for the result to arrive.  But AMs for non-fetching atomic
operations are a little special.  First, we assume that any non-fetching
atomic operation that follows a call to `chpl_comm_impl_task_end()`,
which is called at the end of the user code for each task, must be doing
the `_downEndCount()` for that task.  That operation can be done using
a nonblocking AM, because nothing on the initiating node depends on it.
(Only the code on the remote node waiting for that `endCount` to go to
zero cares about it.)  Any other non-fetching atomic operation could in
theory be done using a nonblocking AM, but then the effect on the target
variable would be a dangling store, because the initiator would have no
way to tell when it had been completed.  We therefore use a blocking AM
so that the initiator does not proceed until the effect of the atomic
operation is visible.  However, to minimize delays we put off waiting
for the corresponding 'done' indicator until the task performs some
other action that has MCM implications.

##### Dangling Regular Stores

The comm layer assumes that with delivery-complete, the effects of
regular RMA PUTs are visible by the time the completion is delivered.
Without delivery-complete this may not be true, but in that case the
comm layer always requests one or more forms of read-after-write message
ordering.  And since reads always use delivery-complete semantics, we
can ensure that a PUT's memory effects are visible by initiating an
appropriate RMA GET after it and then waiting for it to complete.  When
it does, the PUT must be memory-visible as well.

##### Dangling Native Atomics

Unfortunately, based on the `fi_atomic(3)` man page paragraph quoted
above in Completion Levels, the use of delivery-complete completion
semantics and/or read-after-write message ordering are not enough to
ensure the memory visibility of native atomic operations.  That can only
be assured by the delivery of a remote-side completion, whether one
associated with the atomic operation itself or one associated with
another operation ordered after it.  Even then, the mere delivery of
such a completion isn't enough; for the initiator to know that the
memory effect of the operation has become visible the remote side must
somehow (message SEND or RMA PUT) indicate that to the initiating side,
after seeing the completion.

Note that whether the effect of a native atomic operation is visible and
the order in which the effects of multiple such operations are visible
are to some extent orthogonal.  If our message ordering settings are
sufficient to fully order all native atomic operations, then once we
know a given operation's effect on its target variable is visible we
also know that the effects of all the operations before it are visible.
To put it another way, given a sequence of native atomic operations with
no other intervening MCM-related operations, once a remote completion
indicates the last one is memory-visible, we don't need to do anything
else to know that the ones before are visible as well.

The comm layer does not specify remote completions on native atomic
operations.  The only remote completions the comm layer uses are those
associated with the delivery of SENDs into the AM handler's AM request
buffer.  The implication is that forcing the effect of a native atomic
operation on its target variable to be visible would require following
that operation with a blocking AM of some sort.  Since the comm layer
doesn't do that, strictly speaking it is not enforcing MCM conformance
for native atomic operations.  In other words, we're just getting
lucky.  We'll have more to say about this in the section on
memory-order-fence MCM mode, below.

### Program Order

Chapel task creation and task waiting create a conceptual tree of
program statements.  The task bodies, task creation, and task wait
operations create a partial order _<<sub>p</sub>_ of program statements.
For the purposes of this section, the statements in the body of each
Chapel task will be implemented in terms of *load*, *store*, and *atomic
operations*.

- If we have a program snippet without tasks, such as `X; Y;`, where
  _X_ and _Y_ are memory operations, then _X <<sub>p</sub> Y_.

- The program `X; begin{Y}; Z;` implies _X <<sub>p</sub> Y_.  However,
  there is no particular relationship between _Y_ and _Z_ in program
  order.

  #### Implementation Notes

  The compiler arranges (currently via `_upEndCount()` in the module
  code) to call `chpl_comm_task_create()` when parent tasks create
  local child tasks, so that the parent's prior operations can be
  fenced.  There is a related requirement to fence prior operations
  before a task does an on-stmt to a remote node, but the comm layer can
  handle that itself.

- The program `t = begin{Y}; waitFor(t); Z;` implies _Y <<sub>p</sub>
  Z_.

  #### Implementation Notes

  The compiler arranges (currently via `_downEndCount()` in the module
  code) to call `chpl_comm_task_end()` when tasks are ending, so that
  their prior operations can be fenced.  There is a related need to
  fence prior operations at the end of an on-stmt to a remote node, but
  the comm layer can handle that itself.

- _X <<sub>p</sub> Y_ and _Y <<sub>p</sub> Z_ imply _X <<sub>p</sub>
  Z_.

### Memory Order

The memory order _<<sub>m</sub>_ of SC atomic operations in a given
task respects program order as follows:

- If _A<sub>sc</sub>(a) <<sub>p</sub> A<sub>sc</sub>(b)_ then
  _A<sub>sc</sub>(a) <<sub>m</sub> A<sub>sc</sub>(b)_

  #### Implementation Notes

  ##### AM-Mediated Atomics

  The comm layer uses blocking AMs for atomic operations whether or not
  the task has a bound transmit endpoint, and whether the operation is
  fetching or non-fetching.  If the transmit endpoint isn't bound and/or
  the atomic operation is fetching, the task will wait for the blocking
  AM's 'done' indicator in the usual way, before proceeding.  But for a
  non-fetching atomic on a bound transmit endpoint, the task continues
  executing after initiating the AM, and only waits for the AM 'done'
  indicator before the next operation with MCM implications.

  ##### Native Atomics

  Atomic operations implemented natively in libfabric are ordered either
  by using `FI_DELIVERY_COMPLETE` completion semantics, or through some
  form of message ordering.  It is worth noting here that the comm layer
  assumes message ordering settings (at least when applied to both the
  transmit and receive endpoints, as it does) impose order not only on
  the operations but also on the visibility of their effects on their
  target variables.  However, the libfabric man pages don't seem to say
  that anywhere.

---

Every SC atomic operation gets its value from the last SC atomic
operation before it to the same address in the total order
_<<sub>m</sub>_:

- Value of _A<sub>sc</sub>(a)_ = Value of _max<sub><<sub>m</sub></sub>
  (A<sub>sc</sub>'(a)|A<sub>sc</sub>'(a) <<sub>m</sub>
  A<sub>sc</sub>(a))_

---

For data-race-free programs, every load gets its value from the last
store before it to the same address in the total order _<<sub>m</sub>_:

- Value of _L(a)_ = Value of _max<sub><<sub>m</sub></sub> (S(a)|S(a)
  <<sub>m</sub> L(a)_ or _S(a) <<sub>p</sub> L(a))_

  #### Implementation Notes

  The requirement here is just that if an earlier regular store can
  dangle, it must be forced into visibility before the later regular
  load is initiated.  A store can only dangle if it is done natively
  (not via AM) in the absence of delivery-complete.  For tasks with
  bound transmit endpoints, the use of read-after-write message ordering
  means that later loads remain ordered after earlier stores.

  This nevertheless leaves several cases outstanding, all of which
  involve stores followed by loads of the same address by the same
  task (semantically, anyway), but where the two operations are, or at
  least may be, done via different transmit endpoints:
  - tasks not bound to transmit endpoints,
  - a store in a parent task followed by a load in a subsequently
    created child task,
  - a store in a task followed by a load in some other task that waited
    for the first task to terminate, and
  - a store before an on-statement followed by a load in that
    on-statement's body.

  Currently we handle all but the last of these by the simple yet costly
  expedient of doing a same-node dummy RMA GET after the store(s),
  though with bound transmit contexts we delay initiating that RMA GET
  until some other activity requires it.  So for example, if we do a
  series of RMA PUTs and then create a child task, we only do a single
  RMA GET, right before creating that child.

  Stores followed by loads in on-statements are dealt with by asserting
  send-after-write message ordering.

  **_Note (Bug):_** We currently use RMA GETs to enforce order on prior
    native atomic operations.  This is probably a bug, however, because
    with provider(s) which can do native atomic operations we don't
    assert RMA read after atomic message ordering.  We haven't seen any
    problems with this, but for strict correctness we instead may need
    to something that generates a remote completion, such as a blocking
    no-op AM.

---

For data-race-free programs, loads and stores are ordered with SC
atomics.  That is, loads and stores for a given task are in total order
_<<sub>m</sub>_ respecting the following rules which preserve the order
of loads and stores relative to SC atomic operations:

- If _L(a) <<sub>p</sub> A<sub>sc</sub>(b)_ then _L(a) <<sub>m</sub>
  A<sub>sc</sub>(b)_

- If _S(a) <<sub>p</sub> A<sub>sc</sub>(b)_ then _S(a) <<sub>m</sub>
  A<sub>sc</sub>(b)_

- If _A<sub>sc</sub>(a) <<sub>p</sub> L(b)_ then _A<sub>sc</sub>(a)
  <<sub>m</sub> L(b)_

- If _A<sub>sc</sub>(a) <<sub>p</sub> S(b)_ then _A<sub>sc</sub>(a)
  <<sub>m</sub> S(b)_

  #### Implementation Notes

  These clauses are similar to the previous ones, but in terms of the
  implementation they have to do with visibility at the boundaries
  between regular memory references and atomic ones or vice-versa.  As
  with the others, it is dangling stores that we have to be concerned
  with.

  If a regular stores can dangle they must be forced into visibility
  before any later atomic is initiated.  As described previously, the
  current implementation achieves this using dummy regular GETs, perhaps
  with delayed completion waiting.

  Conversely, if an atomic store can dangle it must be forced into
  visibility before any later RMA is initiated.  At present this is done
  using RMA GETs, just like the regular-store case, along with either
  explicit or assumed message ordering.  See the discussions below of
  the message-order and message-order-fence MCM modes for further
  details

---

For data-race-free programs, loads and stores preserve sequential
program behavior.  That is, loads and stores to the same address in a
given task are in the order _<<sub>m</sub>_ respecting the following rules
which preserve sequential program behavior:

- If _L(a) <<sub>p</sub> L'(a)_ then _L(a) <<sub>m</sub> L'(a)_

- If _L(a) <<sub>p</sub> S(a)_ then _L(a) <<sub>m</sub> S(a)_

- If _S(a) <<sub>p</sub> S'(a)_ then _S(a) <<sub>m</sub> S'(a)_

  #### Implementation Notes

  These impose the same requirements on the implementation, and have the
  same solution(s), as the previous clause about same-address
  references, above.

### The Comm Layer's MCM Conformance Modes

The comm layer currently has three MCM conformance "modes", meaning ways
that it can operate in order to ensure conformance with the Chapel
Memory Consistency Model.  These are described in the following
sections, from most-preferred to least-preferred based on performance
expectations.

The comm layer always requests the following capabilities when searching
for providers:

    FI_MSG
    FI_MULTI_RECV
    FI_RMA
    FI_LOCAL_COMM
    FI_REMOTE_COMM

It also requires send-after-send (`FI_ORDER_SAS`) message ordering.
This is done so that message SEND operations for Active Messages are not
reordered.  However, as discussed previously we believe this may be
overkill given how the comm layer actually uses SEND operations.

It then makes three successive attempts with additional capabilities,
message orderings, and other settings as described below, in order to
set the MCM mode.

#### Message-order-fence MCM Mode

In message-order-fence mode the comm layer uses a combination of message
ordering settings and fenced operations to achieve MCM conformance.  It
uses the provider's default completion level, but sets the following
capabilities:

    FI_ATOMIC
    FI_FENCE

and the following message orderings:

    FI_ORDER_ATOMIC_RAW
    FI_ORDER_ATOMIC_WAR
    FI_ORDER_ATOMIC_WAW

Note that there is no explicitly asserted ordering here between RMA and
atomic operations.  That is achieved by by fenced operations and their
completions. 

In this mode, the comm layer uses fenced operations to force the memory
effects of prior RMA writes and native atomic operations to be visible,
when that is needed.  With a bound transmit context we can wait to do
this on an as-needed basis across the same transmit-receive endpoint
pair, but in any other case we have to do it immediately after the
operation that may create the dangling store.

#### Message-order MCM Mode

In message-order mode the comm layer also takes the provider's default
completion level, but adds the following message orderings to force
message sends and all (RMA or native atomic) reads and writes to be
ordered with respect to RMA and native atomic writes:

    FI_ORDER_RAW
    FI_ORDER_WAW
    FI_ORDER_SAW
    FI_ORDER_SAS

However, note that the orderings with respect to native atomics have no
real effect, because we haven't encountered a provider that can match
the requirements here but not those of message-order-fence mode, and the
latter is selected by preference.

In this mode, the comm layer uses RMA reads to force the memory effects
of prior RMA writes and native atomic operations to be visible, when
that is needed.  As with memory-order-fence mode, with a bound transmit
context we can wait to do this on an as-needed basis across the same
transmit-receive endpoint pair, but in any other case we have to do it
immediately after the operation that may create the dangling store.

#### Delivery-complete MCM Mode

This is a fallback mode, because in effect it ends up causing full
network round trips for every operation and that makes it slow.  In this
mode the comm layer asserts the `FI_DELIVERY_COMPLETE` completion
level on all transmit operations.  Since operations can be implicitly
ordered simply by waiting on the completions, there is no need to force
message orderings.

#### MCM Modes in the Implementation Sources

The comm layer has a few cases of in-line control flow based on the MCM
mode being used, but the main way the MCM mode adjusts comm layer
behavior is through so-called _selector_ functions for Active Message
requests, PUTs, GETs, and native atomic operations.  For each of these
there is a conceptually generic function that performs that kind of
operation independent of the MCM mode being used.  That function does
mode-independent setup and tear-down activities, but for the operation
itself it calls the selector which in turn calls a mode-specific
_implementor_ function which does the actual work.  As an aid to
navigating in the comm layer implementation, the following table
summarizes the functions involved:

| operation | generic function | selector function | implementor function |
|:----|:----|:----|:---- |
| AM request | amRequestCommon() | amReqFn_selector() | amReqFn_msgOrdFence() |
| | | | amReqFn_msgOrd() |
| | | | amReqFn_dlvrCmplt() |
| PUT (write) | ofi_put() | rmaPutFn_selector() | rmaPutFn_msgOrdFence() |
| | | | rmaPutFn_msgOrd() |
| | | | rmaPutFn_dlvrCmplt() |
| GET (read) | ofi_get() | rmaGettFn_selector() | rmaGettFn_msgOrdFence() |
| | | | rmaGetFn_msgOrd() |
| | | | rmaGetFn_dlvrCmplt() |
| atomic | ofi_amo() | amoFn_selector() | amoFn_msgOrdFence() |
| | | | amoFn_msgOrd() |
| | | | amoFn_dlvrCmplt() |

