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
    (setFound, setResult) = s1.kth(i+1);

    assert(setFound);
    assert(setResult == now);

    assert(s1.kth(i + 1 + testIters)[0] == false);
  }
}

var arr: [0..#testIters] int;
fillRandom(arr);
doTest(arr);

for i in 0..#testIters {
  arr[i] = i;
}
doTest(arr);
