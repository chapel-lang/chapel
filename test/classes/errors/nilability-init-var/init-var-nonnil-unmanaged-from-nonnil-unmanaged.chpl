//  lhs: unmanaged!  rhs: unmanaged!  ok

class MyClass {
  var x: int;
}

var rhs = new unmanaged MyClass();

var lhs: unmanaged MyClass = rhs;

delete rhs;
