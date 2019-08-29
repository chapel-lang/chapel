//  lhs: shared!  rhs: unmanaged!  error: mm

class MyClass {  var x: int;  }

var lhs = new shared MyClass();
var rhs = new unmanaged MyClass();

lhs = rhs;

compilerError("done");
delete rhs;

