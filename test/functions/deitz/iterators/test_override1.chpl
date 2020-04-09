class C {
  iter foo(n: int) {
    for i in 1..n do
      yield i;
  }
}

class D: C {
  override iter foo(n: int) {
    for i in 1..n by -1 do
      yield i;
  }
}

var c = new unmanaged C();

for i in c.foo(4) do
  writeln(i);
writeln();

delete c;

c = new unmanaged D();

for i in c.foo(4) do
  writeln(i);
writeln();

delete c;
