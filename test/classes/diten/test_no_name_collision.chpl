class outerClass {
  class aClass {
    var a: int;
  }
  proc foo() {
    var c = new unmanaged aClass(4);
    writeln(c.a);
    delete c;
  }
}

class aClass {
  var b: int;
}

proc main() {
  var a = new unmanaged aClass(3);
  var b = new unmanaged outerClass();
  b.foo();
  writeln(a.b);
  delete a;
  delete b;
}
