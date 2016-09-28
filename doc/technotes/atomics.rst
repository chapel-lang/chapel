.. _readme-atomics:

.. default-domain:: chpl

===========================
Runtime Support for Atomics
===========================

The following information is meant to describe the underlying
runtime support for Chapel's Atomic Variables.

For more information on Atomic Variables refer to the :ref:`Chapel
Language Specification <chapel-spec>`, or for a list of available functions on
Atomics see :mod:`Atomics`

For code examples using atomics, see the
:ref:`atomics.chpl <primers-atomics>` primer.

--------
Overview
--------

Atomic variables are a built-in type that support predefined atomic
operations.  Currently, Chapel supports processor-provided atomic
operations on ``bool``, as well as all sizes of ``int``,  ``uint``,
and ``real`` for most backend compilers (see :ref:`readme-chplenv`
for the current list of supported compilers.) Initial support for
network-provided atomic operations is also available. See the
:ref:`platform-specific <platforms-index>` documentation to check if
network-based atomics are available for your platform

The choice of supported atomic variable types as well as the atomic
operations were strongly influenced by the C11 standard. A notable
exception is that Chapel supports atomic
fetch-and-add/fetch-and-subtract operations on ``real`` types as well.

The specific implementation of atomics can be selected via the
environment variable ``CHPL_ATOMICS``.  Similar to the other Chapel
environment variables, an appropriate default is chosen when not
specified, and not all implementations are available for all
settings.  Chapel currently supports three atomics implementations:
``cstdlib``, ``intrinsics`` and ``locks``. This environment variable
also specifies the atomic implementation used by the Chapel runtime.

If compiler support for atomics is available, the atomic operations
will be mapped down the appropriate compiler intrinsics which often
map directly to processor atomics.  If intrinsics are not available,
the atomic implementation defaults to using locks in the form of
Chapel's sync vars. As a result the locks implementation will be
slower than the intrinsic implementation. Since Chapel's atomics
were modeled after the C11 edition of the C standard, the cstdlib
implementation is just a wrapper around C standard atomics.  As C11
support becomes more prevalent and reliable, cstdlib will become the
default in some configurations.

Currently, unless using network atomics, all remote atomic
operations will result in the calling task effectively migrating to
the locale on which the atomic variable was allocated and performing
the atomic operations locally.

If supported, the network atomics implementation can be selected via
the environment variable ``CHPL_NETWORK_ATOMICS``. If set, all
variables declared to be atomic will use the specified network's
atomic operations. It is possible to override this default by using
the undocumented internal function ``chpl__processorAtomicType()``
defined in ``$CHPL_HOME/modules/internal/Atomics.chpl``. Over time
we will add a more principled way for explicitly requesting
processor atomics, and this function may disappear.


For more information about the runtime implementation see
``$CHPL_HOME/runtime/include/atomics/README``.


------------------
Memory Order Notes
------------------

As mentioned in the spec, most atomic operations optionally take a
memory order. However, for the intrinsics and locks implementations,
this argument is ignored. The resulting effect is that all atomic
operations are performed with ``memory_order_seq_cst`` (sequentially
consistent) regardless of the actual order specified. The reason for
this is because the compiler intrinsics used in the runtime have no
way to specify memory order.

The cstdlib implementation uses the specified memory order.


-----------------------------
Variances from the C standard
-----------------------------

While Chapel atomics are modeled after the C standard there are some
notable differences. The primary one is that Chapel supports
fetch-and-add/fetch-and-subtract operations for ``real`` types. It
should be noted that since there is virtually no hardware support for
floating point atomics, our implementation is not very efficient.

As noted in the spec there a few additional methods in Chapel that
are not in C11. They are ``peek``, ``poke``, and ``waitFor``.
``peek`` and ``poke`` are supposed to be relaxed versions of read
and write that allow users to perform reads and writes with more
relaxed memory constraints.  Currently they are implemented as reads
and writes with ``memory_order_relaxed``. ``waitFor`` is a method that
waits until an atomic object has a specific value.  It can yield to
other tasks while waiting.

Chapel currently does not support the memory fences or the
``isLockFree`` method from the C11 spec. They are defined in the
runtime but not in the modules. The primary reason that
``isLockFree`` is not available is that it may not be accurate for
the intrinsics. Without examining each intrinsic operation for each
compiler it is hard to know if they actually map down to lock free
operations. ``threadFence`` and ``signalFence`` are also in the
runtime but not in the modules. The primary reason for this is that
there is no need for them with the intrinsics or locks
implementations, where all our operations use
``memory_order_seq_cst``. They will be added for use with the cstdlib
implementation. The fences are used with other memory_orders to allow
you to create safe programs when atomic operations are using non
sequential memory orders.


-----------
Open issues
-----------

- Atomic bools are only supported for the default size and not
  implemented for all sizes of bools.

- The ``memory_order`` is currently ignored by the intrinsics and locks
  implementations.

- The threadFence and signalFence methods need to be made available
  for use with nonsequential memory orders.


---------------------
Additional References
---------------------

- See the section titled "Memory Consistency Model" in the Chapel
  Language Specification for more information on memory orders and
  Chapel's memory consistency model.
