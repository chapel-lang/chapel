use GPU;

on here.gpus[0] {
  var A : [0..10] real(32);
  label outer for j in 1..3 {
    foreach i in 0..10 {
      assertOnGpu();  // expect this to fail at compile time
      if i==3 then break outer;
      A[i] += i;
    }
  }
  writeln(A);
}

