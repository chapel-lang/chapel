module M2 {
  class C {
    var a = 1, b = 2;
    proc foo() {
      return a+b;
    }
  }
  proc main {
    var c = bar();
    writeln(baz(c));
    delete c;
  }
  proc bar() {
    return new unmanaged C();
  }
  proc baz(obj) {
    return obj.foo();
  }
}
