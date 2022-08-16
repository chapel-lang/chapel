on here.gpus[0] {
  var A: [1..10, 1..10] real;
  forall (i,j) in {1..10, 1..10} do
    A(i,j) = i + j;
  writeln(A);
}
