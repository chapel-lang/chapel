class C {
  proc bar() {
    writeln("C.bar()");
  }
}

class D: C {
  var s;
  override proc bar() {
    writeln("D.bar(", s, ")");
  }
}

class A {
  var i: int;
  proc foo() {
    return new unmanaged C();
  }
}

class B: A {
  var s;
  override proc foo() {
    return new unmanaged D(s);
  }
}

proc main {
  var a: unmanaged A = new unmanaged B(1, "a string");
  var d: unmanaged C = a.foo();

  d.bar();

  delete d;
  delete a;
}
