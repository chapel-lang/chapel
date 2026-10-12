param b = 16.0;

proc testit(param b, param exp) {
  param res = b**exp;
  writeln(b, "**", exp, " = ", res);
}

for param i in 0..8 do
  testit(b, 1.0/(2**i));

for param i in 0..8 do
  testit(b:imag, (1.0/(2**i)):imag);

for param i in 0..8 do
  testit(b:complex, (1.0/(2**i)):complex);
