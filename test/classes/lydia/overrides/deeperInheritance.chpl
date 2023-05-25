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

var a = (new owned A()).borrow();
var b = (new owned B()).borrow();
var c = (new owned C()).borrow();

var c2: borrowed A = (new owned C()).borrow();
var c3: borrowed B = (new owned C()).borrow();

a.foo();
b.foo();
c.foo();

c2.foo();
c3.foo();
