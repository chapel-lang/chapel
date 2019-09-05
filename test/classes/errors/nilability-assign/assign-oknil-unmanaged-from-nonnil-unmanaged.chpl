//  lhs: unmanaged?  rhs: unmanaged!  ok

class MyClass {  var x: int;  }

var lhs: unmanaged MyClass?;
var rhs = new unmanaged MyClass();

lhs = rhs;

delete rhs;

