// In this test, the overridden method is defined outside the class
class A {
}

proc A.foo(x) {
  writeln("in A.foo() with arg ", x);
}

class B: A {
  proc foo(x) {
    super.foo(x);
    writeln("in B.foo() with arg ", x);
  }
}

var a = new A();
var b = new B();

a.foo(3);
b.foo(2);

delete b;
delete a;
