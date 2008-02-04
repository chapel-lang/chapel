class C {
  def foo() : int {
    for i in 1..10 do
      yield i;
  }
}

var c = new C();

for i in c.foo() do
  writeln(i);
