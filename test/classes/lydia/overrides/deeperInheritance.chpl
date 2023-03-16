// Checks that you can access the overridden grandparent method.
class A {
  proc foo() {
    writeln("in A.foo()");
  }
}

class B: A {
  override proc foo() {
    super.foo();
    writeln("in B.foo()");
  }
}

class C: B {
  override proc foo() {
    super.super.foo();
    writeln("in C.foo()");
  }
}

var ownA = new owned A();
var a = ownA.borrow();
var ownB = new owned B();
var b = ownB.borrow();
var ownC = new owned C();
var c = ownC.borrow();

var ownC2 = new owned C();
var c2: borrowed A = ownC2.borrow();
var ownC3 = new owned C();
var c3: borrowed B = ownC3.borrow();

a.foo();
b.foo();
c.foo();

c2.foo();
c3.foo();
