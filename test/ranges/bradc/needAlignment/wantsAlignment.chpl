var r1 = (1..10 by 2).expand(3);
writeln(r1, " yields:");
for i in r1 do
  write(i, " ");
writeln();

var r2 = (1..9 by 2).expand(3);
writeln(r2, " yields:");
for i in r2 do
  write(i, " ");
writeln();

var r3 = (1..10 by 2).exterior(3);
writeln(r3, " yields:");
for i in r3 do
  write(i, " ");
writeln();

var r4 = (1..9 by 2).exterior(3);
writeln(r4, " yields:");
for i in r4 do
  write(i, " ");
writeln();

var r5 = 1..10 by 2 by -1;
writeln(r5, " yields:");
for i in r5 do
  write(i, " ");
writeln();
