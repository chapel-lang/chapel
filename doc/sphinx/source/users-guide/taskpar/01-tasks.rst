:title: Users Guide: Defining Tasks

Defining Tasks
==============

In Chapel, a "task" is a unit of computation that can and should be
executed in parallel with other tasks.  The Chapel program starts
execution as a single task.  As the program runs, additional tasks can
be created to execute in parallel.  Chapel does not automatically
parallelize a program, which is to say, tasks are not automatically
created on the programmer's behalf.  Rather, the programmer must
create all tasks, either explicitly using the constructs in this "Task
Parallelism" section, or implicitly through the use of abstractions
that themselves create tasks.

A Chapel task may be short-lived or it may run for the program's
duration.  It may be defined via a single statement or using as much
code as is required to specify its behavior.  Tasks may be "nested"
meaning that tasks may themselves create additional tasks.

We use the term "task parallel" to describe a coding style in which
the user explicitly creates tasks (e.g., "this task should do this
thing while this other task does this other thing."  Or "These 'k'
tasks should all do this thing.").  This is in contrast to "data
parallel" programming, in which collections of indices or data are
used to specify the parallelism in a computation.

Chapel tasks may share data and synchronize with one another.  They
may block or busy-wait while waiting for actions to be taken by other
tasks, and as a result, Chapel programs can deadlock or livelock.
Tasks may also race with one another, either in benign or problematic
ways.  Chapel strives for generality over safety with its task
parallel features.

When talking about the Chapel language, we tend to avoid the term
"thread" for the most part.  In Chapel, threads are system-level
resources that execute the programmer's tasks (e.g., POSIX threads).
How tasks are mapped down to threads is not defined by the Chapel
language, and implementations may have many different "tasking
layers", each with its own policies for executing tasks on a compute
node's processors.  Generally speaking, though, tasks will be run in
parallel by a system's threads, either using multiple processor cores,
or by multiplexing tasks on a single processor core, or a combination
of both.
