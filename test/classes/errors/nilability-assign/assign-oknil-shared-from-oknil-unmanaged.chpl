//  lhs: shared?  rhs: unmanaged?  error: mm

class MyClass {  var x: int;  }

var lhs: shared MyClass?;
var rhs: unmanaged MyClass?;

lhs = rhs;

compilerError("done");

