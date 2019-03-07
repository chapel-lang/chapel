// We need some parameters:
// param NBUCKETS
// var min
// var max

record Test {
  param NBUCKETS;

  var   max;
  var   min;

  class histo {
    var counts:NBUCKETS*int;
  }

  class myhisto: ReduceScanOp {
    type  eltType;

    const per     = (max - min)/NBUCKETS;
    var   h:histo = new histo();

    proc accumulate(x) {
      h.counts[1 + x/per] += 1;
    }

    proc combine(x) {
      for i in 1..NBUCKETS {
        h.counts[i] += x.h.counts[i];
      }
    }

    proc generate() {
      const result = h;
      h = nil; // giving up ownership - do not delete in deinit()
      return result;
    }

    proc deinit() {
      delete h;
    }
  }

  proc reduceit(array) {
    return myhisto reduce array;
  }
}

var tuple = (0:uint(64),
             15999:uint(64),
             2:uint(64),
             10000:uint(64));

var array:[1..tuple.size] uint(64) = tuple;

var test   = new Test(16, 16000:uint(64), 0:uint(64));
var counts = test.reduceit(array);

writeln(counts);
delete counts;
