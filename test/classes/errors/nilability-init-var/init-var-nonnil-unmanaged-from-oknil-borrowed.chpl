//  lhs: unmanaged!  rhs: borrowed?  errors: nlb, mm

class MyClass {
  var x: int;
}

var rhs: borrowed MyClass?;

var lhs: unmanaged MyClass = rhs;
