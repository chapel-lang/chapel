//  lhs: owned!  rhs: unmanaged?  errors: nlb, mm

class MyClass {  var x: int;  }

var lhs = new owned MyClass();
var rhs: unmanaged MyClass?;

lhs = rhs;

compilerError("done");

