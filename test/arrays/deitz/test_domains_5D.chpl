var x = (1, 2, 3, 4, 5);
writeln(x);

config var n : integer = 3;

var D : domain(5) = [1..n, 1..n, 1..n, 1..n, 1..n];

writeln(D);

writeln("Iterating over D:");
forall ijklm in D do
  writeln(ijklm);
