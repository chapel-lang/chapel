//  lhs: shared?  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs: shared MyClass? = nil;
var rhs = (new owned MyClass()).borrow();

lhs = rhs;

compilerError("done");

