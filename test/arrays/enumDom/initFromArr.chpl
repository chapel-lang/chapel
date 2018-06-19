enum color {red, green, blue, cyan};

var A: [color.red..color.cyan] real = [1.1, 2.2, 3.3, 4.4];

for (i, a) in zip(A.domain, A) do
  writeln("A[", i, "] = ", a);
