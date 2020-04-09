use BitOps;
use Sort;
use Random;
use RadixLSB;
use Time;

config const printStats = false;
config const size = 10000;

proc main() {

  {
    var array:[1..13] int = [ 233, 10, 9, 182, 667, 549, 55, 48, 675, 1100, 32, 13457, 12];
    writeln(array);

    var sortedArray = radixSort(array);
    writeln("sorted array: ",sortedArray);
  }

  var array:[1..size] int;
  fillRandom(array);

  for i in array.domain {
    array[i] = abs(array[i]);
  }

  var t: Timer;
  t.start();

  RadixLSB.radixSort(array);

  t.stop();

  if printStats {
    writeln("Time: ", t.elapsed());
  }

  t.clear();

  writeln("sorted array: ",isSorted(array));
}
