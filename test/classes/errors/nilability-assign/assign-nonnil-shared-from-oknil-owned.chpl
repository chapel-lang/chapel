//  lhs: shared!  rhs: owned?  error: nlb

class MyClass {  var x: int;  }

var lhs = new shared MyClass();
var rhs: owned MyClass?;

lhs = rhs;

compilerError("done");

