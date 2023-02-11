var D = {1..5 by -1};         // neg-stride
var A: [D] int = [i in D] i;

for a in A {
  writeln(a);
}

writeln(A);
