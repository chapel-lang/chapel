//  lhs: unmanaged!  rhs: owned?  errors: nlb, mm

class MyClass {  var x: int;  }

var lhs = new unmanaged MyClass();
var rhs: owned MyClass?;

lhs = rhs;

compilerError("done");
delete lhs;

