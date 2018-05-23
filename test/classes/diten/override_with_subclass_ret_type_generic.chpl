class C {
  proc bar() {
    writeln("C.bar()");
  }
}

class D: C {
  var s;
  proc bar() {
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
  proc foo() {
    return new unmanaged D(s);
  }
}

proc main {
  var a: A = new unmanaged B(1, "a string");
  var d: C = a.foo();

  d.bar();

  delete d;
  delete a;
}
