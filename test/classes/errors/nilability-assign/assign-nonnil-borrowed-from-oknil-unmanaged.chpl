//  lhs: borrowed!  rhs: unmanaged?  error: nlb

class MyClass {  var x: int;  }

var lhs = (new owned MyClass()).borrow();
var rhs: unmanaged MyClass?;

lhs = rhs;

compilerError("done");

