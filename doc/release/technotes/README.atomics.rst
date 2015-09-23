.. _readme-atomics:

===========================================
Current implementation for atomic variables
===========================================

This README describes current implementation of atomic variables in Chapel.

--------
Overview
--------

Atomic variables are variables that support atomic operations.
Currently, Chapel supports processor-provided atomic operations on
bools, all supported sizes of signed and unsigned integer types, and
real(32) and real(64) for several backend compilers (see README.chplenv
for the current list of supported compilers.) Initial support for
network-provided atomic operations is also available. See the
appropriate README for a particular platform to see if network-based
atomics are available.

The choice of supported atomic variable types as well as the atomic
operations were strongly influenced by the C11 standard. A notable
exception is that Chapel supports atomics for reals as well.

The specific implementation of atomics can be selected via the
environment variable CHPL_ATOMICS.  Similar to the other Chapel
environment variables, an appropriate default is chosen when not
specified, and not all implementations are available for all settings.
Chapel currently supports two atomics implementations: intrinsics and
locks. This environment variable also specifies the atomic
implementation used by the Chapel runtime.

If compiler support for atomics is available, the atomic operations
will be mapped down the appropriate compiler intrinsics which often
map directly to processor atomics.  If intrinsics are not available,
the atomic implementation defaults to using locks in the form of
Chapel's sync vars. As a result the locks implementation will be
slower than the intrinsic implementation. Since Chapel's atomics were
modeled after the C11 standard, as C11 support becomes more prevalent,
we will support an implementation that maps to the C11 atomics.

Currently, unless using network atomics, all remote atomic operations
will result in the calling task effectively migrating to the locale on
which the atomic variable was allocated and performing the atomic
operations locally.

If supported, the network atomics implementation can be selected via
the environment variable ``CHPL_NETWORK_ATOMICS``. If set, all variables
declared to be atomic will use the specified network's atomic
operations. It is possible to override this default by using the
explicit type name for the processor-based atomics (available in the
type function ``chpl__atomicType()`` defined in the file
``$CHPL_HOME/modules/internal/Atomics.chpl``).

For more information about the runtime implementation see
``$CHPL_HOME/runtime/include/atomics/README``.

To see a full list of available functions, see
``$CHPL_HOME/modules/internal/Atomics.chpl``.


------------------
Memory Order Notes
------------------

As mentioned in the spec, most atomic operations optionally take a
memory order. However for the current implementations this argument is
ignored in the runtime. The resulting effect is that all atomic
operations are performed with memory_order_seq_cst (sequentially
consistent) regardless of the actual order specified. The reason for
this is because the compiler intrinsics used in the runtime have no
way to specify memory order and locks memory order is bound by the
locking and unlocking of sync vars.

We expect to begin using the specified memory order with a C11
implementation of the atomics.

-------------------------------
Variances from the C11 standard
-------------------------------

While Chapel atomics are modeled after the C11 standard there are some
notable differences. The primary one is that Chapel supports atomics for
real types. It should be noted that since there is virtually no hardware
support for floating point atomics our implementation is not very
efficient.

As noted in the spec there a few additional methods in Chapel that are
not in C11. They are peek, poke, and waitFor. Peek and poke are supposed
to be relaxed versions of read and write that allow users to perform
reads and writes with more relaxed memory constraints. Currently they
are implemented as reads and writes with memory_order_relaxed but since
our implementations ignore memory_order, they are semantically the same
as read and write. waitFor is method that waits until an atomic value
has a specific value.  It can yield to other tasks while waiting.

Chapel currently does not support the memory fences and isLockFree
methods from the C11 spec. They are defined in the runtime but not in
the modules. The primary reason that isLockFree is not available is that
it may not be accurate for the intrinsics. Without examining each
intrinsic operation for each compiler it is hard to know if they
actually map down to lock free operations. threadFence and signalFence
are also in the runtime but not in the modules. The primary reason for
this is that there is no need for them with the current implementation.
All our operations use memory_order_seq_cst. The fences are used with
other memory_orders to allow you to create safe programs when atomic
operations are using non sequential memory orders.


-----------
Open issues
-----------

Below are a few of the known open issues with Chapel atomics.

- Atomic bools are only supported for the default size and not
  implemented for all sizes of bools.

- The memory_order is currently ignored by all implementations.

- Chapel's memory consistency model is still a work-in-progress.


--------
Feedback
--------

If you have any questions regarding atomics in Chapel, please send
mail to chapel_info@cray.com.
