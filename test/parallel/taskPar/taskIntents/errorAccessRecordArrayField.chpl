record R {
  var A: [1..1_000_000] int;
}

var r: R;

forall i in 1..1_000_000 {
  r.A[i] = i;
}

