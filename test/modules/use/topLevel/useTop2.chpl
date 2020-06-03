module MMM {
  proc foo() {
    writeln("In foo");
  }
}

module NNN {
  proc main() {
    use MMM;
    MMM.foo();
  }
}
