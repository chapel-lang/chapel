class Base {
  proc method(c) {
    writeln("Base");
  }
}

record C1 {
  type t;
  proc foo() { writeln("foo"); }
}

record C2 {
  proc bar() { writeln("bar"); }
}

class Sub1: Base {
  override proc method(c: C1) {
    c.foo();
  }
}

class Sub2: Base {
  override proc method(c: C2) {
    c.bar();
  }
}

proc main {
  var s1: borrowed Base?;
  s1 = new borrowed Sub1(); s1!.method(new C1(int));
  s1 = new borrowed Sub2(); s1!.method(new C2());
}
