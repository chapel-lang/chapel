class A {
  param x = 10;
}

class B: A {
  proc foo() {
    var y = x;
    writeln(y);
  }
}

var a = new A();
var b: B;
