//  lhs: owned?  rhs: borrowed!  error: mm

class MyClass {  var x: int;  }

var lhs: owned MyClass?;
var rhs = new borrowed MyClass();

lhs = rhs;

compilerError("done");

