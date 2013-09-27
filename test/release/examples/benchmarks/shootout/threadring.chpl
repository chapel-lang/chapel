/***

  The Computer Language Benchmarks Game
  http://shootout.alioth.debian.org/

  Written by: Sung-Eun Choi (sungeun@cray.com)
	Modified for contribution by: Lydia Duncan (lydia@cray.com)

  Each program should create and keep alive 503 threads, explicity or
  implicitly linked in a ring, and pass a token between one thread and
  the next thread at least N times.

  Each program should

    * create 503 linked threads (named 1 to 503)
    * thread 503 should be linked to thread 1, forming an unbroken ring
    * pass a token to thread 1
    * pass the token from thread to thread N times
    * print the name of the last thread (1 to 503) to take the token

***/

// config consts can be set from the command line upon execution using
// --<name>=<value> syntax
config const N = 1000; // The number of passes that should occur
config const nthreads = 503; // The number of threads to use

const D = {1..nthreads};
var token$: [D] sync int;
// An array of sync variables used to represent the token.

proc main() {
  // Token is started before first thread is created
  // (This is mimicing the style used by C++ version 5)
  token$[1] = 1;

  // Use a coforall here to start everyone in parallel
  coforall i in D {
    ring_link(i);
  }
}

/* Each thread spins, and when the value it reads has been changed, it
   then writes a new value, "passing" the token.  Once the specified
   number of passes has occurred, prints out the thread who is currently
   holding the thread and exit.
 */
proc ring_link(id: int) {
  while (true) {
    const t = token$[id];
    if t <= N {
      token$[id%nthreads+1] = t+1;
    } else {
      writeln(id);
      exit(0);
    }
  }
}

