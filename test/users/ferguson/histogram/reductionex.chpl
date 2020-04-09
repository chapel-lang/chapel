
  param NBUCKETS = 16;
  var max:uint(64) = 16000;
  var min:uint(64) = 0;

  // We need some parameters:
  // param NBUCKETS
  // var min
  // var max

  class myhisto: ReduceScanOp {
    type eltType;
    const per = (max - min)/NBUCKETS;
    var counts:NBUCKETS*int;

    proc identity {
      var result:NBUCKETS*int;
      return result;
    }

    proc accumulateOntoState(ref counts, x:eltType) {
      counts[x/per] += 1;
    }

    proc accumulate(x:eltType) {
      accumulateOntoState(counts, x);
    }

    proc accumulate(other:NBUCKETS*int) {
      for i in 0..#NBUCKETS {
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

  var tuple = (0:uint(64),
               15999:uint(64),
               2:uint(64),
               10000:uint(64));

  var array:[1..tuple.size] uint(64) = tuple;

  var counts = myhisto reduce array;

  writeln(counts);

