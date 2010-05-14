var d : domain(int);
var a: [d] real;

atomic {
  d += 1;
  d += 2;
  d -= 2;
}

a(1) = 7.2;

writeln("domain d = ", d);
writeln("array a = ", a);

