//  lhs: owned!  rhs: owned?  error: nlb

class MyClass {
  var x: int;
}

var rhs: owned MyClass?;

var lhs: owned MyClass = rhs;
