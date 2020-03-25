//  lhs: owned!  rhs: shared?  errors: nlb, mm

class MyClass {  var x: int;  }

var lhs = new owned MyClass();
var rhs: shared MyClass?;

lhs = rhs;

compilerError("done");

