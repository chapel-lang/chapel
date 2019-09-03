module M {
  class C {
    var x: int;
  }

  proc C.foo() {
    writeln("foo(", this, ")");
  }
  proc C.bar() {
    writeln("bar(", this, ")");
  }
  proc baz(arg:C) {
    writeln("baz(", arg, ")");
  }

  proc main() {
    var a: borrowed C? = (new owned C()).borrow();
    a.foo();
    a.bar();
    baz(a);
  }
}
