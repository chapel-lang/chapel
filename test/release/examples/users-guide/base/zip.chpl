var A: [1..5] real;
for (a, i) in zip(A, 1..5) do
  a = i;
writeln(A);
writeln();

for (a, i, j) in zip(A, 1..5, [3, 0, 1, 2, 4]) do
  a = i**j;
writeln(A);
writeln();

for tpl in zip(1..5, [3, 0, 1, 2, 4]) do
  writeln(tpl);
writeln();

for ind in zip(1..3, [(0,2), (3,7), (5,9)]) do
  writeln("ind is ", ind);
writeln();

for (i,j) in zip(1..3, [(0,2), (3,7), (5,9)]) do
  writeln("i is ", i, ", j is ", j);
writeln();

for (i, (j,k)) in zip(1..3, [(0,2), (3,7), (5,9)]) do
  writeln("i is ", i, ", j is ", j, ", k is ", k);
writeln();

for (_, i) in zip(A, 1..) do
  writeln("A has at least ", i, " element(s)!");
writeln();

writeln("This should iterate five times:");
for zip(A, 1..5) do
  writeln("Hi!");
writeln();

for (a, i) in zip(A, 1..) do
  a = i;
writeln(A);
writeln();
