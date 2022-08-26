//  lhs: borrowed!  rhs: borrowed?  error: nlb

class MyClass {  var x: int;  }

var lhs = (new owned MyClass()).borrow();
var rhs: borrowed MyClass?;

lhs = rhs;

compilerError("done");

