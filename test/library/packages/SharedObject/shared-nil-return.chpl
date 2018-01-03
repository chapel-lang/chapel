use SharedObject;

class C { }

class D {
  var sc: Shared(C);
  proc foo() {
    return sc;
  }
}

var d = new D();
d.foo();

delete d;
