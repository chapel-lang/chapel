def foo() {
  for i in 1..5 do
    yield i;
}

def bar(i: int) return i**2;

for i in bar(foo()) do
  writeln(i);
