/* Alternative correctness testing, that explores different array sizes */
config const debug = false;
config const verify = true;
config const printTiming = false;
config const n = 1024;
config const MergeSort_minlen = n/16;
config const quickSort_minlen = n/64;

config const trials = 3;

if debug then writeln("n=", n);

use Time;
use Random;
use Sort;
config const seed = 13579;

enum SortType { BUBBLE=0, INSERTION, MERGE, SELECTION, QUICK, HEAP };

proc main() {
  for param st in 0..5 /* this is annoying */ do
    doSort(st:SortType, trials);
}


const D = {1..n};
proc doSort(param st: SortType, trials: int) {
  const iters = {1..trials};
  var I: [D] real;
  var RefI: domain(I._value.eltType);
  var HistI: [RefI] int;
  var t: Timer;
  for i in iters {
    resetArr(I);
    for i in D do RefI += I[i];
    HistI = 0;
    t.start();
    _doSort(st, I);
    t.stop();
    for i in D do
      HistI[I[i]] += 1;
    if  (+ reduce HistI) != n then writeln(st, " Sort: FAILED");
    else writeln(st, " Sort: SUCCESS");
    RefI.clear();
    if printTiming then
      writeln(st, " Sort: ", t.elapsed(TimeUnits.milliseconds), " ms");
    t.clear();
  }
}

// complete support for first-class function would be nice here
proc _doSort(param st, I) where st==SortType.BUBBLE {
  bubbleSort(I);
}
proc _doSort(param st, I) where st==SortType.INSERTION {
  insertionSort(I);
}
proc _doSort(param st, I) where st==SortType.MERGE {
  mergeSort(I);
}

proc _doSort(param st, I) where st==SortType.SELECTION {
  selectionSort(I);
}

proc _doSort(param st, I) where st==SortType.QUICK {
  quickSort(I);
}

proc _doSort(param st, I) where st==SortType.HEAP {
  heapSort(I);
}

proc resetArr(A: [?D]) {
  fillRandom(A, seed);
}

