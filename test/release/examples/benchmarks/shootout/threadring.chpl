/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Sung-Eun Choi and Lydia Duncan
*/


//
// Note that this program uses the term 'thread' to refer to the
// parallel entities in order to match the benchmark's description.
// In Chapel, these are actually 'tasks' (in Chapel, a 'thread' is the
// system-level resource with which language-level tasks are executed
// (e.g., POSIX threads).
//

config const n = 1000,        // The number of token passes to perform
             nthreads = 503;  // the number of threads to use

//
// An array of per-thread synchronized integers representing mailboxes
// for receiving the token.  By default each element will be 'empty'
// causing reads to block until it becomes full.
//
var mailbox$: [1..nthreads] sync int;


proc main() {
  //
  // Write the number of token passes so far (0) to thread 1's
  // mailbox, giving it the token by making it 'full'.
  //
  mailbox$[1] = 0;

  //
  // Create a task per thread using a 'coforall' over the 'Threads'
  // range.  Index 'tid' stores the thread's ID.
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
    // Read the token value from this thread's mailbox.  If the
    // mailbox is empty, the thread blocks until it's full.
    //
    const numPasses = mailbox$[tid];

    //
    // Increment the number of token passes and store in the next
    // thread's mailbox.
    //
    mailbox$[tid%nthreads+1] = numPasses+1;

    //
    // Have the thread that got the n'th token pass write its ID.
    //
    if numPasses == n then
      writeln(tid);

    //
    // Keep looping until we've passed the token 'n' times.
    //
  } while (numPasses < n);
}
