:title: Users Guide: Task Parallelism Overview

Task Parallelism Overview
=========================

Characterizing Chapel Tasks
---------------------------

In Chapel, a *task* is a unit of computation that can and should be
executed in parallel with other tasks.  Tasks are the basis of all
parallel execution within Chapel.

Chapel programs begin execution as a single task.  As the program
runs, additional tasks can be created to introduce parallel
computation.  Chapel does not automatically parallelize programs,
which is to say, tasks are not automatically created on the
programmer's behalf.  Rather, the programmer must create all tasks,
either directly (using the constructs described in this
:ref:`index-task-parallelism` section), or indirectly
(through the use of :ref:`index-data-parallelism`
abstractions that create the tasks).

A Chapel task may be short-lived, or it may run for the program's
duration.  It may be defined via a single statement or using as much
code as is required to specify its behavior.  Tasks may also be
*nested*, meaning that tasks may themselves create additional tasks.

Chapel tasks may share data with other tasks through traditional
lexical scoping.  Unlike many task-parallel programming models, Chapel
programs are not guaranteed to be correct if tasks are executed
serially and to completion.  Specifically, Chapel tasks may
synchronize with one another.  They may also block or busy-wait while
waiting for actions to be taken by other tasks.  When tasks do not
synchronize with one another correctly, Chapel programs may deadlock
or livelock.  Tasks may also race with one another, either in benign
or problematic ways.


Task Parallelism vs. Data Parallelism
-------------------------------------

Chapel uses the term *task parallel* to describe a coding style in
which tasks are explicitly specified to perform a parallel computation
(e.g., "This task should do this thing while a second task does this
other thing."  Or "These *k* tasks should all do this thing.").  This
is in contrast to *data parallel* programming, in which parallelism is
specified by computations on collections of indices or data.


Tasks vs. Threads
-----------------

We tend to avoid the term *thread* in defining the Chapel language.
In Chapel, threads are system-level resources (e.g., POSIX threads)
that execute a program's tasks.  How tasks are mapped to threads is
not defined by the Chapel language. A Chapel implementation may
support distinct *tasking layers*, each with its own policies for
executing tasks on the target system's resources.  For detailed
information about the tasking layers available in our implementation
of Chapel, refer to :ref:`readme-tasks`.


"Will My Tasks Execute in Parallel?"
------------------------------------

Practically speaking, whether or not a task actually *does* run in
parallel with other tasks depends on:

* the number of simultaneous tasks created by the Chapel program,
* the available system resources, and
* details of the runtime library's tasking implementation which are
  beyond the scope of this introductory description.

Generally speaking, if the number of concurrently executing tasks does
not exceed the number of available processors in the target hardware,
the tasks will be executed in parallel.  So, if you are running on a
4-core laptop and never have more than four tasks running at any given
time, all your tasks should run in parallel.

When the number of tasks created by a Chapel program exceeds the
amount of hardware parallelism available, tasks will typically be
multiplexed across the available hardware over time.  The specifics
depend on how tasks are implemented by the runtime library; again,
see :ref:`readme-tasks` for further details if you're curious to
understand task scheduling in more detail.
