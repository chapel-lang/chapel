//  lhs: borrowed!  rhs: shared?  error: nlb

class MyClass {  var x: int;  }

var lhs = (new owned MyClass()).borrow();
var rhs: shared MyClass?;

lhs = rhs;

compilerError("done");

