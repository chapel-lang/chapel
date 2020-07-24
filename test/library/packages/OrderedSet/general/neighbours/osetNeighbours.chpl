use OrderedSet;
use Random;
use Sort;

config param impl: setImpl;
config const testIters = 128;

proc doTest(arr: [?d] int) {
  var s1 = new orderedSet(int, false, defaultComparator, impl);

  for x in arr {
    s1.add(x);
  }

  sort(arr); 

  for i in 0..#testIters {
    var now = arr[i];

    var setResult: int;
    var setFound: bool = s1.predecessor(now, setResult);

    var arrResult: int;
    var arrFound: bool = i != 0;
    if arrFound then arrResult = arr[i-1];
    
    assert(setFound == arrFound);
    assert(!setFound || setResult == arrResult);

    setFound = s1.successor(now, setResult);

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
