
  module Test {
    /* these should already be defined:
    param NBUCKETS;
    var min;
    var max;
    */

    pragma "use default init"
    class myhisto: ReduceScanOp {
      type eltType;
      var per = (max-min)/NBUCKETS;
      var counts:NBUCKETS*int;

      proc accumulate(x) {
        counts[1 + x/per] += 1;
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
  var min:uint(64) = 0;
  var max:uint(64) = 16000;

  use Test;

  var counts = myhisto reduce array;

  writeln(counts);

