class C {
  var x: int;
  iterator foo() {
    yield this;
  }
}

var c = C(5);

for j in c.foo() do
  writeln(j);
