//  lhs: unmanaged!  rhs: unmanaged?  error: nlb

class MyClass {
  var x: int;
}

var rhs: unmanaged MyClass?;

var lhs: unmanaged MyClass = rhs;
