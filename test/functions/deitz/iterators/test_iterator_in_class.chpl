class C {
  var x: int;
  iterator foo() {
    for i in 1..x do
      yield i;
  }
}

var c = C(5);

for j in c.foo() do
  writeln(j);
