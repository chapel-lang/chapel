//  lhs: borrowed!  rhs: owned!  ok

class MyClass {  var x: int;  }

var ownLhs = new owned MyClass();
var lhs = ownLhs.borrow();
var rhs = new owned MyClass();

lhs = rhs;


