param b = 0.5;

proc testit(param b, param exp) {
  param res = b**exp;
  writeln(b, "**", exp, " = ", res);
}

for param i in 0..10 do
  testit(b, i);

for param i in 0..10 do
  testit(b:imag, i);

for param i in 0..10 do
  testit(b:complex, i);

