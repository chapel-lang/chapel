// Checks that you can access the overridden method even on a different locale
// than where the type itself was defined (but the instance is local).
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

on Locales(1) {
  var a = new A();
  var b = new B();
  a.foo();
  b.foo();
}
