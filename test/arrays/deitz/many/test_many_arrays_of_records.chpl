config param p: int = 32;

record R1 {
  param p: int;
  var x: int;
}

record R2 {
  param p: int;
  var x: int;
}

for param i in 1..p {
  var A1: [1..2] R1(i);
  writeln(A1);
  var A2: [1..2] R2(i);
  writeln(A2);
}
