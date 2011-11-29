iter foo() {
  yield 1;
  yield 2;
  yield 3;
  yield 4;
}

proc goo(y)
  return y;

proc bar(y) {
  var x = goo(y);
  for i in x do
    writeln(i);
}

bar(foo());
