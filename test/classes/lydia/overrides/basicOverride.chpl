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
a.foo();
b.foo();
