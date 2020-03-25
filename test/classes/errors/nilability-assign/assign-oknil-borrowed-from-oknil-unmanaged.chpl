//  lhs: borrowed?  rhs: unmanaged?  ok

class MyClass {  var x: int;  }

var lhs: borrowed MyClass? = nil;
var rhs: unmanaged MyClass?;

lhs = rhs;


