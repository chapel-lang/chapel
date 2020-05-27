module A {
  proc foo(x: int) {
    writeln("A.foo (", x, ")");
  }

  proc foo(s: string) {
    writeln("A.foo \"", s, "\"");
  }
}
module B {
  proc foo(x: int, y: int) {
    writeln("B.foo (", x, ", ", y, ")");
  }
  proc foo() {
    writeln("B.foo");
  }
}
module C {
  public import A.foo;
  public import B.foo;
}
module User {
  import C;

  proc main() {
    C.foo();
    C.foo(3);
    C.foo("blah");
    C.foo(3, 2);
  }
}
