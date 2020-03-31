.. default-domain:: chpl

.. module:: Barriers
   :synopsis: Support for task barriers.

Barriers
========
**Usage**

.. code-block:: chapel

   use Barriers;

Support for task barriers.

The Barrier type provided in this module can be used to prevent tasks
from proceeding until all other related tasks have also reached the
barrier.

In the following example, all of the tasks created by the coforall loop
will print their "entering the barrier" messages in an unspecified order,
then all of the tasks will print their "past the barrier" messages, also
in an unspecified order.  Because of the barrier, all of the
"entering the barrier" messages will be printed before any of the
"past the barrier" messages.

.. code-block:: chapel

  use Barriers;

  config const numTasks = here.maxTaskPar;
  var b = new Barrier(numTasks);

  coforall tid in 1..numTasks {
    writeln("Task ", tid, " is entering the barrier");
    b.barrier();
    writeln("Task ", tid, " is past the barrier");
  }

Note: When a barrier instance goes out of scope it is automatically deleted.
After it is deleted, any copies of the barrier that remain are invalid.

Future direction
----------------
In the future, we expect to add more language-level support for a
"task-team" concept.  A task-team will more directly support collective
operations such as barriers between the tasks within a team.

The current implementation is designed for correctness, but is not expected
to perform well at scale.  We expect performance at scale to improve as
this barrier implementation is optimized and as the task-team concept is
implemented and optimized.

.. enum:: enum BarrierType { Atomic, Sync }

   An enumeration of the different barrier implementations.  Used to choose
   the implementation to use when constructing a new barrier object.
   
   * `BarrierType.Atomic` uses Chapel atomic variables to control the barrier.
   * `BarrierType.Sync` uses Chapel sync variables to control the barrier.
   


.. record:: Barrier

   A barrier that will cause `numTasks` to wait before proceeding. 


   .. method:: proc init(numTasks: int, barrierType: BarrierType = BarrierType.Atomic, reusable: bool = true)

      Construct a new barrier object.
      
      :arg numTasks: The number of tasks that will use this barrier
      :arg barrierType: The barrier implementation to use
      :arg reusable: Incur some extra overhead to allow reuse of this barrier?
      
      

   .. method:: proc barrier()

      Block until all related tasks have called this method.  If `reusable`
      is true, reset the barrier to be used again.
      

   .. method:: proc notify()

      Notify the barrier that this task has reached this point. 

   .. method:: proc wait()

      Wait until `n` tasks have called :proc:`notify`.  If `reusable` is true,
      reset the barrier to be used again.
      
      Note: if `reusable` is true the tasks will wait until all `n` tasks
      have called both :proc:`notify` and :proc:`wait` at which point the
      barrier will automatically be reset for the next use.  If `reusable`
      is false, each task calling :proc:`wait` can return as soon as all
      `n` tasks have called :proc:`notify`.
      

   .. method:: proc check(): bool

      Return `true` if `n` tasks have called :proc:`notify`
      

   .. method:: proc reset(nTasks: int)

      Reset the barrier, setting it to work with `nTasks` tasks.  If some
      (but not all) tasks had already called :proc:`barrier` or :proc:`check`
      when :proc:`reset` is called, the behavior is undefined.
      

