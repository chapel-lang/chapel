var x = 10;

on here.gpus[0] {
  var A: [0..0] int;
  foreach a in A do a = x;
  writeln(A);
}
