module A {
  config param enableInnerFoo = true;

  record Inner {
    proc foo() where enableInnerFoo {
      return 42;
    }
  }
}

module B {
  import A.Inner;

  record Outer {
    forwarding var inner: Inner;
  }
}

module C {
  import B.Outer;

  record Outermost {
    forwarding var outer: Outer;
  }
}

module D {
  import A.Inner;
  import B.Outer;
  import C.Outermost;

  config param startAtCase = 1;

  proc generic(x) {
    writeln(x.foo());
  }

  proc case1() {
    if startAtCase <= 1 {
      generic(new Outermost());
    }
  }

  proc case2() {
    if startAtCase <= 2 {
      proc Outer.foo() do return "Outer foo";
      generic(new Outermost());
    }
  }

  proc case3() {
    if startAtCase <= 3 {
      generic(new Outermost());
    }
  }

  proc main() {
    case1();
    case2();
    case3();
  }
}
