/*  This test uses Chapel's data parallel features to create a
 *  parallel hello world program that utilizes multiple cores on a
 *  single locale (node)
 */


//
// This configuration constant indicates the number of messages to
// print out.  The default can be overridden on the command-line
// (e.g., --numMessages=1000000)
//
config const numMessages = 100;

//
// Here, we use a data parallel forall loop to iterate over a range
// representing the number of messages to print.  In a forall loop,
// the number of tasks used to implement the parallelism is determined
// by the implementation of the thing driving the iteration -- in this
// case, the range.  See $CHPL_HOME/doc/README.executing (controlling
// degree of data parallelism) for more information about controlling
// this number of tasks.
//
// Because the messages are printed within a parallel loop, they may
// be displayed in any order.  The writeln() procedure protects
// against finer-grained interleaving of the messages themselves.
//
forall msg in 1..numMessages do
  writeln("Hello, world! (from iteration ", msg, " of ", numMessages, ")");
