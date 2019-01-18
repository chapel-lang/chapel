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

var c1 = new unmanaged C(), c2 = new unmanaged C();

for i in zip(c1.foo(4), c2.foo(4)) do
  writeln(i);
writeln();

delete c1;
c1 = new unmanaged D();

for i in zip(c1.foo(4), c2.foo(4)) do
  writeln(i);
writeln();

delete c2;
c2 = new unmanaged D();

for i in zip(c1.foo(4), c2.foo(4)) do
  writeln(i);
writeln();

delete c1;
c1 = new unmanaged C();

for i in zip(c1.foo(4), c2.foo(4)) do
  writeln(i);
writeln();

delete c1;
delete c2;
