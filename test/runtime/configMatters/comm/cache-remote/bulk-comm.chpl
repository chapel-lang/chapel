// Test that writes to array elemnets are fenced before a large transfer
// containing those elements is issued.
proc testArr(A) {
  on Locales[numLocales-1] {
    var B: [A.domain] int = 1;
    for i in A.domain.first..#10 {
      A[i] = i;
    }
    A = B;
  }
  forall a in A do assert(a == 1);
}

var small: [1..10] int;
var large: [1..2**23] int; //64 MB
testArr(small);
testArr(large);
