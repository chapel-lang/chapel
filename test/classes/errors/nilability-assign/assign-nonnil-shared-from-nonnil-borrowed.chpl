//  lhs: shared!  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs = new shared MyClass();
var rhs = (new owned MyClass()).borrow();

lhs = rhs;

compilerError("done");

