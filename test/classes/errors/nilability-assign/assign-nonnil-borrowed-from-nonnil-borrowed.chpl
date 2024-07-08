//  lhs: borrowed!  rhs: borrowed!  ok

class MyClass {  var x: int;  }

var ownLhs = new owned MyClass();
var ownRhs = new owned MyClass();
var lhs = ownLhs.borrow();
var rhs = ownRhs.borrow();

lhs = rhs;


