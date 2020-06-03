// chpl --fast chapel-sort-random.chpl

use Sort;
use Time;
use Random;

config const n = 128*1024*1024;

proc main() {
  var A:[1..n] uint;

  fillRandom(A);

  var timer:Timer;
  timer.start();
  sort(A);
  timer.stop();

  writeln("Sorted ", n, " elements in ", timer.elapsed(), " seconds");
  writeln(8*n/timer.elapsed()/1024.0/1024.0, " MiB/s");
}
