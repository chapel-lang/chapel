record R {
  var x;
}

iter R.foo() {
  for i in x do
    yield i;
}

iter R.bar() {
  for i in foo() do
    yield i;
}

var r = new R(1..5);
for i in r.bar() do
  writeln(i);
