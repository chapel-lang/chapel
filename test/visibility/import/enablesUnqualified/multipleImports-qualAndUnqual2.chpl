// Tests behavior when a module is imported twice in the same scope, but with
// different symbols specified for unqualified access
module A {
  var x: int = 4;
  proc foo() {
    writeln("In A.foo()");
  }
}
module B {
  import A;
  import A.x;

  proc main() {
    writeln(x);
    writeln(A.x);
    A.foo();
  }
}
