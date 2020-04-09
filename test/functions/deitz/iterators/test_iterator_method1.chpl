class C {
  iter foo() : int {
    for i in 1..10 do
      yield i;
  }
}

var c = new unmanaged C();

for i in c.foo() do
  writeln(i);

delete c;
