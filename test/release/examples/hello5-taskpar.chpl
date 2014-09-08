/*  This test uses Chapel's task parallel features to create a
 *  parallel hello world program that utilizes multiple cores on a
 *  single locale (node)
 */


//
// Specify the number of tasks to create via a config const.  By
// default, we use the tasking layer's estimate of maximum parallelism
// on the current locale ('here').  This default can be overridden on
// the executable command line (e.g., using --numTasks=3).
//
config const numTasks = here.maxTaskPar;


//
// Create the requested tasks using a 'coforall' loop which will
// create a unique task per iteration.  Here, we're iterating over
// 0..#numTasks which represents the range starting at 0 and
// containing 'numTasks' members (equivalent to 0..numTasks-1).
// Thus, the loop index variable 'tid' will take on unique values
// in the range 0..numTasks-1.
//
// Each iteration prints out a message that is unique according to the
// value of tid.  Due to the task parallelism, the messages may come
// out in any order.  However, the writeln() procedure will prevent
// against finer-grained interleaving of the messages themselves.
//
coforall tid in 0..#numTasks do
  writeln("Hello, world! (from task " + tid + " of " + numTasks + ")");


//
// For further examples of using task parallelism, refer to
// examples/primers/taskParallel.chpl
//
