//  lhs: unmanaged?  rhs: shared!  error: mm

class MyClass {  var x: int;  }

var lhs: unmanaged MyClass?;
var rhs = new shared MyClass();

lhs = rhs;

compilerError("done");

