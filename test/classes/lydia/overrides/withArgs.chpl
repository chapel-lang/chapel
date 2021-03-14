// Checks that accessing the overridden method works even if they both define
// (the same) arguments
class A {
  proc foo(x) {
    writeln("in A.foo() with arg ", x);
  }

  proc bar(x: int) {
    writeln("in A.bar() with arg ", x);
  }
}

class B: A {
  override proc foo(x) {
    super.foo(x);
    writeln("in B.foo() with arg ", x);
  }

  override proc bar(x: int) {
    super.bar(x);
    writeln("in B.bar() with arg ", x);
  }
}

var a = new unmanaged A();
var b = new unmanaged B();

a.foo(3);
b.foo(2);
a.bar(3);
b.bar(2);

delete b;
delete a;
