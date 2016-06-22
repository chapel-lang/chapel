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
  writeln("(p = ", A1[1].p, ", x = ", A1[1].x,
          ") (p = ", A1[2].p, ", x = ", A1[2].x, ")");
  var A2: [1..2] R2(i);
  writeln("(p = ", A2[1].p, ", x = ", A2[1].x,
          ") (p = ", A2[2].p, ", x = ", A2[2].x, ")");
}
