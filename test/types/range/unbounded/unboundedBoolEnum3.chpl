enum color {red, orange, yellow, green, blue, indigo, violet};
use color;

var A: [1..7] color;

for (i,c) in zip(2..4, red..) {
  writeln("color ", i, " = ", c);
}

writeln("---");

for (c,i) in zip(red.., 1..) {
  writeln("color ", i, " = ", c);
}

writeln("---");

forall (i,c) in zip(1..7, red..) {
  A[i] = c;
}

writeln(A);

A = color.violet;

writeln("---");

forall (c,i) in zip(red.., 1..7) {
  A[i] = c;
}


writeln(A);
