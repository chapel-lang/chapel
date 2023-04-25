proc foo(param x : int) do
  return x;

proc foo(x : int) do
  return -1;

var y : int = 2;
for i in 1..10 do
  y = y * y;

writeln(foo(1));
writeln(foo(2));
writeln(foo(y));
