//  lhs: borrowed!  rhs: unmanaged!  ok

class MyClass {  var x: int;  }

var lhs = (new owned MyClass()).borrow();
var rhs = new unmanaged MyClass();

lhs = rhs;

delete rhs;

