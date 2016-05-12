/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Sung-Eun Choi and Lydia Duncan
*/


//
// Note: This program uses 'thread' to refer to Chapel 'tasks'.
//

config const n = 1000,        // The number of token passes to perform
             nthreads = 503;  // the number of threads to use

//
// An array of per-thread synchronized integers for receiving tokens.
// By default elements are 'empty' causing reads to block until they
// become 'full'.
//
var mailbox$: [1..nthreads] sync int;


proc main() {
  //
  // Write the number of token passes so far (0) to thread 1's
  // mailbox, giving it the token by making it 'full'.
  //
  mailbox$[1] = 0;

  //
  // Create a task per thread using a 'coforall' loop.  Index 'tid'
  // stores the thread's ID.
  //
  coforall tid in 1..nthreads do
    passTokens(tid);
}

//
// A routine that each thread runs to pass tokens.
//
proc passTokens(tid) {
  do {
    //
    // Read the number of token passes from our mailbox, blocking
    // until it's 'full' and leaving it 'empty'.  Write the
    // incremented value to the next thread's mailbox, making it
    // 'full'.
    //
    const numPasses = mailbox$[tid];
    mailbox$[tid%nthreads+1] = numPasses+1;

    if numPasses == n then
      writeln(tid);

  } while (numPasses < n);
}
