//  lhs: unmanaged!  rhs: unmanaged!  ok

class MyClass {  var x: int;  }

var lhs = new unmanaged MyClass();
var rhs = new unmanaged MyClass();

lhs = rhs;

delete rhs;
delete lhs;

