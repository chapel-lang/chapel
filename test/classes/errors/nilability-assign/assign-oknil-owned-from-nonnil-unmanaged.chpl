//  lhs: owned?  rhs: unmanaged!  error: mm

class MyClass {  var x: int;  }

var lhs: owned MyClass?;
var rhs = new unmanaged MyClass();

lhs = rhs;

compilerError("done");
delete rhs;

