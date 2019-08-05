class A {
  type myType = real;
}

class B: A {
  var x: myType;
}

var a = new unmanaged A();
var b: unmanaged B?;

delete a;
