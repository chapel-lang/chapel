// Checks that you can access the overridden parent method.
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
    super.foo();
    writeln("in C.foo()");
  }
}

var a = new shared A();
var b = new shared B();
var c = new shared C();

var c2: shared A = new shared C();
var c3: shared B = new shared C();

a.foo();
b.foo();
c.foo();

c2.foo();
c3.foo();
