class C {
  def foo(n: int) {
    for i in 1..n do
      yield i;
  }
}

class D: C {
  def foo(n: int) {
    for i in 1..n by -1 do
      yield i;
  }
}

var c1 = new C(), c2 = new C();

for i in (c1.foo(4), c2.foo(4)) do
  writeln(i);
writeln();

c1 = new D();

for i in (c1.foo(4), c2.foo(4)) do
  writeln(i);
writeln();

c2 = new D();

for i in (c1.foo(4), c2.foo(4)) do
  writeln(i);
writeln();

c1 = new C();

for i in (c1.foo(4), c2.foo(4)) do
  writeln(i);
writeln();
