use Random;
use Time;

enum classVals {ClassA, ClassB, ClassSmall};

config const probClass = ClassSmall;
config const seed = 314159265;
config const printTime = false;

const Class: domain(probClass);

const probSizes: [Class] int = (/ 2**23, 2**25, 2**17 /),
      Bmaxs:     [Class] int = (/ 2**19, 2**21, 2**19 /),
      Imaxs:     [Class] int = (/ 10, 10, 1 /);

config const probSize = probSizes(probClass);
const Bmax = Bmaxs(probClass);
const Imax = Imaxs(probClass);

const D = [1..probSize];

def main() {
  var time = Timer();
  var randomStream = RandomStream(seed);
  var K: [D] int;
  var r: [D] int;
  var tempreals: [1..4] real;
  writeln("Generating keys");
  // Generate Keys
  [i in D] {
    randomStream.fillRandom(tempreals);
    K(i) = (Bmax * (+ reduce tempreals) / 4.0):int;
    r(i) = i;
  }

  writeln("Starting sort");

  time.start();
  [i in 1..Imax] {
    K(i) = i;
    K(i+Imax) = Bmax - i;
    //InsertionSort(K, r);
    QuickSort(K, r);
    partialVerification(r, i);
  }
  time.stop();

  writeln("Performing full verification");
  fullVerification(K, r);

  if printTime then
    writeln("Time: ", time.elapsed(seconds), " seconds.");
} 


def partialVerification(r: [D] int, i: int) {
  // check that certain values of the rank array are correct
  if (probClass == ClassA) {
    assert(r(48427) == 0 + i);
    assert(r(17148) == 18 + i);
    assert(r(23627) == 346 + i);
    assert(r(62548) == 64917 - i);
    assert(r(4431) == 65463 - i);
  } else if (probClass == ClassB) {
    assert(r(2112377) == 104 + i);
    assert(r(662041) == 17523 + i);
    assert(r(5336171) == 123928 + i);
    assert(r(3642833) == 8288932 - i);
    assert(r(4250760) == 8388264 - i);
  } else {
    // Not making any assertions yet.
  }
}


def fullVerification(K: [D] int, r: [D] int) {
  // Check that the rank array is properly sorted
  [i in 1..K.numElements-1] {
    if (K(r(i)) > K(r(i+1))) {
      [j in D] {
        write(K(r(j)), " ");
      }
      writeln();
      [j in D] {
        write(K(j), " ");
      }
      writeln();
      halt("Failed. i = ", i);
    }
  }
}



def QuickSort(Data, ranks: [?Dom]) where Dom.rank == 1 {
  // grab obvious indices
  const lo = Dom.low,
        hi = Dom.high,
        mid = lo + (hi-lo+1)/2;

  // base case
  if (hi - lo < 10) {
    InsertionSort(Data, ranks);
    return;
  }

  // find pivot
  const pivotVal = ranks(mid);
  ranks(mid) = ranks(hi);
  ranks(hi) = pivotVal;

  var loptr = lo,
      hiptr = hi-1;
  for i in lo..hiptr {
    if Data(ranks(i)) < Data(pivotVal) {
      var tmp = ranks(loptr);
      ranks(loptr) = ranks(i);
      ranks(i) = tmp;
      loptr += 1;
    }
  }
  ranks(hi) = ranks(loptr);
  ranks(loptr) = pivotVal;
  //  cobegin {
    QuickSort(Data, ranks[lo..loptr-1]);
    QuickSort(Data, ranks[loptr+1..hi]);
  //  }
}

def InsertionSort(Data, ranks:[?Dom]) where Dom.rank == 1 {
  const lo = Dom.low;
  for i in Dom {
    const ithVal = ranks(i);
    var inserted = false;
    for j in [lo..i) by -1 {
      if (Data(ithVal) < Data(ranks(j))) {
        ranks(j+1) = ranks(j);
      } else {
        ranks(j+1) = ithVal;
        inserted = true;
        break;
      }
    }
    if (!inserted) {
      ranks(lo) = ithVal;
    }
  }
}

