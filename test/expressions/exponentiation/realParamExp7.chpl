param b = 0.5;

proc testit(param exp) {
  param res = b**exp;
  writeln(b, "**", exp, " = ", res);
}

testit(7);
