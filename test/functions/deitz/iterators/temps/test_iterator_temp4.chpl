iter foo() {
  yield 1;
  yield 2;
  yield 3;
  yield 4;
}

proc goo(y)
  return y;

proc bar(y) {
  for i in goo(y) do
    writeln(i);
}

bar(foo());
