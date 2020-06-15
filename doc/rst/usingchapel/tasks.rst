.. _readme-tasks:

============
Chapel Tasks
============

.. contents::

--------
Overview
--------

Chapel programs create new tasks via the begin, cobegin, and coforall
statements.  Tasks are computations that can conceptually execute
concurrently, though they may or may not do so in practice.

An implementation of Chapel must include at least one tasking layer.
A tasking layer will in turn implement *threads* which are a mechanism
for executing work in parallel.

All tasking layers support configuration constants to control system
resources such as the number of threads that are available to execute
tasks and the amount of call stack space reserved for each task.  Generally
speaking, the Chapel programmer can make no assumptions about the
scheduling of threads or the mapping of tasks to threads other than
those semantics defined by the language specification.

This document describes the currently-supported tasking options in more
detail.  The rest of this document includes:

* an overview of the different tasking options
* a detailed description of each tasking option
* a discussion of the number of threads used by each tasking option
* a discussion of call stack sizes and overflow handling
* a list of tasking-related methods on the locale type
* a brief description of future directions for the tasking layer

If you have questions about tasks that are not covered in the following,
please send them to :disguise:`chapel_info@cray.com`.


--------------------------
Task Implementation Layers
--------------------------

This release contains two distinct tasking layers for Chapel tasks.
The user can select between these options by setting the ``CHPL_TASKS``
environment variable to one of the following values:

:qthreads:
  best performance; default for most targets

:fifo:
  most portable, but heavyweight; default for NetBSD and Cygwin

Each tasking layer is described in more detail below:


CHPL_TASKS == qthreads
----------------------

Chapel's default tasking layer implementation for most targets is based
on the Qthreads user-level threading package from Sandia National Labs.
This provides a lightweight implementation of Chapel tasking as well as
an optimized implementation of sync variables.  To use qthreads tasking,
please take the following steps:

1) Ensure that the environment variable ``CHPL_HOME`` points to the
   top-level Chapel directory.

2) Set up your environment to use Qthreads:

   ensure ``CHPL_TASKS`` is not set (if qthreads is the default)

   -- or --

   .. code-block:: sh

     export CHPL_TASKS=qthreads

3) Follow the instructions in :ref:`readme-chplenv` to set up,
   compile and run your Chapel programs.

Please report any apparent bugs in Qthreads tasking to the Chapel team.


Stack overflow detection
========================

The qthreads tasking implementation can arrange to halt programs when
any task overflows its call stack (see `Task Call Stacks`_).
It does this by placing a guard page, which cannot be referenced, at the
end of each task stack.  When a task tries to extend its stack onto a
guard page, it fails with a segfault.

Normally guard pages for stack overflow detection are configured and
enabled.  There is a performance cost for this, however.  We do not have
a quantitative estimate for this cost, but it is a fixed overhead (a
couple of system calls) added to the time needed to run every task, so
qualitatively speaking it will have a greater effect on programs which
create more or shorter-lived tasks than on programs which create fewer
or longer-lived ones.

As described in `Task Call Stacks`_, the execution-time default for
stack overflow checking can be set by using the ``--[no-]stack-checks``
compiler option.  But whatever the default is, at execution time stack
overflow detection can be turned off by setting the environment variable
``QT_GUARD_PAGES`` to any of the values "0", "no", or "false", or on by
setting it to any of "1", "yes", or "true".  When it is off the execution
overhead is negligible (just a couple of scalar tests).  Developers
who wish to remove even this small cost can disable guard pages by
building qthreads with guard pages entirely configured out, as follows:

.. code-block:: sh

  cd $CHPL_HOME/third-party/qthread
  make CHPL_QTHREAD_NO_GUARD_PAGES=yes ... clean all

As noted, running without guard pages can improve performance and thus
may be desirable for production work.  However, if this is done, test
runs at similar scale with guard pages turned on to check for stack
overflow should be done beforehand if possible, because undetected stack
overflows can cause subtle and intermittent errors in execution.


Environment variables
=====================

Qthreads provides a number of environment variables that can be used to
configure its behavior at execution time.  An introduction to these can
be found in the ENVIRONMENT section of the qthread_init man page.  (Note
that although this man page documents variables named ``QTHREAD_*``, each
variable is actually present in both ``QT_*`` and ``QTHREAD_*`` forms,
with the former superseding the latter.)  The qthreads man pages are
available by means of the man -M option, for example:

.. code-block:: sh

  man -M $CHPL_HOME/third-party/qthread/qthread-src/man qthread_init

Note that in some cases there are Chapel environment variables that
override Qthreads counterparts. ``CHPL_RT_NUM_THREADS_PER_LOCALE`` overrides
``QT_HWPAR``, for example.  Whenever a Chapel variable overrides a Qthreads
variable, you should use the Chapel one.


Worker affinity and number
==========================

Simplistically, there are two kinds of threads in Qthreads: shepherds
that manage work distribution, and workers that host qthreads (Chapel
tasks, for our purposes).  The execution-time environment variable
``QT_WORKER_UNIT`` controls how worker threads are distributed on hardware
processors.  The default is "core" to distribute workers across CPU
cores (physical processors).  An alternative is "pu", which distributes
workers across processing units.  These are instances of the processor
architecture, or hardware threads if the cores have those.  Note that
"pu" will be automatically selected if ``CHPL_RT_NUM_THREADS_PER_LOCALE``
is set to anything larger than the number of cores, so it usually isn't
necessary to set ``QT_WORKER_UNIT``.


.. _overloading-with-qthreads:

Overloading system nodes
========================

By default the qthreads tasking implementation is set up to assume that
its process is not competing with anything else for system resources
(CPUs and memory) on its system node.  In this mode, qthreads optimizes
its internal behavior to favor performance over load balancing.  This
works out well for Chapel programs, because normally Chapel runs with
one process (locale) per system node.  However, with ``CHPL_COMM=gasnet``
or ``CHPL_COMM=ofi`` one can run multiple Chapel locales on a single
system node, say for doing multilocale functional correctness testing
with limited system resources.  (See :ref:`readme-multilocale` for more
details.)  When this is done qthreads' optimization for performance can
greatly reduce performance, due to resource starvation among multiple
Chapel processes.  If you need qthreads to share system resources more
cooperatively with other processes set ``CHPL_RT_OVERSUBSCRIBED=yes`` at
execution time (see :ref:`oversubscribed-execution`).


Hwloc
=====

When ``CHPL_TASKS=qthreads``, the default for ``CHPL_HWLOC`` becomes "hwloc",
and the hwloc third-party package will be built.  Qthreads depends on
this package to provide it with a description of the locale hardware, to
support locality and affinity operations.


Further information
===================

For more information on Qthreads, see $CHPL_HOME/third-party/README.


CHPL_TASKS == fifo
------------------

FIFO tasking over POSIX threads (or pthreads) works on all
platforms and is the default for Cygwin and NetBSD. It is
attractive in its portability, though on most platforms it will
tend to be heavier weight than Chapel strictly requires.  FIFO
tasking is also used when Chapel is configured in 'Quick Start'
mode (see :ref:`chapelhome-quickstart`).  To use FIFO tasking,
please take the following steps:

1) Ensure that the environment variable ``CHPL_HOME`` points to the
   top-level Chapel directory.

2) Set up your environment to use FIFO tasking:

   .. code-block:: sh

     export CHPL_TASKS=fifo

3) Follow the instructions in :ref:`readme-chplenv` to set up,
   compile and run your Chapel programs.

In the FIFO tasking implementation, Chapel tasks are mapped to threads
such that each task is executed by a single thread and is run to
completion before giving up that thread.  As a result, a program can
have no more tasks active (that is, created and started) at any given
time than it has threads on which to run those tasks.  It can create
more tasks than threads, but no more tasks will be run at any time
than there are threads.  Excess tasks are placed in a pool where they
will be picked up and started by threads as they complete their tasks.

The threading implementation uses POSIX threads (pthreads) to run Chapel
tasks.  Because pthreads are relatively expensive to create, it does not
destroy them when there are no tasks for them to execute.  Instead they
stay around and continue to check the task pool for tasks to execute.
Setting the number of pthreads is described in `Controlling the Number of Threads`_.


Stack overflow detection
========================

The fifo tasking implementation can arrange to halt programs when any
task overflows its call stack (see `Task Call Stacks`_).  It does
this by placing a guard page, which cannot be referenced, at the end of
each task stack.  When a task tries to extend its stack onto a guard
page, it fails with a segfault.

This feature is enabled in fifo tasking and cannot currently be turned
off.  There is a performance cost for it, which we expect to be small in
most cases.  We do not have a quantitative estimate for this cost, but
it is a fixed overhead (a couple of system calls) added to the time
needed to start each pthread.  Since the pthreads in fifo tasking are
long-lived and can host many tasks over their lifespan, on a per-task
basis we don't expect stack overflow detection to be expensive.


---------------------------------
Controlling the Number of Threads
---------------------------------

The number of threads per compute node used to implement a Chapel
program can be controlled by the ``CHPL_RT_NUM_THREADS_PER_LOCALE``
environment variable.  This may be set to either an explicit number
or one of the following symbolic strings:

  :'MAX_PHYSICAL': number of physical CPUs (cores) on the node
  :'MAX_LOGICAL':  number of logical CPUs (hyperthreads) on the node

If ``CHPL_RT_NUM_THREADS_PER_LOCALE`` is not set, the number of threads is
left up to the tasking layer.  See the case-by-case discussions below
for more details.

The Chapel program will generate an error if the requested number of
threads per locale is too large.  For example, when running multi-locale
programs, the GASNet communication layer typically places an upper bound
of 127 or 255 on the number of threads per locale (There are ways to
work around this assumption on certain platforms -- please contact us at
:disguise:`chapel_info@cray.com` or peruse the GASNet documentation if you need
to do so.)

CHPL_TASKS == fifo
------------------
  The value of ``CHPL_RT_NUM_THREADS_PER_LOCALE`` indicates the maximum
  number of threads that the fifo tasking layer can create on each
  locale to execute tasks.  These threads are created on a demand-driven
  basis, so a program with a small number of concurrent tasks may never
  create the specified number.  If the value is zero, then the number of
  threads will be limited by system resources and other constraints
  (such as GASNet's configuration-time limit).

  The value of ``CHPL_RT_NUM_THREADS_PER_LOCALE`` can have a major impact on
  performance for fifo tasking.  For programs with few inter-task
  dependences and high computational intensity, setting it roughly equal
  to the number of physical CPUs on each locale can lead to near-optimal
  performance.  However, for programs with lots of fine-grained
  synchronization in which tasks frequently block on sync or single
  variables, ``CHPL_RT_NUM_THREADS_PER_LOCALE`` can often exceed the number
  of physical CPUs without an adverse effect on performance since
  blocked threads will not consume the CPU's cycles.

  Note that setting ``CHPL_RT_NUM_THREADS_PER_LOCALE`` too low can result in
  program deadlock for fifo tasking.  For example, for programs written
  with an assumption that some minimum number of tasks are executing
  concurrently, setting ``CHPL_RT_NUM_THREADS_PER_LOCALE`` lower than this
  can result in deadlock if there are not enough threads to implement
  all of the required tasks.  The ``-b/--blockreport`` flag can help debug
  programs like this that appear to be deadlocked.

CHPL_TASKS == qthreads
----------------------
  In the Qthreads tasking layer, ``CHPL_RT_NUM_THREADS_PER_LOCALE``
  specifies the number of system threads used to execute tasks.  The
  default is to use a number of threads equal to the number of physical
  CPUs on the locale.

----------------
Task Call Stacks
----------------

Each task including the main Chapel program has an associated call
stack.  As documented in :ref:`readme-executing`, the ``CHPL_RT_CALL_STACK_SIZE``
environment variable can be used to specify how big these call stacks
will be during execution.  See there for a full description of this
environment variable and the values it can take.

When a task's call chain becomes so deep that it needs more space than
the size of its call stack, stack overflow occurs.  Whether or not a
program checks for stack overflow checking at execution time can be
specified when it is compiled, via the ``--[no-]stack-checks`` compilation
option.  The compile-time default is ``--stack-checks``; ``--no-stack-checks``
can be given directly, and is also implied by ``--no-checks``, which in turn
is implied by ``--fast``.  By default stack overflow checks are enabled.

Chapel does not yet have a consistent, implementation-independent way to
deal with call stack overflow.  Each tasking layer implementation
handles stacks and stack overflow in its own way, as described below.

CHPL_TASKS == fifo
------------------
  In fifo tasking, Chapel tasks use their host pthreads' stacks when
  executing.  If stack checks are enabled, these stacks are created with
  an additional memory page called a "guard page" beyond their end, that
  is marked so that it cannot be referenced.  When stack overflow occurs
  the task's attempt to reference the guard page will cause the OS to
  react as it usually does when bad memory references are done.  On
  Linux, for example, it will kill the program with this message:

    Segmentation fault

  Unfortunately, many other things that cause improper memory references
  result in this same kind of program termination, so as a diagnostic it
  is ambiguous.  However, it does at least prevent the program from
  continuing on in an erroneous state.

CHPL_TASKS == qthreads
----------------------
  Like fifo tasks (see above), qthreads tasking can place guard pages
  beyond the ends of task stacks.  Stack overflow then results in the
  system's usual response to referencing memory that cannot be reached.
  With qthreads tasking, the compiler ``--stack-checks`` setting specifies
  the default setting for execution-time stack overflow checking.  Final
  control over stack overflow checks is provided by the ``QT_GUARD_PAGE``
  environment variable.  See the qthreads subsection of `Task
  Implementation Layers`_ for more information.

----------------------------------------------
Task-Related Quantification Methods on Locales
----------------------------------------------

Several methods on the locale type are available to query the state of
the program with respect to its tasks.

  queuedTasks()
    returns the number of tasks that are ready to run, but
    have not yet begun executing.

  runningTasks()
    returns the number of tasks that have begun executing,
    but have not yet finished.  Note that this number can exceed the
    number of non-idle threads because there are cases in which a thread
    is working on more than one task.  As one example, in fifo tasking,
    when a parent task creates child tasks to execute the iterations of
    a coforall construct, the thread the parent is running on may
    temporarily suspend executing the parent task in order to help with
    the child tasks, until the construct completes.  When this occurs
    the count of running tasks can include both the parent task and a
    child, although strictly speaking only the child is executing
    instructions.

    As another example, any tasking implementation in which threads can
    switch from running one task to running another, such as qthreads,
    can have more tasks running than threads on which to run them.

  blockedTasks()
    returns the number of tasks that are blocked because
    they are waiting on a sync or single variable.  In order to avoid
    unnecessary overheads, in the implementations this method will not
    generate meaningful information unless the program was run with the
    ``-b/--blockreport`` flag.

  totalThreads()
    returns the number of threads that have been created
    since the program started executing, regardless of whether they
    are busy or idle.

  idleThreads()
    returns the number of threads are idle, i.e., not assigned to any task.

In order to use these methods, you have to specify the locale you wish
to query, as in here.runningTasks(), where 'here' is the current
locale.

(Note that these methods are available for all tasking options, but
currently only runningTasks() returns meaningful values for all options.
The others only return meaningful values for ``CHPL_TASKS=fifo``.)


-------------------------
Future Tasking Directions
-------------------------

As Chapel's task parallel implementation matures, we expect to have
multiple task->thread scheduling policies, from literally creating and
destroying new threads with each task (for programmers who want full
control over a thread's lifetime) to automated work stealing and load
balancing at the other end of the spectrum (for programmers who would
prefer not to manage threads or whose programs cannot trivially be
load balanced manually).  Our hope is to leverage existing open source
threading and task management software and to collaborate with others
in these areas, so please contact us at :disguise:`chapel_info@cray.com` if you'd
like to work with us in this area.
