//  lhs: unmanaged!  rhs: unmanaged?  error: nlb

class MyClass {  var x: int;  }

var lhs = new unmanaged MyClass();
var rhs: unmanaged MyClass?;

lhs = rhs;

compilerError("done");
delete lhs;

