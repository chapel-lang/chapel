for i in zip(1..20 by 3, 1.. by 7) do
  writeln(i);

writeln();

for i in zip(2.., 1..5) {
  writeln(i);
  if i(0) == 6 then break;
}

writeln();

for i in zip(..2 by -1, 1..5) do
  writeln(i);
