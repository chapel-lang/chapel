module A {
  var x = 13;

  proc foo() {
    writeln("In A.foo()");
  }
}

module B {
  import A;

  proc main() {
    foo();
  }
}
