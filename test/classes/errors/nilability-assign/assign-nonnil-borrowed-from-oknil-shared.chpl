//  lhs: borrowed!  rhs: shared?  error: nlb

class MyClass {  var x: int;  }

var lhs = new borrowed MyClass();
var rhs: shared MyClass?;

lhs = rhs;

compilerError("done");

