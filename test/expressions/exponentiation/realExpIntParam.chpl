param b = 0.5;

proc testit(param exp) {
  param res = b**exp;
  writeln(b, "**", exp, " = ", res);
}

for param i in 0..10 do
  testit(i);
