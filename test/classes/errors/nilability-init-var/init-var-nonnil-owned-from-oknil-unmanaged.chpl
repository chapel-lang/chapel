//  lhs: owned!  rhs: unmanaged?  errors: nlb, mm

class MyClass {
  var x: int;
}

var rhs: unmanaged MyClass?;

var lhs: owned MyClass = rhs;
