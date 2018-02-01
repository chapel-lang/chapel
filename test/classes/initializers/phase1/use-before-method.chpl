
record B {
  var foo : int;

  proc set(i:int) {
    foo = i;
  }
}

record A {
  var b : B;

  proc init() {
    b.set(1);
    super.init();
  }
}

var a = new A();
