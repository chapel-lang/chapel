
record A {
  var b : B(int);
}

record B {
  type T;
  var a : A;
}

var a = new A();
