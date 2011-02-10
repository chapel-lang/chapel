writeln("(1..10 by 2).expand(3) yields:");
for i in (1..10 by 2).expand(3) do
  writeln(i);

writeln("(1..9 by 2).expand(3) yields:");
for i in (1..9 by 2).expand(3) do
  writeln(i);

writeln("(1..10 by 2).exterior(3) yields:");
for i in (1..10 by 2).exterior(3) do
  writeln(i);

writeln("(1..9 by 2).exterior(3) yields:");
for i in (1..9 by 2).exterior(3) do
  writeln(i);

writeln("1..10 by 2 by -1 yields:");
for i in (1..10 by 2 by -1) do
  writeln(i);
