//  lhs: unmanaged!  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs = new unmanaged MyClass();
var rhs = (new owned MyClass()).borrow();

lhs = rhs;

compilerError("done");
delete lhs;

