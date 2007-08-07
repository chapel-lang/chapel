def foo() {
  yield 1;
  yield 2;
  yield 3;
  yield 4;
}

def goo(y)
  return y;

def bar(y) {
  var x = goo(y);
  for i in x do
    writeln(i);
}

bar(foo());
