config param parSafe = true;
config const n = 50000;

var D1: domain(int, parSafe=parSafe);

serial !parSafe {
  forall i in 0..n do
    D1 += max(int)-i;

  forall i in 0..n by -1 do
    D1 -= max(int)-i;
}

writeln(D1.sorted());
