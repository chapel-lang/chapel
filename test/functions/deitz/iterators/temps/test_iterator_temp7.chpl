iter foo(n: int = 8) {
  for i in 1..n do
    yield i;
}

for i in foo(4) do
  writeln(i);

for i in foo() do
  writeln(i);
