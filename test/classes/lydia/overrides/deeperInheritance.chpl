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

var a = new borrowed A();
var b = new borrowed B();
var c = new borrowed C();

var c2: borrowed A = new borrowed C();
var c3: borrowed B = new borrowed C();

a.foo();
b.foo();
c.foo();

c2.foo();
c3.foo();
