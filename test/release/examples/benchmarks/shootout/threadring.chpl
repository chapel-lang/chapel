/* The Computer Language Benchmarks Game
   http://benchmarksgame.alioth.debian.org/

   contributed by Sung-Eun Choi
   modified by Lydia Duncan
*/


//
// config consts can be set from the command line upon execution using
// --<name>=<value> syntax
//
config const n = 1000,         // The number of passes that should occur
             nthreads = 503;   // The number of threads to use


//
// A range and array used to represent the set of threads and the sync
// variables used to coordinate between them.
//
const Threads = 1..nthreads;
var token$: [Threads] sync int;

proc main() {
  //
  // Signal that the first thread can run once it's started
  //
  token$[1] = 1;

  // Use a coforall here to start everyone in parallel
  coforall t in Threads {
    passTokens(t);
  }
}

//
// Each thread spins, and when the value it reads has been changed, it
// then writes a new value, "passing" the token.  Once the specified
// number of passes has occurred, prints out the thread who is
// currently holding the thread and exit.
//
proc passTokens(id) {
  while (true) {
    const t = token$[id];
    token$[id%nthreads+1] = t+1;
    if t == n + 1 {
      writeln(id);
      return;
    } else if t > n {
      return;
    }
  }
}
