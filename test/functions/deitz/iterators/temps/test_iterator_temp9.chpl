def foo(i: int)
  return i*2;

def bar(i: int)
  return i*4;

for i in bar(foo(1..4)) do
  writeln(i);
