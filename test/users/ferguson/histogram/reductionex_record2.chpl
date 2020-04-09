
  // We need some parameters:
  // param NBUCKETS
  // var min
  // var max

  record Test {
    param NBUCKETS;
    var max;
    var min;

    class myhisto: ReduceScanOp {
      type eltType;
      const per = (max - min)/NBUCKETS;
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

    proc reduceit(array) {
      return myhisto reduce array;
    }
  }

  var tuple = (0:uint(64),
               15999:uint(64),
               2:uint(64),
               10000:uint(64));

  var array:[1..tuple.size] uint(64) = tuple;

  var test = new Test(16, 16000:uint(64), 0:uint(64));
  var counts = test.reduceit(array);

  writeln(counts);

