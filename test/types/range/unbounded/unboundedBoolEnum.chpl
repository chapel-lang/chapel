enum color { red, green, blue };

for i in false.. do
  writeln(i);
writeln();

for i in false.. by 2 do
  writeln(i);
writeln();

for i in color.red.. do
  writeln(i);
writeln();

for i in color.red.. by 2 do
  writeln(i);
writeln();

for i in ..true by -1 do
  writeln(i);
writeln();

for i in ..true by -2 do
  writeln(i);
writeln();

for i in ..color.blue by -1 do
  writeln(i);
writeln();

for i in ..color.blue by -2 do
  writeln(i);
writeln();
