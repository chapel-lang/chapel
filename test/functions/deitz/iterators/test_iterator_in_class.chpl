class C {
  var x: int;
  iter foo() {
    for i in 1..x do
      yield i;
  }
}

var c = new unmanaged C(5);

for j in c.foo() do
  writeln(j);

delete c;
