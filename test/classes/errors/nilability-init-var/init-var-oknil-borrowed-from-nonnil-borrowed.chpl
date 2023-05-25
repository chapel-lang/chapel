//  lhs: borrowed?  rhs: borrowed!  ok

class MyClass {
  var x: int;
}

var rhs = (new owned MyClass()).borrow();

var lhs: borrowed MyClass? = rhs;


