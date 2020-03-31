.. default-domain:: chpl

.. module:: AllLocalesBarriers
   :synopsis: Support for a scalable global barrier between all locales.

AllLocalesBarriers
==================
**Usage**

.. code-block:: chapel

   use AllLocalesBarriers;

Support for a scalable global barrier between all locales.

This module provides a single global barrier named :var:`allLocalesBarrier` that
can be used as a barrier between all locales, optionally with multiple tasks
per locale.

The :var:`allLocalesBarrier` barrier only supports the
:proc:`~Barriers.Barrier.barrier()` and :proc:`~Barriers.Barrier.reset()`
methods of the :attr:`~Barriers.Barrier` interface. By default it can be
used as a barrier between 1 task on each locale. The
:proc:`~Barriers.Barrier.reset()` method can be used change how many tasks
per locale will participate in each barrier.

Use of this barrier is similar to ``shmem_barrier_all()`` or
``MPI_Barrier(MPI_COMM_WORLD)``, except that it's possible for multiple
tasks on the same locale to barrier. In the following examples all tasks
will print "Before barrier" before any print "After barrier"

.. code-block:: chapel

  use AllLocalesBarriers;

  // Barrier across all locales
  coforall loc in Locales do on loc {
    writeln("Before barrier");
    allLocalesBarrier.barrier();
    writeln("After barrier");
  }


  // Barrier across tasks locally before doing a barrier across locales
  const numTasksPerLocale = 4;
  allLocalesBarrier.reset(numTasksPerLocale);

  coforall loc in Locales do on loc {
    coforall tid in 1..numTasksPerLocale {
      writeln("Before barrier");
      allLocalesBarrier.barrier();
      writeln("After barrier");
    }
  }

The implementation is dependent on the communication layer and the
underlying hardware, but typically some sort of tree or dissemination based
barrier that's optimized for the network will be used.

.. data:: const allLocalesBarrier: AllLocalesBarrier = new AllLocalesBarrier(1)

