module M {
  class C {
    var x: int;
  }

  record R {
    var c : borrowed C;

    proc postinit() {
      writeln("created R: ", this);
    }
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

  proc bar() {
    var ownC = new owned C(123); var c : borrowed C? = ownC.borrow();
    var x = new R(c);
  }

  proc testInit() {
    var ownC = new owned C(42); var c : borrowed C? = ownC.borrow();

    var x = new R(c);
    var y = new R(c);
    var z = new R(c);

    bar();
  }

  proc main() {
    var ownC = new owned C(); var a: borrowed C? = ownC.borrow();
    a.foo();
    a.bar();
    baz(a);

    testInit();
  }
}
