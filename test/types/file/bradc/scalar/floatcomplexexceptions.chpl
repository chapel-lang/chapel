
var f: real;
var c: complex;
const zero: real = 0.0; // To avoid C compiler folding of 1.0 / 0.0

f = 1.0 / zero;
writeln("f is: ", f);

f = sqrt(-1.0);
writeln("f is: ", f);

f = -1.0 / zero;
writeln("f is: ", f);

f = -1.0 / max(real);
f = f / max(real);
writeln("f is: ", f);
writeln();

c.re = 1.0 / zero;
writeln("c is: ", c);

c.re = sqrt(-1.0);
writeln("c is: ", c);

c.re = -1.0 / zero;
writeln("c is: ", c);

c.re = -1.0 / max(real);
c.re = c.re / max(real);
writeln("c is: ", c);
writeln();

c = 0.0 + 0.0i;

c.im = 1.0 / zero;
writeln("c is: ", c);

c.im = sqrt(-1.0);
writeln("c is: ", c);

c.im = -1.0 / zero;
writeln("c is: ", c);

c.im = -1.0 / max(real);
c.im = c.im / max(real);
writeln("c is: ", c);
writeln();

