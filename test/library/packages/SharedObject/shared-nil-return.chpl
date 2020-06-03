class C { }

class D {
  var sc: shared C?;
  proc foo() {
    return sc;
  }
}

var d = new unmanaged D();
d.foo();

delete d;
