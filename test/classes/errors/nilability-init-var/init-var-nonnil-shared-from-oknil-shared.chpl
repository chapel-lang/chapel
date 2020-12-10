//  lhs: shared!  rhs: shared?  error: nlb

class MyClass {
  var x: int;
}

var rhs: shared MyClass?;

var lhs: shared MyClass = rhs;
