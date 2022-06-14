enum numbers { mn = min(int), one = 1, two = 2, three = 3, mx = max(int)}
use numbers;

for i in mn.. do
  writeln(i);
writeln();

for i in mn.. by 2 do
  writeln(i);
writeln();

for i in one.. by 2 do
  writeln(i);
writeln();

for i in ..mx by -1 do
  writeln(i);
writeln();

for i in ..mx by -2 do
  writeln(i);
writeln();

for i in ..three by -2 do
  writeln(i);
writeln();
