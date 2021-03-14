module MMM {
  proc foo() {
    writeln("In foo");
  }
}

module NNN {
  proc main() {
    MMM.foo();
  }
}
