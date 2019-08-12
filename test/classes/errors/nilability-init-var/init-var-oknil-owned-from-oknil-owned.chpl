//  lhs: owned?  rhs: owned?  ok

class MyClass {
  var x: int;
}

var rhs: owned MyClass?;

var lhs: owned MyClass? = rhs;


