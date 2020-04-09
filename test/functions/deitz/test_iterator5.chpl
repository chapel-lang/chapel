record R {
  var x: 3*(int,int);
}

var r = new R(x=((1, 2), (3, 4), (5, 6)));

iter R.foo() {
  for param i in 0..2 do
    yield x(i)(0);
}

for i in r.foo() do
  writeln(i);
