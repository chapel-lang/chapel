// Task-parallel hello world

/* This program uses Chapel's `task parallel` features to express an
   explicitly concurrent hello world program that utilizes multiple
   cores on a single `locale` (compute node).
 */


//
// First, we specify the number of tasks to create via a `config
// const`.  By default, set it to the runtime's estimation of maximum
// parallelism that the current locale ('`here`') is capable of
// executing (``.maxTaskPar``).
//
config const numTasks = here.maxTaskPar;


//
// Next, we create the specified number tasks using a `coforall-loop`.
// This is a parallel loop form that will create a distinct task per
// iteration.
//
// This coforall-loop is iterating over the `range` ``0..#numTasks``
// which represents the first `numTasks` integers starting at 0
// (equivalent to ``0..numTasks-1``).  The result will be `numTasks`
// iterations, each of which will be executed as a distinct parallel
// task.
//
// Each iteration prints out a message that is unique based on its
// value of `tid`.  Due to the task parallelism, the messages may be
// printed in any order.  However, the `writeln()` procedure will
// prevent finer-grained interleaving of the messages themselves.
//
coforall tid in 0..#numTasks do
  writeln("Hello, world! (from task ", tid, " of ", numTasks, ")");


//
// For further examples of using task parallelism, refer to
// :ref:`examples/primers/taskParallel.chpl <primers-taskParallel>`.
//
