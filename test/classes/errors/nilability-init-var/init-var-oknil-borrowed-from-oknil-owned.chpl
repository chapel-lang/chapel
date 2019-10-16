//  lhs: borrowed?  rhs: owned?  ok

class MyClass {
  var x: int;
}

var rhs: owned MyClass?;

var lhs: borrowed MyClass? = rhs;


