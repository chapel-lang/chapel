// This test demonstrates a bug that existed with keyPart(_tuple)
// for the default comparator.

use Sort;
use Random;

{
  var A = [(2,3), (6,5), (1,2), (2,2)];
  writeln("before sort A is: ", A);
  sort(A);
  writeln("after sort A is:  ", A);
}

{
  var tup = (2,3);
  var cmp = new defaultComparator();
  assert(cmp.keyPart(tup, 0) == (keyPartStatus.returned, 2));
  assert(cmp.keyPart(tup, 1) == (keyPartStatus.returned, 3));
  assert(cmp.keyPart(tup, 2) == (keyPartStatus.pre, 0));
}

config const n = 1_000_000;
{
  var A: [0..<n] (int, int);
  var B: [0..<n] int;
  var C: [0..<n] int;
  fillRandom(B, min=1, max=10, seed=1);
  fillRandom(C, min=1, max=10, seed=2);
  forall (a, b, c) in zip(A, B, C) {
    a = (b,c);
  }

  sort(A);

  forall idx in 0..<n {
    if idx > 0 {
      assert(A[idx-1](0) <= A[idx](0));
      if A[idx-1](0) == A[idx](0) {
        assert(A[idx-1](1) <= A[idx](1));
      }
    }
  }
}
