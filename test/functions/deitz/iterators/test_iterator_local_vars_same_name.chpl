iter foo() {
  for i in 1..5 do
    yield i;
  for i in 1..5 by -1 do
    yield i;
}

for i in foo() do
  writeln(i);
