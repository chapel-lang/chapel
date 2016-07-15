/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Sung-Eun Choi, Lydia Duncan, and Brad Chamberlain
*/

config const n = 1000,        // The number of token passes to perform
             ntasks = 503;    // the number of tasks ("threads") to use

//
// An array of per-task synchronized integers for receiving tokens.
// By default, its elements are 'empty' causing reads to block until
// they become 'full'.
//
var mailbox$: [1..ntasks] sync int;

proc main() {
  //
  // Write the number of token passes so far (0) to task 1's mailbox,
  // giving it the token by making it 'full'.
  //
  mailbox$[1] = 0;

  //
  // Create tasks using a 'coforall' loop.  Index 'tid' stores the
  // task's ID.
  //
  coforall tid in 1..ntasks do
    passTokens(tid);
}

//
// A routine that each task runs to pass tokens.
//
proc passTokens(tid) {
  do {
    //
    // Read the number of token passes from our mailbox, blocking
    // until it's 'full' and leaving it 'empty'.  Write the
    // incremented value to the next task's mailbox, making it 'full'.
    //
    const numPasses = mailbox$[tid];
    mailbox$[tid%ntasks+1] = numPasses+1;

    if numPasses == n then
      writeln(tid);

  } while (numPasses < n);
}
