param b = 16.0;

proc testit(param exp) {
  param res = b**exp;
  writeln(b, "**", exp, " = ", res);
}

for param i in 0..8 do
  testit(1.0/(2**i));
