//  lhs: owned?  rhs: shared!  error: mm

class MyClass {  var x: int;  }

var lhs: owned MyClass?;
var rhs = new shared MyClass();

lhs = rhs;

compilerError("done");

