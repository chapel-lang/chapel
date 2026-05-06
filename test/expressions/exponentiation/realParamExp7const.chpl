param b = 0.5;

proc testit(param exp) {
  const res = b**exp;
  writeln(b, "**", exp, " = ", res);
}

testit(7);
