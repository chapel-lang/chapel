:title: Users Guide: **forall** loops

.. _ug-forall:

**forall**-loops: data-parallel loops
=====================================

The **forall-loop** is Chapel's central concept for expressing data
parallelism.  Forall-loops are similar to :ref:`coforall-loops
<ug-coforall>` in that both are parallel variants of Chapel's
:ref:`for-loop <ug-forloops>`.  However, where a coforall-loop creates
a concurrent task per iteration and a for-loop is executed serially by
a single task, a forall-loop may use an arbitrary number of tasks to
execute the loop.  As a result, it may execute serially using a single
task, or it can use any number of tasks up to the number of iterations
(or even beyond, though that is unusual).

More specifically, the actual number of tasks used to execute a
forall-loop is determined by its iterand expressions.  The iterands
specify *parallel iterators*, either directly or via expressions whose
types support default parallel iterator methods (ranges, domains, and
arrays being key examples).  These iterators typically select
the number of tasks to use based on dynamic information such as the
size of the loop and/or the number of available processors.  In
addition to specifying the degree of concurrency to use, parallel
iterators also determine where their tasks run and how the loop's
iterations will be divided between them.  These choices are referred
to as the *scheduling policy* for the loop.

In practice, the default parallel iterators for many expressions
create a number of tasks proportional to the amount of system
parallelism that they are targeting.  For example, by default a local
forall loop will typically create a number of tasks equal to the
number of processor cores available on its compute node.  In contrast,
a forall loop over a distributed array will typically create a task
per core across all of the compute nodes to which the array is
distributed.

As with coforall-loops, the original task that encounters a
forall-loop will not proceed until the tasks executing the loop's
iterations have completed.

Motivation for forall-loops
---------------------------

As a simple motivating example, consider the following serial loop
that increments every element in a million-element array *A*:

.. literalinclude:: examples/users-guide/datapar/forall.chpl
  :caption:
  :language: chapel
  :lines: 6-7,10-12
  :dedent: 2

Since the array is large and the loop's iterations are independent of
one other, it would likely be beneficial to accelerate this computation by
executing it in parallel.  A coforall-loop could be used to express
the parallelism:

.. literalinclude:: examples/users-guide/datapar/forall.chpl
  :language: chapel
  :lines: 16-17
  :dedent: 2

However, a coforall-loop would likely be overkill for this situation
since it would create one million tasks, each of which is only
responsible for incrementing a single element.  For most hardware
platforms, this would generate more concurrency than the system could
execute in parallel.  Moreover, the cost of creating, scheduling, and
destroying the tasks would likely overshadow the small amount of
computation that each one performs.  Better would be to create a
smaller number of tasks and assign a number of loop iterations to
each, as a means of amortizing the task creation overheads while
avoiding overwhelming the target system with too much concurrency.

This is precisely what forall-loops are for.  While they result in
parallel execution like coforalls, they typically use a number of
tasks that is appropriate for the system, dividing the loop's
iterations between them.  Thus, we could write this parallel loop as:

.. literalinclude:: examples/users-guide/datapar/forall.chpl
  :language: chapel
  :lines: 21-22
  :dedent: 2
   

In slightly more detail, the number of tasks used to implement a
forall-loop is determined by its iterand expression(s), in this case
the array *A*.  As declared here, *A* uses the default implementation
(*domain map*) which causes it to be allocated locally to the
compute node on which the active task is running.  The default
parallel iterator for *A* creates a number of tasks equal to the number
of available processor cores and divides the iterations evenly between
them.  For example, if this program were running on a quad-core
processor, *A*'s parallel iterator would create four tasks and assign
each of them 250,000 (a quarter) of the array elements / loop
iterations.  Note that these policy choices are simply defaults and
can be overridden by the programmer using various mechanisms
covered in subsequent sections.


Characteristics of Forall-loop Bodies
-------------------------------------

Because the number of tasks used to execute a forall-loop is not
well-defined, such loops must be *serializable*.  This means that the
loop's iterations must be written such that they can legally be
executed by a single task.  At the same time, since the loop's
iterations are likely to execute in parallel, there should be no
sensitivity to the order in which they will execute—in practice, the
iterands may schedule them in whatever order they choose.  To that
end, by using a forall-loop, the user is asserting that it is safe to
run the iterations serially or in parallel, and in any order.

Practically speaking, these characteristics mean that there can be no
synchronization dependencies between iterations of a forall-loop as
there could be in a coforall-loop; doing so could cause deadlock
depending on how the iterations are mapped to tasks.  In addition,
there should typically be no loop-carried dependences across
iterations since doing so can lead to race conditions and
nondeterministic behavior.

As a simple example of a race condition in a forall loop, consider the
following example whose output is nondeterministic in Chapel:

.. literalinclude:: examples/users-guide/datapar/forall.chpl
  :language: chapel
  :lines: 31-32
  :dedent: 2

Intuitively, it appears that the user hopes to replace each of the
interior elements of *A* with the sum of its neighbors in parallel.
However, whenever multiple tasks are used to implement the loop, there
will be a race.  Specifically, for a given iteration *i*, there is no
guarantee that the ``A[i-1]`` and ``A[i+1]`` elements it reads will
still contain the original input values of *A* as opposed to ones that
were just computed and written by another task running in parallel.
The resulting values of the array elements will therefore depend on
the number of tasks running, how the iterations are divided between
them, and the specific timing of when each iteration executes relative
to its neighbors.  In other words, the outcome is highly
unpredictable.

In contrast, if the forall-loop were executed by a single task
serially and in a specific order, it would be deterministic.  However,
it would still not implement the presumed computation since some reads
of neighboring elements would necessarily refer to values that had
already been updated.  For example, if the elements were computed in
ascending order, each access to ``A[i-1]`` would read a value
that had just overwritten the intended value in the previous iteration.

The lesson in this example is that when using a forall-loop, the
programmer asserts that the loop's iterations can execute safely in
parallel and is responsible for the outcome when they cannot.  Chapel
does not prevent nondeterministic forall loops from being written, it
merely implements the parallelism that the programmer specifies.

Here are two variations of the previous loop which *are*
parallel-safe:

.. literalinclude:: examples/users-guide/datapar/forall.chpl
  :language: chapel
  :lines: 38-39, 42-45
  :dedent: 2

In the first loop, the race condition is eliminated since the loop
only iterates over every other element of *A*.  This guarantees that
the elements of *A* being written are independent of the ones being
read and eliminates the potential for races between tasks.  In the
second loop, the overlap is eliminated by reading from *A* and writing
into a second array, *B*.  In this way, it again guarantees that no
task will read values that are potentially being written by other
tasks in parallel.  

Starting from our original array *A* which stored 1.0 in every
position, the results of *A* and *B* after executing the loops above
would be as follows (for *n* == 8):

.. literalinclude:: examples/users-guide/datapar/forall.good
  :language: text
  :lines: 3, 5

Zippered forall loops
---------------------

Like for- and coforall-loops, forall-loops support zippered iteration.
In this case, the first iterand in the zippering (called the *leader*)
controls the loop scheduling policies.  All the other iterands
(*followers*) are implemented in a way that guarantees that
corresponding iterations will line up in spite of the parallel
execution.  How such iterators are written will be covered in later
sections.
