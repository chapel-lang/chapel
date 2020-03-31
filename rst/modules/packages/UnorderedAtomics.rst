.. default-domain:: chpl

.. module:: UnorderedAtomics
   :synopsis: .. warning::

UnorderedAtomics
================
**Usage**

.. code-block:: chapel

   use UnorderedAtomics;


.. warning::
  This module represents work in progress. The API is unstable and likely to
  change over time.

This module provides unordered versions of non-fetching atomic operations for
all ``int``, ``uint``, and ``real`` types.  Unordered versions of
:proc:`~Atomics.add()`, :proc:`~Atomics.sub()`, :proc:`~Atomics.or()`,
:proc:`~Atomics.and()`, and :proc:`~Atomics.xor()` are provided. The results
of these functions are not visible until task or forall termination or an
explicit :proc:`unorderedAtomicTaskFence()`, but they can provide a
significant speedup for bulk atomic operations that do not require ordering:

.. code-block:: chapel

  use UnorderedAtomics;

  const numTasksPerLocale = here.maxTaskPar,
        iters = 10000;

  var a: atomic int;

  coforall loc in Locales do on loc do
    coforall 1..numTasksPerLocale do
      for i in 1..iters do
        a.unorderedAdd(i); // unordered atomic add

  // no fence required, fenced at task termination

  const itersSum = iters*(iters+1)/2, // sum from 1..iters
        numTasks = numLocales * numTasksPerLocale;
  assert(a.read() == numTasks * itersSum);

It's important to be aware that unordered atomic operations are not
consistent with regular atomic operations and updates may not be visible
until the task or forall that issued them terminates or they are explicitly
fenced with :proc:`unorderedAtomicTaskFence()`.

.. code-block:: chapel

  var a: atomic int;
  a.unorderedAdd(1);
  writeln(a);        // can print 0 or 1
  unorderedAtomicTaskFence();
  writeln(a);        // prints 1

Generally speaking they are useful for when you have a large batch of atomic
updates to perform and the order of those operations doesn't matter.

.. note::
  Currently, these are only optimized for ``CHPL_NETWORK_ATOMICS=ugni``.
  Processor atomics or any other implementation falls back to ordered
  operations. Under ugni these operations are internally buffered. When the
  buffers are flushed, the operations are performed all at once. Cray Linux
  Environment (CLE) 5.2.UP04 or newer is required for best performance. In
  our experience, unordered atomics can achieve up to a 5X performance
  improvement over ordered atomics for CLE 5.2UP04 or newer.


.. method:: proc AtomicT.unorderedAdd(value: T): void

   Unordered atomic add. 

.. method:: proc AtomicT.unorderedSub(value: T): void

   Unordered atomic sub. 

.. method:: proc AtomicT.unorderedOr(value: T): void

   Unordered atomic or. 

.. method:: proc AtomicT.unorderedAnd(value: T): void

   Unordered atomic and. 

.. method:: proc AtomicT.unorderedXor(value: T): void

   Unordered atomic xor. 

.. function:: proc unorderedAtomicTaskFence(): void

   
   Fence any pending unordered atomics issued by the current task.
   

