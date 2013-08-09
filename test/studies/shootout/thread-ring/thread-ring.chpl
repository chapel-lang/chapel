/***

  thread-ring benchmark from The Computer Language Benchmarks Game
  http://shootout.alioth.debian.org/

  Written by: Sung-Eun Choi (sungeun@cray.com)

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

config const N = 1000, nthreads = 503;
config param use_coforall = true;
var D: domain(1) = {1..nthreads};
var token: [D] sync int;

proc main() {
  // init (not strictly needed, i.e., MTA semantics)
  [i in D] token(i).reset();

  writeln(nthreads, " threads.  ", N, " trips.");

  if use_coforall {
    // Could use a coforall here (and have thread "0" start everyone)
    // - threads thats start coforall grabs an iteration
    for i in 0..nthreads do {
      begin ring_link(i);
		}
  } else {
    // start the N threads
    for i in D do
      begin ring_link(i);

    // let's get it started!
    token(1).writeEF(1);

    // wait..  someone else will call exit
    while (true) {}
  }
}

proc ring_link(id: int) {
  if (id > 0) {
    while (true) {
      const t = token(id).readFE();
      if t <= N {
        token(id%nthreads+1).writeEF(t+1);
      } else {
        writeln(id);
        exit(0);
      }
    }
  } else {
    // let's get it started!
    token(1).writeEF(1);
  }
}

