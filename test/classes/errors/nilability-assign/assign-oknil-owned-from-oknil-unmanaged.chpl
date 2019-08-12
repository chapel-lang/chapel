//  lhs: owned?  rhs: unmanaged?  error: mm

class MyClass {  var x: int;  }

var lhs: owned MyClass?;
var rhs: unmanaged MyClass?;

lhs = rhs;

compilerError("done");

