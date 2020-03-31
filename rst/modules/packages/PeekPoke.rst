.. default-domain:: chpl

.. module:: PeekPoke
   :synopsis: .. warning::

PeekPoke
========
**Usage**

.. code-block:: chapel

   use PeekPoke;


.. warning::
  This module is unstable and the API is likely to change over time.

This module provides `peek()` and `poke()` operations on atomics. `peek()`
and `poke()` are non-atomic read and write operations. They can provide a
performance improvement when local reads/writes are performed and atomicity
is not required. For example they can be used to initialize an array of
atomics or perform a reduction when no concurrent updates are occurring.

.. code-block:: chapel

  use BlockDist, PeekPoke;

  const space = {1..1000};
  const D = space dmapped Block(space);
  var A: [D] atomic int;

  forall i in D do
    A[i].poke(i);

  const sum = + reduce A.peek();
  writeln(sum); // 500500


.. method:: proc AtomicBool.peek(): bool

   
   Non-atomically reads the stored value.
   

.. method:: proc AtomicBool.poke(value: bool): void

   
   Non-atomically writes `value`.
   

.. method:: proc AtomicT.peek(): T

   
   Non-atomically reads the stored value.
   

.. method:: proc AtomicT.poke(value: T): void

   
   Non-atomically writes `value`.
   

