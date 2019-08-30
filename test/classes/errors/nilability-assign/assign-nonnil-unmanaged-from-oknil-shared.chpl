//  lhs: unmanaged!  rhs: shared?  errors: nlb, mm

class MyClass {  var x: int;  }

var lhs = new unmanaged MyClass();
var rhs: shared MyClass?;

lhs = rhs;

compilerError("done");
delete lhs;

