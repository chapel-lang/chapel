

class C {
  iter foo(x = 10) { yield x; yield x; yield x; }
}

class D : C {
  override iter foo(x = 20) { yield x; yield x; }
}

class E : D {
  override iter foo(x = 30) { yield x; }
}  

var ownD = new owned D();
var d = ownD.borrow();
for i in d.foo() do
  write(i, " ");
writeln();

var ownC = new owned D();
var c: borrowed C = ownC.borrow();
for i in c.foo() do
  write(i, " ");
writeln();

var ownC2 = new owned C();
var c2: borrowed C = ownC2.borrow();
for i in c2.foo() do
  write(i, " ");
writeln();

var ownE = new owned E();
var e = ownE.borrow();
for i in e.foo() do
  write(i, " ");
writeln();

var ownE2 = new owned E();
var e2: borrowed D = ownE2.borrow();
for i in e2.foo() do
  write(i, " ");
writeln();

var ownE3 = new owned E();
var e3: borrowed C = ownE3.borrow();
for i in e3.foo() do
  write(i, " ");
writeln();
