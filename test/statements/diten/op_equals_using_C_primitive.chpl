proc use_op_equals(ref a1, a2) {
  a1 += a2;
  a1 *= a2;
}

proc main {
  var r1 = 1.2;
  var r2 = 2.1;
  use_op_equals(r1, r2);

  writeln(r1);
}
