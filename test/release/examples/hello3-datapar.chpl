// Data-parallel hello world

/* This program uses Chapel's data parallel features to create a
   parallel hello world program that utilizes multiple cores on a
   single `locale` (compute node).
 */


//
// The following `configuration constant` indicates the number of
// messages to print out.  The default can be overridden on the
// command-line (e.g., ``./hello --numMessages=1000000``).
//
config const numMessages = 100;

//
// Next, we use a data-parallel `forall-loop` to iterate over a
// `range` representing the number of messages to print.  By default,
// forall-loops will typically be executed cooperatively by a number
// of tasks proportional to the hardware parallelism on which the loop
// is running. Ranges like ``1..numMessages`` are always local to the
// current task's locale, so this forall-loop will execute using the
// number of local processing units or cores.
//
// Because the messages are printed within a parallel loop, they may
// be displayed in any order.  The `writeln()` procedure protects
// against finer-grained interleaving of the messages themselves.
//
forall msg in 1..numMessages do
  writeln("Hello, world! (from iteration ", msg, " of ", numMessages, ")");

//
// For further examples of using data parallelism, refer to the data
// parallel :ref:`primer examples <primers>`.
//