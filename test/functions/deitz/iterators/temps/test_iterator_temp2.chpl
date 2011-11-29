iter foo() {
  yield 1;
  yield 2;
  yield 3;
  yield 4;
}

proc bar(x) {
  for i in x do
    writeln(i);
}

bar(foo());
