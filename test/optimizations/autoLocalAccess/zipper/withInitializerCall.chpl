use BlockDist;

record R {
  var x: int;
}

class C {
  var x: int;
}

{
  var A = newBlockArr({1..10}, R);
  forall (i, loopIdx) in zip(A.domain, 1..) {
    A[i] = new R[i*loopIdx];
  }
}

{
  var A = newBlockArr({1..10}, unmanaged C?);
  forall (i, loopIdx) in zip(A.domain, 1..) {
    A[i] = new unmanaged C[i*loopIdx];
  }
}

{
  var A = newBlockArr({1..10}, owned C?);
  forall (i, loopIdx) in zip(A.domain, 1..) {
    A[i] = new owned C[i*loopIdx];
  }
}

{
  var A = newBlockArr({1..10}, shared C?);
  forall (i, loopIdx) in zip(A.domain, 1..) {
    A[i] = new shared C[i*loopIdx];
  }
}
