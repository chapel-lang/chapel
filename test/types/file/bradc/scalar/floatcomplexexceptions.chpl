
var f: float;
var c: complex;

f = 1.0 / 0.0;
writeln("f is: ", f);

f = sqrt(-1.0);
writeln("f is: ", f);

f = -1.0 / 0.0;
writeln("f is: ", f);

f = -1.0 / max(float);
f = f / max(float);
writeln("f is: ", f);
writeln();

c.real = 1.0 / 0.0;
writeln("c is: ", c);

c.real = sqrt(-1.0);
writeln("c is: ", c);

c.real = -1.0 / 0.0;
writeln("c is: ", c);

c.real = -1.0 / max(float);
c.real = c.real / max(float);
writeln("c is: ", c);
writeln();

c = 0.0 + 0.0i;

c.imag = 1.0 / 0.0;
writeln("c is: ", c);

c.imag = sqrt(-1.0);
writeln("c is: ", c);

c.imag = -1.0 / 0.0;
writeln("c is: ", c);

c.imag = -1.0 / max(float);
c.imag = c.imag / max(float);
writeln("c is: ", c);
writeln();

