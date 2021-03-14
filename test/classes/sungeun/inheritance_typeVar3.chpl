class A {
  type myType = real;
}

class B: A {
  proc foo(x: myType) {
    writeln(x);
  }
}

var a = new borrowed A();
var b: borrowed B?;
