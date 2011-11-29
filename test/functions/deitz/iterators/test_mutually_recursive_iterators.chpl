iter foo(n: int): int {
  if n <= 1 then {
    yield 1;
    return;
  }
  for i in bar(n-1) do
    yield i;
}

iter bar(n: int): int {
  if n <= 1 then {
    yield 1;
    return;
  }
  for i in foo(n-1) do
    yield i;
}

for i in foo(10) do
  writeln(i);
