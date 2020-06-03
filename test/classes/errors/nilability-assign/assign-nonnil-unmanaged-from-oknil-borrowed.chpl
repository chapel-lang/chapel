//  lhs: unmanaged!  rhs: borrowed?  errors: nlb, mm

class MyClass {  var x: int;  }

var lhs = new unmanaged MyClass();
var rhs: borrowed MyClass?;

lhs = rhs;

compilerError("done");
delete lhs;

