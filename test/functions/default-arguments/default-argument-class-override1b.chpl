

class C {
  iter foo(x = 10) { yield x; yield x; yield x; }
}

class D : C {
  override iter foo(x = 20) { yield x; yield x; }
}

class E : D {
  override iter foo(x = 30) { yield x; }
}  

var d = new borrowed D();
for i in d.foo() do
  write(i, " ");
writeln();

var c: borrowed C = new borrowed D();
for i in c.foo() do
  write(i, " ");
writeln();

var c2: borrowed C = new borrowed C();
for i in c2.foo() do
  write(i, " ");
writeln();

var e = new borrowed E();
for i in e.foo() do
  write(i, " ");
writeln();

var e2: borrowed D = new borrowed E();
for i in e2.foo() do
  write(i, " ");
writeln();

var e3: borrowed C = new borrowed E();
for i in e3.foo() do
  write(i, " ");
writeln();
