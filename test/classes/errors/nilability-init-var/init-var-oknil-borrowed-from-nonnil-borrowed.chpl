//  lhs: borrowed?  rhs: borrowed!  ok

class MyClass {
  var x: int;
}

var ownRhs = new owned MyClass();
var rhs = ownRhs.borrow();

var lhs: borrowed MyClass? = rhs;


