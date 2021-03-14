//  lhs: owned!  rhs: borrowed?  errors: nlb, mm

class MyClass {  var x: int;  }

var lhs = new owned MyClass();
var rhs: borrowed MyClass?;

lhs = rhs;

compilerError("done");

