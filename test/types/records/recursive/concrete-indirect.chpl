
record A {
  var b : B;
}

record B {
  var a : A;
}

var a = new A();
