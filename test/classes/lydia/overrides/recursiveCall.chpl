// Checks that accessing the overridden method works even if they both define
// (the same) arguments and there's a recursive call
class A {
  proc foo(x) {
    writeln("in A.foo() with arg ", x);
  }
}

class B: A {
  proc foo(x) {
    if (x > 5) {
      super.foo(x);
    } else {
      foo(x + 1);
    }
    writeln("in B.foo() with arg ", x);
  }
}

var a = new A();
var b = new B();

a.foo(3);
b.foo(2);

delete b;
delete a;
