module OuterModule {
  module Test {
    import OuterModule.{NBUCKETS, per};
    /* these should already be defined:
    param NBUCKETS;
    var per;
    */

    class myhisto: ReduceScanOp {
      type eltType;
      var counts:NBUCKETS*int;

      proc identity {
        var result:NBUCKETS*int;
        return result;
      }

      proc accumulateOntoState(ref counts, x:eltType) {
        counts[1 + x/per] += 1;
      }

      proc accumulate(x:eltType) {
        accumulateOntoState(counts, x);
      }

      proc accumulate(other:NBUCKETS*int) {
        for i in 1..NBUCKETS {
          counts[i] += other[i];
        }
      }

      proc combine(x) {
        accumulate(x.counts);
      }

      proc generate() {
        return counts;
      }

      proc clone() {
        return new unmanaged myhisto(eltType=eltType);
      }
    }
  }

  var tuple = (0:uint(64),
               15999:uint(64),
               2:uint(64),
               10000:uint(64));

  var array:[1..tuple.size] uint(64) = tuple;

  param NBUCKETS = 16;
  var per:uint(64) = 16000/NBUCKETS;

  use Test;

  var counts = myhisto reduce array;

  writeln(counts);
}
