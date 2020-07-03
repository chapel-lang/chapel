use BlockDist;

record R {
  var x: int;
}

class C {
  var x: int;
}

{
  var A = newBlockArr({1..10}, R);
  forall i in A.domain {
    A[i] = new R[i];
  }
}

{
  var A = newBlockArr({1..10}, unmanaged C?);
  forall i in A.domain {
    A[i] = new unmanaged C[i];
  }

  for a in A do delete a;
}

{
  var A = newBlockArr({1..10}, owned C?);
  forall i in A.domain {
    A[i] = new owned C[i];
  }
}

{
  var A = newBlockArr({1..10}, shared C?);
  forall i in A.domain {
    A[i] = new shared C[i];
  }
}
