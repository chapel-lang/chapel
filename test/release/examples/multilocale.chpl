/*
 * Multilocale Example
 *
 * This example showcases the on-statement.  To run this example, use
 * the "-nl #" flag to specify the number of locales.  For example, to
 * run on 2 locales, run "a.out -nl 2".
 *
 */

use Time;

//
// Output the number of locales we are running on by accessing the
// numLocales constant.
//
writeln("running this example on ", numLocales, " locales");

//
// Iterate over the Locales array and use the on-statement to execute
// code on every locale.  The predefined constant 'here' corresponds
// to the locale the program is currently running and 'here.id'
// contains a unique integer between 0 and numLocales-1.
//
for loc in Locales on loc {
  writeln("hello locale ", here.id);
}

//
// Define a Node class for linked lists.
//
class Node {
  var data: real;
  var next: Node;
}

//
// Build a linked list containing numLocales nodes where each node in
// the linked list resides on a subsequent locale.
//
var head = new Node(0);
var current = head;
for i in 1..numLocales-1 on Locales(i) {
  current.next = new Node(i);
  current = current.next;
}

//
// Loop over the linked list and output each node and the locale on
// which it exists using .locale.id to determine the locale.  This
// computation takes place entirely on Locales(0) and accesses remote
// memory as necessary
//
current = head;
while current {
  writeln("node with data = ", current.data, " on locale ", current.locale.id);
  current = current.next;
}

//
// Define a couple of config constants
//   use --parallelize=false to execute this example serially
//   use --benchmark=false to squelch timing (used for testing)
//   use --startFibonacci=# to choose an initial Fibonacci number
//
config const
  parallelize = true,
  benchmark = true,
  startFibonacci = 10;

//
// Declare an array on Locale(0) and assign a Fibonacci number to each
// element in the array.  Compute the Fibonacci numbers in parallel
// across the locales.  The LocaleSpace domain is the constant domain
// over which the Locales array is declared; its range is from 0 to
// numLocales-1.  The slowFibonacci function is defined below.
//
// Performance warning: Use --parallelize=false to run this example
// serially in order to gauge speedup, but note that this is an
// initial prototype of Chapel and the multilocale features, in
// particular, are highly unoptimized.  To see speedup, you will need
// to use a machine with multiple cores or processors, or multiple
// machines.  If the computation is too fast, you will not be able to
// measure any speedup.  To increase the duration of the computation,
// try increasing the value of the config constant startFibonacci on
// the command line using the flag "--startFibonacci=37" but do not
// increase the number too quickly as it will quickly swamp your
// machine.  To find a good number, try running on a single locale at
// first.
//
var A: [LocaleSpace] int(64);

var timer: Timer;
if benchmark then
  timer.start();
serial !parallelize coforall loc in Locales on loc {
  A(here.id) = slowFibonacci(startFibonacci);
}
if benchmark {
  timer.stop();
  writeln("parallel execution time = ", timer.elapsed(), " seconds");
}

writeln("A = ", A);

//
// Define slowFibonacci function to return the ith Fibonacci number
// using a naive recursive algorithm.
//
def slowFibonacci(i: int(64)): int(64)
  return if i <= 1 then i else slowFibonacci(i-1) + slowFibonacci(i-2);
