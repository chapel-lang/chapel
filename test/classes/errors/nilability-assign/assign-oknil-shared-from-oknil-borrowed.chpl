//  lhs: shared?  rhs: borrowed?  error: mm

class MyClass {  var x: int;  }

var lhs: shared MyClass? = nil;
var rhs: borrowed MyClass?;

lhs = rhs;

compilerError("done");

