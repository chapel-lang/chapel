// Checks that you can access the overridden method even on a different locale
// than where the type itself was defined (but the instance is local).
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

var a = new unmanaged A();
var b = new unmanaged B();

on Locales(1) {
  var a2 = new unmanaged A();
  var b2 = new unmanaged B();

  a.foo();
  b.foo();

  a2.foo();
  b2.foo();

  delete b2;
  delete a2;
}

delete b;
delete a;

