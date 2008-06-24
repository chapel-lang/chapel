def foo(n: int): int {
  if n <= 1 then
    return 1;
  for i in bar(n-1) do
    yield i;
}

def bar(n: int): int {
  if n <= 1 then
    return 1;
  for i in foo(n-1) do
    yield i;
}

for i in foo(10) do
  writeln(i);
