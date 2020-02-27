//  lhs: borrowed?  rhs: unmanaged!  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass? = nil;
var rhs = new unmanaged MyClass();

lhs = rhs;

delete rhs;

