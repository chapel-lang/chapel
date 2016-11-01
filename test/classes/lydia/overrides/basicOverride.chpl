// Checks that you can access the overridden method.
class A {
  proc foo() {
    writeln("in A.foo()");
  }
}

class B: A {
  proc foo() {
    super.foo();
    writeln("in B.foo()");
  }
}

var a = new A();
var b = new B();
var b2: A = new B();

a.foo();
b.foo();

b2.foo();

delete b2;

delete b;
delete a;
