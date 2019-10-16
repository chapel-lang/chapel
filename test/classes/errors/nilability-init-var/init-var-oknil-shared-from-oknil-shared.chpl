//  lhs: shared?  rhs: shared?  ok

class MyClass {
  var x: int;
}

var rhs: shared MyClass?;

var lhs: shared MyClass? = rhs;


