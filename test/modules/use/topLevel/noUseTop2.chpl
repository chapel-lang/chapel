module MMM {
  proc foo() {
    writeln("In foo");
  }
}

module NNN {
  MMM.foo();
  proc main() {
    use MMM;
  }
}
