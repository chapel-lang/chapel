//  lhs: borrowed?  rhs: shared?  ok

class MyClass {
  var x: int;
}

var rhs: shared MyClass?;

var lhs: borrowed MyClass? = rhs;


