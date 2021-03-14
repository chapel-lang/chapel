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
    var c : borrowed C? = (new owned C(123)).borrow();
    var x = new R(c);
  }

  proc testInit() {
    var c : borrowed C? = (new owned C(42)).borrow();

    var x = new R(c);
    var y = new R(c);
    var z = new R(c);

    bar();
  }

  proc main() {
    var a: borrowed C? = (new owned C()).borrow();
    a.foo();
    a.bar();
    baz(a);

    testInit();
  }
}
