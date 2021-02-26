use OrderedSet;
use Random;
use Sort;


config const testIters = 128;

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
    (setFound, setResult) = s1.predecessor(now);

    var arrResult: int;
    var arrFound: bool = i != 0;
    if arrFound then arrResult = arr[i-1];
    
    assert(setFound == arrFound);
    assert(!setFound || setResult == arrResult);

    (setFound, setResult) = s1.successor(now);

    arrFound = i != testIters - 1;
    if arrFound then arrResult = arr[i+1];
    
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
