class C {
  var x: int;
  iter foo() {
    yield this;
  }
}

var c = new unmanaged C(5);

for j in c.foo() do
  writeln(j);

delete c;
