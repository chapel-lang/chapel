use CTypes;
use Time;

config const printTime = true;
config const trials: c_size_t = 1_000_000;

var s: stopwatch;
s.start();

coforall 1..here.maxTaskPar {
  for t in 1..trials {
    var p = allocate(int, t);
    deallocate(p);
  }
}

if printTime then
  writef("%i alloc: %.2dr\n", trials*here.maxTaskPar, s.elapsed());
s.clear();
