class C {
  var x: int;
  def foo() {
    yield this;
  }
}

var c = new C(5);

for j in c.foo() do
  writeln(j);
