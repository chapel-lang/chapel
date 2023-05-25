var r = 1..6;
var A: [r by -1] int = [i in r by -1] i;  // neg-stride
for a in A[r by -1] do                    // neg-stride
  writeln(a);
