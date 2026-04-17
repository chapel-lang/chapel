.. _readme-optimization:

=========================================
Optimizing Performance of Chapel Programs
=========================================

One of the benefits of the Chapel programming language is that it makes
it easy to get started writing parallel and distributed programs.
Features including distributed arrays, implicit communication, and global
view make it easier to go from a serial program, to a parallel program,
to a distributed-memory parallel program. However, these same features
can also create performance problems. It takes additional work to go from
a distributed-memory parallel program to one that performs well. This
document provides a guide of sorts along the way. It will highlight the
issues that one needs to be aware of when optimizing the performance of a
Chapel program and talk about how the performance issues can be resolved.

.. note::

   This document is focused on how Chapel users can improve the
   performance of their applications. There are many potential areas for
   improvement in the compiler, runtime, and standard library that can
   also improve the performance of Chapel programs. In fact, some
   optimizations to the Chapel programming system have made benchmarks
   run many times faster without needing any change to the application
   source code at all! These are important techniques as well, but they
   are out of scope for this document.

.. note::

   This document is a draft and a work in progress. Updates to it based
   on experience with optimization are welcome!


A Few Words About Process
-------------------------

Before we dive into optimization techniques, let's briefly discuss some
good programming practices when working with Chapel. Optimization efforts
will involve changing the code, and optimizations that cause the program
to stop working correctly aren't any use at all. It needs to be easy to
check that everything is working.

Revision Control
~~~~~~~~~~~~~~~~

It's important to use revision control tools (e.g. ``git``). Revision
control helps with optimization specifically:

 * You can go back to an earlier version if your changes break the
   program or make it slower

 * You can record your progress towards improving performance

Testing
~~~~~~~

It's important to have tests that are easy to run in order to check that
your program is working correctly.

We have used the following steps when testing a new change to a parallel
and distributed Chapel application:

 * Compile with checks enabled (i.e. without ``--fast``)

   * Test with 1 locale and ``--dataParTasksPerLocale=1``
     (``dataParTasksPerLocale`` is described in :ref:`data_parallel_knobs`)

   * Gradually increase ``--dataParTasksPerLocale`` until you are
     satisfied it is working. At a minimum, try leaving this argument off
     to run with number of tasks == number of cores.

   * Test while simulating multiple locales on your development system
     (see the next section)

     Gradually increase the number of locales and the number of tasks
     until you are satisfied it is working.

You can compile with ``--fast`` and measure performance before or after
this process. If you are targeting distributed memory, it's ideal to test
performance on a multi-node system. Occasionally, it's useful to simulate
multiple locales on a laptop (described below). Be aware that the
performance of such a simulation won't necessarily match a real
multi-node system.

.. _optimization-oversubscribed:

Simulating Multiple Locales on a Laptop
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

It's possible to run multiple processes on your laptop or workstation
that simulate the communication that happens on a big multi-node
system. This is also called running **locally oversubscribed**.

In this setting, the program will generally run much slower than it would
when running in a multi-node system. As a result, you might need to run
with a smaller problem size to test in this way. Nonetheless, it is
possible to identify and optimize communication. Communication occurs in
Chapel programs when using ``on`` statements and also when reading or
writing data (such as array elements) that are stored on a remote locale.

In communication-bound applications, optimizing the communication can
make the locally oversubscribed program run faster.  Another important
technique is, when working locally oversubscribed, try to reduce the
communication counts (communication counts are discussed below).  The
communication counts will match (or come close to matching) between
running locally oversubscribed and running on a multi-node system with
the same number of locales.  Significant reductions in communication
counts will generally lead to improvements in the scalability of your
application on multiple nodes.

To run locally oversubscribed, build Chapel with GASNet with the UDP
conduit. Note that GASNet with the UDP conduit is not a high-performing
configuration; in this setting that is OK because the communication
overhead of UDP will approximate the communication overhead on a real
multi-node system. You can use these commands to build with the UDP
conduit:

  .. code-block:: bash

    export CHPL_COMM=gasnet
    export CHPL_COMM_SUBSTRATE=udp
    make

Then, run with these settings:

  .. code-block:: bash

    export CHPL_RT_OVERSUBSCRIBED=yes
    export GASNET_SPAWNFN=L
    export CHPL_RT_MASTERIP=127.0.0.1

You might also wish to run with a lower number of threads per simulated
locale in order to reduce interference between the processes. For
example, if you are working on a system with 32 cores, you might run
with ``CHPL_RT_NUM_THREADS_PER_LOCALE=8 ./application -nl 4``. That
uses 8 threads per locale, with 4 locales, so a total of 32 threads.

See also :ref:`num-threads-per-locale`,
:ref:`oversubscribed-execution`, :ref:`readme-gasnet-emulating-multilocale`,
and :ref:`using-udp`.


Measuring Performance
~~~~~~~~~~~~~~~~~~~~~

Reproducible Performance Tests
  It's important to have performance tests that are easy to run that you
  can use to measure your program's performance. These performance test
  cases need to be easy to repeat. Usually, that means setting aside some
  problem configuration and input data to use as the main performance
  test case. In some cases, you will need a battery of performance
  tests instead of a single test case.

Timing Regions
  A common practice is to use :chpl:record:`~Time.stopwatch` to time
  different regions of your program.  This coarse-grained timing
  information indicates where your program is spending the most time. The
  longest-running regions are the places where optimization will give the
  most improvement to the program's performance. You can print out the
  performance of each region after it is timed. It's a good idea to use a
  ``config const`` to make the timing only print when you are studying
  performance.

Counting Communication Events
  If you need to do most of your work by running locally oversubscribed
  (see :ref:`optimization-oversubscribed`) then it's particularly
  useful to measure performance by counting the number of communication
  events. Counting communication events can also be useful as a secondary
  performance metric for multi-node runs. Reducing the number of
  communication events is a useful strategy for optimizing communication.

  You can use the :chpl:mod:`CommDiagnostics` module and
  ``startCommDiagnostics()`` followed by ``getCommDiagnostics()`` to
  count communication events. It's common to print out the communication
  counts with ``writeln(getCommDiagnostics())``.

  When measuring communication counts, it's important to be aware of
  these best practices:

    * Compile with ``--fast`` to avoid communication events related to
      error checking

    * Communication count information will be easier to understand if you
      compile with ``--no-cache-remote`` because the cache for remote
      data, which is enabled by default, will cause the counts to include
      more categories.

    * Counting communication events can slow down execution time
      significantly. As a result, it's a good practice to control
      communication diagnostics with a ``config const`` and to measure
      timing only when communication counting is off.

  Communication counts provide a way to compare communication performance
  independent of where you are running. For example, you might measure
  and seek to reduce the communication counts when running oversubscribed
  on a laptop or workstation. Significant reductions in communication
  counts from such an effort should also help with multi-node performance
  when you are able to run on a big system.


Overall Flow
~~~~~~~~~~~~

The general flow of performance optimization will be:

 * use techniques like timing the different phases of your computation to
   try to understand which parts are the slow or not getting faster as you
   add more locales or cores
 * come up with a theory as to what could be improved by thinking about
   what is happening when your program is running and/or by looking at
   timing or communication count information
 * make an adjustment (the adjustment only needs to be good enough to
   measure its impact on performance at this step)
 * measure its impact on performance
 * if it is good, clean up the adjustment, perform correctness testing,
   and commit/PR it to your program's revision control; if not, try
   something else
 * repeat

Configuration Matters
---------------------

The first thing to check is that you are using Chapel in its
highest-performing configuration for your system.

 * use ``--fast`` when compiling Chapel programs for performance to tell
   the compiler to optimize instead of adding correctness checks

 * use the default configuration rather than the quickstart configuration

   .. note::

     You can show the current configuration with ``printchplenv`` or
     ``chpl --print-chpl-settings``

   * In particular, ``CHPL_TASKS=qthreads`` is generally faster than
     ``CHPL_TASKS=fifo``, and ``CHPL_TARGET_MEM=jemalloc`` is generally
     faster than ``CHPL_TARGET_MEM=cstdlib`` (see also
     :ref:`readme-chplenv.CHPL_TASKS` and
     :ref:`readme-chplenv.CHPL_TARGET_MEM`)

   * ``CHPL_TARGET_COMPILER=llvm`` might or might not be faster than
     using the C backend with something like
     ``CHPL_TARGET_COMPILER=gnu``; however the performance of the LLVM
     backend is more reliable (see also
     :ref:`readme-chplenv.CHPL_COMPILER`)

     * if you are using ``CHPL_LLVM=system``, it's a good idea to match the
       version of LLVM bundled in the Chapel release if possible as this
       has recieved the most attention and testing (see also
       :ref:`readme-chplenv.CHPL_LLVM`)

 * For multi-locale programs, use a high-performance networking configuration

   * The following configurations support portability and correctness
     testing but **are NOT designed for high performance**:

     * ``CHPL_COMM=gasnet`` ``CHPL_COMM_SUBSTRATE=udp``

     * ``CHPL_COMM=gasnet`` ``CHPL_COMM_SUBSTRATE=mpi``

   * High-performance networking configuration will depend on your
     system:

     * For HPE Cray EX, use ``CHPL_COMM=ofi`` (see also :ref:`readme-cray`)
     * For InfiniBand systems, use ``CHPL_COMM=gasnet``
       ``CHPL_COMM_SUBSTRATE=ibv`` (see also :ref:`readme-infiniband`)
     * For Omni-Path systems, use ``CHPL_COMM=gasnet`` and
       ``CHPL_COMM_SUBSTRATE=ofi`` (see also :ref:`readme-omnipath`)

 * If you are not using multiple locales, set ``CHPL_COMM=none`` or
   compile your application with ``--local`` for the best performance
   (see :ref:`optimization-wide-pointers`).

Settings to Adjust to Improve Performance
-----------------------------------------

This section contains some easy things to try in order to improve
performance.

``--fast``
  If you haven't been using ``--fast`` yet please do! It should be used
  when measuring performance. Since it disables bounds checking, make
  sure that your development flow includes correctness tests that aren't
  compiled with ``--fast``.

``--no-ieee-float`` / ``--ieee-float``
  By default, only floating point optimizations that are relatively
  benign are enabled. Depending on your application, you might use
  ``--no-ieee-float`` to enable optimizations that might impact the
  numerical accuracy. Or, if your program relies on floating point
  operations happening in the order written for numerical accuracy, you
  should use ``--ieee-float``.

colocales
  In some settings, running with multiple colocales per node can improve
  performance. For example, to run on 8 nodes with 2 processes per node,
  you could use ``-nl 8x2``. That will result in 16 locales; where each
  node has 2 locales. Using colocales can help with memory bandwidth on
  NUMA systems and also can better use the networking resources on a node
  to help to make communication more efficient.

``--auto-aggregation``
  This compiler flag enables an optimization that automatically uses
  :chpl:mod:`CopyAggregation` to improve multilocale performance. It is
  not on by default because it can slow down some applications. In
  particular, this optimization can be beneficial if your application has
  small forall loops that do a lot of random remote accesses, but if the
  forall loops do mostly local accesses, it can hurt performance. See also
  :ref:`optimization-fine-comm` which discusses the problem that
  aggregators solve.

``--no-cache-remote`` / ``--cache-remote``
  The cache for remote data is a runtime component that helps to reduce
  fine-grained communication. It is enabled by default, but in some
  cases, an application will run faster with it disabled. It is also
  usually a good idea to disable it when investigating the sources
  of communication as the communication logs are simpler when it is
  disabled.


``CHPL_TARGET_CPU``
  Using ``native`` or the CPU family that you are targeting, rather than
  ``none`` or ``unknown``, can allow using newer instruction sets (e.g.
  AVX512) and improve performance.

Network-specific communication settings

  Please see the documentation for the network that you are using for
  more details on what can be adjusted. Some specific variables you might
  consider:

    * :ref:`CHPL_RT_COMM_OFI_DEDICATED_AMH_CORES <readme-libfabric-CHPL_RT_COMM_OFI_DEDICATED_AMH_CORES>`
      when using ``CHPL_COMM=ofi`` can be used to dedicate a core to
      service requests that arrive over the network

    * :ref:`CHPL_RT_COMM_GASNET_DEDICATED_PROGRESS_CORE <readme-infiniband-CHPL_RT_COMM_GASNET_DEDICATED_PROGRESS_CORE>`
      when using ``CHPL_COMM=gasnet`` with ``CHPL_COMM_SUBSTRATE=ibv``
      offers a similar capability for InfiniBand

..
  comment: cover ``--llvm-wide-opt`` when it becomes less experemental

Fundamental Issues
------------------

This section covers issues that are fundamental to the Chapel programming
model. As a result, people optimizing Chapel programs should be aware of
them.

.. _optimization-accidental-comm:

Accidental Communication
~~~~~~~~~~~~~~~~~~~~~~~~

The Chapel programming model supports implicit communication in order to
make it easier to write distributed-memory programs. While those first
distributed-memory programs are easier to write, they might include
accidental communication. The accidental communication can be a big
barrier to scalability because it's frequently accessing the same memory
on one node repeatedly.

Addressing accidental communication consists of two parts. First, the
accidental communication needs to be identified. Second, the code needs
to be modified to avoid the accidental communication.

Here are a few strategies to identify accidental communication:

 1. Use ``local`` blocks (see also :ref:`readme-local`). ``local`` blocks
    are an unstable feature that instructs the compiler that there
    should be no communication within the code in that block, including
    in functions called from within the local block. When the program is
    compiled with ``--fast --local-checks`` (or with the default of full
    checking), the compiler will emit code to halt if code running in a
    ``local`` block needs to communicate. If you have compiled with
    :ref:`CHPL_UNWIND != none <readme-chplenv.CHPL_UNWIND>`, you can
    even see the stack trace for the code which caused communication you
    did not expect.

    ``local`` blocks have a secondary advantage of allowing the compiler
    to optimize: it optimizes assuming that all code in ``local`` blocks does
    not communicate if you compile with ``--fast``.

    Note that ``local`` blocks can slow down compilation significantly if
    not used sparingly. If compilation time is important, consider using
    them as a debugging tool and then removing to improve compilation speed.

 2. Use :chpl:mod:`CommDiagnostics` on-the-fly reporting.  The
    :chpl:mod:`CommDiagnostics` module provides mechanisms for
    on-the-fly reporting with ``startVerboseComm()``.  This on-the-fly
    reporting can even include stack traces if you compile with
    ``-scommDiagsStacktrace=true`` and have built Chapel with
    :ref:`CHPL_UNWIND != none <readme-chplenv.CHPL_UNWIND>`. The
    on-the-fly reporting provides a relatively easy way to see what
    communication events are common in your program. It can be a lot of
    output though.  This strategy works reasonably well for finding
    accidental communication that is a performance problem because, if
    accidental communication is happening in a key performance-critical
    inner loop, verbose comms reporting will report on that accidental
    communication many times.  With some light scripting to process the
    verbose comms reporting, you can identify which parts of the code
    have the most communication.

Here are some strategies you can use to adjust your code to avoid
accidental communication:

 1. If the value being accidentally communicated can be stored in a
    variable outside of the distributed loop or if it can be stored in a
    module-scope variable, storing it in a ``const`` variable can enable
    a key compiler optimization called *remote value forwarding*. This
    optimization allows the compiler to move the value of the variable
    along with the message sent to a remote locale to set up work.
    However, it only works if the compiler can prove that the value will
    not change. ``const`` helps because it indicates to the compiler
    that the value won't change. For example:

    .. code-block:: chapel

      {
        var A = blockDist.createArray(...);
        var x = 22;
        forall elt in A {
          elt = x;  // uh-oh, x might be read remotely on each iteration!
        }
        const y = 22;
        forall elt in A {
          elt = y;  // expect the value of 'y' to be sent along with tasks
        }
      }

    .. note::

      Actually, in this specific example, ``x`` will be copied to each
      task implementing the first ``forall`` loop due to
      :ref:`Forall_Intents`. The unnecessary communication described
      could occur if ``x`` were a record.

 2. If the accidental communication is within a distributed ``forall``
    loop, you can change it from being once per iteration to once per
    task by using the ``in`` intent. For example:

    .. code-block:: chapel

       var A = blockDist.createArray(...);
       var x = 22;
       forall elt in A {
         elt = x;  // uh-oh, x might be read remotely on each iteration!
       }
       forall elt in A with (in x) {
         elt = x;  // ah, now x is only read once per task, at least
       }

 3. If the code is using a structure like ``coforall loc in Locales``,
    you can create a temporary local variable to store a local copy of
    the variable. For example:

    .. code-block:: chapel

       var x = [1,2,3,4];
       coforall loc in Locales {
         on loc {
           var myX = x;
           f(myX); // do something with myX
         }
       }

.. _optimization-wide-pointers:

Wide Pointer Overhead
~~~~~~~~~~~~~~~~~~~~~

The Chapel compiler will emit code working with pointers in many cases
(for array elements, references, class instances, ...). When the Chapel
compiler is compiling for multiple locales and it is unable to prove that
a pointer is local, it will emit a wide pointer. The wide pointer encodes
an address along with a locale where the value is stored. In cases where
the code is working with local memory but the compiler can't prove that,
there will be additional overhead due to the code working with a wide
pointer.

It is relatively easy to detect if this is a performance problem for a
Chapel program because it has a pretty clear symptom that you can see by
compiling two different ways and running on 1 locale. Measure the
performance of your program compiled with ``CHPL_COMM=none`` and/or
``--local``. Compare that performance with the performance of your
program with ``CHPL_COMM`` other than ``none`` and/or with
``--no-local``. You are seeing wide pointer overhead if the ``--local``
version is significantly faster than the ``--no-local`` version.

What can be done about it?

 * use ``local`` blocks to tell the compiler that code within a block
   will not communicate. That allows it to remove wide pointers for that
   code. See also :ref:`optimization-accidental-comm`.
 * use ``localSubdomain`` to compute the local index set as a local domain
 * use ``localSlice`` to compute a non-distributed array slice that refers
   to the local portion
 * compile with ``--report-auto-local-access`` to see which local array
   accesses are optimized by the compiler. Use ``localAccess`` for
   accesses that aren't optimized.


   .. note::

      A bit more about the Automatic Local Access (ALA) optimization

      The Chapel compiler can optimize distributed array accesses inside
      ``forall`` loops if it can determine that they can be always local.
      Here are some cases that it can optimize:

      .. code-block:: chapel

        var A, B: [D] int;
        var OtherDomain = ...

        forall i in A.domain { ... A[i] ... }
        forall i in B.domain { ... A[i] ... }
        forall i in D { ... A[i] ... }
        forall i in D.expand(-1) { ... A[i] ... }
        forall i in OtherDomain { ... A[i] ... } // with some execution time checks


      This optimization can also be effective when Stencil operations
      when Stencil-distributed arrays are used:

      .. code-block:: chapel

        use StencilDist;

        var A = stencilDist.createArray({0..<n}, fluff=(1,), int);

        forall i in A.domain { ... A[i-1] ... A[i] ... A[i+1] ... }

      The following compiler flags can be used to investigate and modify
      ALA behavior

        * ``--report-auto-local-access``: Generates a report showing
          which accesses are optimized and which are not, with some
          explanation as to what caused the final decision for the
          optimization.

        * ``--no-auto-local-access``: Disables the optimization. We are not
          aware of a case where ALA hurts performance. There could be a
          slight improvement in compilation time by disabling the
          optimization, but we haven't observed it to have a significant
          impact.

        * ``--no-dynamic-auto-local-access``: Disables the dynamic portion
          of the optimization. In some cases, the compiler can introduce
          loop clones, where the executed clone will be chosen at
          execution time depending on some dynamic checks. This behavior
          can be turned off with this flag, where the optimization will
          be effective only when it can be proven statically

.. _optimization-fine-comm:

Fine-Grained Communication
~~~~~~~~~~~~~~~~~~~~~~~~~~

It's easy to write Chapel programs that use fine-grained communication.
Such programs will work with many small messages and the performance will
be sensitive to the latency of a message in the network. To make such
programs perform and scale better, try to get the communication to use
larger messages so that the network is operating more in a
bandwidth-bound way rather than a latency-bound way.

Let's look at an example of permuting an array. Suppose that we have
three distributed arrays all over the distributed domain ``D``:

 * ``In`` stores input values

 * ``Idx`` stores a permutation; each index in ``D`` occurs exactly once
   in ``Idx``.

 * ``Out`` stores the result of permuting ``In`` according to ``Idx``

In particular, we will set ``Out[Idx[i]] = In[i];`` for each ``i`` in ``D``.

If we write that in the simplest way, we get this code:

.. literalinclude:: ../../../test/technotes/optimization/aggregation.chpl
   :language: chapel
   :start-after: START_UNOPT
   :end-before: END_UNOPT

However that code will do fine-grained PUT operations on the network;
``Out[Idx[i]]`` will refer to the array element to store into, and since
that array element is typically stored on a different locale, the
compiler will generate a PUT operation for ``Out[Idx[i]] = In[i]``.

We can optimize this by using the :chpl:mod:`CopyAggregation` module.
Here is the optimized version:

.. literalinclude:: ../../../test/technotes/optimization/aggregation.chpl
   :language: chapel
   :start-after: START_OPT
   :end-before: END_OPT

In this case, the optimization required two changes to the program:

 1. Create a copy aggregator per-task. Here, we use the ``forall`` intent
    ``with`` clause to create a ``DstAggregator`` per task. The
    ``DstAggregator`` will optimize the PUT operations by combining PUT
    operations destined for the same locale.

 2. Use the copy aggregator instead of plain old assignment. The call
    ``agg.copy(X, Y)`` is functionally similar to ``X = Y``.

How much does optimizing this example improve performance? With a quick
test on 16 nodes, we can see (through use of the
:chpl:mod:`CommDiagnostics` module) that the number of PUTs is
approximately 1000 times less with the optimized version. By timing the
relevant loop, we can see that the performance is approximately 30 times
better. In this simple case, with the ``--auto-aggregation`` flag, the
compiler can even make these adjustments automatically.

Also note that the :chpl:mod:`CopyAggregation` module used here is most
applicable when working with random access. If you are copying contiguous
regions of arrays, using an optimized slice assignment can perform well
as well to perform bulk communication. See :ref:`optimization-slices` for
some important caveats.

Load Imbalance
~~~~~~~~~~~~~~

The term *load imbalance* describes a situation where a parallel program
won't run as fast as it could because the parallel work is not evenly
divided up among the hardware elements doing the work. As a contrived
example of load imbalance, suppose you have 4 locales, and locale 0 does
4 times as much work as the other locales. Locales 1, 2, and 3 will spend
most of their time waiting for locale 0, which is not ideal.

Load imbalance can be a particularly problematic issue when working with
a parallel computation that has different phases. For Chapel programs,
these phases are probably represented by different parallel loops or with
barriers. If one task in a parallel region (such as a ``forall`` or
``coforall`` loop) takes a significantly longer time than the other
tasks, the time the loop takes will be this long time, since the program
cannot proceed with the code after the loop until all of the tasks have
completed.

How can load imbalance be identified?

 * Within a parallel region, different tasks are taking very different
   amounts of time
 * When profiling, the program spends a lot of time waiting for tasks to
   complete or waiting for barriers

How can load imbalance be addressed?

 * There are often ways to improve the algorthim to address load
   imbalance. For example, graph partitioners are an important technology
   that can help one balance the storage of a data structure and the
   computation that goes with it. More generally, you might be able to
   rearrange the data and the computation to be performed so that when
   working with the new arrangement there is better load balance.

 * In some cases, :chpl:mod:`DynamicIters` might help. You can use it to
   write parallel loops that dynamically load balance.


Current Issues
--------------

This section contains issues that, ideally, the Chapel compiler and
runtime would address. However, as they may come up in practice, it's
important to be aware of them and their workarounds.

Distributed Array Field Access Can Result in Unnecessary Communication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Issue: https://github.com/chapel-lang/chapel/issues/10160

When a class contains a field that is a distributed array, a distributed
``forall`` loop using a class instance will generate communication in
order to read the field's value (repeatedly) even though the field's
value should be a privatized distributed array.

This issue can be avoided by creating a ``ref`` or ``const ref`` that
refers to the distributed array. This ``ref`` or ``const ref`` can be
created outside of the ``forall`` loop and reused within it to avoid the
problem.

.. _optimization-slices:

Creating Too Many Distributed Objects / Unoptimized Slice Assignments
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Issue: https://github.com/chapel-lang/chapel/issues/16133

In principle, distributed array creation does some work on each locale. As a
result, it's not going to go faster when adding more locales. That can
cause performance or scaling issues if a program tries to create too many
distributed arrays.

One aspect of the current implementation is that array slices of
distributed arrays are also distributed objects and work is required on
each locale to create one. That means that creating a slice, as in
``MyDistributedArray[1..10]`` can actually be quite slow.

One common scenario is that data needs to be copied between regions of
arrays. For example
``MyDistributedArray[1..10] = MyOtherArray[11..20];``.
The compiler can optimize this kind of
assignment in many cases today with Array View Elision (AVE). However, in
order to optimize it in this way, both sides of the ``=`` need to be
slice expressions. Note that in some cases, one might create a local
array to store the contents of a remote slice; that can be done with AVE
by redundantly slicing the local array, e.g.:
``var Loc:[1..10] int; Loc[1..10] = MyDistributedArray[1..10];```

Unoptimized Distributions
~~~~~~~~~~~~~~~~~~~~~~~~~

Issue: https://github.com/chapel-lang/chapel/issues/27334

The Chapel programming language is designed to support many distributions
for domains and arrays. That includes distributions created by users.
However, the present situation is that performance of Chapel programs
depends on optimizations in the implementations of these domain/array
distributions. These optimizations are present and reasonably well tuned
for the :chpl:mod:`BlockDist` distribution. Other distributions might not
be optimized and have scaling issues.

Performance Problems with Multidimensional Zippered Iteration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Issue: https://github.com/chapel-lang/chapel/issues/13147

Zippered iteration for multidimensional arrays/domains is much slower
than zippered iteration for 1D arrays/domains. Since promoted calls, such
as ``MyArray + MyOtherArray`` are implemented with zippered iteration,
this problem also applies to that case.

Potential ways to avoid this problem:

 * use loops over a multidimensional domain that avoid zippering
 * express the computation with nested loops per dimension
 * use 1D arrays and explicitly compute 1D indices from 2D conceptual indices
 * create a 1D copy of the array (with ``reshape`` -- note that in the
   future, we expect to have a way to ``reshape`` without copying)

Cooperative Scheduling and Remote Tasks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Issue: https://github.com/chapel-lang/chapel/issues/27332

Chapel's tasking model currently uses cooperative scheduling. That means
that, once a task starts running on a core, it will be the only thing
running on that core until the task either yields execution or ends.

Occasionally, this can lead to problems when combined with ``on``
statements. The ``on`` statements generally create a task on a remote
locale. Those remote tasks will never get a chance to run if all of the
cores are busy with existing tasks. This problem is rare, but it can
cause performance issues if it comes up. The solution is to
periodically call :ref:`currentTask.yieldExecution() <Yield_Task_Execution>`
from any polling loops. This is already done in
``waitFor()`` methods on atomics (see also
:ref:`Functions_on_Atomic_Variables`).

Tools for Understanding Performance
-----------------------------------

.. note::

   This section is under construction. Contributions are welcome!

Tools for Understanding Communication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Several tools are available to help optimize communication in order to
improve the distributed-memory scalability of your program.

``local`` blocks
  ``local`` blocks are an unstable feature that instructs the compiler
  to assume that there will be no communication in the body of the
  ``local`` block. Moreover, if compiling with ``--checks`` (generally
  the case without ``--fast``) the program will halt if code in a
  ``local`` block requires communication. As a result, ``local`` blocks
  are useful both as an optimization tool and a way of discovering
  unintended communication.

:chpl:mod:`CommDiagnostics` on-the-fly reporting
  The :chpl:mod:`CommDiagnostics` module provides mechanisms for
  on-the-fly reporting with ``startVerboseComm()``.  This on-the-fly
  reporting can even include stack traces if you compile with
  ``-scommDiagsStacktrace=true`` and have built Chapel with
  ``CHPL_UNWIND != none`` (see :ref:`readme-chplenv.CHPL_UNWIND`).
  The on-the-fly reporting provides a relatively easy way to see what
  communication events are common in your program. It can be a lot of output,
  though.

:chpl:mod:`CommDiagnostics` comm counting
  :chpl:mod:`CommDiagnostics` also provides a way to count communication
  events. Note that the communication count information will be easier to
  understand if you compile with ``--no-cache-remote`` since that will
  reduce the number of categories of events. Comm counts
  information provide a way to compare communication performance
  independent of where you are running. For example, you might measure
  and seek to reduce the communication counts when running oversubscribed
  on a laptop or workstation. Reductions in communication counts from
  such an effort should also help with multi-node performance when you
  are able to run on a big system.

..
   comment: consider adding

   Tools for Understanding Single-Node Performance

   This section still needs to be written. Some ideas for what to
   include:

     * comparing against other implementations
     * timing different regions
     * using a profiler
     * how to look at LLVM IR or assembly for a function
