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
   performance of their applications today. There are many potential
   areas for improvement in the compiler, runtime, and standard library
   that can also improve the performance of Chapel programs. In fact,
   some optimizations to the Chapel programming system have made
   benchmarks run many times faster without needing any change to the
   source code at all! These are important techniques as well, but they
   are out of scope for this document.

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

I use the following steps when testing a new change to a parallel and
distributed Chapel application:

 * Compile with checking enabled (i.e. without ``--fast``)

   * Test with 1 locale and `--dataParTasksPerLocale=1`

   * Gradually increase ``--dataParTasksPerLocale`` until you are
     satisfied it is working. At a minimum, try leaving this argument off
     to run with number of tasks == number of cores.

   * Test with multiple locales locally (e.g. with
     ``CHPL_COMM=gasnet`` and ``CHPL_COMM_SEGMENT=udp``).
     Gradually increase the number of locales and the number of tasks
     until you are satisfied it is working.

   * Run on a real distributed-memory system and measure it's
     performance. See the below section for hints as to how to use a
     performant configuration of Chapel!

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

Overall Flow
~~~~~~~~~~~~

The general flow of performance optimization will be:

 * come up with a theory as to what could be improved
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

 * use ``--fast`` when compiling Chapel programs for performance

 * use the default configuration rather than the quickstart configuration

   * In particular, ``CHPL_TASKS=qthreads`` is generally faster than
     ``CHPL_TASKS=fifo``, and ``CHPL_TARGET_MEM=jemalloc`` is generally
     faster than ``CHPL_TARGET_MEM=cstdlib``

   * ``CHPL_TARGET_COMPILER=llvm`` might or might not be faster than
     using the C backend with something like
     ``CHPL_TARGET_COMPILER=gnu``; however the performance of the LLVM
     backend is more reliable

     * if you are using a system LLVM, it's a good idea to match the
       version of LLVM bundled in the Chapel release if possible as this
       has recieved the most attention and testing

 * For multi-locale programs, use a high-performance networking configuration

   * The following configurations support portability and correctness
     testing but **are NOT designed for high performance**:

     * ``CHPL_COMM=gasnet`` ``CHPL_COMM_SUBSTRATE=udp``

     * ``CHPL_COMM=gasnet`` ``CHPL_COMM_SUBSTRATE=mpi``

   * High-performance networking configuration will depend on your
     system:

     * For HPE Cray EX, use ``CHPL_COMM=ofi``
     * For InfiniBand systems, use ``CHPL_COMM=gasnet``
       ``CHPL_COMM_SUBSTRATE=ibv``
     * For Omni-Path systems, use ``CHPL_COMM=gasnet`` and
       ``CHPL_COMM_SUBSTRATE=ofi``.


Settings to Adjust to Improve Performance
-----------------------------------------

This section contains some easy things to try in order to improve
performance.

``--fast`` : If you haven't been using ``--fast`` yet please do!

``--no-ieee-float`` / ``--ieee-float`` : By default, only floating point
optimizations that are relatively benign are enabled. Depending on your
application, you might use ``--no-ieee-float`` to enable optimizations
that might impact the numerical accuracy. Or, if your program relies on
floating point operations happening in the order written for numerical
accuracy, you should use ``--ieee-float``.

colocales : In some settings, running with multiple colocales per node
can improve performance. For example, to run on 8 nodes with 2 processes
per node, you could use ``-nl 8x2``. That will result in 16 locales;
where each node has 2 locales. Using colocales can help with memory
bandwidth on NUMA systems and also can better use the networking
resources on a node to help to make communication more efficient.

``--auto-aggregation`` : This compiler flag enables an optimization that
automatically uses aggregators to improve multilocale performance.

``--no-cache-remote`` / ``--cache-remote`` : The cache for remote data is
a runtime component that helps to reduce fine-grained communication. It
is enabled by default, but in some cases, an application will run faster
with it disabled.

``CHPL_TARGET_CPU`` : Using ``native`` or the CPU family that you are
targeting, rather than ``none`` or ``unknown``, can allow using newer
instruction sets (e.g. AVX512) and improve performance.

..
  comment: cover ``--llvm-wide-opt`` when it becomes less experemental

Fundamental Issues
------------------

This section covers issues that are fundamental to the Chapel programming
model. As a result, people optimizing Chapel programs should be aware of
them.

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

TODO TODO

Fine-Grained Communication
~~~~~~~~~~~~~~~~~~~~~~~~~~

Load Imbalance
~~~~~~~~~~~~~~


Current Issues
--------------

This section contains issues that, ideally, the Chapel compiler and
runtime would address. However, as they may come up in practice, it's
important to be aware of them and their workarounds.

Cooperative Scheduling and Remote Tasks
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

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

Performance Problems with Multidimensional Zippered Iteration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Issue: https://github.com/chapel-lang/chapel/issues/13147)

Zippered iteration for multidimensional arrays/domains is much slower
than zippered iteration for 1D arrays/domains. Since promoted calls, such
as ``MyArray + MyOtherArray`` are implemented with zippered iteration,
this problem also applies to that case.

Potential ways to avoid this problem:
 * express the computation with nested loops per dimension
 * use 1D arrays and explicitly compute 1D indices from 2D conceptual indices
 * create a 1D copy of the array (with ``reshape`` -- note that in the
   future, we expect to have a way to ``reshape`` without copying)


Tools for Understanding Performance
-----------------------------------

Tools for Understanding Communication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Strategies for Understanding Communication

Tools for Understanding Single-Node Performance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


