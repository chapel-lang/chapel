use BlockDist;

proc main() {
  var D = {1..10, 1..10};
  var Space = D dmapped Block(D);
  var A : [Space] int;

  var idx = A.localSubdomain().first;

  //
  // Test that BlockArr.dsiAccess takes in the index argument by some kind of
  // 'in' intent such that the local block inside BlockArr.dsiAccess is not
  // violated.
  //
  // This relies on 'idx' being a wide reference once passed to dsiAccess.
  //
  on Locales.tail() {
    assert(idx.locale != here);
    assert(A.localSubdomain().contains(idx) == false);
    A.dsiAccess(idx) = 100;
  }

  assert(A[idx] == 100);
}
