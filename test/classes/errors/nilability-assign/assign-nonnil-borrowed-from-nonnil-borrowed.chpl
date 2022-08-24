//  lhs: borrowed!  rhs: borrowed!  ok

class MyClass {  var x: int;  }

var lhs = (new owned MyClass()).borrow();
var rhs = (new owned MyClass()).borrow();

lhs = rhs;


