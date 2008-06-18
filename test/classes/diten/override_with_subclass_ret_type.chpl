class ic_A {
  var i: int;
  def bar() {
    writeln("ic_A.bar()");
  }
}

class ic_B: ic_A {
  var s: real;
  def bar() {
    writeln("ic_B.bar()");
  }
}


class A {
  var n: int;
  def foo() {
    return new ic_A(1);
  }
}

class B: A {
  var r: real;
  def foo() {
    return new ic_B(1, 2.0);
  }
}

def main {
  var a: A = new B(1, 2.0);
  var ic_b = a.foo();
  ic_b.bar();
}
