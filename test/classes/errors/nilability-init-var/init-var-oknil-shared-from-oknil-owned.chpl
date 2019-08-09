//  lhs: shared?  rhs: owned?  ok

class MyClass {
  var x: int;
}

var rhs: owned MyClass?;

var lhs: shared MyClass? = rhs;


