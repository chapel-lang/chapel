proc foo(i: int) do
  return i*2;

proc bar(i: int) do
  return i*4;

for i in bar(foo(1..4)) do
  writeln(i);
