def foo() {
  yield 1;
  yield 2;
  yield 3;
  yield 4;
}

def goo(y)
  return y;

def bar(y) {
  for i in goo(y) do
    writeln(i);
}

bar(foo());
