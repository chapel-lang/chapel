//  lhs: borrowed!  rhs: shared!  ok

class MyClass {  var x: int;  }

var lhs = (new owned MyClass()).borrow();
var rhs = new shared MyClass();

lhs = rhs;


