record R {
  var x;
}


proc main() {
  var A: [1..3, 1..3] int;

  var r1 = new R(A);
  var r2 = new R(A);

  r1.x[2,2] = 1;
  writeln(r1);
  writeln(r2);
}
