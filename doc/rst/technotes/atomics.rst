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
``cstdlib`` provides the best performance. For information on how the
default is chosen see :ref:`readme-chplenv.CHPL_ATOMICS`.

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


---------------------
Additional References
---------------------

- See the section titled "Memory Consistency Model" in the Chapel
  Language Specification for more information on memory orders and
  Chapel's memory consistency model.
