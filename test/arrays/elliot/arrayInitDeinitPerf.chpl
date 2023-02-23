use Time, CTypes;
config const size = 40_000_000;
config const innerSize = 40;

config const printTiming = true;

proc timeit(type T, desc: string) {
  var t: stopwatch;
  {
    t.start();
    var A: T;
    if printTiming {
      writef("%s   init: %.3drs\n", desc, t.elapsed());
      t.clear();
    }
  }
  if printTiming {
    writef("%s deinit: %.3drs\n", desc, t.elapsed());
    t.clear();
  }
}

record RAlloc {
  var p: c_ptr(int);
  proc init() {
    p = c_malloc(int, innerSize);
    for i in 0..<innerSize do p[i] = 0;
  }
  proc deinit() { c_free(p); }
}

timeit([1..size][1..innerSize] int, "AoA");
timeit([1..size] RAlloc,            "AoR");
