//  lhs: borrowed!  rhs: owned?  error: nlb

class MyClass {  var x: int;  }

var lhs = new borrowed MyClass();
var rhs: owned MyClass?;

lhs = rhs;

compilerError("done");

