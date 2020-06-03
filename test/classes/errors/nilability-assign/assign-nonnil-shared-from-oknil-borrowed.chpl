//  lhs: shared!  rhs: borrowed?  errors: nlb, mm

class MyClass {  var x: int;  }

var lhs = new shared MyClass();
var rhs: borrowed MyClass?;

lhs = rhs;

compilerError("done");

