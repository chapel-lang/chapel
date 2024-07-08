//  lhs: borrowed?  rhs: borrowed!  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass? = nil;
var ownRhs = new owned MyClass();
var rhs = ownRhs.borrow();

lhs = rhs;


