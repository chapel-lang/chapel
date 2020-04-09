
record B {
  var foo : int;
}

record A {
  var b : B;

  proc init() {
    b.foo = 5;
  }
}

var a = new A();
