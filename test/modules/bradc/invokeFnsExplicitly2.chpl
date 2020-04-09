module M1 {
  use M2;

  proc foo() {
    writeln("In M1's foo()");
  }

  proc main() {
    M1.foo();
    M2.foo();
    baz();
  }
}

module M2 {
  use M1;

  proc foo() {
    writeln("In M2's foo()");
  }

  proc baz() {
    M1.foo();
    M2.foo();
  }
}
