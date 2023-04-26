use GPU;

on here.gpus[0] {
  var A : [0..10] real(32);
  foreach i in 0..10 {
    assertOnGpu();
    for j in 1..3 {
      if j == 3 then continue;
      A[i] += i;
    }
  }
  writeln(A);
}

