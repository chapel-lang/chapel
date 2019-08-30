module M {
  class C { }

  proc C.foo() { }
  proc C.bar() { }

  proc baz(arg:C) { }

  proc main() {
    var a: borrowed C?;

    var b: borrowed C?;

    a.foo();

    b.bar();

    baz(b);
  }
}
