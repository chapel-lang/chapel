//  lhs: borrowed!  rhs: owned?  error: nlb

class MyClass {
  var x: int;
}

var rhs: owned MyClass?;

var lhs: borrowed MyClass = rhs;
