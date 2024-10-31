on here.gpus[0] {
  var A: [1..10] int;
  foreach a in A do a = 42;
  writeln(A);
}
