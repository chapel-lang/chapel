//  lhs: borrowed!  rhs: unmanaged!  ok

class MyClass {  var x: int;  }

var ownLhs = new owned MyClass();
var lhs = ownLhs.borrow();
var rhs = new unmanaged MyClass();

lhs = rhs;

delete rhs;

