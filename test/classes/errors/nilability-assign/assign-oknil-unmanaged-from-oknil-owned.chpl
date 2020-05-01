//  lhs: unmanaged?  rhs: owned?  error: mm

class MyClass {  var x: int;  }

var lhs: unmanaged MyClass? = nil;
var rhs: owned MyClass?;

lhs = rhs;

compilerError("done");

