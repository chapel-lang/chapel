var x, y: complex;
x = 2.56 + 9.0i;
y = (3.12, 8.7): complex;
var z = (4.2, 6.1);

writeln(x);
writeln(y);
writeln(z);

var m = 2: uint(64);
var n = 2: uint(32);
var i = 1;
var j = 1;

while (n > 0) do {
  n *= 2;
  i += 1;
}

while (m > 0) do {
  m *= 2;
  j += 1;
}

writeln("For 32-bit integers, 2 ** (",i,") overflows.");
writeln("For 64-bit integers, 2 ** (",j,") overflows.");
