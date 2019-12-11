use DynamicIters;

for lo in 0..2 {
  for al in lo..lo+1 {
    var r = lo..#100 by 10 align al;
    var A: [r] int;

    for i in dynamic(r, chunkSize=5) { A[i] = 1; }
    writeln(A);
  }
}

for lo in 0..2 {
  for al in lo..lo+1 {
    var r = lo..#100 by 10 align al;
    var A: [r] int;

    forall i in dynamic(r, chunkSize=5) { A[i] = 1; }
    writeln(A);
  }
}

for lo in 0..2 {
  for al in lo..lo+1 {
    var r = lo..#100 by 10 align al;
    var A: [r] int;

    forall (i,j) in zip(dynamic(r, chunkSize=5), r) { assert(i==j); A[j] = 1; }
    writeln(A);
  }
}

for lo in 0..2 {
  for al in lo..lo+1 {
    var r = lo..#100 by 10 align al;
    var A: [r] int;

    forall (i,j) in zip(r, dynamic(r, chunkSize=5)) { assert(i==j); A[j] = 1; }
    writeln(A);
  }
}
