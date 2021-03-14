config var n : int = 6;

var D : domain(1) = {1..n};

writeln(D);

writeln("Iterating over D:");
for i in D do
  writeln("  ", i);
