module MMM {
  proc foo() {
    writeln("In foo");
  }
}

module NNN {
  use MMM;
  proc main() {
    MMM.foo();
  }
}
