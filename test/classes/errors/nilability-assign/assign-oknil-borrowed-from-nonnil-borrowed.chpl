//  lhs: borrowed?  rhs: borrowed!  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass? = nil;
var rhs = (new owned MyClass()).borrow();

lhs = rhs;


