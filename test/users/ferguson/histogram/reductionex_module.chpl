
  module Test {
    /* these should already be defined:
    param NBUCKETS;
    param PER;
    */

    class myhisto: ReduceScanOp {
      type eltType;
      var counts:NBUCKETS*int;

      proc accumulate(x) {
        counts[1 + x/PER] += 1;
      }
      proc combine(x) {
        for i in 1..NBUCKETS {
          counts[i] += x.counts[i];
        }
      }
      proc generate() {
        return counts;
      }
    }
  }

  var tuple = (0:uint(64),
               15999:uint(64),
               2:uint(64),
               10000:uint(64));

  var array:[1..tuple.size] uint(64) = tuple;

  param NBUCKETS = 16;
  param PER = 16000/NBUCKETS;

  use Test;

  var counts = myhisto reduce array;

  writeln(counts);

