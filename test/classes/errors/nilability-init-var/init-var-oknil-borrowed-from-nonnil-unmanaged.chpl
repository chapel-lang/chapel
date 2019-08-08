//  lhs: borrowed?  rhs: unmanaged!  ok

class MyClass {
  var x: int;
}

var rhs = new unmanaged MyClass();

var lhs: borrowed MyClass? = rhs;

delete rhs;
