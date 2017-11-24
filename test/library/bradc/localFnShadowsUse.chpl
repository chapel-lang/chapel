module M1 {
  use M2;

  proc foo() {
    writeln("In M1's foo()");
  }

  proc main() {
    foo();
  }
}

module M2 {
  proc foo() {
    writeln("In M2's foo()");
  }
}
