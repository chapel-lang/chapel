use CTypes;
use Time;
use RangeChunk;

type eltType = int;
param eltSize = numBytes(eltType);
const GB = 2**30:real;
const MB = 2**20:real;

config const correctness = false;
config const n = 1_000_000_000;
config const nIter = 10;

enum testMode {assign, serCopy, parCopy};

config const mode = testMode.assign;


var A, B: [1..n] eltType;

proc timerWrite(name, ref t) {
  if !correctness {
    writef("%10s: %.5dr\n", name, t.elapsed()); t.clear();
  }
}

// Warmup (and correctness)
A = B;
var t: stopwatch;

t.start();
select mode {
  when testMode.assign {
    for i in 0..#nIter {
      A = B;
    }
  }
  when testMode.serCopy {
    for i in 0..#nIter {
      memcpy(c_ptrTo(B[1]), c_ptrTo(A[1]), (n*numBytes(int)).safeCast(c_size_t));
    }
  }
  when testMode.parCopy {
    for i in 0..#nIter {
      const numTasks = here.maxTaskPar;
      coforall tid in 0..#numTasks {
        var c = chunk(1..n, numTasks, tid);
        memcpy(c_ptrTo(B[c.first]), c_ptrTo(A[c.first]), (c.size*numBytes(int)).safeCast(c_size_t));
      }
    }
  }
}
t.stop();


if !correctness {
  writef("Throughput (MB/s): %.5dr\n", n*eltSize*nIter/MB/t.elapsed());
}
