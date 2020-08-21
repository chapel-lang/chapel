use OrderedSet;
use Random;
use Sort;


config const testIters = 128;

/* Find upperBound in sorted arr */
proc upperBound(arr: [?d] int, target: int, out result): bool {
  for x in arr {
    if x > target {
      result = x;
      return true;
    }
  }
  return false;
}

proc doTest(arr: [?d] int) {
  var s1 = new orderedSet(int, false, defaultComparator);

  for x in arr {
    s1.add(x);
  }

  sort(arr); 

  for i in 0..#testIters {
    var now = arr[i];

    var setResult: int;
    var setFound: bool;
    (setFound, setResult) = s1.upperBound(now);

    var arrResult: int;
    var arrFound: bool = upperBound(arr, now, arrResult);
    
    assert(setFound == arrFound);
    assert(!setFound || setResult == arrResult);

    now = arr[i] - 1;

    (setFound, setResult) = s1.upperBound(now);
    assert(setFound);

    arrFound = upperBound(arr, now, arrResult);
    
    assert(setFound == arrFound);
    assert(!setFound || setResult == arrResult);
  }
}

var arr: [0..#testIters] int;
fillRandom(arr);
doTest(arr);

for i in 0..#testIters {
  arr[i] = i;
}
doTest(arr);
