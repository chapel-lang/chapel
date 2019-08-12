//  lhs: unmanaged!  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs = new unmanaged MyClass();
var rhs = new borrowed MyClass();

lhs = rhs;

compilerError("done");
delete lhs;

