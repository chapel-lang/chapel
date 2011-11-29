proc foo(param x : int)
  return x;

proc foo(x : int)
  return -1;

var y : int = 2;
for i in 1..10 do
  y = y * y;

writeln(foo(1));
writeln(foo(2));
writeln(foo(y));
