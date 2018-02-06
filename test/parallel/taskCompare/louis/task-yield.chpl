config const N = 1000, nthreads = 503, verbose = false;
proc main() {
  if (verbose) {
    writeln(nthreads, " threads.  ", N, " trips.");
  }

  // Use a coforall here (and have thread "0" start everyone)
  // - thread that starts coforall grabs an iteration
  coforall i in 0..nthreads do {
    for 1 .. N do chpl_task_yield();
  }
}