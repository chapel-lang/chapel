record R {
  var x;
}

def R.foo() {
  for i in x do
    yield i;
}

def R.bar() {
  for i in foo() do
    yield i;
}

var r = new R(1..5);
for i in r.bar() do
  writeln(i);
