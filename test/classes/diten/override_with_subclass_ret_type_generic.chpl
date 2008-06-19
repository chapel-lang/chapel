class C {
  def bar() {
    writeln("C.bar()");
  }
}

class D: C {
  var s;
  def bar() {
    writeln("D.bar(", s, ")");
  }
}

class A {
  var i: int;
  def foo() {
    return new C();
  }
}

class B: A {
  var s;
  def foo() {
    return new D(s);
  }
}

def main {
  var a: A = new B(1, "a string");
  var d: C = a.foo();
  d.bar();
}
