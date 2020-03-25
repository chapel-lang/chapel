class A {
  type myType = real;
}

class B: A {
  proc foo() {
    var x: myType;
    writeln(x);
  }
}

var a = new unmanaged A();
var b: unmanaged B?;

delete a;
