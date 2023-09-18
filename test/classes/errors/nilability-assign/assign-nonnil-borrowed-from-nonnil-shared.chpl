//  lhs: borrowed!  rhs: shared!  ok

class MyClass {  var x: int;  }

var ownLhs = new owned MyClass();
var lhs = ownLhs.borrow();
var rhs = new shared MyClass();

lhs = rhs;


