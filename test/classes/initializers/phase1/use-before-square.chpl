
record B {
  var foo : int;

  proc this(i:int) ref {
    return foo;
  }
}

record A {
  var b : B;

  proc init() {
    b[1] = 1;
  }
}

var a = new A();
