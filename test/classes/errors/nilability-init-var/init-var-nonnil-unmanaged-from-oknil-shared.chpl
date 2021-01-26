//  lhs: unmanaged!  rhs: shared?  errors: nlb, mm

class MyClass {
  var x: int;
}

var rhs: shared MyClass?;

var lhs: unmanaged MyClass = rhs;
