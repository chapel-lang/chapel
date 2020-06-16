use BlockDist;

var D = newBlockDom({1..10});

var A: [D] int;

// there must be only one static check, and two accesses optimized
forall i in D {
  if i>1 && i<10 then
    A[i] = A[i-1] + A[i] + A[i+1];
}
