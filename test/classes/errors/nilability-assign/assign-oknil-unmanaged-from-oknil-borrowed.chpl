//  lhs: unmanaged?  rhs: borrowed?  error: mm

class MyClass {  var x: int;  }

var lhs: unmanaged MyClass?;
var rhs: borrowed MyClass?;

lhs = rhs;

compilerError("done");

