writeln("(1..10 by 2).expand(3) yields:");
var r1 = (1..10 by 2).expand(3);
writeln(r1);
for i in r1 do
  writeln(i);

writeln("(1..9 by 2).expand(3) yields:");
var r2 = (1..9 by 2).expand(3);
writeln(r2);
for i in r2 do
  writeln(i);

writeln("(1..10 by 2).exterior(3) yields:");
var r3 = (1..10 by 2).exterior(3);
writeln(r3);
for i in r3 do
  writeln(i);

writeln("(1..9 by 2).exterior(3) yields:");
var r4 = (1..9 by 2).exterior(3);
writeln(r4);
for i in r4 do
  writeln(i);

writeln("1..10 by 2 by -1 yields:");
var r5 = 1..10 by 2 by -1;
writeln(r5);
for i in r5 do
  writeln(i);
