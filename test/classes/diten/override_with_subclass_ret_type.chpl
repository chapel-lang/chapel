class ic_A {
  var i: int;
  proc bar() {
    writeln("ic_A.bar()");
  }
}

class ic_B: ic_A {
  var s: real;
  override proc bar() {
    writeln("ic_B.bar()");
  }
}


class A {
  var n: int;
  proc foo() {
    return new unmanaged ic_A(1);
  }
}

class B: A {
  var r: real;
  override proc foo() {
    return new unmanaged ic_B(1, 2.0);
  }
}

proc main {
  var a: unmanaged A = new unmanaged B(1, 2.0);
  var ic_b = a.foo();

  ic_b.bar();

  delete ic_b;
  delete a;
}
