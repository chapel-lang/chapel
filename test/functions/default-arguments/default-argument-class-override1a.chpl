var x10 = 10, x20 = 20, x30 = 30;

class C {
  proc foo(x = x10) { writeln("C.foo:", x); }
}

class D : C {
  override proc foo(x = x20) { writeln("D.foo:", x); }
}

class E : D {
  override proc foo(x = x30) { writeln("E.foo:", x); }
}  

var ownD = new owned D();
var d = ownD.borrow();
d.foo();

var ownC = new owned D();
var c: borrowed C = ownC.borrow();
c.foo();

var ownC2 = new owned C();
var c2: borrowed C = ownC2.borrow();
c2.foo();

var ownE = new owned E();
var e = ownE.borrow();
e.foo();

var ownE2 = new owned E();
var e2: borrowed D = ownE2.borrow();
e2.foo();

var ownE3 = new owned E();
var e3: borrowed C = ownE3.borrow();
e3.foo();
