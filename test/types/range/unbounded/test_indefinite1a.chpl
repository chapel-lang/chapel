for i in zip(1..5, 2..) do
  writeln(i);

writeln();

for i in zip(1..20 by 3, 1.. by 7) do
  writeln(i);

writeln();

for i in zip(1..5, ..2 by -1) do
  writeln(i);
