iter foo(n: int) {
  for i in 1..n do
    yield i;
}

for i in foo(1..5) do
  writeln(i);
