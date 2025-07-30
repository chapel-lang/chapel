.. _readme-foreach:

====================
The `foreach` Loop
====================

A ``foreach`` loop signifies that loop iterations are order-independent. This
enables the compiler to vectorize the loop or to offload its execution to the
GPU. In contrast, a ``for`` loop is order-dependent; and a ``forall`` loop can
create parallel tasks and distribute iterations based on the iterator, as well
as being order-independent.

The syntax of the ``foreach`` loop is similar to the other ``for``-like loops:

.. literalinclude:: ../../../test/technotes/doc-examples/ForeachExamples.chpl
   :language: chapel
   :start-after: START_EXAMPLE_0
   :end-before: STOP_EXAMPLE_0
   :dedent:

Vectorizability of a ``foreach`` loop will be determined transitively based on
vectorizability of the iterator it runs over. All default range, domain and
array iterators are vectorizable (i.e. implemented using ``foreach``).
Therefore, the snippet above will be vectorized when run on a suitable
architecture.

Same transitivity applies to vectorizability of ``forall`` loops, as well.

.. literalinclude:: ../../../test/technotes/doc-examples/ForeachExamples.chpl
   :language: chapel
   :start-after: START_EXAMPLE_1
   :end-before: STOP_EXAMPLE_1
   :dedent:

The ``forall`` above will distribute the execution to all the locales and all
the tasks within them. As the default parallel iterators for ``A.domain`` is
implemented using ``foreach``, tasks executing parts of this loop will benefit
from vectorization.

Status and Future Work
----------------------

- The current implementation does not make full use of vectorization hinting.
  We hope to expand the coverage especially to vectorize outer loops.

- ``foreach`` loops support ``with`` clauses. However, the behavior for vector-
  or GPU-based execution for shadow variables may change in the future.
