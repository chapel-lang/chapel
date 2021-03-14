//  lhs: owned?  rhs: unmanaged!  error: mm

class MyClass {
  var x: int;
}

var rhs = new unmanaged MyClass();

var lhs: owned MyClass? = rhs;
