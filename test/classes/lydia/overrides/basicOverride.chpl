// Checks that you can access the overridden method.
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

var a = new owned A();
var b = new owned B();
var b2: owned A = new owned B();

a.foo();
b.foo();

b2.foo();
