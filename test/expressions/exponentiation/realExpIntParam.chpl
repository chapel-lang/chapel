param b = 0.5;

proc testit(param exp) {
  param res = b**exp;
  writeln(b, "**", exp, " = ", res);
}

for param i in 0..8 do
  if i != 7 then
    testit(i);
