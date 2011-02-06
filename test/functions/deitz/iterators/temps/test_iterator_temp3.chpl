iter foo() {
  yield 1;
  yield 2;
  yield 3;
  yield 4;
}

for i in foo() do
  writeln(i);
