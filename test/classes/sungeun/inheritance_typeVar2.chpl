class A {
  type myType = real;
}

class B: A {
  proc foo() {
    var x: myType;
    writeln(x);
  }
}

var a = new A();
var b: B;
